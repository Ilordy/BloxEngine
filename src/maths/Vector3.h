#pragma once
#include "core/blx_defines.h"

namespace BlxMath
{
    struct Vector3
    {
        f32 x, y, z;

        inline Vector3() : x(0), y(0), z(0) {}
        inline Vector3(f32 scalar) : x(scalar), y(scalar), z(scalar) {}
        inline Vector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

        inline Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

        inline Vector3& operator=(const Vector3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        inline Vector3& operator+=(const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        inline Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        inline Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        inline bool operator==(const Vector3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        inline void Scale(f32 scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }
    };
} // namespace BlxMath