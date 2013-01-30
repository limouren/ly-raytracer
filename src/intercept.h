#ifndef INTERSECTION_H
#define INTERSECTION_H


#include "config.h"


BEGIN_RAYTRACER


// TODO(kent): Refactor and remove forward declaration
class Material;
class Primitive;


class Intercept {
  public:
    bool enter;
    P_FLT t;

    Material * material;
    Primitive * primitive;

    Intercept() {}
    Intercept(P_FLT t, bool enter):
      t(t), enter(enter), material(NULL), primitive(NULL) {}
};


END_RAYTRACER


#endif
