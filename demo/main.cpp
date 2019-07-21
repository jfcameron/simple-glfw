// © 2019 Joseph Cameron - All Rights Reserved

#include <cstdlib>

#include <gdk/window.h>

using namespace gdk;

int main(int argc, char **argv)
{
    SimpleGLFWWindow window("name");

    while(!window.shouldClose())
    {
        window.pollEvents();

        window.SwapBuffer(); 
    }

    return EXIT_SUCCESS;
}

