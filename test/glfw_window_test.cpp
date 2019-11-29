// © 2019 Joseph Cameron - All Rights Reserved

#include <string>
#include <vector>

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

        REQUIRE(!window.shouldClose());
    }

    SECTION("can set cursor graphic by system provided graphics")
    {
        window.setCursor(glfw_window::standard_cursor_graphic::ibeam);
    }

    SECTION("can change cursor graphic by user defined rgba32bit 16x16 bitmap")
    {
        glfw_window::cursor_image_type icon;

        for (int i(0); i < (16 * 16 * 4); ++i) icon[i] = static_cast<std::byte>(0xff);

        window.setCursor(icon);
    }

    SECTION("can set icon graphics")
    {
        glfw_window::icon_image_collection_type icons;

        glfw_window::icon_image_type icon;

        icon.width_pixels = 32;
        icon.height_pixels = 32;

        icon.data_rgba32 = decltype(icon.data_rgba32)(
            icon.width_pixels * icon.height_pixels * glfw_window::icon_image_type::CHANNEL_COUNT, 
            static_cast<std::byte>(0xff));

        icons.push_back(icon);
    }

    SECTION("move semantics work")
    {
        glfw_window a(name);      
       
        auto b = std::move(a);

        REQUIRE(b.getName() == name);
    }
}

