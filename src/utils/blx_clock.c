#include "blx_clock.h"
#include "internal/platform/platform.h"

void blxUpdateClock(blxClock* clock)
{
    if (clock->startTime != 0) {
        clock->elaspedTime = PlatformGetTime() - clock->startTime;
    }
}

void blxStartClock(blxClock* clock)
{
    clock->startTime = PlatformGetTime();
    clock->elaspedTime = 0;
}

void blxStopClock(blxClock* clock)
{
    clock->startTime = 0;
}
