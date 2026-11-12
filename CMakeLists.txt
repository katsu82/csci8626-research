cmake_minimum_required(VERSION 3.11)
project(hello_window VERSION 0.1.0)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

set(glfw3_DIR "Dependencies/GLFW/lib/cmake/glfw3")

add_executable(hello_window openGLTest.cpp glad.c particle.cpp Vector3.cpp)

target_include_directories(hello_window PRIVATE Dependencies)

find_package(OpenGL REQUIRED)

find_package(glfw3 3.5 REQUIRED)

target_link_libraries(hello_window glfw glm::glm)