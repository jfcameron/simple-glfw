// © Joseph Cameron - All Rights Reserved

#ifndef GDK_GLFW_WINDOW_H
#define GDK_GLFW_WINDOW_H

#include <gdk/window.h>

struct GLFWwindow;
struct GLFWcursor;

namespace gdk {
    class glfw_window final : public window {
    public:
        using window_pointer_type = std::shared_ptr<GLFWwindow>;
        using cursor_pointer_type = std::unique_ptr<GLFWcursor, std::function<void(GLFWcursor *)>>;

        //! Makes a window instance using glfw as the implementation
        static std::shared_ptr<glfw_window> make(const std::string_view aName = "Window");

        //! Processess all glfw events
        /// \attention this must be called in order for any glfw_windows to respond to input events. typically call it once per frame in your game loop.
        static void poll_events();

        //! Redraws the window's canvas. 
        /// \attention must be called to see an updated image. usually done once per frame
        void swap_buffers();

        //! gets a shared_ptr to the underlying glfw window
        window_pointer_type ptr_to_implementation();

        //! indicates whether or not the window should close
        //TODO: if this is actually useful, promote it to the interface class
        virtual bool should_close() const override;
		virtual window_aspect_ratio_type aspect_ratio() const override;
        virtual cursor_position_type cursor_position() const override;
        virtual std::string_view name() const override;
        virtual void set_cursor(const cursor_image_type &aRGBA32PNG) override;
        virtual void set_cursor(const standard_cursor_graphic cursor) override;
        virtual void set_icons(const icon_image_collection_type &aIconImages) override;
        virtual window_size_type window_size() const override;
		virtual void close() override;

    private:
        glfw_window(const std::string_view aName, const window_size_type &aWindowSize);

        cursor_pointer_type m_pGLFWCursor;
        std::string m_Name;
        window_pointer_type m_pGLFWWindow;
        window_size_type m_WindowSize;
    };
}

#endif

