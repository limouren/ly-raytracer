#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"

BEGIN_RAYTRACER


class Ray {
  public:
    Point orig;
    Vector3D dir;

    Ray(): orig(Point()), dir(Vector3D()) {}

    Ray(Point orig, Vector3D dir): orig(orig), dir(dir) {}

    Ray& operator =(const Ray &ray) {
      if (this == &ray) {
        return *this;
      }

      orig = ray.orig;
      dir = ray.dir;

      return *this;
    }

    const Point rayPoint(P_FLT t) const {
      return orig + (dir * t);
    }
};


END_RAYTRACER


#endif
