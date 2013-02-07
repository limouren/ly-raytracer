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
    Point3D * vertex1,
            * vertex2,
            * vertex3;

    Triangle(Material * material, Point3D * pointA, Point3D * pointB,
             Point3D * pointC): Plane(material) {
      vertex1 = pointA;
      vertex2 = pointB;
      vertex3 = pointC;

      normal = crossProduct(*vertex2 - *vertex1, *vertex3 - *vertex1);
      normal.normalize();

      d = -dotProduct(*vertex1, normal);
    };

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const;
};


class TexturedTriangle: public Triangle {
  public:
    Vector2D * vertexTextureCoord1,
             * vertexTextureCoord2,
             * vertexTextureCoord3;

    TexturedTriangle(Material * material,
                     Point3D * pointA, Point3D * pointB, Point3D * pointC,
                     Vector2D * textureCoordA, Vector2D * textureCoordB,
                     Vector2D * textureCoordC):
      Triangle(material, pointA, pointB, pointC) {
      vertexTextureCoord1 = textureCoordA;
      vertexTextureCoord2 = textureCoordB;
      vertexTextureCoord3 = textureCoordC;
    }

    TexturedTriangle(Material * material, Texture * texture,
                     Point3D * pointA, Point3D * pointB, Point3D * pointC,
                     Vector2D * textureCoordA, Vector2D * textureCoordB,
                     Vector2D * textureCoordC):
      Triangle(material, pointA, pointB, pointC) {
      texture = texture;
      vertexTextureCoord1 = textureCoordA;
      vertexTextureCoord2 = textureCoordB;
      vertexTextureCoord3 = textureCoordC;
    }

    const Vector2D getBarycentricCoord(const Point3D &point) const;
};


class TrianglePatch: public Triangle {
  public:
    Vector3D * vertexNormal1,
             * vertexNormal2,
             * vertexNormal3;

    TrianglePatch(Material * material,
                  Point3D * pointA, Point3D * pointB, Point3D * pointC,
                  Vector3D * normalA, Vector3D * normalB, Vector3D * normalC):
      Triangle(material, pointA, pointB, pointC) {
      vertexNormal1 = normalA;
      vertexNormal2 = normalB;
      vertexNormal3 = normalC;
    }

    const Vector3D normalAt(const Point3D &point) const;
    const Vector3D normalAt(const Vector2D &barycentricCoord) const;
};


// TODO(kent): Is there a better name that isn't "TexturedTrianglePatch"?
// TODO(kent): Do this without virtual inheritance and deadly diamond
class PhongTriangle: public Triangle {
  public:
    Vector2D * vertexTextureCoord1,
             * vertexTextureCoord2,
             * vertexTextureCoord3;
    Vector3D * vertexNormal1,
             * vertexNormal2,
             * vertexNormal3;

    PhongTriangle(Material * material, Point3D * pointA, Point3D * pointB,
                  Point3D * pointC, Vector3D * normalA, Vector3D * normalB,
                  Vector3D * normalC, Vector2D * textureCoordA,
                  Vector2D * textureCoordB, Vector2D * textureCoordC):
      Triangle(material, pointA, pointB, pointC) {
      vertexNormal1 = normalA;
      vertexNormal2 = normalB;
      vertexNormal3 = normalC;
      vertexTextureCoord1 = textureCoordA;
      vertexTextureCoord2 = textureCoordB;
      vertexTextureCoord3 = textureCoordC;
    }

    const Vector2D getBarycentricCoord(const Point3D &point) const;
    // const Vector3D normalAt(const Point3D &point) const;
    // const Vector3D normalAt(const Vector2D &barycentricCoord) const;
};


END_RAYTRACER


#endif
