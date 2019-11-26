// © 2019 Joseph Cameron - All Rights Reserved

#include <string>

#include <jfc/catch.hpp>
#include <jfc/types.h>

#include <jfc/glfw_window.h>

using namespace gdk;

TEST_CASE("glfw_window tests", "[glfw_window]")
{
    constexpr auto name("Cool Window");

    glfw_window window(name);      

    SECTION("constructs valid window")
    {
        REQUIRE(window.getPointerToGLFWWindow());

        REQUIRE(window.getName() == name);
    }

    SECTION("move semantics work")
    {
        glfw_window a(name);      
       
        auto b = std::move(a);

        REQUIRE(b.getName() == name);
    }
}

