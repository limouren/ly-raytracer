#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"

BEGIN_RAYTRACER


class Ray {
  public:
    Point3D orig;
    Vector3D dir;

    Ray(): orig(Point3D()), dir(Vector3D()) {}

    Ray(Point3D orig, Vector3D dir): orig(orig), dir(dir) {}

    inline Ray& operator =(const Ray &ray) {
      if (this == &ray) {
        return *this;
      }

      orig = ray.orig;
      dir = ray.dir;

      return *this;
    }

    inline const Point3D rayPoint(P_FLT t) const {
      return orig + (dir * t);
    }
};


END_RAYTRACER


#endif
