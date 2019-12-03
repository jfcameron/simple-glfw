# © 2018 Joseph Cameron - All Rights Reserved

unset(dependencies)

if(NOT CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(dependencies
        "glfw" # some cleanup here gotta be done
    )
jfc_add_dependencies(${dependencies})
else()
    set(glfw_INCLUDE_DIR "/Users/josephcameron/Workspace/emscripten/system/include")

    project("glfw")
endif()

