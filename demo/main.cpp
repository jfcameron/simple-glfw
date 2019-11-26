// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <vector>
#include <iostream>

#include <jfc/glfw_window.h>

#include <GLFW/glfw3.h>

#ifdef JFC_TARGET_PLATFORM_Emscripten
#include <emscripten/emscripten.h>
#endif

using namespace gdk;

std::vector<glfw_window> windows;

void do_frame() // This all works but its a real nasty hack. Im confused about this "Window" abstraction. it encapsulates window realted stuff, primarily init and cleanup but then the user is expected to circumvent it to access glfw. hmm. so how do I hide this emcsript difference?
{
    static int a = 0;

    std::cout << "blar\n";
    
    //glClear(GL_COLOR_BUFFER_BIT);
    //glfwSwapBuffers(window);
    //windows.begin()->pollEvents();

    glfwPollEvents();

    windows.begin()->swapBuffer();
}

int main(int argc, char **argv)
{
    for (int i = 0; i < 1; ++i) windows.push_back(glfw_window("demo"));

#ifdef JFC_TARGET_PLATFORM_Emscripten
    emscripten_set_main_loop(do_frame, 0, 1);
#endif

    while (windows.size()) for (decltype(windows)::size_type i = 0; i < windows.size();)
    {
        glfwPollEvents();

        if (!windows[i].shouldClose())
        {
            windows[i].swapBuffer(); 

            ++i;
        }
        else windows.erase(windows.begin() + i);
    }

    return EXIT_SUCCESS;
}

