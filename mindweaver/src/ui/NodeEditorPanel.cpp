#include "ui/NodeEditorPanel.h"

// Core backend headers (needed for implementation details)
#include "core/Graph.h"
#include "core/Link.h"
#include "core/Node.h"
#include "core/Pin.h"
#include "core/Position.h"
#include "core/UUID.h"

// ImGui and ImNodes
#include <imgui.h>
#include <imnodes.h> // Include ImNodes header

#include <algorithm> // For std::remove_if
#include <iostream>  // For debugging

namespace MindWeaver
{

    // Static helper method implementation
    int NodeEditorPanel::GetImNodeID(const MindWeaver::UUID &uuid) { return uuid.to_int_for_imnodes(); }

    NodeEditorPanel::NodeEditorPanel(const std::string &panel_name) : m_PanelName(panel_name), m_Graph(nullptr)
    {
        ImNodes::CreateContext(); // Create ImNodes context on panel creation
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
        // ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkCreationOnSnap); // Optional
        std::cout << "NodeEditorPanel: ImNodes context created." << std::endl;
    }

    NodeEditorPanel::~NodeEditorPanel()
    {
        if (ImNodes::GetCurrentContext())
        {                              // Check if context exists before destroying
            ImNodes::DestroyContext(); // Destroy ImNodes context on panel destruction
            std::cout << "NodeEditorPanel: ImNodes context destroyed." << std::endl;
        }
    }

    void NodeEditorPanel::SetGraph(std::shared_ptr<Graph> graph_ptr) { m_Graph = graph_ptr; }

    void NodeEditorPanel::Render()
    {
        if (!m_Graph)
        {
            // Optionally render a message if no graph is set
            ImGui::Begin(m_PanelName.c_str());
            ImGui::Text("No graph loaded.");
            ImGui::End();
            return;
        }

        ImGui::Begin(m_PanelName.c_str()); // ImGui window for the panel

        ImNodes::BeginNodeEditor();

        DrawNodes();
        DrawLinks();

        ImNodes::EndNodeEditor(); // Finalize editor state for this frame

        // Handle interactions AFTER EndNodeEditor
        HandleLinkCreation();
        HandleLinkDeletion();
        HandleNodeInteraction();

        ImGui::End(); // End ImGui window
    }

    void NodeEditorPanel::DrawNodes()
    {
        if (!m_Graph)
            return;

        const auto &nodes = m_Graph->GetNodes();
        for (const auto &backend_node : nodes)
        {
            if (!backend_node)
                continue;

            const int node_imnodes_id = GetImNodeID(backend_node->id);
            ImNodes::SetNodeGridSpacePos(node_imnodes_id, ImVec2(backend_node->position.x, backend_node->position.y));

            ImNodes::BeginNode(node_imnodes_id);

            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(backend_node->name.c_str());
            ImNodes::EndNodeTitleBar();

            for (const auto &pair : backend_node->inputPins)
            {
                const auto &pin = pair.second;
                if (!pin)
                    continue;
                ImNodes::BeginInputAttribute(GetImNodeID(pin->id));
                ImGui::TextUnformatted(pin->name.c_str());
                ImNodes::EndInputAttribute();
            }

            for (const auto &pair : backend_node->outputPins)
            {
                const auto &pin = pair.second;
                if (!pin)
                    continue;
                ImNodes::BeginOutputAttribute(GetImNodeID(pin->id));
                ImGui::TextUnformatted(pin->name.c_str());
                ImNodes::EndOutputAttribute();
            }

            ImNodes::EndNode();
        }
    }

    void NodeEditorPanel::DrawLinks()
    {
        if (!m_Graph)
            return;

        const auto &links = m_Graph->GetLinks();
        for (const auto &backend_link : links)
        {
            if (!backend_link)
                continue;
            ImNodes::Link(GetImNodeID(backend_link->id), GetImNodeID(backend_link->startPinID),
                          GetImNodeID(backend_link->endPinID));
        }
    }

