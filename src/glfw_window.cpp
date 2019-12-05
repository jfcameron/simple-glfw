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
#include <mutex>
#include <sstream>

static constexpr char TAG[] = "glfw_window";

//! 16x16 RGBA32 Image of a gauntlet
static const unsigned char cursor_contrast_data[] = {
    0x24, 0x26, 0x2b, 0xff, 0x3b, 0x3f, 0x47, 0xff, 0x18, 0x1b, 0x1b, 0x40,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x3a, 0x3c, 0x3c, 0x6d,
    0x1b, 0x1e, 0x1e, 0x80, 0x1f, 0x23, 0x23, 0x3a, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x1d, 0x1f, 0x20, 0xd4, 0x65, 0x74, 0x82, 0xff,
    0x46, 0x4c, 0x51, 0xff, 0x1c, 0x20, 0x20, 0xf4, 0x28, 0x2a, 0x2d, 0xf7,
    0x44, 0x4a, 0x51, 0xff, 0x34, 0x3b, 0x42, 0xff, 0x23, 0x27, 0x2a, 0xf8,
    0x2f, 0x33, 0x3a, 0x33, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00, 0x35, 0x3a, 0x40, 0x2b,
    0x30, 0x38, 0x40, 0xff, 0x9b, 0xa2, 0xa9, 0xff, 0x42, 0x4b, 0x57, 0xff,
    0x65, 0x6c, 0x73, 0xff, 0x49, 0x4b, 0x50, 0xff, 0x5d, 0x66, 0x6d, 0xff,
    0x30, 0x39, 0x40, 0xff, 0x1c, 0x1f, 0x22, 0xff, 0x2e, 0x32, 0x3b, 0x47,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x1b, 0x1b, 0x1e, 0x7d, 0x4d, 0x55, 0x5a, 0xff,
    0xa0, 0xa6, 0xac, 0xff, 0x35, 0x3e, 0x47, 0xff, 0x92, 0x97, 0x9c, 0xff,
    0x89, 0x8d, 0x90, 0xff, 0x91, 0x9e, 0xa7, 0xff, 0x47, 0x53, 0x5f, 0xff,
    0x1e, 0x21, 0x23, 0xcb, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x5a, 0x5a, 0x5a, 0x00, 0x5e, 0x63, 0x6a, 0x80,
    0x36, 0x3f, 0x46, 0xff, 0x51, 0x5b, 0x68, 0xff, 0xbf, 0xc1, 0xc3, 0xff,
    0xc2, 0xc7, 0xcc, 0xff, 0x7a, 0x86, 0x96, 0xff, 0x34, 0x3c, 0x44, 0xff,
    0x19, 0x1c, 0x1c, 0xff, 0x12, 0x12, 0x12, 0xff, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00, 0x7c, 0x7c, 0x7c, 0x00,
    0x3f, 0x48, 0x4c, 0xff, 0x29, 0x2f, 0x35, 0xff, 0x4c, 0x56, 0x60, 0xff,
    0x65, 0x72, 0x7e, 0xff, 0xc0, 0xc6, 0xcb, 0xff, 0xc7, 0xca, 0xcc, 0xff,
    0x67, 0x6f, 0x7a, 0xff, 0x24, 0x28, 0x2a, 0xff, 0x16, 0x18, 0x1a, 0xff,
    0x24, 0x28, 0x2d, 0x40, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x7c, 0x7c, 0x7c, 0x00, 0x3c, 0x46, 0x4c, 0xff, 0x1a, 0x1d, 0x1f, 0xff,
    0x47, 0x4e, 0x55, 0xff, 0x4d, 0x58, 0x61, 0xff, 0xa5, 0xad, 0xb5, 0xff,
    0xcc, 0xce, 0xce, 0xff, 0x83, 0x8c, 0x99, 0xff, 0x2b, 0x32, 0x38, 0xff,
    0x33, 0x3b, 0x42, 0xff, 0x3b, 0x41, 0x49, 0xb0, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x7c, 0x7c, 0x7c, 0x00, 0x38, 0x42, 0x4b, 0xff,
    0x4e, 0x57, 0x60, 0xff, 0x16, 0x16, 0x18, 0xff, 0x7b, 0x83, 0x8b, 0xff,
    0x6b, 0x78, 0x85, 0xff, 0xcc, 0xce, 0xcf, 0xff, 0x9a, 0xa1, 0xa5, 0xff,
    0x2e, 0x34, 0x39, 0xff, 0x2f, 0x37, 0x3a, 0xff, 0x38, 0x3f, 0x44, 0xff,
    0x22, 0x22, 0x24, 0x40, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x13, 0x13, 0x14, 0x47, 0x23, 0x28, 0x2a, 0xff, 0x24, 0x2a, 0x2d, 0xff,
    0x82, 0x89, 0x8e, 0xff, 0x47, 0x52, 0x5d, 0xff, 0x92, 0x9c, 0xa6, 0xff,
    0xa1, 0xa5, 0xab, 0xff, 0x6b, 0x70, 0x74, 0xff, 0x5d, 0x6f, 0x7b, 0xff,
    0x80, 0x8b, 0x99, 0xff, 0x40, 0x49, 0x54, 0xff, 0x28, 0x28, 0x31, 0x40,
    0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x1f, 0x22, 0x25, 0x49,
    0x20, 0x25, 0x27, 0xff, 0x54, 0x5e, 0x67, 0xff, 0x39, 0x42, 0x4c, 0xff,
    0x4b, 0x55, 0x61, 0xff, 0x9f, 0xa3, 0xa7, 0xff, 0x94, 0x97, 0x9a, 0xff,
    0x58, 0x60, 0x69, 0xff, 0x64, 0x75, 0x7e, 0xff, 0x72, 0x7d, 0x89, 0xff,
    0x28, 0x2f, 0x3e, 0xff, 0x1d, 0x20, 0x20, 0x40, 0xbc, 0xbc, 0xbc, 0x00,
    0xff, 0xff, 0xff, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x49, 0x28, 0x2e, 0x36, 0xff,
    0x29, 0x2c, 0x31, 0xff, 0x7e, 0x8a, 0x95, 0xff, 0xb5, 0xb8, 0xbb, 0xff,
    0xae, 0xb2, 0xb2, 0xff, 0x66, 0x6f, 0x78, 0xff, 0x30, 0x38, 0x40, 0xff,
    0x51, 0x61, 0x68, 0xff, 0x41, 0x4e, 0x57, 0xff, 0x1d, 0x22, 0x24, 0xff,
    0x1d, 0x20, 0x21, 0x57, 0xff, 0xff, 0xff, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x47, 0x3f, 0x48, 0x50, 0xff, 0x44, 0x52, 0x5f, 0xff,
    0xaf, 0xb3, 0xb8, 0xff, 0xb2, 0xb6, 0xb8, 0xff, 0xa0, 0xa2, 0xa4, 0xff,
    0x46, 0x4e, 0x57, 0xff, 0x2c, 0x34, 0x39, 0xff, 0x29, 0x32, 0x37, 0xff,
    0x19, 0x1b, 0x1e, 0xff, 0x11, 0x11, 0x11, 0x80, 0xff, 0xff, 0xff, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x1e, 0x20, 0x21, 0xbf,
    0x23, 0x25, 0x28, 0xff, 0x74, 0x7d, 0x85, 0xff, 0xa5, 0xa9, 0xac, 0xff,
    0x61, 0x66, 0x6a, 0xff, 0x16, 0x16, 0x18, 0xff, 0x1a, 0x1b, 0x1c, 0xff,
    0x21, 0x24, 0x25, 0xff, 0x1a, 0x1d, 0x1d, 0xff, 0x11, 0x11, 0x11, 0x7e,
    0xff, 0xff, 0xff, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x24, 0x28, 0x29, 0x38, 0x1a, 0x1b, 0x1c, 0xff, 0x3c, 0x43, 0x49, 0xff,
    0x2d, 0x32, 0x37, 0xff, 0x1d, 0x1f, 0x22, 0xff, 0x21, 0x27, 0x2a, 0xff,
    0x24, 0x27, 0x2a, 0xff, 0x20, 0x22, 0x23, 0xff, 0x15, 0x15, 0x18, 0xd3,
    0x11, 0x11, 0x11, 0x15, 0xff, 0xff, 0xff, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x19, 0x19, 0x1d, 0xab,
    0x22, 0x24, 0x28, 0xff, 0x25, 0x2a, 0x2f, 0xff, 0x32, 0x3e, 0x48, 0xff,
    0x38, 0x40, 0x49, 0xff, 0x1e, 0x1f, 0x23, 0xff, 0x13, 0x13, 0x15, 0xaa,
    0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00, 0xff, 0xff, 0xff, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00,
    0x12, 0x12, 0x14, 0x3b, 0x12, 0x13, 0x13, 0xcb, 0x1d, 0x1f, 0x23, 0xff,
    0x22, 0x28, 0x2d, 0xff, 0x14, 0x15, 0x16, 0xd4, 0x1d, 0x1d, 0x22, 0x3e,
    0x11, 0x11, 0x11, 0x00, 0x11, 0x11, 0x11, 0x00, 0xbc, 0xbc, 0xbc, 0x00,
    0xff, 0xff, 0xff, 0x00
};

