# © Joseph Cameron - All Rights Reserved

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/jfc-cmake/jfclib.cmake")

option(JFC_BUILD_DEMO "Build the demo" ON)
option(JFC_BUILD_DOCS "Build documentation" ON)
option(JFC_BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(extern)

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(platform_dependencies "GLEW")
endif()

jfc_project(library
    NAME "simpleglfw"
    VERSION 0.5
    DESCRIPTION "raii wrapper for glfw window, build scripts for crossplat deployment"
    C++_STANDARD 17
    C_STANDARD 90
    TYPE STATIC

    PUBLIC_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/glfw/include
        ${glfw_INCLUDE_DIR}

    PRIVATE_INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include

    LIBRARIES
        ${glfw_LIBRARIES}

    DEPENDENCIES
        "glfw"
        ${platform_dependencies}

    SOURCE_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/glfw/src/glfw_window.cpp 
)

if (JFC_BUILD_DEMO)
    add_subdirectory(demo)
endif()

if (JFC_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JFC_BUILD_DOCS)
    add_subdirectory(docs)
endif()