    void NodeEditorPanel::HandleLinkCreation()
    {
        if (!m_Graph)
            return;

        int start_attr_imnodes_id, end_attr_imnodes_id;
        if (ImNodes::IsLinkCreated(&start_attr_imnodes_id, &end_attr_imnodes_id))
        {
            MindWeaver::UUID start_pin_uuid, end_pin_uuid;
            bool start_found = false, end_found = false;

            // Find backend pins (this can be slow, consider a map for ImNodeID -> Pin UUID/ptr)
            const auto &nodes = m_Graph->GetNodes();
            for (const auto &node : nodes)
            {
                if (!node)
                    continue;
                if (start_found && end_found)
                    break;

                for (const auto &pair : node->outputPins)
                { // Assuming links go from output
                    if (GetImNodeID(pair.second->id) == start_attr_imnodes_id)
                    {
                        start_pin_uuid = pair.second->id;
                        start_found = true;
                        break;
                    }
                }
                if (start_found && end_found)
                    break;
                for (const auto &pair : node->inputPins)
                { // to input
                    if (GetImNodeID(pair.second->id) == end_attr_imnodes_id)
                    {
                        end_pin_uuid = pair.second->id;
                        end_found = true;
                        break;
                    }
                }
            }
            // Fallback: check if the user dragged from an input to an output
            if (!start_found || !end_found)
            {
                start_found = false;
                end_found = false; // Reset for reverse check
                for (const auto &node : nodes)
                {
                    if (!node)
                        continue;
                    if (start_found && end_found)
                        break;
                    for (const auto &pair : node->inputPins)
                    { // Start from input
                        if (GetImNodeID(pair.second->id) == start_attr_imnodes_id)
                        {
                            start_pin_uuid = pair.second->id;
                            start_found = true;
                            break;
                        }
                    }
                    if (start_found && end_found)
                        break;
                    for (const auto &pair : node->outputPins)
                    { // End at output
                        if (GetImNodeID(pair.second->id) == end_attr_imnodes_id)
                        {
                            end_pin_uuid = pair.second->id;
                            end_found = true;
                            break;
                        }
                    }
                }
            }

            if (start_found && end_found)
            {
                // TODO: Add validation (e.g., type compatibility, prevent input-to-input)
                MindWeaver::UUID new_link_uuid = MindWeaver::UUID::generate();
                m_Graph->AddLink(std::make_shared<MindWeaver::Link>(new_link_uuid, start_pin_uuid, end_pin_uuid));
                std::cout << "Link created in backend: " << new_link_uuid.to_string() << std::endl;
            }
            else
            {
                std::cerr << "Error: Could not find backend pins for new link." << std::endl;
            }
        }
    }

    void NodeEditorPanel::HandleLinkDeletion()
    {
        if (!m_Graph)
            return;

        int link_imnodes_id_destroyed;
        if (ImNodes::IsLinkDestroyed(&link_imnodes_id_destroyed))
        {
            // Find the link in the backend by its ImNodes ID (hashed UUID)
            const auto &links = m_Graph->GetLinks();
            MindWeaver::UUID link_to_remove_uuid;
            bool found = false;
            for (const auto &link_ptr : links)
            {
                if (GetImNodeID(link_ptr->id) == link_imnodes_id_destroyed)
                {
                    link_to_remove_uuid = link_ptr->id;
                    found = true;
                    break;
                }
            }
            if (found)
            {
                m_Graph->RemoveLink(link_to_remove_uuid);
                std::cout << "Link destroyed in backend (UUID): " << link_to_remove_uuid.to_string() << std::endl;
            }
            else
            {
                std::cout << "Attempted to destroy link with ImNodes ID: " << link_imnodes_id_destroyed
                          << " but not found in backend." << std::endl;
            }
        }
    }

    void NodeEditorPanel::HandleNodeInteraction()
    {
        if (!m_Graph)
            return;

        const int num_selected_nodes = ImNodes::NumSelectedNodes();
        if (num_selected_nodes > 0)
        {
            std::vector<int> selected_node_imnodes_ids(num_selected_nodes);
            ImNodes::GetSelectedNodes(selected_node_imnodes_ids.data());

            for (int selected_node_imnodes_id : selected_node_imnodes_ids)
            {
                // Find the backend node corresponding to this ImNodes ID
                const auto &nodes = m_Graph->GetNodes(); // Get a const reference
                for (const auto &backend_node_ptr : nodes)
                { // Iterate over shared_ptr
                    if (backend_node_ptr && GetImNodeID(backend_node_ptr->id) == selected_node_imnodes_id)
                    {
                        ImVec2 current_imnodes_pos = ImNodes::GetNodeGridSpacePos(selected_node_imnodes_id);
                        if (backend_node_ptr->position.x != current_imnodes_pos.x ||
                            backend_node_ptr->position.y != current_imnodes_pos.y)
                        {
                            backend_node_ptr->SetPosition(
                                MindWeaver::Position(current_imnodes_pos.x, current_imnodes_pos.y));
                            // std::cout << "Backend Node " << backend_node_ptr->name << " position updated." << std::endl;
                        }
                        break;
                    }
                }
            }
        }
    }

} // namespace MindWeaver
