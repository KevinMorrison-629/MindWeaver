cmake_minimum_required(VERSION 3.20)
project(MindWeaver_Impl CXX)

set(APPLICATION_NAME "MindWeaver")
set(PYBINDINGS_NAME "mindweaver_py")

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_executable(${APPLICATION_NAME}
    ${CMAKE_SOURCE_DIR}/mindweaver/main.cpp
    ${SOURCES}
)

target_link_libraries(${APPLICATION_NAME} PRIVATE
    pybind11::embed
    Python3::Python
    imgui
    imnodes
    glfw
    glad::glad
)

target_include_directories(${APPLICATION_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/mindweaver/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imnodes
    ${Python3_INCLUDE_DIRS}
)

target_compile_definitions(${APPLICATION_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# ---- Python binding module ----
pybind11_add_module(${PYBINDINGS_NAME} binding.cpp)

target_link_libraries(${PYBINDINGS_NAME} PRIVATE
    imgui
    imnodes
    glad::glad
    glfw
    Python3::Python
)

target_include_directories(${PYBINDINGS_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/imgui
    ${CMAKE_SOURCE_DIR}/extern/imnodes
    ${Python3_INCLUDE_DIRS}
)