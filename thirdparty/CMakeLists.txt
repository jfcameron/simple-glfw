# © 2018 Joseph Cameron - All Rights Reserved

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(dependencies
        "glfw"
    )
jfc_add_dependencies(${dependencies})
else()
    set(glfw_INCLUDE_DIR "/Users/josephcameron/Workspace/emscripten/system/include")

    project("glfw")

    add_custom_target(glfw)
endif()

