# Collect every file inside res/ so they appear in Visual Studio's Solution Explorer.
# CMake won't try to compile images/sounds — it only compiles .cpp files.
# CONFIGURE_DEPENDS: if you add or remove a file in res/, CMake will re-run automatically
# on the next build instead of you having to manually re-run CMake.
file(GLOB_RECURSE GAME_ASSETS CONFIGURE_DEPENDS "res/*")

add_executable(game

    # --- Entrypoint ---
    src/main.cpp

    # --- Scenes ---
    src/scenes/TestScene.cpp
    src/scenes/TestScene.h

    # --- Components (headers only — no .cpp needed, data structs) ---
    src/components/BulletComponent.h
    src/components/DeadComponent.h
    src/components/EnemyComponent.h
    src/components/HealthComponent.h
    src/components/LifetimeComponent.h
    src/components/RenderComponent.h
    src/components/SpriteComponent.h
    src/components/TargetComponent.h
    src/components/TowerComponent.h
    src/components/VFX/LaserComponent.h

    # --- Events ---
    src/events/DeathEvent.h
    src/events/PlaceTowerRequestEvent.cpp
    src/events/PlaceTowerRequestEvent.h
    src/events/RemoveTowerRequestEvent.cpp
    src/events/RemoveTowerRequestEvent.h
    src/events/ShotFiredEvent.h
    src/events/TowerPlacedEvent.h

    # --- Systems ---
    src/systems/CollisionSystem.cpp
    src/systems/CollisionSystem.h
    src/systems/DamageSystem.cpp
    src/systems/DamageSystem.h
    src/systems/DeathSystem.cpp
    src/systems/DeathSystem.h
    src/systems/InputSystem.h
    src/systems/PathFollowingSystem.cpp
    src/systems/PathFollowingSystem.h
    src/systems/PlacementSystem.cpp
    src/systems/PlacementSystem.h
    src/systems/RenderSystem.h
    src/systems/ShootingSystem.cpp
    src/systems/ShootingSystem.h
    src/systems/VFXSystem.cpp
    src/systems/VFXSystem.h
    src/systems/WaveSpawnerSystem.cpp
    src/systems/WaveSpawnerSystem.h

    # --- Assets (listed for VS visibility only, not compiled) ---
    ${GAME_ASSETS}
)

# Mirror the real folder structure inside VS Solution Explorer.
# TREE mode walks the directory and recreates it as filters in VS.
# Without this, all files appear in a flat list at the top level.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES
    src/main.cpp
    src/scenes/TestScene.cpp
    src/scenes/TestScene.h
    src/components/BulletComponent.h
    src/components/DeadComponent.h
    src/components/EnemyComponent.h
    src/components/HealthComponent.h
    src/components/LifetimeComponent.h
    src/components/RenderComponent.h
    src/components/SpriteComponent.h
    src/components/TargetComponent.h
    src/components/TowerComponent.h
    src/components/VFX/LaserComponent.h
    src/events/DeathEvent.h
    src/events/PlaceTowerRequestEvent.cpp
    src/events/PlaceTowerRequestEvent.h
    src/events/RemoveTowerRequestEvent.cpp
    src/events/RemoveTowerRequestEvent.h
    src/events/ShotFiredEvent.h
    src/events/TowerPlacedEvent.h
    src/systems/CollisionSystem.cpp
    src/systems/CollisionSystem.h
    src/systems/DamageSystem.cpp
    src/systems/DamageSystem.h
    src/systems/DeathSystem.cpp
    src/systems/DeathSystem.h
    src/systems/InputSystem.h
    src/systems/PathFollowingSystem.cpp
    src/systems/PathFollowingSystem.h
    src/systems/PlacementSystem.cpp
    src/systems/PlacementSystem.h
    src/systems/RenderSystem.h
    src/systems/ShootingSystem.cpp
    src/systems/ShootingSystem.h
    src/systems/VFXSystem.cpp
    src/systems/VFXSystem.h
    src/systems/WaveSpawnerSystem.cpp
    src/systems/WaveSpawnerSystem.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_ASSETS})

# target_link_libraries: Tells CMake what other libraries game needs.
#
#   engine  — our ECS static library (defined in engine/CMakeLists.txt)
#             Because engine used PUBLIC in its target_include_directories,
#             game automatically gets engine/include/ on its include path.
#             No extra include setup needed here for <ecs/World.h> etc.
#
#   raylib  — the renderer/input library (made available by cmake/Raylib.cmake)
#             game/ owns raylib. The engine knows nothing about it.
#
# PRIVATE means: game needs these, but nothing links to game so it doesn't matter.
# (An executable is the end of the dependency chain — nothing links to an .exe.)
target_link_libraries(game PRIVATE
    engine
    raylib
)

# target_include_directories for game's OWN source folder.
# This lets files inside game/src/ write:
#   #include "components/RenderComponent.h"
#   #include "systems/InputSystem.h"
# instead of spelling out the full path every time.
# PRIVATE: only game itself uses this path (correct — it's game-internal).
target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# After game.exe is built, copy the entire res/ folder next to it.
# $<TARGET_FILE_DIR:game> resolves to the folder where game.exe lands,
# e.g. build/game/Debug/ — so the game can load files with:
#   LoadTexture("res/textures/player.png")
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:game>/res
    COMMENT "Copying res/ to build output directory"
)
