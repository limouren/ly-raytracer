#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"

BEGIN_RAYTRACER


class Ray {
  public:
    Vector orig, dir;

    Ray(): orig(Vector()), dir(Vector()) {}

    Ray(Vector orig, Vector dir): orig(orig), dir(dir) {}

    Ray& operator =(const Ray &ray) {
      if (this == &ray) {
        return *this;
      }

      orig = ray.orig;
      dir = ray.dir;

      return *this;
    }

    Point ray_point(P_FLT t) {
      Vector point_vector = orig + (dir * t);
      return Point(point_vector);
    }
};


END_RAYTRACER


#endif