using namespace gdk;

glfw_window::glfw_window(const std::string_view aName, 
    const window_size_type &aScreenSize)
: m_pGLFWWindow([&]()
{        
    glfwSetErrorCallback([](int, const char *msg)
    {
        throw std::runtime_error(std::string(TAG).append("/").append(msg));
    });

    static std::once_flag glfwInitFlag;

    std::call_once(glfwInitFlag, []()
    {
        if (!glfwInit()) throw std::runtime_error(std::string(TAG).append("/glfwInit failed"));

        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    });

    if (GLFWwindow *const pWindow = glfwCreateWindow(aScreenSize.first, aScreenSize.second, aName.data(), nullptr, nullptr))
    {
        glfwMakeContextCurrent(pWindow);

        // Vsync controller. if not called, the interval is platform dependent. 0 is off. 
        // negative values allow for swapping even if the backbuffer arrives late (vendor extension dependent).
        glfwSwapInterval(-1); 

#if !defined JFC_TARGET_PLATFORM_Emscripten
        //! if the mouse is locked, then use unaccellerated mouse input
        if (glfwRawMouseMotionSupported()) glfwSetInputMode(pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow); //Must be called once to see the buffer we cleared

        glfwPollEvents(); //Must be called once for window to render

        glfwSetWindowUserPointer(pWindow, static_cast<void *>(this));
            
        glfwSetWindowSizeCallback(pWindow, [](GLFWwindow *const pCurrentGLFWwindow, int aX, int aY)
        {
            if (auto pCurrentWrapper = static_cast<glfw_window *const>(glfwGetWindowUserPointer(pCurrentGLFWwindow)))
            {
                pCurrentWrapper->m_WindowSize.first =  aX;
                pCurrentWrapper->m_WindowSize.second = aY;
            }
            else throw std::runtime_error(std::string(TAG).append("/wrapper associated with current glfw window instance is null"));
        });

        glfwSetWindowCloseCallback(pWindow, [](GLFWwindow *const pCurrentWindow)
        {
            glfwSetWindowShouldClose(pCurrentWindow, GLFW_TRUE);
        });

#if defined JFC_TARGET_PLATFORM_Linux || defined JFC_TARGET_PLATFORM_Windows
        static std::once_flag glewInitFlag;

        std::call_once(glewInitFlag, []()
        {
            if (GLenum err = glewInit() != GLEW_OK) 
            {
                std::stringstream ss;

                ss << TAG << "/glewinit failed: " << glewGetErrorString(err);
            
                throw std::runtime_error(ss.str()); 
            }
        });
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
, m_WindowSize(aScreenSize.first, aScreenSize.second)
{
    cursor_image_type image;

    for (int i(0); i < (16 * 16 * 4);++i) image[i] = static_cast<std::byte>(cursor_contrast_data[i]);

    setCursor(image);

    icon_image_type icon;

    icon.width_pixels = 16;
    icon.height_pixels = 16;
    
    for (int i(0); i < (16 * 16 * 4);++i) icon.data_rgba32.push_back(static_cast<std::byte>(cursor_contrast_data[i]));

    setIcons({icon});
}

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

void glfw_window::setIcons(const icon_image_collection_type &aIconImages)
{
    std::vector<GLFWimage> glfwImages;

    for (const auto &iconImage : aIconImages)
    {
        GLFWimage image;
        image.width = iconImage.width_pixels;
        image.height = iconImage.height_pixels;
        image.pixels = const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(&iconImage.data_rgba32[0]));

        glfwImages.push_back(std::move(image));
    }

    glfwSetWindowIcon(m_pGLFWWindow.get(), aIconImages.size(), &glfwImages[0]);
}

void glfw_window::setCursor(const cursor_image_type &aRGBA32PNG)
{
    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(&aRGBA32PNG[0]));
    
    m_pGLFWCursor = decltype(m_pGLFWCursor)(
        glfwCreateCursor(&image, 0, 0),
        [](GLFWcursor *p)
        {
            if (p) glfwDestroyCursor(p);
        });

    glfwSetCursor(m_pGLFWWindow.get(), m_pGLFWCursor.get());
}

void glfw_window::setCursor(const standard_cursor_graphic cursor)
{
    decltype(GLFW_ARROW_CURSOR) glfwStandardCursor;

    switch(cursor)
    {
        case standard_cursor_graphic::arrow: glfwStandardCursor = GLFW_ARROW_CURSOR; break;
        case standard_cursor_graphic::ibeam: glfwStandardCursor = GLFW_IBEAM_CURSOR; break;
        case standard_cursor_graphic::crosshair: glfwStandardCursor = GLFW_CROSSHAIR_CURSOR; break;
        case standard_cursor_graphic::hand: glfwStandardCursor = GLFW_HAND_CURSOR; break;
        case standard_cursor_graphic::horizontal_resizer: glfwStandardCursor = GLFW_HRESIZE_CURSOR; break;
        case standard_cursor_graphic::vertical_resizer: glfwStandardCursor = GLFW_VRESIZE_CURSOR; break;
        
        default: throw std::invalid_argument("unhandled standard_cursor_graphic type");
    }

    m_pGLFWCursor = decltype(m_pGLFWCursor)(
        glfwCreateStandardCursor(glfwStandardCursor),
        [](GLFWcursor *p)
        {
            if (p) glfwDestroyCursor(p);
        });

    glfwSetCursor(m_pGLFWWindow.get(), m_pGLFWCursor.get());
}

