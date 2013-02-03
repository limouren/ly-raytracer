#include "config.h"

#include "mesh.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


class Box;
class Material;
class Point3D;


void TriangleMesh::buildBoundingVollume(std::vector<Point3D> points) {
  std::vector<Point3D>::iterator itr = points.begin();

  Point3D maxExt = *itr,
          minExt = *itr;
  while (itr < points.end()) {
    maxExt = max(maxExt, *itr);
    minExt = min(minExt, *itr);
    itr++;
  }

  Vector3D safetyMargin = Vector3D(P_FLT_EPSILON * 2.0,
                                   P_FLT_EPSILON * 2.0,
                                   P_FLT_EPSILON * 2.0);

  maxExt += safetyMargin;
  minExt -= safetyMargin;

  boundingVolume = new Box(minExt, maxExt);
}


const int TriangleMesh::intersect(const Ray &ray, Intercept intercepts[],
                                  Material * entryMat) const {
  if (!(boundingVolume->intersect)(ray)) {
    return 0;
  }

  return RAYTRACER_NAMESPACE::intersect(ray, triangles, intercepts, entryMat);
}


END_RAYTRACER
