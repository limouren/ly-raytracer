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
  int dominantIndex;
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

  // inspired by povray triangle intersection
  // idea: cross product each vector after projecting to YZ/XZ/XY plane
  dominantIndex = normal.dominantIndex();
  switch (dominantIndex) {
    case 0:  // x dominant, normal.x > 0
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex[0])->y) * ((vertex[1])->z - (vertex[0])->z) >
          (v - (vertex[0])->z) * ((vertex[1])->y - (vertex[0])->y)) {
        return 0;
      }
      if ((u - (vertex[1])->y) * ((vertex[2])->z - (vertex[1])->z) >
          (v - (vertex[1])->z) * ((vertex[2])->y - (vertex[1])->y)) {
        return 0;
      }
      if ((u - (vertex[2])->y) * ((vertex[0])->z - (vertex[2])->z) >
          (v - (vertex[2])->z) * ((vertex[0])->y - (vertex[2])->y)) {
        return 0;
      }

      break;

    case 1:  // y dominant, normal.y > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex[0])->x) * ((vertex[1])->z - (vertex[0])->z) <
          (v - (vertex[0])->z) * ((vertex[1])->x - (vertex[0])->x)) {
        return 0;
      }
      if ((u - (vertex[1])->x) * ((vertex[2])->z - (vertex[1])->z) <
          (v - (vertex[1])->z) * ((vertex[2])->x - (vertex[1])->x)) {
        return 0;
      }
      if ((u - (vertex[2])->x) * ((vertex[0])->z - (vertex[2])->z) <
          (v - (vertex[2])->z) * ((vertex[0])->x - (vertex[2])->x)) {
        return 0;
      }

      break;

    case 2:  // z dominant, normal.z > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - (vertex[0])->x) * ((vertex[1])->y - (vertex[0])->y) >
          (v - (vertex[0])->y) * ((vertex[1])->x - (vertex[0])->x)) {
        return 0;
      }
      if ((u - (vertex[1])->x) * ((vertex[2])->y - (vertex[1])->y) >
          (v - (vertex[1])->y) * ((vertex[2])->x - (vertex[1])->x)) {
        return 0;
      }
      if ((u - (vertex[2])->x) * ((vertex[0])->y - (vertex[2])->y) >
          (v - (vertex[2])->y) * ((vertex[0])->x - (vertex[2])->x)) {
        return 0;
      }

      break;

    case 3:  // x dominant, normal.x < 0
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex[0])->y) * ((vertex[1])->z - (vertex[0])->z) <
          (v - (vertex[0])->z) * ((vertex[1])->y - (vertex[0])->y)) {
        return 0;
      }
      if ((u - (vertex[1])->y) * ((vertex[2])->z - (vertex[1])->z) <
          (v - (vertex[1])->z) * ((vertex[2])->y - (vertex[1])->y)) {
        return 0;
      }
      if ((u - (vertex[2])->y) * ((vertex[0])->z - (vertex[2])->z) <
          (v - (vertex[2])->z) * ((vertex[0])->y - (vertex[2])->y)) {
        return 0;
      }

      break;

    case 4:  // y dominant, normal.y < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex[0])->x) * ((vertex[1])->z - (vertex[0])->z) >
          (v - (vertex[0])->z) * ((vertex[1])->x - (vertex[0])->x)) {
        return 0;
      }
      if ((u - (vertex[1])->x) * ((vertex[2])->z - (vertex[1])->z) >
          (v - (vertex[1])->z) * ((vertex[2])->x - (vertex[1])->x)) {
        return 0;
      }
      if ((u - (vertex[2])->x) * ((vertex[0])->z - (vertex[2])->z) >
          (v - (vertex[2])->z) * ((vertex[0])->x - (vertex[2])->x)) {
        return 0;
      }

      break;

    case 5:  // z dominant, normal.z < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - (vertex[0])->x) * ((vertex[1])->y - (vertex[0])->y) <
          (v - (vertex[0])->y) * ((vertex[1])->x - (vertex[0])->x)) {
        return 0;
      }
      if ((u - (vertex[1])->x) * ((vertex[2])->y - (vertex[1])->y) <
          (v - (vertex[1])->y) * ((vertex[2])->x - (vertex[1])->x)) {
        return 0;
      }
      if ((u - (vertex[2])->x) * ((vertex[0])->y - (vertex[2])->y) <
          (v - (vertex[2])->y) * ((vertex[0])->x - (vertex[2])->x)) {
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


// This is inefficient and really shouldn't be used
const Vector3D TrianglePatch::normalAt(const Point3D &point) const {
  P_FLT vertexWeight[3], scale;
  Vector3D result;

  vertexWeight[0] = 1 / (*vertex[0] - point).length();
  vertexWeight[1] = 1 / (*vertex[1] - point).length();
  vertexWeight[2] = 1 / (*vertex[2] - point).length();


  result = *vertexNormal[0] * vertexWeight[0] +
           *vertexNormal[1] * vertexWeight[1] +
           *vertexNormal[2] * vertexWeight[2];
  result.normalize();
  return result;
}


END_RAYTRACER
