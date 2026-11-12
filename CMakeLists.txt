cmake_minimum_required(VERSION 3.10)

project(application)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
    #src/main.cpp
    src/CodeGenerator.cpp
    src/Sauvgarde.cpp
    src/Managers/SceneManager.cpp
    src/Managers/ShaderManager.cpp
    src/Managers/MaterialManager.cpp
    src/Managers/ModelManager.cpp
    src/Noeuds/SceneNode.cpp
    src/Noeuds/SceneNode_Cube.cpp
    src/Noeuds/SceneNode_Camera.cpp
    src/Noeuds/SceneNode_Camera2D.cpp
    src/Noeuds/SceneNode_Sphere.cpp
    src/Noeuds/SceneNode_Cylindre.cpp
    src/Noeuds/SceneNode_Cone.cpp
    src/Noeuds/SceneNode_Plan.cpp
    src/Noeuds/SceneNode_Model.cpp
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

add_executable(application src/main.cpp ${SRCS})
set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
target_compile_options(application PRIVATE -g -O0)# changer en 02 ? 

# Warnings uniquement sur TES fichiers (src/), pas sur lib/
set(MES_SRCS ${SRCS})
list(FILTER MES_SRCS INCLUDE REGEX "^src/")
set_source_files_properties(${MES_SRCS} PROPERTIES
    COMPILE_OPTIONS "-Wall;-Wextra;-Werror=return-type;-Wno-missing-field-initializers;-Wno-unused-parameter"
)
# tinyfiledialogs.c est dans src/ mais n'est pas à toi : on coupe ses warnings
set_source_files_properties(src/tinyfiledialogs.c PROPERTIES COMPILE_OPTIONS "-w")
target_include_directories(application SYSTEM PRIVATE
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

# ==============================================================================
#                             SECTION TESTS (CATCH2)
# ==============================================================================

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(TEST_SOURCES
    tests/Test_Noeuds.cpp
    tests/Test_Sauvegarde.cpp
    tests/Test_Exportation.cpp
)

add_executable(RlenderTests ${TEST_SOURCES} ${SRCS})

target_include_directories(RlenderTests SYSTEM PRIVATE
    lib/imgui
    lib/raylib
    lib/rlImGui
)

#lien avec Catch2
target_link_libraries(RlenderTests PRIVATE Catch2::Catch2WithMain)

if(WIN32)
    target_link_libraries(RlenderTests PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/raylib/libraylib.a
        opengl32 gdi32 winmm
    )
elseif(APPLE)
    target_link_libraries(RlenderTests PRIVATE raylib)
else()
    target_link_libraries(RlenderTests PRIVATE raylib GL m pthread dl rt X11)
endif()

# --- ACTIVATION DE LA DÉTECTION DES FUITES DE MÉMOIRE (ASan) ---
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(RlenderTests PRIVATE -fsanitize=address -g -fno-omit-frame-pointer)
    target_link_options(RlenderTests PRIVATE -fsanitize=address)
endif()