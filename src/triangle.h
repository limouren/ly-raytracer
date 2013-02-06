#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "config.h"

#include "intercept.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"
#include "vector.h"


BEGIN_RAYTRACER


class Triangle: public Plane {
  public:
    Point3D * vertex[3];

    Triangle(Material * material, Point3D * pointA, Point3D * pointB,
             Point3D * pointC): Plane(material) {
      vertex[0] = pointA;
      vertex[1] = pointB;
      vertex[2] = pointC;

      normal = crossProduct(*vertex[1] - *vertex[0],
                            *vertex[2] - *vertex[0]);
      normal.normalize();

      d = -dotProduct(*vertex[0], normal);
    };

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const;
};


class TexturedTriangle: public Triangle {
  public:
    Vector2D vertexTextureCoord[3];

    const Vector2D getBarycentrCoord(const Point3D &point) const;
};


class TrianglePatch: public Triangle {
  public:
    Vector3D * vertexNormal[3];

    TrianglePatch(Material * material, Point3D * pointA, Point3D * pointB,
                  Point3D * pointC, Vector3D * normalA, Vector3D * normalB,
                  Vector3D * normalC):
      Triangle(material, pointA, pointB, pointC) {
      vertexNormal[0] = normalA;
      vertexNormal[1] = normalB;
      vertexNormal[2] = normalC;
    }

    const Vector3D normalAt(const Point3D &point) const;
    const Vector3D normalAt(const Vector2D &barycentricCoord) const;
};


// TODO(kent): Is there a better name that isn't "TexturedTrianglePatch"?
// TODO(kent): Do this without virtual inheritance and deadly diamond
class PhongTriangle: public TexturedTriangle, public TrianglePatch {
};


END_RAYTRACER


#endif
