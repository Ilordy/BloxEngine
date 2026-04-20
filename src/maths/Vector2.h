#pragma once
#include "core/blx_defines.h"

namespace BlxMath
{
    struct Vector2
    {
        f32 x, y;

        inline Vector2() : x(0), y(0) {}
        inline Vector2(f32 scalar) : x(scalar), y(scalar) {}
        inline Vector2(f32 x, f32 y) : x(x), y(y) {}
        inline Vector2(const Vector2& other) : x(other.x), y(other.y) {}

        inline Vector2& operator=(const Vector2& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        inline Vector2 operator+(const Vector2& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        inline Vector2& operator+=(const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        inline Vector2 operator-(const Vector2& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        inline Vector2& operator-=(const Vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        inline bool operator==(const Vector2& other) const
        {
            return x == other.x && y == other.y;
        }

        inline void Scale(f32 scalar)
        {
            x *= scalar;
            y *= scalar;
        }
    };
} // namespace BlxMath