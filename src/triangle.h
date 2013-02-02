#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "config.h"

#include "intercept.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"


BEGIN_RAYTRACER


class Triangle: public Plane {
  public:
    Point3D vertex[3];

    Triangle(Material * material, const Point3D &pointA, const Point3D &pointB,
             const Point3D &pointC): Plane(material) {
      vertex[0] = pointA;
      vertex[1] = pointB;
      vertex[2] = pointC;

      normal = crossProduct(vertex[1] - vertex[0],
                            vertex[2] - vertex[0]);
      normal.normalize();

      d = -dotProduct(vertex[0], normal);
    };

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
};


END_RAYTRACER


#endif
