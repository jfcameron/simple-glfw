// © 2018-2019 Joseph Cameron - All Rights Reserved

#include <simpleglfw/buildinfo.h>

#include <jfc/window.h>
#include <jfc/window_glfw.h>

#ifdef JFC_TARGET_PLATFORM_Emscripten
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#elif defined JFC_TARGET_PLATFORM_Linux || defined JFC_TARGET_PLATFORM_Windows
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>

static constexpr char TAG[] = "SimpleGLFWWindow";

// Oddity that only affects mac. Should look through glfw git log / release notes to see if addressed
static constexpr int WINDOWS_SIZE_SCALE =
#ifdef JFC_TARGET_PLATFORM_Darwin
2
#else
1
#endif
;

namespace gdk
{
    SimpleGLFWWindow::SimpleGLFWWindow(const std::string_view aName, const window_size_type &aScreenSize)
    : m_pGLFWWindow([&]()
        {        
            glfwSetErrorCallback([](int, const char *msg)
            {
                throw std::runtime_error(std::string(TAG).append("/").append(msg));
            });
        
            if (!glfwInit()) throw std::runtime_error(std::string(TAG).append("/glfwInit failed"));
        
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, true);

            if (GLFWwindow *const pWindow = glfwCreateWindow(aScreenSize.first, aScreenSize.second, aName.data(), nullptr, nullptr))
            {
                glfwMakeContextCurrent(pWindow);

                glfwSwapInterval(-1); //Vsync controller. if not called, the interval is platform dependent. 0 is off. negative values allow for swapping even if the backbuffer arrives late (vendor extension dependent).

                glClearColor(0, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                glfwSwapBuffers(pWindow); //Must be called once to see the buffer we cleared

                glfwPollEvents(); //Must be called once for window to render

                glfwSetWindowUserPointer(pWindow, static_cast<void *>(this));
                    
                glfwSetWindowSizeCallback(pWindow, [](GLFWwindow *const pCurrentGLFWwindow, int aX, int aY)
                {
                    if (SimpleGLFWWindow *const pCurrentWrapper = static_cast<SimpleGLFWWindow *const>(glfwGetWindowUserPointer(pCurrentGLFWwindow)))
                    {
                        pCurrentWrapper->m_WindowSize.first =  aX * WINDOWS_SIZE_SCALE;
                        pCurrentWrapper->m_WindowSize.second = aY * WINDOWS_SIZE_SCALE;
                    }
                    else throw std::runtime_error(std::string(TAG).append("/wrapper associated with current glfw window instance is null"));
                });

                glfwSetWindowCloseCallback(pWindow, [](GLFWwindow *const pCurrentWindow)
                {
                    glfwSetWindowShouldClose(pCurrentWindow, GLFW_TRUE);
                });

#if defined JFC_TARGET_PLATFORM_Linux || defined JFC_TARGET_PLATFORM_Windows
                if (GLenum err = glewInit() != GLEW_OK) 
                {
                    std::stringstream ss;

                    ss << TAG << "/glewinit failed: " << glewGetErrorString(err);
                
                    throw std::runtime_error(ss.str()); 
                }
#endif

                return pWindow;
            }
            else throw std::runtime_error(std::string(TAG).append("/glfwCreateWindow failed. Can the environment provide a GLES2.0/WebGL1.0 context?"));
        }()
        ,[](GLFWwindow *const ptr)
        {
            glfwDestroyWindow(ptr);
        })
        , m_Name(aName)
        , m_WindowSize({aScreenSize.first * WINDOWS_SIZE_SCALE, aScreenSize.second * WINDOWS_SIZE_SCALE})
    {}

    GLFWwindow *const SimpleGLFWWindow::getPointerToGLFWWindow()
    {
        return m_pGLFWWindow.get();
    }

    void SimpleGLFWWindow::swapBuffer()
    {
        glfwSwapBuffers(m_pGLFWWindow.get());
    }

    /*double SimpleGLFWWindow::GetTime()
    {
        return glfwGetTime();
    }*/

    SimpleGLFWWindow::window_size_type SimpleGLFWWindow::getWindowSize() const       
    {
        return m_WindowSize;
    }

    bool SimpleGLFWWindow::getMouseButton(const int aButton) const
    {
        return glfwGetMouseButton(m_pGLFWWindow.get(), aButton);
    }

    SimpleGLFWWindow::cursor_position_type SimpleGLFWWindow::getCursorPos() const
    {
        double x,y;
        glfwGetCursorPos(m_pGLFWWindow.get(), &x, &y);

        return cursor_position_type(static_cast<cursor_position_type::first_type>(x), static_cast<cursor_position_type::second_type>(y));
    }

    bool SimpleGLFWWindow::getKey(const int aKeyCode) const
    {
        return static_cast<bool>(glfwGetKey(m_pGLFWWindow.get(), aKeyCode));
    }

    void SimpleGLFWWindow::pollEvents()
    {
        //glfwMakeContextCurrent(m_pGLFWWindow.get()); //TODO: call makecontextcurrent or soemthing

        glfwPollEvents();
    }

    std::string_view SimpleGLFWWindow::getName() const
    {
        return m_Name;
    }

    bool SimpleGLFWWindow::shouldClose() const
    {
        return glfwWindowShouldClose(m_pGLFWWindow.get());
    }
}

