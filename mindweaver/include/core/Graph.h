#pragma once

#include "Link.h"
#include "Node.h"

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Project Namespace
namespace MindWeaver
{

    class Graph
    {
    public:
        Graph(const std::string &graph_name) : name(graph_name) {}

        void AddNode(std::shared_ptr<Node> node)
        {
            if (node)
            {
                nodes.push_back(node);
                node_map[node->id] = node;
            }
        }

        void RemoveNode(const UUID &node_id)
        {
            nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                                       [&](const std::shared_ptr<Node> &n) { return n && n->id == node_id; }),
                        nodes.end());
            node_map.erase(node_id);
            // Also remove links connected to this node
            RemoveLinksConnectedToNode(node_id);
        }

        std::shared_ptr<Node> GetNode(const UUID &node_id) const
        {
            auto it = node_map.find(node_id);
            return (it != node_map.end()) ? it->second : nullptr;
        }

        void AddLink(std::shared_ptr<Link> link)
        {
            if (link)
            {
                links.push_back(link);
            }
        }

        void RemoveLink(const UUID &link_id)
        {
            links.erase(std::remove_if(links.begin(), links.end(),
                                       [&](const std::shared_ptr<Link> &l) { return l && l->id == link_id; }),
                        links.end());
        }

        const std::vector<std::shared_ptr<Node>> &GetNodes() const { return nodes; }
        const std::vector<std::shared_ptr<Link>> &GetLinks() const { return links; }
        const std::string &GetName() const { return name; }

    private:
        void RemoveLinksConnectedToNode(const UUID &node_id)
        {
            links.erase(std::remove_if(links.begin(), links.end(),
                                       [&](const std::shared_ptr<Link> &link)
                                       {
                                           if (!link)
                                               return false;
                                           // Check if either end of the link connects to a pin on the removed node
                                           std::shared_ptr<Node> start_node_owner = GetNodeOwningPin(link->startPinID);
                                           std::shared_ptr<Node> end_node_owner = GetNodeOwningPin(link->endPinID);
                                           return (start_node_owner && start_node_owner->id == node_id) ||
                                                  (end_node_owner && end_node_owner->id == node_id);
                                       }),
                        links.end());
        }

        std::shared_ptr<Node> GetNodeOwningPin(const UUID &pin_id) const
        {
            for (const auto &node : nodes)
            {
                if (!node)
                    continue;
                if (node->GetInputPin(pin_id) || node->GetOutputPin(pin_id))
                {
                    return node;
                }
            }
            return nullptr;
        }

        std::string name;
        std::vector<std::shared_ptr<Node>> nodes;
        std::vector<std::shared_ptr<Link>> links;
        std::unordered_map<UUID, std::shared_ptr<Node>> node_map; // For quick lookup
    };

} // namespace MindWeaver
