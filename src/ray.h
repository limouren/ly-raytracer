#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"
#include "material.h"


BEGIN_RAYTRACER


class Ray {
  public:
    Material * medium;
    Point3D orig;
    Vector3D dir;

    Ray() {}
    Ray(Point3D orig, Vector3D dir, Material * medium):
      orig(orig), dir(dir), medium(medium) {}

    inline Point3D rayPoint(P_FLT t) const {
      return orig + (dir * t);
    }
};


END_RAYTRACER


#endif
