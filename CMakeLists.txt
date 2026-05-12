cmake_minimum_required(VERSION 3.16)
# cmake_minimum_required: Tells CMake "I need at least this version to work correctly".
# 3.16 ships with Visual Studio 2019 and Ubuntu 20.04, so it's a safe baseline.

project(SiboarkaECS VERSION 1.0 LANGUAGES CXX)
# project(): Sets the project name and language.
# This also defines useful variables you can use anywhere in CMake:
#   ${PROJECT_NAME}         → "ECSEngine"
#   ${PROJECT_VERSION}      → "1.0"
#   ${PROJECT_SOURCE_DIR}   → the folder this file is in

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# REQUIRED = CMake will give a clear error if your compiler doesn't support C++20.

# Pull in the raylib download/build logic from a separate file.
# include() pastes the contents of that file here — it's just for organisation.
include(cmake/Raylib.cmake)
include(cmake/ImGui.cmake)

# add_subdirectory() tells CMake: "go read the CMakeLists.txt in that subfolder".
# Order matters: engine must be listed BEFORE game, because game depends on engine.
add_subdirectory(engine)
add_subdirectory(game)
