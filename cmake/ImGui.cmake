# ImGui + rlImGui — fetched and built as static libraries.
# We use FetchContent_Populate instead of FetchContent_MakeAvailable so we
# can create our own targets manually, avoiding conflicts with any CMakeLists
# that the upstream repos might ship.
include(FetchContent)

# ---------------------------------------------------------------------------
# Dear ImGui
# ---------------------------------------------------------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.92.8     # pinned stable release
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# ImGui ships no CMakeLists, so we build the target ourselves.
# These are the core files; imgui_demo.cpp is optional but useful for reference.
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)
# PUBLIC so that anything linking imgui automatically gets the include path.
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# ---------------------------------------------------------------------------
# rlImGui  (raylib <-> Dear ImGui backend)
# ---------------------------------------------------------------------------
FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG        552a3ad44f130d05055e00e11e1c3f7cf45e5657  # pinned to commit compatible with imgui v1.92.8
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(rlimgui)
if(NOT rlimgui_POPULATED)
    FetchContent_Populate(rlimgui)
endif()

add_library(rlimgui STATIC
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)
# PUBLIC: game code only needs to #include "rlImGui.h", no manual path required.
target_include_directories(rlimgui PUBLIC ${rlimgui_SOURCE_DIR})
# rlImGui delegates rendering to raylib and UI to imgui — link both.
target_link_libraries(rlimgui PUBLIC imgui raylib)
# raylib 5.0 uses IsTextureReady(); the function was renamed to IsTextureValid() in 5.5.
target_compile_definitions(rlimgui PRIVATE IsTextureValid=IsTextureReady)
