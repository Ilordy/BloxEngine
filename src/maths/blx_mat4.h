#pragma once
#include "core/blx_defines.h"
#include "maths/mathTypes.h"

//TODO: Look into SIMD and SIMD operations for performance improvements.
// TODO: This functions should be taking in pointers for simd

#define BLX_MAT4_ZERO ((blxMat4){0.0f, 0.0f, 0.0f, 0.0f, \
                                   0.0f, 0.0f, 0.0f, 0.0f, \
                                   0.0f, 0.0f, 0.0f, 0.0f, \
                                   0.0f, 0.0f, 0.0f, 0.0f})

#define BLX_MAT4_IDENTITY ((blxMat4){1.0f, 0.0f, 0.0f, 0.0f, \
                                       0.0f, 1.0f, 0.0f, 0.0f, \
                                       0.0f, 0.0f, 1.0f, 0.0f, \
                                       0.0f, 0.0f, 0.0f, 1.0f})

BLXINLINE blxVec3 blxMat4_MultVec3(blxMat4 mat, blxVec3 vec, float last)
{
    blxVec3 result;
    result.x = mat.m00 * vec.x + mat.m01 * vec.y + mat.m02 * vec.z + mat.m03 * last;
    result.y = mat.m10 * vec.x + mat.m11 * vec.y + mat.m12 * vec.z + mat.m13 * last;
    result.z = mat.m20 * vec.x + mat.m21 * vec.y + mat.m22 * vec.z + mat.m23 * last;
    return result;
}

BLXINLINE blxMat4 blxMat4_Mult(blxMat4 a, blxMat4 b)
{
    blxMat4 result;

    result.raw[0][0] = a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02 + a.m30 * b.m03;
    result.raw[0][1] = a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02 + a.m31 * b.m03;
    result.raw[0][2] = a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02 + a.m32 * b.m03;
    result.raw[0][3] = a.m03 * b.m00 + a.m13 * b.m01 + a.m23 * b.m02 + a.m33 * b.m03;
    result.raw[1][0] = a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12 + a.m30 * b.m13;
    result.raw[1][1] = a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13;
    result.raw[1][2] = a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13;
    result.raw[1][3] = a.m03 * b.m10 + a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13;
    result.raw[2][0] = a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22 + a.m30 * b.m23;
    result.raw[2][1] = a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23;
    result.raw[2][2] = a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23;
    result.raw[2][3] = a.m03 * b.m20 + a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23;
    result.raw[3][0] = a.m00 * b.m30 + a.m10 * b.m31 + a.m20 * b.m32 + a.m30 * b.m33;
    result.raw[3][1] = a.m01 * b.m30 + a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33;
    result.raw[3][2] = a.m02 * b.m30 + a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33;
    result.raw[3][3] = a.m03 * b.m30 + a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33;

    return result;
}

BLXINLINE blxMat4 blxMat4_Inverse(blxMat4 mat)
{
    blxMat4 result;
    float det = mat.m00 * (mat.m11 * mat.m22 - mat.m12 * mat.m21) -
        mat.m01 * (mat.m10 * mat.m22 - mat.m12 * mat.m20) +
        mat.m02 * (mat.m10 * mat.m21 - mat.m11 * mat.m20);

    if (det == 0.0f) {
        // Matrix is singular, return identity matrix
        return BLX_MAT4_IDENTITY;
    }

    float invDet = 1.0f / det;

    result.raw[0][0] = (mat.m11 * mat.m22 - mat.m12 * mat.m21) * invDet;
    result.raw[0][1] = (mat.m02 * mat.m21 - mat.m01 * mat.m22) * invDet;
    result.raw[0][2] = (mat.m01 * mat.m12 - mat.m02 * mat.m11) * invDet;
    result.raw[0][3] = 0.0f;
    result.raw[1][0] = (mat.m12 * mat.m20 - mat.m10 * mat.m22) * invDet;
    result.raw[1][1] = (mat.m00 * mat.m22 - mat.m02 * mat.m20) * invDet;
    result.raw[1][2] = (mat.m02 * mat.m10 - mat.m00 * mat.m12) * invDet;
    result.raw[1][3] = 0.0f;
    result.raw[2][0] = (mat.m10 * mat.m21 - mat.m11 * mat.m20) * invDet;
    result.raw[2][1] = (mat.m01 * mat.m20 - mat.m00 * mat.m21) * invDet;
    result.raw[2][2] = (mat.m00 * mat.m11 - mat.m01 * mat.m10) * invDet;
    result.raw[2][3] = 0.0f;
    result.raw[3][0] = 0.0f;
    result.raw[3][1] = 0.0f;
    result.raw[3][2] = 0.0f;
    result.raw[3][3] = 1.0f;

    return result;
}

BLXINLINE blxMat4 blxMat4_Transpose(blxMat4 mat)
{
    blxMat4 result;

    result.raw[0][0] = mat.raw[0][0];
    result.raw[0][1] = mat.raw[1][0];
    result.raw[0][2] = mat.raw[2][0];
    result.raw[0][3] = mat.raw[3][0];
    result.raw[1][0] = mat.raw[0][1];
    result.raw[1][1] = mat.raw[1][1];
    result.raw[1][2] = mat.raw[2][1];
    result.raw[1][3] = mat.raw[3][1];
    result.raw[2][0] = mat.raw[0][2];
    result.raw[2][1] = mat.raw[1][2];
    result.raw[2][2] = mat.raw[2][2];
    result.raw[2][3] = mat.raw[3][2];
    result.raw[3][0] = mat.raw[0][3];
    result.raw[3][1] = mat.raw[1][3];
    result.raw[3][2] = mat.raw[2][3];
    result.raw[3][3] = mat.raw[3][3];

    return result;
}