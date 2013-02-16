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
    BoundingVolume() {}

    virtual void getBox(Point3D * minExt, Point3D * maxExt) const {}
    virtual const bool intersect(const Ray &ray) const {}

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

    void getBox(Point3D * minExt, Point3D * maxExt) const {
      *minExt = this->minExt;
      *maxExt = this->maxExt;
    }


    inline const bool intersect(const Ray &ray) const;
};


END_RAYTRACER


#endif
