#pragma once

#include <memory>
#include <string>
#include <vector>

// Forward declare core types to reduce header dependencies
namespace MindWeaver
{
    class Graph;
    struct Node; // For internal iteration, full definition not strictly needed in header
    struct Link; // For internal iteration
    struct Pin;  // For internal iteration
    struct UUID; // For GetImNodeID signature
} // namespace MindWeaver

namespace MindWeaver
{

    class NodeEditorPanel
    {
    public:
        NodeEditorPanel(const std::string &panel_name = "Node Editor");
        ~NodeEditorPanel(); // Important: Destroy ImNodes context here

        // Prevent copy and assignment
        NodeEditorPanel(const NodeEditorPanel &) = delete;
        NodeEditorPanel &operator=(const NodeEditorPanel &) = delete;

        void SetGraph(std::shared_ptr<Graph> graph_ptr);
        void Render(); // This will contain ImGui::Begin, ImNodes::BeginNodeEditor, etc.

        const std::string &GetName() const { return m_PanelName; }
        // bool IsOpen() const { return m_IsOpen; } // Optional for visibility control
        // void SetOpen(bool open) { m_IsOpen = open; }

    private:
        void DrawNodes();
        void DrawLinks();
        void HandleLinkCreation();
        void HandleLinkDeletion();
        void HandleNodeInteraction(); // For updating backend positions

        // Helper to get a unique int ID for ImNodes from our UUID
        static int GetImNodeID(const MindWeaver::UUID &uuid);

        std::string m_PanelName;
        // bool m_IsOpen = true; // Optional
        std::shared_ptr<Graph> m_Graph; // The panel operates on this graph
    };

} // namespace MindWeaver