#pragma once
#include "Defines.h"

typedef struct {
    double startTime;
    double elaspedTime;
}blxClock;

void blxUpdateClock(blxClock* clock);

void blxStartClock(blxClock* clock);

void blxStopClock(blxClock* clock);