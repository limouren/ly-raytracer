#include "config.h"

#include "mesh.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


class Box;
class Material;
class Point3D;


inline void TriangleMesh::buildBoundingVolume() {
  Point3D maxExt = points[0],
          minExt = points[0];
  for (int i = 1; i < pointNum; i++) {
    maxExt = max(maxExt, points[i]);
    minExt = min(minExt, points[i]);
  }

  boundingVolume = new Box(minExt, maxExt);
}

inline void TriangleMesh::constructTriangles(
  const std::vector<int *> &triangleDefs) {
  std::vector<MODEL_CLS *> triangleVector;
  std::vector<int *>::const_iterator itr;
  if (normalNum == 0 && textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangleVector.push_back(
        new Triangle(material,
                     &points[(*itr)[0]],
                     &points[(*itr)[1]],
                     &points[(*itr)[2]]));
    }
  } else if (textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangleVector.push_back(
        new TrianglePatch(material,
                          &points[(*itr)[0]],
                          &points[(*itr)[1]],
                          &points[(*itr)[2]],
                          &normals[(*itr)[3]],
                          &normals[(*itr)[4]],
                          &normals[(*itr)[5]]));
    }
  } else if (normalNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangleVector.push_back(
        new TexturedTriangle(material,
                             texture,
                             &points[(*itr)[0]],
                             &points[(*itr)[1]],
                             &points[(*itr)[2]],
                             &textureCoords[(*itr)[3]],
                             &textureCoords[(*itr)[4]],
                             &textureCoords[(*itr)[5]]));
    }
  } else {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangleVector.push_back(
        new PhongTriangle(material,
                          texture,
                          &points[(*itr)[0]],
                          &points[(*itr)[1]],
                          &points[(*itr)[2]],
                          &normals[(*itr)[3]],
                          &normals[(*itr)[4]],
                          &normals[(*itr)[5]],
                          &textureCoords[(*itr)[6]],
                          &textureCoords[(*itr)[7]],
                          &textureCoords[(*itr)[8]]));
    }
  }

  triangles = buildModelTreeNode(triangleVector, 0);
}


const int TriangleMesh::intersect(const Ray &ray, Intercept intercepts[],
                                  Material * entryMat) const {
  if (!(boundingVolume->intersect)(ray)) {
    return 0;
  }

  return RAYTRACER_NAMESPACE::intersect(ray, triangles, intercepts, entryMat);
}


END_RAYTRACER
