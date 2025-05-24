#pragma once

#include "UUID.h"

/// @brief Project Namespace
namespace MindWeaver
{

    /// @brief Represents the type of data or control carried by a pin.
    enum class PinType
    {
        Exec,   /// @brief Execution flow pin (controls the order of execution between nodes).
        Int,    /// @brief Integer data pin.
        Float,  /// @brief Floating-point data pin.
        Bool,   /// @brief Boolean data pin (true/false).
        String, /// @brief String data pin.
        Vector, /// @brief Vector data pin
        Class,  /// @brief Class data pin
    };

    /// @brief Indicates whether a pin is used for input or output.
    enum class PinDirection
    {
        Input, /// @brief Input pin (receives data or execution).
        Output /// @brief Output pin (sends data or execution).
    };

    /// @brief Represents a single input or output connection point on a node.
    struct Pin
    {
        UUID id;                /// @brief Unique identifier for the pin.
        std::string name;       /// @brief Display name of the pin.
        PinType type;           /// @brief The type of the pin (Exec, Int, Float, etc.).
        PinDirection direction; /// @brief The direction of the pin (Input or Output).
        UUID ownerNodeID;       /// @brief ID of the node this pin belongs to.

        /// @brief Constructs a new Pin with specified properties.
        /// @param pin_id Unique identifier for the pin.
        /// @param pin_name Logical name of the pin.
        /// @param pin_type Type of the pin.
        /// @param pin_dir Direction of the pin.
        /// @param node_id ID of the node that owns this pin.
        Pin(UUID pin_id, const std::string &pin_name, PinType pin_type, PinDirection pin_dir, UUID node_id)
            : id(pin_id), name(pin_name), type(pin_type), direction(pin_dir), ownerNodeID(node_id)
        {
            /// @todo Implement any backend-specific initialization for a pin
        }
    };
} // namespace MindWeaver