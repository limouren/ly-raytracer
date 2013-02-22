#ifndef VOLUME_H
#define VOLUME_H


#include "config.h"


#include "point.h"


BEGIN_RAYTRACER


class Ray;


class BoundingBox {
  public:
    Point3D minExt, maxExt, center;

    BoundingBox(const P_FLT minX, const P_FLT minY, const P_FLT minZ,
                const P_FLT maxX, const P_FLT maxY, const P_FLT maxZ):
      minExt(Point3D(minX, minY, minZ)),
      maxExt(Point3D(maxX, maxY, maxZ)) {
      center = (minExt + maxExt) * 0.5f;
    }

    BoundingBox(const Point3D &minExt, const Point3D &maxExt):
      minExt(minExt), maxExt(maxExt), center((minExt + maxExt) * 0.5f) {}

    inline bool intersect(const Ray &ray) const;
    P_FLT surfaceArea() const {
      return (maxExt - minExt).boxArea();
    }
};


END_RAYTRACER


#endif
