cmake_minimum_required(VERSION 3.10)
project(hello_window VERSION 0.1.0)

set(glfw3_DIR "Dependencies/GLFW/lib/cmake/glfw3")

add_executable(hello_window openGLTest.cpp glad.c)

target_include_directories(hello_window PRIVATE Dependencies)

find_package(OpenGL REQUIRED)

find_package(glfw3 3.5 REQUIRED)

target_link_libraries(hello_window glfw)