cmake_minimum_required(VERSION 3.16)

# set project name (thus .exe name)
# make sure to refer this with ${PROJECT_NAME} everywhere else
project(menv)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" main.cpp)
# This will not recursively find .cpp in subfolders, so
# after you have imgui or other UI component, use
file(GLOB_RECURSE SOURCES "src/*.cpp" "imgui/*.cpp" "imgui/backend/*.cpp" "wasp/*.anim" main.cpp)


# Add header files
set(
    HEADERS
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
    # also, add this after you have imgui
    include/imgui
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)

# Move assets to correct path relative to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/Debug/shaders"
)

add_dependencies(${PROJECT_NAME} CopyShaders)