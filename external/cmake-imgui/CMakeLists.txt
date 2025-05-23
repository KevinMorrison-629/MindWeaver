cmake_minimum_required(VERSION 3.10)
project(imgui_submodule NONE)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../imgui)

file(GLOB IMGUI_SRC
    "${IMGUI_SOURCE_DIR}/*.cpp"
)
set(IMGUI_BACKEND_SRC
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(IMGUI_BACKEND_HEADERS
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.h
)

add_library(imgui STATIC 
    ${IMGUI_SRC}
    ${IMGUI_BACKEND_SRC}
    ${IMGUI_BACKEND_HEADERS}
)

target_include_directories(imgui PUBLIC
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_SOURCE_DIR}/backends
)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

