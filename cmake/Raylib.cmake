# FetchContent is a CMake built-in module for downloading dependencies at configure time.
# Think of it like npm (JavaScript) or pip (Python), but for C++ CMake projects.
include(FetchContent)

FetchContent_Declare(
    raylib                          # The name we use to refer to this dependency in CMake
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0              # ALWAYS pin a specific version tag.
                                    # Never use "main" — it changes and will break your build.
    GIT_SHALLOW    TRUE             # Download only the tagged commit, not the full git history.
                                    # This makes the first download much faster.
)

# Turn off raylib's own built-in examples and games.
# Without these lines, CMake would also try to compile hundreds of extra .c files.
# CACHE BOOL "" FORCE means: set this option and override any user setting.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

# CMake 4.0 removed compatibility with cmake_minimum_required values older than 3.5.
# Raylib 5.0's own CMakeLists.txt uses an older value, which triggers an error.
# This tells CMake to treat raylib's minimum as 3.5 and proceed anyway.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# FetchContent_MakeAvailable() does the actual work:
#   1. First run: downloads raylib source from GitHub into build/_deps/raylib-src/
#   2. Configures and compiles it as part of your build
#   3. After first run: uses the cached copy (fast)
# After this call, the "raylib" CMake target exists and can be used in target_link_libraries().
FetchContent_MakeAvailable(raylib)
