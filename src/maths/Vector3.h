#pragma once
#include "core/blx_defines.h"

namespace BlxMath
{
    struct Vector3
    {
        float x, y, z;

        Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3() : x(0), y(0), z(0) {}

        // copy assignment operator
        Vector3& operator=(const Vector3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        // copy constructor
        Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z)
        {
        }

        // move constructor
        Vector3(Vector3&& other) noexcept : x(other.x), y(other.y), z(other.z)
        {
            other.x = 0;
            other.y = 0;
            other.z = 0;
        }

        // move assignment operator
        Vector3& operator=(Vector3&& other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                z = other.z;

                other.x = 0;
                other.y = 0;
                other.z = 0;
            }
            return *this;
        }

        bool operator==(const Vector3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        ~Vector3()
        {
        }
    }; 
} // namespace BlxMath