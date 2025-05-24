#include "Application.h"

// UI Panel
#include "ui/NodeEditorPanel.h"

// Core Data Structures
#include "core/Graph.h"
#include "core/Node.h"
#include "core/Pin.h"
#include "core/Position.h"
#include "core/UUID.h"

// ImGui and backends
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// GLAD (OpenGL Loader)
#include <glad/glad.h>
// GLFW (Windowing) - Must be included after GLAD
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>

static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

namespace MindWeaver
{

    Application::Application(const std::string &title, int width, int height)
        : m_WindowTitle(title), m_Width(width), m_Height(height)
    {
        if (!InitWindow())
        {
            Shutdown();
            throw std::runtime_error("Failed to initialize window systems.");
        }
        if (!InitImGui())
        {
            Shutdown();
            throw std::runtime_error("Failed to initialize ImGui.");
        }

        m_GraphInstance = std::make_shared<Graph>("MainGraph");
        m_NodeEditorPanelInstance = std::make_unique<NodeEditorPanel>("Node Editor");
        m_NodeEditorPanelInstance->SetGraph(m_GraphInstance);

        // Add sample nodes
        auto node1 = std::make_shared<Node>(UUID::generate(), "Start Event", NodeType::ExecutionFlow);
        node1->SetPosition(Position(100.f, 100.f));
        node1->AddOutputPin("Exec Out", PinType::Exec);
        node1->AddInputPin("Condition", PinType::Bool);
        m_GraphInstance->AddNode(node1);

        auto node2 = std::make_shared<Node>(UUID::generate(), "Process Data", NodeType::Function);
        node2->SetPosition(Position(350.f, 150.f));
        node2->AddInputPin("Exec In", PinType::Exec);
        node2->AddInputPin("Input Value", PinType::Int);
        node2->AddOutputPin("Next Exec", PinType::Exec);
        node2->AddOutputPin("Result", PinType::Float);
        m_GraphInstance->AddNode(node2);
    }

    Application::~Application() { Shutdown(); }

    bool Application::InitWindow()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwDestroyWindow(m_Window);
            glfwTerminate();
            return false;
        }
        std::cout << "GLAD Initialized. OpenGL Version: " << reinterpret_cast<const char *>(glGetString(GL_VERSION))
                  << std::endl;
        return true;
    }

    bool Application::InitImGui()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        if (!ImGui_ImplGlfw_InitForOpenGL(m_Window, true))
        {
            std::cerr << "Failed to initialize ImGui GLFW backend" << std::endl;
            return false;
        }
        if (!ImGui_ImplOpenGL3_Init(m_GlslVersion))
        {
            std::cerr << "Failed to initialize ImGui OpenGL3 backend (GLSL: " << m_GlslVersion << ")" << std::endl;
            return false;
        }
        std::cout << "ImGui Initialized." << std::endl;
        return true;
    }

    void Application::Run() { MainLoop(); }

    void Application::MainLoop()
    {
        while (!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();
            NewFrame();

            ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID(), ImGui::GetMainViewport(),
                                         ImGuiDockNodeFlags_PassthruCentralNode);

            if (m_NodeEditorPanelInstance)
            {
                m_NodeEditorPanelInstance->Render();
            }
            // ImGui::ShowDemoWindow(); // Optional for debugging

            RenderFrame();
        }
    }

    void Application::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Application::RenderFrame()
    {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(m_Window);
    }

    void Application::Shutdown()
    {
        m_NodeEditorPanelInstance.reset(); // Destructor will call ImNodes::DestroyContext()
        m_GraphInstance.reset();

        if (ImGui::GetCurrentContext())
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        if (m_Window)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
        std::cout << "Application Shutdown." << std::endl;
    }

} // namespace MindWeaver