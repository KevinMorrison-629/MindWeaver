#pragma once

#include <memory>
#include <string>

// Forward declarations
struct GLFWwindow;

namespace MindWeaver
{
    // Forward declare your panel and core data structures
    class NodeEditorPanel;
    class Graph;

    class Application
    {
    public:
        Application(const std::string &title, int width = 1280, int height = 720);
        ~Application();

        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

        void Run();

    private:
        bool InitWindow();
        bool InitImGui();

        void MainLoop();
        void NewFrame();
        void RenderFrame();
        void Shutdown();

        GLFWwindow *m_Window = nullptr;
        std::string m_WindowTitle;
        int m_Width;
        int m_Height;
        const char *m_GlslVersion = "#version 330";

        std::shared_ptr<Graph> m_GraphInstance;
        std::unique_ptr<NodeEditorPanel> m_NodeEditorPanelInstance;
    };

} // namespace MindWeaver