# SiboarkaECS

A learning-focused **Entity Component System (ECS)** game engine written in C++20, with a tower defense demo built on top of it.

Built as a hands-on way to learn ECS architecture from scratch. No game engine framework — just a custom ECS core, [raylib](https://www.raylib.com/) for rendering, and CMake to tie it together.

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

---

## Requirements

- [CMake](https://cmake.org/) 3.16+
- A C++20 compiler — MSVC 2019+, GCC 10+, or Clang 10+
- Internet access on the **first build** (CMake downloads raylib 5.0 automatically via FetchContent)

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

> First build takes ~30 seconds longer while raylib downloads and compiles. Subsequent builds are fast.

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
│   └── Raylib.cmake          ← FetchContent download for raylib 5.0
├── engine/                   ← pure ECS library (zero raylib dependency)
│   ├── include/ecs/
│   │   ├── World.h           ← entity registry
│   │   ├── System.h          ← base system class
│   │   ├── EventBus.h        ← typed event system
│   │   ├── SceneManager.h    ← scene stack
│   │   ├── components/       ← engine-owned components
│   │   └── systems/          ← engine-owned systems (physics, collision)
│   └── src/
│       └── World.cpp
└── game/                     ← demo executable (uses raylib)
    ├── res/                  ← assets (sprites, etc.)
    └── src/
        ├── components/       ← game-specific components
        ├── systems/          ← game-specific systems
        ├── events/           ← game-specific events
        ├── scenes/           ← scene definitions
        └── main.cpp
```

---

## Architecture note

The engine and game are kept deliberately separate:

- `engine/` knows **nothing** about raylib — it is a reusable ECS library
- `game/` owns all rendering and game logic, and depends on both `engine` and `raylib`

This means you can swap the renderer or reuse the engine core in a completely different project.
