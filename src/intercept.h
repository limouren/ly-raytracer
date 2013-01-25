#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "config.h"


BEGIN_RAYTRACER


// TODO: Refactor and remove forward declaration
class Primitive;


class Intercept {
  public:
    bool enter;
    P_FLT t;
    Primitive * primitive;

    Intercept() {}
    Intercept(P_FLT t, bool enter): t(t), enter(enter) {}
};


END_RAYTRACER


#endif
