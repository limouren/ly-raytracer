#ifndef MESH_H
#define MESH_H


#include <vector>

#include "config.h"

#include "point.h"
#include "primitive.h"
#include "tree.h"
#include "vector.h"


BEGIN_RAYTRACER


class Intercept;
class Material;
class Ray;
class Texture;
class Transform;


class TriangleMesh: public Primitive {
  private:
    Node * triangleTree;
    unsigned int pointNum, normalNum, texCoordNum;
    Point3D * points;
    Vector3D * normals;
    Vector2D * texCoords;
    std::vector<Primitive *> triangles;

  public:
    // This constructor designed according to BART specifications
    TriangleMesh(Material * material,
                 Texture * texture,
                 std::vector<Point3D> _points,
                 std::vector<Vector3D> _normals,
                 std::vector<Vector2D> _texCoords,
                 std::vector<int *> triangleDefs):
      Primitive(material, texture), triangleTree(NULL) {
      pointNum = _points.size();
      points = new Point3D[pointNum];
      for (int i = 0; i < pointNum; i++) {
        points[i] = _points[i];
      }

      normalNum = _normals.size();
      normals = new Vector3D[normalNum];
      for (int i = 0; i < normalNum; i++) {
        normals[i] = _normals[i];
      }

      texCoordNum = _texCoords.size();
      texCoords = new Vector2D[texCoordNum];
      for (int i = 0; i < texCoordNum; i++) {
        texCoords[i] = _texCoords[i];
      }

      constructTriangles(triangleDefs);
    }

    ~TriangleMesh() {
      delete [] normals;
      delete [] points;
      delete [] texCoords;

      if (triangleTree != NULL && triangleTree->type) {
        delete triangleTree;
      }
      while (!triangles.empty()) {
        delete triangles.back();
        triangles.pop_back();
      }
    }

    void buildBoundingBox();
    void constructTriangles(const std::vector<int *> &triangleDefs);
    void init();
    int intersect(const Ray &ray, Intercept intercepts[]) const;
    void transform(Transform * transform);
};


END_RAYTRACER


#endif
