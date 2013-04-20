#ifndef SRC_TRIANGLE_H
#define SRC_TRIANGLE_H


#include <vector>

#include "config.h"

#include "point.h"
#include "shapes.h"
#include "vector.h"


BEGIN_RAYTRACER


class Intercept;
class Material;
class Transform;
class Ray;


class Triangle: public Plane {
  private:
    unsigned char dominantIndex;

  public:
    Point3D vertex1,
            vertex2,
            vertex3;

    Triangle(Material * material,
             const Point3D &pointA, const Point3D &pointB,
             const Point3D &pointC):
      Plane(material), vertex1(pointA), vertex2(pointB), vertex3(pointC) {
      normal = crossProduct(vertex2 - vertex1, vertex3 - vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(vertex1, normal);
    };

    Triangle(Material * material, Texture * texture,
             const Point3D &pointA, const Point3D &pointB,
             const Point3D &pointC):
      Plane(material, texture),
      vertex1(pointA), vertex2(pointB), vertex3(pointC) {
      normal = crossProduct(vertex2 - vertex1, vertex3 - vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(vertex1, normal);
    };

    virtual ~Triangle() {}

    void buildBoundingBox();
    virtual int intersect(const Ray &ray, Intercept intercepts[]) const;
    std::vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform);
    virtual Primitive * interpolate(Primitive * primitive,
                                    const P_FLT ratio) const;
};


class TexturedTriangle: public Triangle {
  public:
    Vector2D vertexTexCoord1,
             vertexTexCoord2,
             vertexTexCoord3;

    TexturedTriangle(Material * material, Texture * texture,
                     const Point3D &pointA, const Point3D &pointB,
                     const Point3D &pointC, const Vector2D &texCoordA,
                     const Vector2D &texCoordB, const Vector2D &texCoordC):
      Triangle(material, texture, pointA, pointB, pointC),
      vertexTexCoord1(texCoordA), vertexTexCoord2(texCoordB),
      vertexTexCoord3(texCoordC) {}

    ~TexturedTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTexColor(const std::vector<P_FLT> mapping) const;
    Primitive * interpolate(Primitive * primitive, const P_FLT ratio) const;
};


class TrianglePatch: public Triangle {
  public:
    Vector3D vertexNormal1,
             vertexNormal2,
             vertexNormal3;

    TrianglePatch(Material * material,
                  const Point3D &pointA, const Point3D &pointB,
                  const Point3D &pointC, const Vector3D &normalA,
                  const Vector3D &normalB, const Vector3D &normalC):
      Triangle(material, pointA, pointB, pointC),
      vertexNormal1(normalA), vertexNormal2(normalB), vertexNormal3(normalC) {}

    ~TrianglePatch() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Primitive * interpolate(Primitive * primitive, const P_FLT ratio) const;
    void transform(Transform * transform);
};


// TODO(kent): Is there a better name that isn't "TexturedTrianglePatch"?
// TODO(kent): Do this without virtual inheritance and deadly diamond
class PhongTriangle: public Triangle {
  public:
    Vector2D vertexTexCoord1,
             vertexTexCoord2,
             vertexTexCoord3;
    Vector3D vertexNormal1,
             vertexNormal2,
             vertexNormal3;

    PhongTriangle(Material * material, Texture * texture,
                  const Point3D &pointA, const Point3D &pointB,
                  const Point3D &pointC, const Vector3D &normalA,
                  const Vector3D &normalB, const Vector3D &normalC,
                  const Vector2D &texCoordA, const Vector2D &texCoordB,
                  const Vector2D &texCoordC):
      Triangle(material, texture, pointA, pointB, pointC),
      vertexNormal1(normalA), vertexNormal2(normalB), vertexNormal3(normalC),
      vertexTexCoord1(texCoordA), vertexTexCoord2(texCoordB),
      vertexTexCoord3(texCoordC) {}

    ~PhongTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTexColor(const std::vector<P_FLT> mapping) const;
    Primitive * interpolate(Primitive * primitive, const P_FLT ratio) const;
    void transform(Transform * transform);
};


END_RAYTRACER


#endif  // SRC_TRIANGLE_H
