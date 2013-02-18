#ifndef TRIANGLE_H
#define TRIANGLE_H


#include <vector>

#include "config.h"

#include "intercept.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"
#include "vector.h"


BEGIN_RAYTRACER


class Triangle: public Plane {
  private:
    unsigned char dominantIndex;

  public:
    Point3D * vertex1,
            * vertex2,
            * vertex3;

    Triangle(Material * material,
             Point3D * pointA, Point3D * pointB, Point3D * pointC):
      Plane(material) {
      vertex1 = pointA;
      vertex2 = pointB;
      vertex3 = pointC;

      normal = crossProduct(*vertex2 - *vertex1, *vertex3 - *vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(*vertex1, normal);

      buildBoundingVolume();
    };

    Triangle(Material * material, Texture * texture,
             Point3D * pointA, Point3D * pointB, Point3D * pointC):
      Plane(material, texture) {
      vertex1 = pointA;
      vertex2 = pointB;
      vertex3 = pointC;

      normal = crossProduct(*vertex2 - *vertex1, *vertex3 - *vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(*vertex1, normal);

      buildBoundingVolume();
    };

    virtual ~Triangle() {
      delete boundingVolume;
    }

    void buildBoundingVolume();
    int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    std::vector<P_FLT> inverseMap(const Point3D &point) const;
};


class TexturedTriangle: public Triangle {
  public:
    Vector2D * vertexTextureCoord1,
             * vertexTextureCoord2,
             * vertexTextureCoord3;

    TexturedTriangle(Material * material, Texture * texture,
                     Point3D * pointA, Point3D * pointB, Point3D * pointC,
                     Vector2D * textureCoordA, Vector2D * textureCoordB,
                     Vector2D * textureCoordC):
      Triangle(material, texture, pointA, pointB, pointC) {
      vertexTextureCoord1 = textureCoordA;
      vertexTextureCoord2 = textureCoordB;
      vertexTextureCoord3 = textureCoordC;
    }

    ~TexturedTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTextureColor(const std::vector<P_FLT> mapping) const;
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

    ~TrianglePatch() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
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

    PhongTriangle(Material * material, Texture * texture,
                  Point3D * pointA, Point3D * pointB, Point3D * pointC,
                  Vector3D * normalA, Vector3D * normalB, Vector3D * normalC,
                  Vector2D * textureCoordA, Vector2D * textureCoordB,
                  Vector2D * textureCoordC):
      Triangle(material, texture, pointA, pointB, pointC) {
      vertexNormal1 = normalA;
      vertexNormal2 = normalB;
      vertexNormal3 = normalC;
      vertexTextureCoord1 = textureCoordA;
      vertexTextureCoord2 = textureCoordB;
      vertexTextureCoord3 = textureCoordC;
    }

    ~PhongTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTextureColor(const std::vector<P_FLT> mapping) const;
};


END_RAYTRACER


#endif
