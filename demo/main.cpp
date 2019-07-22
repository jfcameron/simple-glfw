// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <vector>

#include <jfc/window.h>

using namespace gdk;

int main(int argc, char **argv)
{
    std::vector<SimpleGLFWWindow> windows;

    for (int i = 0; i < 3; ++i) windows.push_back(SimpleGLFWWindow("demo"));

    while (windows.size()) for (decltype(windows)::size_type i = 0; i < windows.size();)
    {
        if (!windows[i].shouldClose())
        {
            windows[i].pollEvents();
            windows[i].swapBuffer(); 

            ++i;
        }
        else windows.erase(windows.begin() + i);
    }

    return EXIT_SUCCESS;
}

