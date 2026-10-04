cmake_minimum_required(VERSION 3.10)

project(application)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
    src/main.cpp
    src/CodeGenerator.cpp
    src/Sauvgarde.cpp
    src/Managers/SceneManager.cpp
    src/Managers/ShaderManager.cpp
    src/Noeuds/SceneNode.cpp
    src/Noeuds/SceneNode_Cube.cpp
    src/Noeuds/SceneNode_Camera.cpp
    src/Noeuds/SceneNode_Camera2D.cpp
    src/Noeuds/SceneNode_Sphere.cpp
    src/Noeuds/SceneNode_Cylindre.cpp
    src/Noeuds/SceneNode_Cone.cpp
    src/Noeuds/SceneNode_Plan.cpp
    src/Noeuds/LightNode.cpp
    src/Noeuds/DirectionalLightNode.cpp
    src/Interface.cpp
    src/tinyfiledialogs.c
    lib/rlImGui/rlImGui.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/ImGuizmo.cpp
    lib/imgui/ImSequencer.cpp
)

add_executable(application ${SRCS})

target_include_directories(application PRIVATE
    lib/imgui
    lib/raylib
    lib/rlImGui
)

if(WIN32)
    #conf WINDOWS (statique)
    # Force l'inclusion statique des bibliothèques standards C et C++
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    target_link_libraries(application PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/raylib/libraylib.a
        opengl32
        gdi32
        winmm
    )
elseif(APPLE)
    # macOS (raylib installed with Homebrew)
    # Homebrew lives in /opt/homebrew (Apple Silicon) or /usr/local (Intel);
    # add its prefix so find_package() can locate raylib's CMake config.
    execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(HOMEBREW_PREFIX)
        list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
    endif()
 
    find_package(raylib REQUIRED)
 
    target_link_libraries(application PRIVATE raylib)
else()
    #conf LINUX (dynamique)
    target_link_libraries(application PRIVATE
        raylib
        GL
        m
        pthread
        dl
        rt
        X11
    )
endif()