cmake_minimum_required(VERSION 3.0)

project(application)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
    src/main.cpp
    src/CodeGenerator.cpp
    src/sauvgarde.cpp
    lib/rlImGui/rlImGui.cpp
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
)

add_executable(application ${SRCS})

target_include_directories(application PRIVATE
    lib/imgui
    lib/raylib
    lib/rlImGui
)

if(WIN32)
    #conf WINDOWS (statique)
    target_link_libraries(application PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/raylib/libraylib.a
        opengl32
        gdi32
        winmm
    )
else()
    #conf LINUX / MacOS (dynamique)
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
