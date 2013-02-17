#ifndef MESH_H
#define MESH_H


#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "model.h"
#include "point.h"
#include "ray.h"
#include "texture.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


class Intercept;


class TriangleMesh: public Primitive {
  private:
    MODEL_CLS * triangleTree;
    int pointNum, normalNum, textureCoordNum;
    Point3D * points;
    Vector3D * normals;
    Vector2D * textureCoords;
    std::vector<MODEL_CLS *> triangles;

  public:
    // This constructor designed according to BART specifications
    TriangleMesh(Material * material,
                 Texture * texture,
                 std::vector<Point3D> _points,
                 std::vector<Vector3D> _normals,
                 std::vector<Vector2D> _textureCoords,
                 std::vector<int *> triangleDefs):
      Primitive(material, texture) {
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

      textureCoordNum = _textureCoords.size();
      textureCoords = new Vector2D[textureCoordNum];
      for (int i = 0; i < textureCoordNum; i++) {
        textureCoords[i] = _textureCoords[i];
      }

      buildBoundingVolume();
      constructTriangles(triangleDefs);
    }

    inline void buildBoundingVolume();

    inline void constructTriangles(const std::vector<int *> &triangleDefs);

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    const Vector3D normalAt(const Point3D &point) {
      // This shouldn't be getting called.
      printf("ERROR: Calling unimplemented TriangleMesh normalAt.\n");
      exit(1);
    }

    ~TriangleMesh() {
      delete boundingVolume;
      delete [] normals;
      delete [] points;
      delete [] textureCoords;

      if (triangleTree != NULL && triangleTree->type != 0) {
        delete triangleTree;
      }
      while (!triangles.empty()) {
        delete triangles.back();
        triangles.pop_back();
      }
    }
};


END_RAYTRACER


#endif
