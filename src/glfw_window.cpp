// © 2018-2019 Joseph Cameron - All Rights Reserved

#include <simpleglfw/buildinfo.h>

#include <jfc/glfw_window.h>

#ifdef JFC_TARGET_PLATFORM_Emscripten
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES2
#elif defined JFC_TARGET_PLATFORM_Linux || defined JFC_TARGET_PLATFORM_Windows
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>

static constexpr char TAG[] = "glfw_window";

// Oddity that only affects mac. Should look through glfw git log / release notes to see if addressed
static constexpr int WINDOWS_SIZE_SCALE =
#ifdef JFC_TARGET_PLATFORM_Darwin
1
#else
1
#endif
;

using namespace gdk;

glfw_window::glfw_window(const std::string_view aName, 
    const window_size_type &aScreenSize)
: m_pGLFWWindow([&]()
{        
    glfwSetErrorCallback([](int, const char *msg)
    {
        throw std::runtime_error(std::string(TAG).append("/").append(msg));
    });

    if (!glfwInit()) throw std::runtime_error(std::string(TAG).append("/glfwInit failed"));

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if (GLFWwindow *const pWindow = glfwCreateWindow(aScreenSize.first, aScreenSize.second, aName.data(), nullptr, nullptr))
    {
        glfwMakeContextCurrent(pWindow);

        // Vsync controller. if not called, the interval is platform dependent. 0 is off. 
        // negative values allow for swapping even if the backbuffer arrives late (vendor extension dependent).
        glfwSwapInterval(-1); 

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow); //Must be called once to see the buffer we cleared

        glfwPollEvents(); //Must be called once for window to render

        glfwSetWindowUserPointer(pWindow, static_cast<void *>(this));
            
        glfwSetWindowSizeCallback(pWindow, [](GLFWwindow *const pCurrentGLFWwindow, int aX, int aY)
        {
            if (auto pCurrentWrapper = static_cast<glfw_window *const>(glfwGetWindowUserPointer(pCurrentGLFWwindow)))
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

GLFWwindow *const glfw_window::getPointerToGLFWWindow()
{
    return m_pGLFWWindow.get();
}

void glfw_window::swapBuffer()
{
    glfwSwapBuffers(m_pGLFWWindow.get());
}

glfw_window::window_size_type glfw_window::getWindowSize() const       
{
    return m_WindowSize;
}

bool glfw_window::getMouseButton(const int aButton) const
{
    return glfwGetMouseButton(m_pGLFWWindow.get(), aButton);
}

glfw_window::cursor_position_type glfw_window::getCursorPos() const
{
    double x,y;
    glfwGetCursorPos(m_pGLFWWindow.get(), &x, &y);

    return cursor_position_type(
        static_cast<cursor_position_type::first_type>(x), 
        static_cast<cursor_position_type::second_type>(y));
}

bool glfw_window::getKey(const int aKeyCode) const
{
    return static_cast<bool>(glfwGetKey(m_pGLFWWindow.get(), aKeyCode));
}

std::string_view glfw_window::getName() const
{
    return m_Name;
}

bool glfw_window::shouldClose() const
{
    return glfwWindowShouldClose(m_pGLFWWindow.get());
}

bool glfw_window::operator==(const glfw_window &a) const
{
    return m_pGLFWWindow == a.m_pGLFWWindow;
}

bool glfw_window::operator!=(const glfw_window &a) const
{
    return !(*this == a);
}

void glfw_window::setIcon(std::vector<std::byte> aRGBA32PNG)
{
    // TODO total hack. Incomplete.
    static constexpr int SIZE(64);

    //need to delete it as well etc.
    unsigned char pixels[SIZE * SIZE * 4];
    memset(pixels, 0xff, sizeof(pixels));
    GLFWimage *image = new GLFWimage();
    image->width = SIZE;
    image->height = SIZE;
    image->pixels = pixels;

    glfwSetWindowIcon(m_pGLFWWindow.get(), 1, image);
}

void glfw_window::setCursor(std::vector<std::byte> aRGBA32PNG)
{
    // TODO total hack. Incomplete.
    //need to delete it as well etc.
    unsigned char pixels[16 * 16 * 4];
    memset(pixels, 0xff, sizeof(pixels));
    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = pixels;
    
    m_pGLFWCursor = decltype(m_pGLFWCursor)(
        glfwCreateCursor(&image, 0, 0),
        [](GLFWcursor *p)
        {
            if (p) glfwDestroyCursor(p);
        });

    glfwSetCursor(m_pGLFWWindow.get(), m_pGLFWCursor.get());
}

