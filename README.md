# SiboarkaECS

A learning-focused **Entity Component System (ECS)** game engine written in C++20, with a tower defense demo and an in-engine editor built on top of it.

Built as a hands-on way to learn ECS architecture from scratch. No game engine framework — just a custom ECS core, [raylib](https://www.raylib.com/) for rendering, [Dear ImGui](https://github.com/ocornut/imgui) for the editor UI, and CMake to tie it together.

> No code has been written by Claude. Claude is used as a teacher/guide.

---

## What's inside

### Engine (`engine/`)
A self-contained ECS library with **no raylib dependency**:

| Feature | Description |
|---|---|
| `World` | Entity registry — create, destroy, attach/query components |
| `System` | Base class with a pure virtual `update()` |
| `EventBus` | Typed publish/subscribe event system |
| `SceneManager` | Stack-based scene switching |
| `Logger` | Simple logging utility |
| Physics | `CollisionSystem`, `GravitySystem`, `MovementSystem`, `PhysicsResponseSystem` |
| Components | `TransformComponent`, `ColliderComponent`, `GravityComponent`, `TagComponent`, and more |

### Game (`game/`)
A tower defense prototype that runs on the engine:

| System | Role |
|---|---|
| `WaveSpawnerSystem` | Spawns enemy waves on a timer |
| `PathFollowingSystem` | Moves enemies along a predefined path |
| `PlacementSystem` | Handles tower placement input |
| `ShootingSystem` | Towers detect and shoot at enemies |
| `DamageSystem` | Resolves projectile hits and health |
| `InputSystem` | Player input handling |
| `RenderSystem` | Draws everything via raylib |
| `VFXSystem` | Visual effects (laser beams, hit flashes) |

### Editor
An Unreal-inspired in-engine editor built with Dear ImGui + rlImGui:

| Panel | Role |
|---|---|
| **Viewport** | Game renders to a `RenderTexture2D` displayed as a resizable panel |
| **Hierarchy** | Lists all tagged entities; click to select |
| **Inspector** | Live-edits components (`TransformComponent`, `HealthComponent`, etc.) on the selected entity |
| **Play/Pause** | Freezes game time while keeping the editor interactive |

Mouse input is scoped to the viewport content area — clicking editor panels never leaks into the game.

---

## Requirements

- [CMake](https://cmake.org/) 3.16+
- A C++20 compiler — MSVC 2019+, GCC 10+, or Clang 10+
- Internet access on the **first build** (CMake downloads raylib, Dear ImGui, and rlImGui automatically via FetchContent)

---

## Build & Run

### Command line

```bash
git clone https://github.com/echo-matt/SiboarkaECS.git
cd SiboarkaECS

mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

Then run:

```bash
./game/Debug/game.exe
```

> First build takes ~30–60 seconds while raylib, ImGui, and rlImGui download and compile. Subsequent builds are fast.

### Visual Studio

1. Generate the solution:
   ```bash
   mkdir build && cd build && cmake ..
   ```
2. Open `build/SiboarkaECS.sln` in Visual Studio
3. Right-click `game` in Solution Explorer → **Set as Startup Project**
4. Press **F5**

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
│   │   ├── World.h           ← entity registry + InputState
│   │   ├── System.h          ← base system class
│   │   ├── EventBus.h        ← typed event system
│   │   ├── SceneManager.h    ← scene stack
│   │   ├── components/       ← engine-owned components
│   │   └── systems/          ← engine-owned systems (physics, collision)
│   └── src/
│       └── World.cpp
└── game/                     ← demo executable (uses raylib + imgui)
    ├── res/                  ← assets (sprites, etc.)
    └── src/
        ├── components/       ← game-specific components
        ├── systems/          ← game-specific systems
        ├── events/           ← game-specific events
        ├── scenes/           ← scene definitions
        └── main.cpp          ← game loop + editor panels
```

---

## Architecture note

The engine and game are kept deliberately separate:

- `engine/` knows **nothing** about raylib or ImGui — it is a reusable ECS library
- `game/` owns all rendering, game logic, and editor UI, and depends on `engine`, `raylib`, and `imgui`

This means you can swap the renderer or reuse the engine core in a completely different project.
