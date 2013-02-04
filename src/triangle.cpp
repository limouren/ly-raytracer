#include "config.h"

#include "intercept.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
const int Triangle::intersect(const Ray &ray, Intercept intercepts[],
                              Material * entryMat) const {
  P_FLT normalDotOrig, normalDotDir,
        t, u, v;

  normalDotDir = dotProduct(normal, ray.dir);
  if (fIsZero(normalDotDir)) {
    return 0;
  }

  normalDotOrig = dotProduct(normal, ray.orig);

  t = -(d + normalDotOrig) / normalDotDir;

  if (!fGreaterZero(t)) {
    return 0;
  }

  // ref: povray triangle intersection
  switch (normal.dominantIndex()) {
    case 0:
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((vertex[1].y - u) * (vertex[1].z - vertex[0].z) >
          (vertex[1].z - v) * (vertex[1].y - vertex[0].y)) {
        return 0;
      }
      if ((vertex[2].y - u) * (vertex[2].z - vertex[1].z) >
          (vertex[2].z - v) * (vertex[2].y - vertex[1].y)) {
        return 0;
      }
      if ((vertex[0].y - u) * (vertex[0].z - vertex[2].z) >
          (vertex[0].z - v) * (vertex[0].y - vertex[2].y)) {
        return 0;
      }
      break;

    case 1:
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((vertex[1].x - u) * (vertex[1].z - vertex[0].z) >
          (vertex[1].z - v) * (vertex[1].x - vertex[0].x)) {
        return 0;
      }
      if ((vertex[2].x - u) * (vertex[2].z - vertex[1].z) >
          (vertex[2].z - v) * (vertex[2].x - vertex[1].x)) {
        return 0;
      }
      if ((vertex[0].x - u) * (vertex[0].z - vertex[2].z) >
          (vertex[0].z - v) * (vertex[0].x - vertex[2].x)) {
        return 0;
      }

      break;

    case 2:
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((vertex[1].x - u) * (vertex[1].y - vertex[0].y) >
          (vertex[1].y - v) * (vertex[1].x - vertex[0].x)) {
        return 0;
      }
      if ((vertex[2].x - u) * (vertex[2].y - vertex[1].y) >
          (vertex[2].y - v) * (vertex[2].x - vertex[1].x)) {
        return 0;
      }
      if ((vertex[0].x - u) * (vertex[0].y - vertex[2].y) >
          (vertex[0].y - v) * (vertex[0].x - vertex[2].x)) {
        return 0;
      }

      break;
  }

  intercepts[0].t = t;
  intercepts[0].enter = (normalDotDir < 0);
  intercepts[0].material = entryMat;
  intercepts[0].primitive = this;
  return 1;
}


END_RAYTRACER
