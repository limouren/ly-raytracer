#ifndef SOLID_H
#define SOLID_H


#include "config.h"
#include "material.h"
#include "surface.h"


BEGIN_RAYTRACER


// Ref: An Introduction to Ray Tracing; A.S. Glassner (1989)
class MODEL_CLS {
  public:
    int composite_flag;

    MODEL_CLS(int composite_flag): composite_flag(composite_flag) {}
};


class Composite: MODEL_CLS {
  public:
    int op;
    struct MODEL_CLS * left;
    struct MODEL_CLS * right;

    Composite(): MODEL_CLS(1) {}
};


class Primitive: MODEL_CLS {
  public:
    Material * material;
    Surface * surface;

    Primitive(): MODEL_CLS(0) {}
};


END_RAYTRACER


#endif
