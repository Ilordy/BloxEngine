#pragma once
#include "core/blx_defines.h"

// TODO: THIS FILE IS SUBJECT TO A NAME CHANGE!

namespace BlxMath {
    /**
     * @brief Clamp an integer value to be no less than zero.
     * 
     * @param value The integer value to clamp.
     * @return 0 if value is less than 0, otherwise returns value.
     */
    BLXINLINE int ClampZero(int value) {
        return value < 0 ? 0 : value;
    }
}