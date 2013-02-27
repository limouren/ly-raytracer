#ifndef SRC_UTILS_H
#define SRC_UTILS_H


#include "time.h"

#include "config.h"


BEGIN_RAYTRACER


float SecsPerClock = 1.0f / (threadNum * CLOCKS_PER_SEC);
float SecsPerClockMT = 1.0f / (static_cast<float>(threadNum) *
                               static_cast<float>(CLOCKS_PER_SEC));


float clockTime(clock_t startTimer, clock_t endTimer) {
  return static_cast<float>(endTimer - startTimer) * SecsPerClock;
}
float clockTimeMT(clock_t startTimer, clock_t endTimer) {
  return static_cast<float>(endTimer - startTimer) * SecsPerClockMT;
}


END_RAYTRACER


#endif
