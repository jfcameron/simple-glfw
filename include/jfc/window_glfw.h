// © 2018 Joseph Cameron - All Rights Reserved

#ifndef SIMPLE_GLFW_WINDOW_H
#define SIMPLE_GLFW_WINDOW_H

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <jfc/window.h>

struct GLFWwindow;

namespace gdk
{
    /// \brief RAII wrapper for GLFW Window
    /// \todo hide init and terminate inside of the constructor and destructor
    /// \todo pWindow should be a shared_ptr, the shared_ptr should clean the ptr. getPointer should return a weak_ptr
    class SimpleGLFWWindow final : public Window
    {
    public:
        using window_size_type = std::pair<int, int>;
        using cursor_position_type = std::pair<double, double>;

    private:
        //glfw::windowsizecallback_type WindowSizeCallback;
        
        std::shared_ptr<GLFWwindow> m_pGLFWWindow;
        
        std::string m_Name;

        window_size_type m_WindowSize;

    public:
        //! indicates whether or not the window should close
        bool shouldClose() const;

        virtual std::string_view getName() const override;
        
        //! Get size in pixels of the window
        window_size_type getWindowSize() const;


        //! Attach a callback to the window which will be called whenver the window is resized and provides that size info.
        /// \WARN You can only attach one function at a time.
        /// \WARN Should this support an arbitray number of callbacks?
        //void SetWindowSizeCallback(windowsizecallback_type aWindowSizeCallback);
        
        //! Get state of button by index
        bool getMouseButton(const int aButton) const;

        //! Get position of the mouse cursor
        cursor_position_type getCursorPos() const;

        //! Get a key value
        bool getKey(const int aKeyCode) const;

        //! Get the amount of time since the window was created
        /// \WARN What is the unit?
        //static double GetTime();

        //! Causes GLFW to update event states
        void pollEvents();

        //! get direct access to the GLFW pointer, primarily to allow GLFW dependant projects to work alongside this wrapper.
        GLFWwindow *const getPointerToGLFWWindow();

        //! Swap the front buffer (currently presented on display) with the backbuffer (currently being written to)
        void swapBuffer();

        SimpleGLFWWindow(const std::string_view aName = "Window", const window_size_type &aWindowSize = {400, 300});
    };
}

#endif

