#pragma once

/// @brief Project Namespace
namespace MindWeaver
{

    // A simple 2D vector (used to avoid dependencies on ImGui's ImVec2 in core data classes.)
    struct Position
    {
        float x; /// @brief x-position
        float y; /// @brief y-position

        /// @brief Position constructor (initializes position to (0.0, 0.0))
        Position() : x(0.0f), y(0.0f) {}

        /// @brief Position constructor
        /// @param x_val x-position
        /// @param y_val y-position
        Position(float x_val, float y_val) : x(x_val), y(y_val) {}

        /// @brief Add two Position objects
        /// @param other The Position to add
        /// @return the sum of the two position vectors
        Position operator+(const Position &other) const { return Position(x + other.x, y + other.y); }

        /// @brief Subtract two Position objects
        /// @param other The Position to subtract
        /// @return the difference of the two position vectors
        Position operator-(const Position &other) const { return Position(x - other.x, y - other.y); }

        /// @brief Multiply two Position objects (component-wise)
        /// @param other The Position to multiply
        /// @return the product of the two position vectors
        Position operator*(float scalar) const { return Position(x * scalar, y * scalar); }

        /// @brief Divide two Position objects (component-wise)
        /// @param other The Position to divide
        /// @return the quotient of the two position vectors
        Position operator/(float scalar) const { return Position(x / scalar, y / scalar); }
    };

} // namespace MindWeaver