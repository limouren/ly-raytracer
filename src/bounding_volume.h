#ifndef VOLUME_H
#define VOLUME_H


#include "config.h"

#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


Vector3D epsilonVector(P_FLT_EPSILON);


class Ray;


class BoundingBox {
  public:
    Point3D minExt, maxExt;

    BoundingBox(const P_FLT minX, const P_FLT minY, const P_FLT minZ,
                const P_FLT maxX, const P_FLT maxY, const P_FLT maxZ):
      minExt(Point3D(minX, minY, minZ) - epsilonVector),
      maxExt(Point3D(maxX, maxY, maxZ) + epsilonVector) {
    }

    BoundingBox(const Point3D &minExt, const Point3D &maxExt):
      minExt(minExt - epsilonVector), maxExt(maxExt + epsilonVector) {}

    inline Point3D center() {
      return (minExt + maxExt) * 0.5f;
    }

    inline bool intersect(const Ray &ray, P_FLT tCeil) const;
};


END_RAYTRACER


#endif
