#pragma once
#include "core/blx_defines.h"

/// @brief A clock struct to keep track of time.
typedef struct {
    /// @brief The start time of the clock.
    double startTime;
    /// @brief The elapsed time of the clock.
    double elaspedTime;
}blxClock;

/// @brief Updates the clock (Call this at the end for timing).
/// @param clock 
void blxUpdateClock(blxClock* clock);

/// @brief Starts the clock.
/// @param clock The clock to start.
void blxStartClock(blxClock* clock);

/// @brief Stops the clock.
/// @param clock 
void blxStopClock(blxClock* clock);