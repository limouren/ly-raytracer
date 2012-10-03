#ifndef SURFACE_H
#define SURFACE_H

#include "config.h"

#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


class Surface {
  public:
    int intersect(Ray &ray, P_FLT &intersects);
    bool contains(Point &point);
    const Vector normalAt(Point &point);
};


class Sphere: Surface {
  public:
    P_FLT radius;
    Point * center;

    Sphere(P_FLT x, P_FLT y, P_FLT z, P_FLT radius): radius(radius) {
      // TODO: Clean up this new point in destructor
      center = new Point(x, y, z);
    }
    Sphere(Point &center, P_FLT radius): center(&center), radius(radius) {}

    int intersect(Ray &ray, P_FLT ** intersects);
    const Vector normalAt(Point &point);
};


END_RAYTRACER


#endif
