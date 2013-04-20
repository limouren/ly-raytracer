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
    Intercept(const P_FLT t, const bool enter, Material * material,
              const Primitive * primitive):
      enter(enter), t(t), material(material),
      primitive(const_cast<Primitive *>(primitive)) {}
};


END_RAYTRACER


#endif
