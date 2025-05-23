#pragma once

#include "UUID.h"

/// @brief Project Namespace
namespace MindWeaver
{

    /// @brief Represents a connection between two pins in the node graph.
    /// A link connects an output pin (start) to an input pin (end), allowing
    /// data or execution flow between nodes.
    struct Link
    {
        UUID id;         /// @brief Unique identifier for the link.
        UUID startPinID; /// @brief The ID of the starting pin (typically an output pin).
        UUID endPinID;   /// @brief The ID of the ending pin (typically an input pin).

        /// @brief Constructs a new Link between two pins.
        /// @param id Unique identifier for the link.
        /// @param start The ID of the output/start pin.
        /// @param end The ID of the input/end pin.
        Link(UUID id, UUID start, UUID end) : id(id), startPinID(start), endPinID(end) {};
    };
} // namespace MindWeaver