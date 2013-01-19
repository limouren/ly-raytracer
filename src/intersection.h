#ifndef INTERSECTION_H
#define #INTERSECTION_H


#include "config.h"
#include "material.h"
#include "solid.h"
#include "vector.h"


BEGIN_RAYTRACER


class Intersection {
  public:
    P_FLT t;
    Solid * solid;
    bool enter;
    Material * material;
};


END_RAYTRACER


#endif
