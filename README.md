# SiboarkaECS

A learning-focused **Entity Component System (ECS)** game engine in C++20, with a tower-defence demo and an in-engine editor built on top of it.

No engine framework — a custom ECS core, [raylib](https://www.raylib.com/) for rendering, [Dear ImGui](https://github.com/ocornut/imgui) for the editor, CMake to tie it together.

> No code has been written by Claude. Claude is used as a teacher/guide.

---

## Design decisions

The interesting part of this project is not the feature list — it's why the storage looks the way it does, and what each choice costs.

### Entity identity — generational handles

An `Entity` is a 32-bit handle, not an index:

```
 31                  12 11        0
 ┌──────────────────────┬───────────┐
 │   index (20 bits)    │ gen (12b) │
 └──────────────────────┴───────────┘
```

Destroyed indices are recycled through a free list, and the generation counter for that index is incremented on destruction. A handle is alive only if its generation still matches the world's.

**Why:** without recycling, identifiers grow monotonically and the sparse array (below) would have to be sized by *total entities ever created* rather than *peak live entities* — unbounded in a game that spawns a projectile per shot. Recycling bounds it; generations are what make recycling safe, converting a silent wrong-object read into a detectable failure.

**Bonus:** liveness became one indexed array read, replacing a hash lookup into a node-based `unordered_set`.

### Component storage — sparse sets

Each component type gets a `SparseSet<T>` holding three parallel arrays:

```
sparse      indexed by entity index → position in dense
dense       [ 20481 , 8193 , 36865 ]      who owns each position
components  [   T   ,  T   ,   T   ]      the actual data, packed, no gaps
```

`has`, `get`, `insert`, and `remove` are all O(1) with no hashing. Removal is swap-and-pop: the last element backfills the vacated slot, so components stay contiguous and iteration is a bare index loop with unit stride.

**Why:** a component store has to answer two questions that pull in opposite directions — *"what is entity 47's transform?"* (wants an index) and *"give me every transform"* (wants contiguity). A hash map answers the first and destroys the second. A packed vector answers the second and can't answer the first. The sparse set buys both by spending `MAX_ENTITIES × 4` bytes per component type on a mostly-empty index array.

### Type erasure — `IComponentStore`

`SparseSet<Transform>` and `SparseSet<Health>` are unrelated types, so they can't share a container. Both derive from `IComponentStore`, which exposes only the operations whose signatures don't mention `T` — `remove`, `size`, `has`, and `entities()`. `World` holds `unordered_map<type_index, unique_ptr<IComponentStore>>`.

The type lookup therefore happens **once per store**, not once per component access.

Keeping `dense` as a plain `vector<Entity>` — rather than merging it with the component array — is what allows `entities()` to live on the untyped base at all. That's what makes the query path below possible.

### Queries — drive the smallest store

`getEntitiesWith<A, B>()` looks up each type's store, picks the one with the fewest entries, walks its dense array, and probes the others. Cost is proportional to the *smallest* store rather than to the live entity count.

An overload takes a caller-owned buffer and `clear()`s it, so per-frame queries reuse their allocation instead of building a fresh vector every frame.

### What this design gives up

Stated plainly, because these are real:

- **Reference invalidation.** A `T&` obtained from the world is invalidated when *any other* entity's `T` is removed — swap-and-pop can move the last element on top of the vacated slot. A hash map would not do this. Structural changes must stay outside iteration loops.
- **Volatile iteration order.** Dense order is insertion order mutated by every removal. Any system whose output depends on visit order (tie-breaking between equidistant targets, for example) can change behaviour as a side effect of an unrelated entity's death.
- **Memory proportional to `MAX_ENTITIES`, not to live entities**, for every component type. ~20 KB per type at the current bound. Most of it is never touched, so it costs address space rather than working set — but it is reserved.

---

## Known limitations

Honest list of what's weak, roughly in order of impact:

| Area | Status |
|---|---|
| **Collision broad phase** | Still O(n²) — every pair tested every frame. This dominates everything the storage work improved. A uniform grid is the next planned change. |
| **Tests** | None yet. `SparseSet` is the highest-value target: property-based testing against a `std::map` reference model. |
| **Benchmarks** | The storage migration has not been measured. Expectation is no visible difference at the demo's scale and a clear one at several thousand entities; that needs verifying rather than asserting. |
| **Multi-component iteration** | Multi-type queries still probe per entity. A single-type `ComponentView<T>` (dense iteration yielding entity + component) is in progress; the multi-type case is where archetypes would start to pay. |
| **Release-mode preconditions** | Several are guarded by `assert` only, so Debug and Release differ in behaviour on violation. |

---

## What's inside

### Engine (`engine/`) — no raylib dependency

| Piece | Role |
|---|---|
| `World` | Entity registry — handles, lifetime, component storage, queries |
| `SparseSet<T>` | Per-type component storage |
| `IComponentStore` | Type-erased store interface |
| `System` | Base class with a pure virtual `update()` |
| `EventBus` | Typed publish/subscribe events |
| `SceneManager` | Stack-based scene switching |
| `Logger` | Logging utility |
| Systems | `MovementSystem`, `GravitySystem`, `PhysicsResponseSystem`, `CollisionEventSystem` |
| Components | `TransformComponent`, `ColliderComponent`, `GravityComponent`, `TagComponent`, and others |

### Game (`game/`) — tower-defence demo

| System | Role |
|---|---|
| `WaveSpawnerSystem` | Spawns enemy waves on a timer |
| `PathFollowingSystem` | Steers enemies toward the nearest waypoint |
| `PlacementSystem` | Tower placement and removal |
| `ShootingSystem` | Target acquisition and firing |
| `CollisionSystem` | Broad + narrow phase, emits `CollisionEvent` |
| `DamageSystem` | Resolves hits and health |
| `DeathSystem` | Deferred entity destruction |
| `GameStateSystem` | Win/lose and wave progression |
| `InputSystem` | Player input |
| `RenderSystem` | Draws everything via raylib |
| `VFXSystem` | Laser beams, hit flashes |

`CollisionSystem` and `RenderSystem` live in `game/` rather than `engine/` because they touch raylib types — see the architecture rule below.

### Editor

Unreal-inspired panels built with Dear ImGui + rlImGui:

| Panel | Role |
|---|---|
| **Viewport** | Game renders to a `RenderTexture2D` shown as a resizable panel |
| **Hierarchy** | Lists tagged entities; click to select |
| **Inspector** | Live-edits components on the selected entity |
| **Play/Pause** | Freezes game time while the editor stays interactive |

Mouse input is scoped to the viewport content area, so clicking editor panels never leaks into the game.

---

## Requirements

- CMake 3.16+
- A C++20 compiler — MSVC 2019+, GCC 10+, or Clang 10+
- Internet access on the **first build** (raylib, Dear ImGui, and rlImGui are fetched via `FetchContent`)

## Build & Run

```bash
cmake -S . -B build
cmake --build build --config Debug
./build/game/Debug/game.exe
```

First configure takes ~30–60 seconds while dependencies download and compile.

**Visual Studio:** run the configure step above, open `build/SiboarkaECS.sln`, set `game` as the startup project, press F5.

> Changing a class's *members* (adding, removing, or reordering) requires a full rebuild, not an incremental one — every translation unit that includes the header has to agree on the layout. Use `cmake --build build --config Debug --clean-first`.

---

## Project structure

```
SiboarkaECS/
├── CMakeLists.txt            ← root build file
├── cmake/
│   ├── Raylib.cmake          ← FetchContent for raylib 5.0
│   └── ImGui.cmake           ← FetchContent for Dear ImGui v1.92.8 + rlImGui
├── engine/                   ← pure ECS library (zero raylib dependency)
│   ├── include/ecs/
│   │   ├── Types.h           ← Entity handle layout, MAX_ENTITIES
│   │   ├── World.h           ← registry: handles, storage, queries
│   │   ├── System.h          ← base system class
│   │   ├── EventBus.h        ← typed event system
│   │   ├── SceneManager.h    ← scene stack
│   │   ├── storage/          ← SparseSet, IComponentStore, ComponentView
│   │   ├── components/       ← engine-owned components
│   │   └── systems/          ← engine-owned systems
│   └── src/World.cpp
└── game/                     ← demo executable (raylib + imgui)
    ├── res/                  ← assets
    └── src/
        ├── components/       ← game-specific components
        ├── systems/          ← game-specific systems
        ├── events/           ← game-specific events
        ├── scenes/           ← scene definitions
        └── main.cpp          ← game loop + editor panels
```

Sources are auto-discovered by `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`. The patterns are not symmetric: the engine globs `src/*.cpp` and `include/ecs/*.h`, while the game globs both extensions anywhere under `src/`. A header placed in `engine/include/` instead of `engine/include/ecs/` still compiles but never appears in the IDE tree.

---

## Architecture rule

> `engine/` must never `#include <raylib.h>`.

A component or system lives where its dependencies live. Anything touching a raylib type (`Color`, `Texture2D`, `Rectangle`) belongs in `game/src/`; anything that is pure ECS belongs in `engine/`.

The consequence is that `engine/` is a reusable ECS library — the renderer can be swapped, or the core lifted into an unrelated project, without touching it.
