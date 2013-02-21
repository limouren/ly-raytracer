#ifndef SRC_UTILS_H
#define SRC_UTILS_H


#include "time.h"

#include "config.h"


BEGIN_RAYTRACER


const float SecsPerClock = 1.0f / (static_cast<float>(threadNum) *
                                   static_cast<float>(CLOCKS_PER_SEC));


float clockTime(clock_t startTimer, clock_t endTimer) {
  return static_cast<float>(endTimer - startTimer) * SecsPerClock;
}


END_RAYTRACER


#endif
