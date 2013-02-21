#include "config.h"

#include "bounding_volume.h"
#include "material.h"
#include "mesh.h"
#include "point.h"
#include "trace.h"
#include "transform.h"
#include "vector.h"


BEGIN_RAYTRACER


void TriangleMesh::buildBoundingBox() {
  boundingBox = boundingBoxBuilder(triangles);
}

void TriangleMesh::constructTriangles(
  const std::vector<int *> &triangleDefs) {
  std::vector<int *>::const_iterator itr;
  if (normalNum == 0 && textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangles.push_back(
        new Triangle(material,
                     &points[(*itr)[0]],
                     &points[(*itr)[1]],
                     &points[(*itr)[2]]));
    }
  } else if (textureCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangles.push_back(
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
      triangles.push_back(
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
      triangles.push_back(
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

  triangleTree = buildModelTreeNode(triangles, 0);
}


int TriangleMesh::intersect(const Ray &ray, Intercept intercepts[]) const {
  if (!(boundingBox->intersect)(ray)) {
    return 0;
  }

  return RAYTRACER_NAMESPACE::intersect(ray, triangleTree, intercepts);
}


void TriangleMesh::transform(Transform * transform) {
  for (int i = 0; i < pointNum; i++) {
    transform->transformPoint(&points[i]);
  }
  for (int i = 0; i < normalNum; i++) {
    transform->transformVector(&normals[i]);
  }

  Triangle * triangle;
  for (std::vector<MODEL_CLS *>::iterator itr = triangles.begin();
       itr != triangles.end(); itr++) {
    triangle = static_cast<Triangle *>(*itr);
    triangle->transform(transform);
    triangle->buildBoundingBox();
  }
  buildBoundingBox();
  triangleTree = buildModelTreeNode(triangles, 0);
}


END_RAYTRACER
