#pragma once

#include "core/blx_defines.h"
#include "maths/mathTypes.h"

#define BLX_QUATERNION_IDENTITY ((blxQuaternion){0.0f, 0.0f, 0.0f, 1.0f})

/// @brief Normalizes a quaternion.
/// @note Normalization ensures that the quaternion has a magnitude of 1, which is important for representing rotations correctly.
/// @param q The quaternion to normalize.
/// @return The normalized quaternion.
BLXINLINE blxQuaternion blxQuat_Normalize(blxQuaternion q)
{
    float mag = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    // Avoid division by zero
    if (mag == 0.0f) {
        return BLX_QUATERNION_IDENTITY;
    }

    return (blxQuaternion) { q.x / mag, q.y / mag, q.z / mag, q.w / mag };
}

/// @brief Multiplies two quaternions. (Commonly used for combining rotations)
/// @note Quaternion multiplication is not commutative, meaning the order of multiplication matters.
/// @param a The first quaternion.
/// @param b The second quaternion.
/// @return A new quaternion with a rotation of b first, followed by a.
BLXINLINE blxQuaternion blxQuat_Mult(blxQuaternion a, blxQuaternion b)
{
    blxQuaternion result;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    return result;
}

//TODO: move These functions to physics internal file..

/// @brief For Physics implementation only
BLXINLINE blxQuaternion blxQuat_RotateByVector(blxQuaternion q, blxVec3 v)
{
    blxQuaternion result = { v.x, v.y, v.z, 0.0f };
    result = blxQuat_Mult(q, result);
    return result;
}

/// @brief For Physics implementation only
BLXINLINE blxQuaternion blxQuat_AddScaledVector(blxQuaternion q, blxVec3 v, float scale)
{
    blxQuaternion qv = { v.x * scale, v.y * scale, v.z * scale, 0.0f };
    qv = blxQuat_Mult(qv, q);

    return { qv.x * 0.5f + q.x,
        qv.y * 0.5f + q.y,
        qv.z * 0.5f + q.z,
        qv.w * 0.5f + q.w };
}

//ENDTODO