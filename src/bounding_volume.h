#ifndef VOLUME_H
#define VOLUME_H


#include "config.h"


#include "point.h"


BEGIN_RAYTRACER


class Ray;


class BoundingBox {
  public:
    Point3D minExt, maxExt;

    BoundingBox(const P_FLT minX, const P_FLT minY, const P_FLT minZ,
                const P_FLT maxX, const P_FLT maxY, const P_FLT maxZ):
      minExt(Point3D(minX, minY, minZ)),
      maxExt(Point3D(maxX, maxY, maxZ)) {}

    BoundingBox(const Point3D &minExt, const Point3D &maxExt):
      minExt(minExt), maxExt(maxExt) {}

    inline bool intersect(const Ray &ray) const;
};


END_RAYTRACER


#endif
