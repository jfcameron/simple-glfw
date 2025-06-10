// © Joseph Cameron - All Rights Reserved

#include <gdk/glfw_window.h>

#ifdef JFC_TARGET_PLATFORM_Emscripten
#include <emscripten/emscripten.h>
#endif

#include <cstdlib>
#include <vector>
#include <iostream>

using namespace gdk;

std::vector<std::shared_ptr<glfw_window>> windows;

void do_frame()
{
    static int a = 0;

    glfw_window::poll_events();

    (*windows.begin())->swap_buffers();
}

int main(int argc, char **argv)
{
    std::cout << "beginning demo...\n";

    for (int i(0); i < 1; ++i) windows.push_back(glfw_window::make("demo"));

#ifdef JFC_TARGET_PLATFORM_Emscripten
    emscripten_set_main_loop(do_frame, 0, 1);
#else
    while (windows.size()) for (decltype(windows)::size_type i = 0; i < windows.size();) {
        glfw_window::poll_events();

        if (!windows[i]->should_close())
        {
            windows[i]->swap_buffers(); 

            ++i;
        }
        else windows.erase(windows.begin() + i);
    }
#endif

    return EXIT_SUCCESS;
}

