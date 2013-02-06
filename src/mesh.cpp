#include "config.h"

#include "mesh.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


class Box;
class Material;
class Point3D;


void TriangleMesh::buildBoundingVollume(const std::vector<Point3D> &points) {
  std::vector<Point3D>::const_iterator itr = points.begin();

  Point3D maxExt = *itr,
          minExt = *itr;
  while (itr < points.end()) {
    maxExt = max(maxExt, *itr);
    minExt = min(minExt, *itr);
    itr++;
  }

  boundingVolume = new Box(minExt, maxExt);
}

void TriangleMesh::constructTriangles(const std::vector<int *> &triangleDefs) {
  std::vector<MODEL_CLS *> triangleVector;
  std::vector<int *>::const_iterator itr;
  if (normalNum == 0 && textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      Triangle * triangle = new Triangle(material,
                                         &points[(*itr)[0]],
                                         &points[(*itr)[1]],
                                         &points[(*itr)[2]]);
      triangleVector.push_back(triangle);
    }
  } else if (textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      TrianglePatch * triangle = new TrianglePatch(material,
                                                   &points[(*itr)[0]],
                                                   &points[(*itr)[1]],
                                                   &points[(*itr)[2]],
                                                   &normals[(*itr)[3]],
                                                   &normals[(*itr)[4]],
                                                   &normals[(*itr)[5]]);
      triangleVector.push_back(triangle);
    }
  }

  triangles = buildModelTree(triangleVector);
}


const int TriangleMesh::intersect(const Ray &ray, Intercept intercepts[],
                                  Material * entryMat) const {
  if (!(boundingVolume->intersect)(ray)) {
    return 0;
  }

  return RAYTRACER_NAMESPACE::intersect(ray, triangles, intercepts, entryMat);
}


END_RAYTRACER
