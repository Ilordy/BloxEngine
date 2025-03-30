#include "blx_clock.h"
#include "internal/platform/platform.h"

void blxUpdateClock(blxClock* clock)
{
    if (clock->startTime != 0) {
        clock->elaspedTime = blxPlatform_GetTime() - clock->startTime;
    }
}

void blxStartClock(blxClock* clock)
{
    clock->startTime = blxPlatform_GetTime();
    clock->elaspedTime = 0;
}

void blxStopClock(blxClock* clock)
{
    clock->startTime = 0;
}
