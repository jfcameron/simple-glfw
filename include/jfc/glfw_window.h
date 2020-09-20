// © 2018 Joseph Cameron - All Rights Reserved

#ifndef SIMPLE_GLFW_WINDOW_H
#define SIMPLE_GLFW_WINDOW_H

#include <array>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct GLFWwindow;
struct GLFWcursor;

namespace gdk
{
    /// \brief RAII wrapper for GLFW Window.
    /// GLFW functions that operate on glfwWindow pointers are wrapped as methods.
    /// TODO add support for a OnWindowSizeChanged callback!
    /// TODO wrap remaining functions here: https://www.glfw.org/docs/latest/window_guide.html
    class glfw_window final
    {
    public:
        //! type alias for window size
        using window_size_type = std::pair<int, int>;

		//! precision type of aspect ratio
		using window_aspect_ratio_type = double;
        
        //! alias for cursor position
        using cursor_position_type = std::pair<double, double>;
        
        //! unique_ptr type for glfwptr
        using window_pointer_type = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow *)>>;

        //! unique_ptr type for the cursor image
        using cursor_pointer_type = std::unique_ptr<GLFWcursor, std::function<void(GLFWcursor *)>>;

        //! alias for cursor image size. GLFW has strict requirements about this: 16x16 RGBA32 (1 byte per channel)
        using cursor_image_type = std::array<std::byte, 16 * 16 * 4>;

        //! POD structure for holding icon image data. RGBA32 format of userdefined dimensions
        struct icon_image_type
        {
            size_t width_pixels; //!< number of pixels wide
            size_t height_pixels; //!< number of pixels tall

            std::vector<std::byte> data_rgba32; //!< raw byte data

            static constexpr size_t CHANNEL_COUNT = 4; //!< number of channels per pixel
        };

        //! alias for collection of icon images.
        using icon_image_collection_type = std::vector<icon_image_type>;

        //! cursor graphics that are provided by the system.
        enum class standard_cursor_graphic
        {
            arrow, //!< regular arrow cursor
            ibeam, //!< text input I-beam cursor shape
            crosshair, //!< cross shaped cursor
            hand, //!< indexing finger
            horizontal_resizer, //!< cursor used to indicate some GUI element is being horizontally resized
            vertical_resizer //!< cursor used to indicate some GUI element is being vertically resized
        };

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
        //! provide a collection of icons of userdefined sizes, to be used by the system to render program icons where required.
        /// The icon whose dimensions best fit the system's requirements for some specific use case (window frame, 
        /// item in a taskbar, etc.) will be used for that purpose. 
        /// It is best practice to provide images of the same subject at a variety of resolutions, to minimize the undesirable 
        /// visual effects of a minifying or maximizing function (non-uniform column widths and or row heights, blurring).
        /// Common icon sizes are 16x16, 32x32, 48x48.
        /// Data format is RGBA 32bit: 4 channels, 1 byte per channel.
        /// \warning does not affect MacOS. Runtime icon is determined by icons provided in the application bundle. This must be handled at bundle creation time.
        void setIcons(const icon_image_collection_type &aIconImages);
        
        //! sets the window's cursor graphic from a 16x16 RGBA32 bitmap
        void setCursor(const cursor_image_type &aRGBA32PNG);

        //! set the window's cursor graphic from a standard graphic provided by the system
        void setCursor(const standard_cursor_graphic cursor);

        //! indicates whether or not the window should close
        bool shouldClose() const;

        //!  get the name of the window
        std::string_view getName() const; 
        
        //! Get size in pixels of the window
        window_size_type getWindowSize() const;

		//! Get the aspect ratio of the window
		window_aspect_ratio_type getAspectRatio() const;

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
        glfw_window(const std::string_view aName = "Window", const window_size_type &aWindowSize = {400, 300});
    };
}

#endif

