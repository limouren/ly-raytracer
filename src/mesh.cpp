#include "config.h"

#include "bounding_volume.h"
#include "material.h"
#include "mesh.h"
#include "mesh_triangle.h"
#include "point.h"
#include "trace.h"
#include "transform.h"
#include "tree.h"
#include "vector.h"


BEGIN_RAYTRACER


void TriangleMesh::buildBoundingBox() {
  // Build mesh bounding box
  Point3D maxExt = points[0],
          minExt = points[0];
  for (int i = 1; i < pointNum; i++) {
    maxExt = pointMax(maxExt, points[i]);
    minExt = pointMin(minExt, points[i]);
  }

  BoundingBox * newBox = new BoundingBox(minExt, maxExt);
  if (newBox && boundingBox) {
    delete boundingBox;
  }
  boundingBox = newBox;
}

void TriangleMesh::constructTriangles(
  const std::vector<int *> &triangleDefs) {
  std::vector<int *>::const_iterator itr;
  if (normalNum == 0 && texCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangles.push_back(
        new MeshTriangle(material,
                         &points[(*itr)[0]],
                         &points[(*itr)[1]],
                         &points[(*itr)[2]]));
    }
  } else if (texCoordNum == 0) {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangles.push_back(
        new PatchMeshTriangle(material,
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
        new TexMeshTriangle(material,
                            texture,
                            &points[(*itr)[0]],
                            &points[(*itr)[1]],
                            &points[(*itr)[2]],
                            &texCoords[(*itr)[3]],
                            &texCoords[(*itr)[4]],
                            &texCoords[(*itr)[5]]));
    }
  } else {
    for (itr = triangleDefs.begin(); itr != triangleDefs.end(); itr++) {
      triangles.push_back(
        new PhongMeshTriangle(material,
                              texture,
                              &points[(*itr)[0]],
                              &points[(*itr)[1]],
                              &points[(*itr)[2]],
                              &normals[(*itr)[3]],
                              &normals[(*itr)[4]],
                              &normals[(*itr)[5]],
                              &texCoords[(*itr)[6]],
                              &texCoords[(*itr)[7]],
                              &texCoords[(*itr)[8]]));
    }
  }
}


void TriangleMesh::init() {
  buildBoundingBox();

  Node * newTree = buildBVHTree(triangles.begin(), triangles.end());
  if (newTree && triangleTree) {
    delete triangleTree;
  }
  triangleTree = newTree;
}


int TriangleMesh::intersect(const Ray &ray, Intercept intercepts[]) const {
  return RAYTRACER_NAMESPACE::intersect(ray, triangleTree, intercepts);
}


void TriangleMesh::transform(Transform * transform) {
  for (int i = 0; i < pointNum; i++) {
    transform->transformPoint(&points[i]);
  }

  for (int i = 0; i < normalNum; i++) {
    transform->transformNormal(&normals[i]);
  }

  init();
}


END_RAYTRACER
