/**
 * @file blx_vec3.h
 * @brief Header file for 3D vector mathematics.
 *
 * This header provides declarations and utilities for performing mathematical operations
 * on 3-dimensional vectors, including arithmetic, geometric computations, and utility functions.
 * It is intended for use in applications requiring vector algebra, such as graphics, physics, and simulations.
 *
 * @note At the moment this is only used for the blox engine physics engine, and will eventually maybe replace CGLM.
 */

#pragma once

#include "core/blx_defines.h"
#include "maths/mathTypes.h"

// TODO: Look into SIMD and SIMD operations for performance improvements.

// TODO: MOVE VEC3 TO COMMON FILE



#define BLX_VEC3_ZERO ((blxVec3){0.0f, 0.0f, 0.0f})
#define BLX_VEC3_ONE ((blxVec3){1.0f, 1.0f, 1.0f})

BLXINLINE float blxVec3_Magnitude(blxVec3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

BLXINLINE float blxVec3_SquaredMagnitude(blxVec3 vec)
{
    return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

BLXINLINE blxVec3 blxVec3_Normalize(blxVec3 vec)
{
    float mag = blxVec3_Magnitude(vec);
    // Avoid division by zero
    if (mag == 0.0f) {
        return BLX_VEC3_ZERO;
    }

    return (blxVec3) { vec.x / mag, vec.y / mag, vec.z / mag };
}

BLXINLINE blxVec3 blxVec3_Add(blxVec3 a, blxVec3 b)
{
    return (blxVec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

BLXINLINE blxVec3 blxVec3_Subtract(blxVec3 a, blxVec3 b)
{
    return (blxVec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

BLXINLINE blxVec3 blxVec3_Mult(blxVec3 vec, float scalar)
{
    return (blxVec3) { vec.x * scalar, vec.y * scalar, vec.z * scalar };
}

BLXINLINE blxVec3 blxVec3_Div(blxVec3 vec, float scalar)
{
    // Avoid division by zero
    if (scalar == 0.0f) {
        return BLX_VEC3_ZERO;
    }
    return (blxVec3) { vec.x / scalar, vec.y / scalar, vec.z / scalar };
}

BLXINLINE blxVec3 blxVec3_Scale(blxVec3 a, blxVec3 b)
{
    return (blxVec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

BLXINLINE float blxVec3_Dot(blxVec3 a, blxVec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

BLXINLINE blxVec3 blxVec3_Cross(blxVec3 a, blxVec3 b)
{
    return (blxVec3) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

BLXINLINE blxVec3 blxVec3_AddScaledVector(blxVec3 a, blxVec3 vectorToScale, float scale)
{
    return (blxVec3) {
        a.x + vectorToScale.x * scale,
        a.y + vectorToScale.y * scale,
        a.z + vectorToScale.z * scale
    };
}

/// @brief Creates an orthonormal basis from three vectors for a right handed coordinate system.
/// This function takes three vectors and modifies them such that:
/// - The first vector (`a`) is normalized.
/// - The third vector (`c`) is the cross product of the first two vectors, normalized.
/// - The second vector (`b`) is recalculated to be orthogonal to both `a` and `c`.\
///@note This function assumes that the input vectors `a` and `b` are not parallel. 
///If they are, the function will not be able to create an orthonormal basis.
BLXINLINE void blxVec3_MakeOrthoNormal(blxVec3* a, blxVec3* b, blxVec3* c)
{
    // Normalize the first vector
    *a = blxVec3_Normalize(*a);

    // c is the cross product of a and b, which will be orthogonal to both
    *c = blxVec3_Cross(*a, *b);

    // If the magnitude of c is 0, it means a and b are parallel
    // And we cannot form an orthonormal basis.
    if (blxVec3_SquaredMagnitude(*c) == 0.0f) {
        return;
    }
    
    // Normalize the third vector
    *c = blxVec3_Normalize(*c);
    // Recalculate b to ensure orthogonality
    *b = blxVec3_Cross(*c, *a);
}


