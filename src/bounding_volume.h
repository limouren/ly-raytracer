#ifndef VOLUME_H
#define VOLUME_H


#include "config.h"

#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


class BoundingVolume {
  public:
    virtual const int intersect(const Ray &ray, Intercept intercepts[]) {}
    virtual const Vector3D normalAt(const Point3D &point) {}

    BoundingVolume() {}

    virtual ~BoundingVolume() {}
};


class Box: public BoundingVolume {
  public:
    Point3D minExt, maxExt;

    Box(P_FLT min_x, P_FLT min_y, P_FLT min_z,
        P_FLT max_x, P_FLT max_y, P_FLT max_z):
      minExt(Point3D(min_x, min_y, min_z)),
      maxExt(Point3D(max_x, max_y, max_z)) {}

    Box(const Point3D &minExt, const Point3D &maxExt):
      minExt(minExt), maxExt(maxExt), BoundingVolume() {}

    const int intersect(const Ray &ray, Intercept intercepts[]);
    const Vector3D normalAt(const Point3D &point);
};


END_RAYTRACER


#endif
