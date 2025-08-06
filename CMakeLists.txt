# experimental enable std module -> update uuid according to cmake version: https://github.com/Kitware/CMake/blob/master/Help/dev/experimental.rst
if(WIN32) 
    cmake_minimum_required(VERSION 3.31.6 FATAL_ERROR)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
else()
    cmake_minimum_required(VERSION 4.0.3 FATAL_ERROR)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
endif()

# ----------------------------------
# external libraries
# ----------------------------------

# glfw
add_subdirectory(external/glfw)

# glad
add_subdirectory(external/glad/cmake)
glad_add_library(glad_gl_core_46 STATIC API gl:core=4.6)

# imgui
add_library(imgui 
    STATIC
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui
    PUBLIC
        external/imgui)
target_link_libraries(imgui 
    PUBLIC
        glfw)
target_compile_definitions(imgui 
    PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD
        IMGUI_DEFINE_MATH_OPERATORS)

# glm
add_library(glm 
    STATIC)
target_sources(glm
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        external/glm/glm/glm.cppm)
target_include_directories(glm
    PRIVATE
        external/glm)
target_compile_features(glm
    PRIVATE 
        cxx_std_23)
target_compile_definitions(glm 
    PUBLIC
        GLM_ENABLE_EXPERIMENTAL)

# ----------------------------------
# Miezi project
# ----------------------------------

# create project
project(Miezi 
    VERSION 0.1.0
    DESCRIPTION "Miezi Graphics Library"
    LANGUAGES C CXX)

if(NOT WIN32) 
    # enable std module
    set(CMAKE_CXX_MODULE_STD ON)
endif()

# executable
add_executable(Miezi)

# get module files
file(GLOB_RECURSE MODULES 
    CONFIGURE_DEPENDS 
        src/*.cppm)

# set files for target
target_sources(Miezi 
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ${MODULES}
    PRIVATE 
        src/main.cpp)

# set cxx standard
target_compile_features(Miezi
    PRIVATE 
        cxx_std_23
    INTERFACE
        cxx_std_20)

# include directories
target_include_directories(Miezi 
    PRIVATE 
        src
    PUBLIC
        include)

# linked libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(Miezi 
    PUBLIC
        glm
        imgui
    PRIVATE
        Threads::Threads
        OpenGL::GL 
        OpenGL::GLU
        glad_gl_core_46)

# set OS define
if(WIN32) 
    set(OS_DEFINE MZ_OS_WINDOWS)
elseif(UNIX AND NOT APPLE)	
    set(OS_DEFINE MZ_OS_LINUX)
else()
    message(FATAL_ERROR "Unsupported OS, CMake will exit.")
endif()

# defines
target_compile_definitions(Miezi 
    PRIVATE 
        ${OS_DEFINE})

# compile options
target_compile_options(Miezi 
    PRIVATE 
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>)