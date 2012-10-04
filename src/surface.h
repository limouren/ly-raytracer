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
    const Vector normalAt(Point &point);
};

// Simple surfaces start here
class Triangle {
  public:
    Point v1, v2, v3; // Vertices
    Vector norm; // Precomputed

    Triangle(Point &v1, Point &v2, Point &v3);
    Triangle(P_FLT x1, P_FLT y1, P_FLT x2, P_FLT y2, P_FLT x3, P_FLT y3);

    int intersect(Ray &ray, P_FLT &intersects);
    const Vector normalAt(Point &point);
};

// Quadratic surfaces start here

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
