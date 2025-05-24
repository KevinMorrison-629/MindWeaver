#pragma once

#include "ui/Widget.h"

#include <imgui.h>
#include <imnodes.h>

#include <memory>
#include <string>
#include <vector>

/// @brief Project Namespace
namespace MindWeaver
{

    class Window : public Widget
    {
    public:
        Window(const std::string &title, bool *p_open = nullptr, ImGuiWindowFlags flags = 0)
            : m_Title(title), m_pOpen(p_open), m_Flags(flags)
        {
        }

        void Draw() override
        {
            if (m_pOpen && !*m_pOpen)
            {
                return; // Don't draw if closed
            }

            ImGui::Begin(m_Title.c_str(), m_pOpen, m_Flags);
            DrawContents(); // Call derived class method to draw window content
            ImGui::End();
        }

        void AddWidget(std::shared_ptr<Widget> widget) { m_Widgets.push_back(widget); }

    protected:
        // Derived windows must implement this
        virtual void DrawContents()
        {
            for (const auto &widget : m_Widgets)
            {
                widget->Draw();
            }
        }

    private:
        std::string m_Title;
        bool *m_pOpen;
        ImGuiWindowFlags m_Flags;
        std::vector<std::shared_ptr<Widget>> m_Widgets;
    };

    // Example simple widget
    class TextWidget : public Widget
    {
    public:
        TextWidget(const std::string &text) : m_Text(text) {}
        void Draw() override { ImGui::TextUnformatted(m_Text.c_str()); }

    private:
        std::string m_Text;
    };

} // namespace MindWeaver