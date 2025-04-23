#pragma once
#include "core/blx_logger.h"
#include "core/blx_string.h"

///@brief Expects value to be equal to expected.
///@param value The value to check.
///@param expected The expected value.
#define testShould_Be(value, expected) \
{\
    if (value != expected) {\
        BLXERROR("Test failed! Expected: %d, but got: %d at %s:%d", expected, value, __FILE__, __LINE__);\
        return BLX_FALSE;\
    }\
}

///@brief Expects value to be not equal to expected.
///@param value The value to check.
///@param expected The expected value.
#define testShould_NotBe(value, expected) \
{\
    if (value == expected) {\
        BLXERROR("Test failed! both value and expected are equal: %d at %s:%d", expected, value, __FILE__, __LINE__);\
        return BLX_FALSE;\
    }\
}

///@brief Does a String comparison and Expects value to be equal to expected. 
///Both Strings should be null terminated. This function is case sensitive.
///@param value The value to check.
///@param expected The expected value.
#define testShould_StringEqual(value, expected) \
{\
    if (!blxStr_Cmp(value, expected)) {\
        BLXERROR("Test failed! Expected: %s, but got: %s at %s:%d", expected, value, __FILE__, __LINE__);\
        return BLX_FALSE;\
    }\
}

///@brief Expects value to be true.
///@param value The value to check.
#define testShould_BeTrue(value) \
{\
    if (value != BLX_TRUE) {\
        BLXERROR("Test failed! Expected value to be true, but got false instead at %s:%d", __FILE__, __LINE__);\
        return BLX_FALSE;\
    }\
}

///@brief Expects value to be false.
///@param value The value to check.
#define testShould_BeFalse(value) \
{\
    if (value != BLX_FALSE) {\
        BLXERROR("Test failed! Expected value to be false, but got true instead at %s:%d", __FILE__, __LINE__);\
        return BLX_FALSE;\
    }\
}