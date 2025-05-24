#pragma once

#include <imgui.h>
#include <imnodes.h>

#include <memory>
#include <vector>

/// @brief Project Namespace
namespace MindWeaver
{

    class Widget
    {
    public:
        virtual ~Widget() = default;
        virtual void Draw() = 0; // Pure virtual function
    };

} // namespace MindWeaver