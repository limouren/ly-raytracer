#ifndef SRC_MESH_TRIANGLE_H
#define SRC_MESH_TRIANGLE_H


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


class MeshTriangle: public Plane {
  private:
    unsigned char dominantIndex;

  public:
    Point3D * vertex1,
            * vertex2,
            * vertex3;

    MeshTriangle(Material * material,
                 Point3D * pointA, Point3D * pointB, Point3D * pointC):
      Plane(material), vertex1(pointA), vertex2(pointB), vertex3(pointC) {
      normal = crossProduct(*vertex2 - *vertex1, *vertex3 - *vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(*vertex1, normal);
    };
    MeshTriangle(Material * material, Texture * texture,
                 Point3D * pointA, Point3D * pointB, Point3D * pointC):
      Plane(material, texture),
      vertex1(pointA), vertex2(pointB), vertex3(pointC) {
      normal = crossProduct(*vertex2 - *vertex1, *vertex3 - *vertex1);
      normal.normalize();

      dominantIndex = normal.dominantIndex();

      d = -dotProduct(*vertex1, normal);
    };

    virtual ~MeshTriangle() {}

    void buildBoundingBox();
    int intersect(const Ray &ray, Intercept intercepts[]) const;
    std::vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform) {
      buildBoundingBox();
    }
};


class TexMeshTriangle: public MeshTriangle {
  public:
    Vector2D * vertexTextureCoord1,
             * vertexTextureCoord2,
             * vertexTextureCoord3;

    TexMeshTriangle(Material * material, Texture * texture,
                    Point3D * pointA, Point3D * pointB, Point3D * pointC,
                    Vector2D * textureCoordA, Vector2D * textureCoordB,
                    Vector2D * textureCoordC):
      MeshTriangle(material, texture, pointA, pointB, pointC),
      vertexTextureCoord1(textureCoordA), vertexTextureCoord2(textureCoordB),
      vertexTextureCoord3(textureCoordC) {}

    ~TexMeshTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTextureColor(const std::vector<P_FLT> mapping) const;
};


class PatchMeshTriangle: public MeshTriangle {
  public:
    Vector3D * vertexNormal1,
             * vertexNormal2,
             * vertexNormal3;

    PatchMeshTriangle(Material * material,
                      Point3D * pointA, Point3D * pointB, Point3D * pointC,
                      Vector3D * normalA, Vector3D * normalB,
                      Vector3D * normalC):
      MeshTriangle(material, pointA, pointB, pointC),
      vertexNormal1(normalA), vertexNormal2(normalB), vertexNormal3(normalC) {}

    ~PatchMeshTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
};


// TODO(kent): Is there a better name that isn't "TexPatchMeshTriangle"?
// TODO(kent): Do this without virtual inheritance and deadly diamond
class PhongMeshTriangle: public MeshTriangle {
  public:
    Vector2D * vertexTextureCoord1,
             * vertexTextureCoord2,
             * vertexTextureCoord3;
    Vector3D * vertexNormal1,
             * vertexNormal2,
             * vertexNormal3;

    PhongMeshTriangle(Material * material, Texture * texture,
                      Point3D * pointA, Point3D * pointB, Point3D * pointC,
                      Vector3D * normalA, Vector3D * normalB,
                      Vector3D * normalC, Vector2D * textureCoordA,
                      Vector2D * textureCoordB, Vector2D * textureCoordC):
      MeshTriangle(material, texture, pointA, pointB, pointC),
      vertexNormal1(normalA), vertexNormal2(normalB), vertexNormal3(normalC),
      vertexTextureCoord1(textureCoordA), vertexTextureCoord2(textureCoordB),
      vertexTextureCoord3(textureCoordC) {}

    ~PhongMeshTriangle() {}

    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    Color getTextureColor(const std::vector<P_FLT> mapping) const;
};


END_RAYTRACER


#endif  // SRC_MESH_TRIANGLE_H
