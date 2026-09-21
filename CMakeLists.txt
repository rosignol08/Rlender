cmake_minimum_required(VERSION 3.5)

project(application)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
    src/main.cpp
    src/CodeGenerator.cpp
    src/Sauvgarde.cpp
    src/SceneManager.cpp
    src/interface.cpp
    src/tinyfiledialogs.c
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
    # Windows (static raylib shipped in lib/raylib)
    target_include_directories(application PRIVATE lib/raylib)
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
 
    # The imported target already carries raylib's include path and its
    # frameworks (OpenGL, Cocoa, IOKit...), so nothing else is needed here.
    # GL / rt / X11 do not exist on macOS.
    target_link_libraries(application PRIVATE raylib)
else()
    # Linux (dynamic)
    target_include_directories(application PRIVATE lib/raylib)
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

