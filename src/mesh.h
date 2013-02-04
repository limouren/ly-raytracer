#ifndef MESH_H
#define MESH_H


#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "model.h"
#include "point.h"
#include "ray.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


class Intercept;


class TriangleMesh: public Primitive {
  private:
    BoundingVolume * boundingVolume;
    MODEL_CLS * triangles;
    std::vector<Point3D> points;

  public:
    // This constructor designed according to BART specifications
    TriangleMesh(Material * material,
                 std::vector<Point3D> points,
                 std::vector<Vector3D> normals,
                 std::vector<Vector2D> textureCoords,
                 std::vector<int *> triangleDefs):
      points(points) {
      buildBoundingVollume(points);

      // TODO(kent): Handle textured triangles
      std::vector<MODEL_CLS *> triangleVector;
      for (int i = 0; i < triangleDefs.size(); i++) {
        Triangle * triangle = new Triangle(material,
                                           &points[triangleDefs[i][0]],
                                           &points[triangleDefs[i][1]],
                                           &points[triangleDefs[i][2]]);
        triangleVector.push_back(triangle);
      }
      triangles = buildModelTree(triangleVector);
    }

    void buildBoundingVollume(std::vector<Point3D> points);

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    const Vector3D normalAt(const Point3D &point) {
      // This shouldn't be getting called.
      printf("WARNING: Calling unimplemented TriangleMesh normalAt.\n");
      exit(1);
      Vector3D vector;
      return vector;
    }

    ~TriangleMesh() {
      delete boundingVolume;
      delete triangles;
    }
};


END_RAYTRACER


#endif
