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
        UUID id;           /// @brief Unique identifier for the node.
        std::string name;  /// @brief Display name of the node.
        NodeType type;     /// @brief Type of the node (e.g., ControlFlow, Function, Variable, Operator).
        Position position; /// @brief Node position in Workspace

        std::unordered_map<UUID, std::shared_ptr<Pin>> inputPins;  /// @brief Map of input pins.
        std::unordered_map<UUID, std::shared_ptr<Pin>> outputPins; /// @brief Map of output pins.

        /// @brief Constructs a new node with the specified ID, name, and type.
        /// @param id Unique identifier for the node.
        /// @param name Name of the node.
        /// @param type Type of the node (e.g., Operator, Function).
        Node(UUID id, const std::string &name, NodeType type) : id(id), name(name), type(type) {};

        /// @brief Adds an input pin to the node's backend representation.
        /// @param pin_name Logical name of the input pin.
        /// @param pin_type Logical type of data for the input pin.
        /// @return A shared pointer to the newly created input Pin.
        std::shared_ptr<Pin> AddInputPin(const std::string &pin_name, PinType pin_type)
        {
            UUID pinID = UUID::generate(); // Generate UUID using your static method
            auto pin = std::make_shared<Pin>(pinID, pin_name, pin_type, PinDirection::Input, this->id);
            inputPins.emplace(pinID, pin);
            return pin;
        }

        /// @brief Adds an output pin to the node's backend representation.
        /// @param pin_name Logical name of the output pin.
        /// @param pin_type Logical type of data for the output pin.
        /// @return A shared pointer to the newly created output Pin.
        std::shared_ptr<Pin> AddOutputPin(const std::string &pin_name, PinType pin_type)
        {
            UUID pinID = UUID::generate(); // Generate UUID using your static method
            auto pin = std::make_shared<Pin>(pinID, pin_name, pin_type, PinDirection::Output, this->id);
            outputPins.emplace(pinID, pin);
            return pin;
        }

        /// @brief Set the stored position of the node
        /// @param pos2D Position of the node in ImNodes grid space
        void SetPosition(const Position pos2D) { position = pos2D; }

        /// @brief Retrieves an input pin by its ID.
        /// @param pin_id The UUID of the input pin.
        /// @return A shared_ptr to the Pin if found, nullptr otherwise.
        std::shared_ptr<Pin> GetInputPin(const UUID &pin_id) const
        {
            auto it = inputPins.find(pin_id);
            return (it != inputPins.end()) ? it->second : nullptr;
        }

        /// @brief Retrieves an output pin by its ID.
        /// @param pin_id The UUID of the output pin.
        /// @return A shared_ptr to the Pin if found, nullptr otherwise.
        std::shared_ptr<Pin> GetOutputPin(const UUID &pin_id) const
        {
            auto it = outputPins.find(pin_id);
            return (it != outputPins.end()) ? it->second : nullptr;
        }
    };
} // namespace MindWeaver