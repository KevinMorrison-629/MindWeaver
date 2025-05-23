#pragma once

#include "Pin.h"
#include "Position.h"
#include "UUID.h"

#include <string>
#include <unordered_map>
#include <vector>

/// @brief Project Namespace
namespace MindWeaver
{

    /// @brief Represents different types of nodes in the visual scripting graph.
    enum class NodeType
    {
        ExecutionFlow, /// @brief A execution flow noe (e.g., OnBeginPlay, ConstructGlobals)
        ControlFlow,   /// @brief A control flow node (e.g., loops, conditionals).
        Function,      /// @brief A function node that executes a specific action or calculation. (can be user-defined)
        Variable,      /// @brief A variable node representing a stored value.
        Operator,      /// @brief A math and logical operation node (e.g., addition, subtraction, AND, OR).

    };

    /// @brief Represents a node in the visual scripting system.
    /// A node may contain inputs, outputs, and properties that are manipulated in the script.
    struct Node
    {
        UUID id;          /// @brief Unique identifier for the node.
        std::string name; /// @brief Display name of the node.
        NodeType type;    /// @brief Type of the node (e.g., ControlFlow, Function, Variable, Operator).

        std::unordered_map<UUID, std::shared_ptr<Pin>> inputs;  /// @brief List of input pins for the node.
        std::unordered_map<UUID, std::shared_ptr<Pin>> outputs; /// @brief List of output pins for the node.
        Position position;                                      /// @brief Node position in Workspace

        /// @brief Constructs a new node with the specified ID, name, and type.
        /// @param id Unique identifier for the node.
        /// @param name Name of the node.
        /// @param type Type of the node (e.g., Operator, Function).
        Node(UUID id, const std::string &name, NodeType type) : id(id), name(name), type(type) {};

        /// @brief Adds an input pin to the node.
        /// @param name Name of the input pin.
        /// @param type Type of data for the input pin (e.g., Int, Bool).
        void AddInput(const std::string &name, PinType type)
        {
            UUID pinID = UUID::generate();
            inputs.emplace(pinID, std::make_shared<Pin>(pinID, name, type, PinDirection::Input));
        }

        /// @brief Adds an output pin to the node.
        /// @param name Name of the output pin.
        /// @param type Type of data for the output pin (e.g., Int, Float).
        void AddOutput(const std::string &name, PinType type)
        {
            UUID pinID = UUID::generate();
            outputs.emplace(pinID, std::make_shared<Pin>(pinID, name, type, PinDirection::Output));
        }

        /// @brief Set the stored position of the node
        /// @param pos2D Position of the node in ImNodes grid space
        void SetPosition(const Position pos2D) { position = pos2D; }
    };
} // namespace MindWeaver