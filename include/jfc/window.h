// © 2019 Joseph Cameron - All Rights Reserved

#ifndef JFC_WINDOW_H
#define JFC_WINDOW_H

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace gdk 
{
    /// \brief user-facing interface type for windows
    class Window
    {
    public:
        virtual std::string_view getName() const = 0;


    };
}

#endif

