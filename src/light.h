#ifndef LIGHT_H
#define LIGHT_H

#include "config.h"

#include "color.h"
#include "point.h"


BEGIN_RAYTRACER

class Light {
  public:
    C_FLT intensity; // 0.0 - 1.0
    Color color;
    Point orig;
};


END_RAYTRACER


#endif
