add_library(engine STATIC

    # --- Source files (.cpp) — these are compiled into the library ---
    src/World.cpp

    # --- Core (ECS infrastructure) ---
    include/ecs/EventBus.h
    include/ecs/Logger.h
    include/ecs/Scene.h
    include/ecs/SceneManager.h
    include/ecs/System.h
    include/ecs/Types.h
    include/ecs/World.h

    # --- Components ---
    include/ecs/components/ColliderComponent.h
    include/ecs/components/ControllableComponent.h
    include/ecs/components/GravityComponent.h
    include/ecs/components/GroundedComponent.h
    include/ecs/components/ProjectileComponent.h
    include/ecs/components/StaticComponent.h
    include/ecs/components/TagComponent.h
    include/ecs/components/TransformComponent.h

    # --- Systems ---
    include/ecs/systems/CollisionEventSystem.h
    include/ecs/systems/GravitySystem.h
    include/ecs/systems/MovementSystem.h
    include/ecs/systems/PhysicsResponseSystem.h

    # --- Events ---
    include/ecs/events/CollisionDirection.h

    # NOT listed here — these depend on raylib and live in game/src/:
    #   RenderComponent.h, RenderSystem.h, InputSystem.h
)

# source_group mirrors the include/ecs/ folder tree inside VS Solution Explorer.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "include" FILES
    include/ecs/EventBus.h
    include/ecs/Logger.h
    include/ecs/Scene.h
    include/ecs/SceneManager.h
    include/ecs/System.h
    include/ecs/Types.h
    include/ecs/World.h
    include/ecs/components/ColliderComponent.h
    include/ecs/components/ControllableComponent.h
    include/ecs/components/GravityComponent.h
    include/ecs/components/GroundedComponent.h
    include/ecs/components/ProjectileComponent.h
    include/ecs/components/StaticComponent.h
    include/ecs/components/TagComponent.h
    include/ecs/components/TransformComponent.h
    include/ecs/systems/CollisionEventSystem.h
    include/ecs/systems/GravitySystem.h
    include/ecs/systems/MovementSystem.h
    include/ecs/systems/PhysicsResponseSystem.h
    include/ecs/events/CollisionDirection.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES
    src/World.cpp
)

# target_include_directories: Tells CMake (and the compiler) where to look for #include files.
#
# PUBLIC means this path is shared with anyone who links to `engine`.
# So when `game` does:   target_link_libraries(game PRIVATE engine)
# ... `game` can also write:  #include <ecs/World.h>
# without having to specify the path again. CMake propagates it automatically.
#
# ${CMAKE_CURRENT_SOURCE_DIR} = the directory this CMakeLists.txt is in = engine/
# So we're exposing: engine/include/ as a public include path.
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# target_compile_features: Ensures C++20 is used when compiling engine,
# AND is required from anyone linking to engine (PUBLIC propagation again).
target_compile_features(engine PUBLIC cxx_std_20)

# engine does NOT link raylib — it has no raylib dependency.
# Only game/ links raylib, because only game/ knows about rendering and input.
