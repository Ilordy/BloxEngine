#pragma once
#include "core/blx_defines.h"
#include "maths/mathTypes.h"

// TODO: Look into SIMD and SIMD operations for performance improvements.

#define BLX_MAT3_ZERO ((blxMat3){0.0f, 0.0f, 0.0f, \
                               0.0f, 0.0f, 0.0f, \
                               0.0f, 0.0f, 0.0f})

#define BLX_MAT3_IDENTITY ((blxMat3){1.0f, 0.0f, 0.0f, \
                                    0.0f, 1.0f, 0.0f, \
                                    0.0f, 0.0f, 1.0f})

BLXINLINE blxVec3 blxMat3_MultVec3(blxMat3 mat, blxVec3 vec) {
    blxVec3 result;
    result.x = mat.m00 * vec.x + mat.m01 * vec.y + mat.m02 * vec.z;
    result.y = mat.m10 * vec.x + mat.m11 * vec.y + mat.m12 * vec.z;
    result.z = mat.m20 * vec.x + mat.m21 * vec.y + mat.m22 * vec.z;
    return result;
}

BLXINLINE blxMat3 blxMat3_Mult(blxMat3 a, blxMat3 b) {
    blxMat3 result;

    result.raw[0][0] = a.m00 * b.m00 + a.m10 * b.m01 + a.m20 * b.m02;
    result.raw[0][1] = a.m01 * b.m00 + a.m11 * b.m01 + a.m21 * b.m02;
    result.raw[0][2] = a.m02 * b.m00 + a.m12 * b.m01 + a.m22 * b.m02;
    result.raw[1][0] = a.m00 * b.m10 + a.m10 * b.m11 + a.m20 * b.m12;
    result.raw[1][1] = a.m01 * b.m10 + a.m11 * b.m11 + a.m21 * b.m12;
    result.raw[1][2] = a.m02 * b.m10 + a.m12 * b.m11 + a.m22 * b.m12;
    result.raw[2][0] = a.m00 * b.m20 + a.m10 * b.m21 + a.m20 * b.m22;
    result.raw[2][1] = a.m01 * b.m20 + a.m11 * b.m21 + a.m21 * b.m22;
    result.raw[2][2] = a.m02 * b.m20 + a.m12 * b.m21 + a.m22 * b.m22;

    return result;
}

BLXINLINE blxMat3 blxMat3_Inverse(blxMat3 mat) {
    blxMat3 result;
    //float det = 


    float det = mat.m00 * (mat.m11 * mat.m22 - mat.m12 * mat.m21) -
                mat.m01 * (mat.m10 * mat.m22 - mat.m12 * mat.m20) +
                mat.m02 * (mat.m10 * mat.m21 - mat.m11 * mat.m20);

    if (det == 0.0f) {
        // Matrix is singular, return identity matrix
        return BLX_MAT3_IDENTITY;
    }

    float invDet = 1.0f / det;

    result.raw[0][0] = (mat.m11 * mat.m22 - mat.m12 * mat.m21) * invDet;
    result.raw[0][1] = (mat.m02 * mat.m21 - mat.m01 * mat.m22) * invDet;
    result.raw[0][2] = (mat.m01 * mat.m12 - mat.m02 * mat.m11) * invDet;
    result.raw[1][0] = (mat.m12 * mat.m20 - mat.m10 * mat.m22) * invDet;
    result.raw[1][1] = (mat.m00 * mat.m22 - mat.m02 * mat.m20) * invDet;
    result.raw[1][2] = (mat.m02 * mat.m10 - mat.m00 * mat.m12) * invDet;
    result.raw[2][0] = (mat.m10 * mat.m21 - mat.m11 * mat.m20) * invDet;
    result.raw[2][1] = (mat.m01 * mat.m20 - mat.m00 * mat.m21) * invDet;
    result.raw[2][2] = (mat.m00 * mat.m11 - mat.m01 * mat.m10) * invDet;

    return result;
}

BLXINLINE blxMat3 blxMat3_Transpose(blxMat3 mat) {
    blxMat3 result;
    
    result.raw[0][0] = mat.raw[0][0];
    result.raw[0][1] = mat.raw[1][0];
    result.raw[0][2] = mat.raw[2][0];
    result.raw[1][0] = mat.raw[0][1];
    result.raw[1][1] = mat.raw[1][1];
    result.raw[1][2] = mat.raw[2][1];
    result.raw[2][0] = mat.raw[0][2];
    result.raw[2][1] = mat.raw[1][2];
    result.raw[2][2] = mat.raw[2][2];

    return result;
}