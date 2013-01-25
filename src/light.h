#ifndef LIGHT_H
#define LIGHT_H

#include "config.h"

#include "color.h"
#include "point.h"


BEGIN_RAYTRACER

class Light {
  public:
    Color color;
    Point orig;

    Light(Point orig, Color color):
      color(color), orig(orig) {}
};


END_RAYTRACER


#endif
