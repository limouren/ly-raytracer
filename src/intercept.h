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
    const Primitive * primitive;

    Intercept() {}
    Intercept(const P_FLT t, const bool enter, Material * material,
              const Primitive * primitive):
      t(t), enter(enter), material(material), primitive(primitive) {}
};


END_RAYTRACER


#endif
