// © 2018 Joseph Cameron - All Rights Reserved

#ifndef SIMPLE_GLFW_WINDOW_H
#define SIMPLE_GLFW_WINDOW_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct GLFWcursor;
struct GLFWwindow;

namespace gdk
{
    /// \brief RAII wrapper for GLFW Window.
    /// GLFW functions that operate on glfwWindow pointers are wrapped as methods.
    /// TODO add support for a OnWindowSizeChanged callback
    /// TODO wrap remaining functions here: https://www.glfw.org/docs/latest/window_guide.html
    class glfw_window final
    {
    public:
        //! type alias for window size
        using window_size_type = std::pair<int, int>;
        
        //! alias for cursor position
        using cursor_position_type = std::pair<double, double>;
        
        //! unique_ptr type for glfwptr
        using window_pointer_type = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow *)>>;

        //! unique_ptr type for the cursor image
        using cursor_pointer_type = std::unique_ptr<GLFWcursor, std::function<void(GLFWcursor *)>>;

    private:
        //! pointer to the glfw window
        window_pointer_type m_pGLFWWindow;

        //! pointer to the window's cursor
        cursor_pointer_type m_pGLFWCursor;
       
        //! window name
        std::string m_Name;

        //! size of the window
        window_size_type m_WindowSize;

    public:
        // -==--==-=--= TODO
        //! sets the window icon. 
        // Think about this.. this has no effect on mac. hmm
        void setIcon(std::vector<std::byte> aRGBA32PNG);
        
        //! sets the cursor for the window
        void setCursor(std::vector<std::byte> aRGBA32PNG);

        // -==--==-=--=

        //! indicates whether or not the window should close
        bool shouldClose() const;

        //!  get the name of the window
        std::string_view getName() const; 
        
        //! Get size in pixels of the window
        window_size_type getWindowSize() const;

        //! Get state of button by index
        bool getMouseButton(const int aButton) const;

        //! Get position of the mouse cursor
        cursor_position_type getCursorPos() const;

        //! Get a key value
        bool getKey(const int aKeyCode) const;

        //! get direct access to the GLFW pointer, primarily to allow GLFW dependant projects to work alongside this wrapper.
        GLFWwindow *const getPointerToGLFWWindow();

        //! Swap the front buffer (currently presented on display) with the backbuffer (currently being written to)
        void swapBuffer();

        //! support equality semantics
        bool operator==(const glfw_window &) const;
        //! support equality semantics
        bool operator!=(const glfw_window &) const;

        //! explicitly delete copy semantics
        glfw_window &operator=(const glfw_window &) const = delete;
        //! explicitly delete copy semantics
        glfw_window(const glfw_window &) = delete;

        //! support move semantics
        glfw_window &operator=(glfw_window &&) = default;
        //! support move semantics
        glfw_window(glfw_window &&) = default;

        //! standard constructor
        glfw_window(const std::string_view aName = "Window", 
            const window_size_type &aWindowSize = {400, 300});
    };
}

#endif

