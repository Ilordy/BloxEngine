#pragma once

/// @brief A 3D vector type used for various mathematical operations.
typedef union blxVec3
{
     /// @brief The raw array representation of the vector.
    float raw[3];
    struct {
        float x;
        float y;
        float z;
    };

    struct {
        float r;
        float g;
        float b;
    };

}blxVec3;

/// @brief Quaternion type used for representing rotations in 3D space.
typedef union blxQuaternion
{
    float raw[4];
    struct {
        float x;
        float y;
        float z;
        /// @brief The real part of the quaternion, often referred to as the scalar part.
        float w;
    };

    struct {
        float i;
        float j;
        float k;
        /// @brief The real part of the quaternion, often referred to as the scalar part.
        float r;
    };

}blxQuaternion;


/// @brief A 3x3 matrix type used for various mathematical operations.
/// @note Blox Engine uses column-major order for matrices, meaning that the first index represents the column and the second index represents the row.
typedef union blxMat3
{
    float raw[3][3];

    struct {
        /// @brief first column (ex: m01 = first column, second row)
        float m00, m01, m02;
        /// @brief second column (ex: m12 = second column, third row)
        float m10, m11, m12;
        /// @brief third column (ex: m22 = third column, third row)
        float m20, m21, m22;
    };
}blxMat3;


// TODO: Add 16 byte alignment for both mat4 and vec4 types.

/// @brief A 4x4 matrix type used for various mathematical operations.
/// @note Blox Engine uses column-major order for matrices, meaning that the first index represents the column and the second index represents the row.
typedef union blxMat4
{
    float raw[4][4];

    struct {
        /// @brief first column (ex: m01 = first column, second row)
        float m00, m01, m02, m03;
        /// @brief second column (ex: m12 = second column, third row)
        float m10, m11, m12, m13;
        /// @brief third column (ex: m22 = third column, third row)
        float m20, m21, m22, m23;
        /// @brief fourth column (ex: m33 = fourth column, fourth row)
        float m30, m31, m32, m33;
    };
}blxMat4;
