#ifndef SOLID_H
#define SOLID_H


#include "config.h"
#include "material.h"
#include "surface.h"


BEGIN_RAYTRACER


// Ref: An Introduction to Ray Tracing; A.S. Glassner (1989)
class Solid {
  public:
    int composite_flag;

    Solid(int composite_flag): composite_flag(composite_flag) {}
};


class Composite: Solid {
  public:
    int op;
    struct Solid * left;
    struct Solid * right;

    Composite(): Solid(1) {}
};


class Primitive: Solid {
  public:
    Material * material;
    Surface * surface;

    Primitive(): Solid(0) {}
};


END_RAYTRACER


#endif
