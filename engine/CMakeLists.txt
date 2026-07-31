# GLOB_RECURSE + CONFIGURE_DEPENDS: CMake rescans these patterns on every build
# and re-runs configure automatically when a file is added/removed/renamed.
# No more manually editing this list for every new file — see docs/ for the tradeoffs
# (mainly: a stray file silently joins the build instead of via an explicit, reviewable diff).
#
# NOT globbed under game/src/ — these depend on raylib and live there instead:
#   RenderComponent.h, RenderSystem.h, InputSystem.h
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "include/ecs/*.h")

add_library(engine STATIC
    ${ENGINE_SRC}
    ${ENGINE_HEADERS}
)

# source_group mirrors the real folder tree inside VS/Rider's project view.
# Each TREE root only accepts files that live under it, so src/ and include/ stay separate calls.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "include" FILES ${ENGINE_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${ENGINE_SRC})

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
