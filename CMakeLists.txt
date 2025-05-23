cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "MindWeaver")
project(${PROJECT_NAME} LANGUAGES C CXX)

# Tell CMake to use vcpkg manifest
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "Vcpkg toolchain file")

# Compile settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(pybind11 CONFIG REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


# Add ImGui and ImNodes external submodules
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cmake-imgui ${CMAKE_BINARY_DIR}/external/imgui)
set(IMNODES_IMGUI_TARGET_NAME "imgui") # need to set the imgui target
add_subdirectory(external/imnodes)

# Add your src
add_subdirectory(mindweaver)