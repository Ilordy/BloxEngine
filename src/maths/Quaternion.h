#pragma once

namespace BlxMath
{
    struct Quaternion
    {
        float x, y, z, w;

        Quaternion(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Quaternion() : x(0), y(0), z(0), w(1) {}

        // copy assignment operator
        Quaternion& operator=(const Quaternion& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        // copy constructor
        Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w)
        {
        }

        // move constructor
        Quaternion(Quaternion&& other) noexcept : x(other.x), y(other.y), z(other.z), w(other.w)
        {
            other.x = 0;
            other.y = 0;
            other.z = 0;
            other.w = 1;
        }

        // move assignment operator
        Quaternion& operator=(Quaternion&& other) noexcept
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;

                other.x = 0;
                other.y = 0;
                other.z = 0;
                other.w = 1;
            }
            return *this;
        }

        bool operator==(const Quaternion& other) const
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        ~Quaternion()
        {
        }
    };
} // namespace BlxMath