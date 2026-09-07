cmake_minimum_required( VERSION 3.6 )

project(keklib)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory("deps/glfw")

include_directories("${GLFW_SOURCE_DIR}/deps")
set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

set(${PROJECT_NAME}-SRC "src/main.cpp")

add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}-SRC} ${GLAD_GL})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
