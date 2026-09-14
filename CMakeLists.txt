cmake_minimum_required(VERSION 3.6)

project(keklib)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory("deps/glfw")

set(${PROJECT_NAME}-SRC
        src/keklib.cpp)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}-SRC})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${OPENGL_INCLUDE_DIRS}
        "${GLFW_SOURCE_DIR}/deps"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ${OPENGL_LIBRARIES}
        glfw
)