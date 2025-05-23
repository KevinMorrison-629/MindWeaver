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

        /// @brief Constructs a new Pin with specified properties.
        /// @param id Unique identifier for the pin.
        /// @param name Display name of the pin.
        /// @param type Type of the pin.
        /// @param dir Direction of the pin.
        Pin(UUID id, const std::string &name, PinType type, PinDirection dir)
            : id(id), name(name), type(type), direction(dir) {};
    };
} // namespace MindWeaver