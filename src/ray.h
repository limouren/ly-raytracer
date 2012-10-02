#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"

BEGIN_RAYTRACER


class Ray {
  public:
    Point orig;
    Vector dir;

    Ray(): orig(Point()), dir(Vector()) {}

    Ray(Point orig, Vector dir): orig(orig), dir(dir) {}

    Ray& operator =(const Ray &ray) {
      if (this == &ray) {
        return *this;
      }

      orig = ray.orig;
      dir = ray.dir;

      return *this;
    }

    Point ray_point(P_FLT t) {
      return orig + (dir * t);
    }
};


END_RAYTRACER


#endif
