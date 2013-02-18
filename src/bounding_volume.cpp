#include <algorithm>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "vector.h"



BEGIN_RAYTRACER


using namespace std;


// Ref: Glassner -An Introduction to Ray Tracing - P.65-66
inline bool Box::intersect(const Ray &ray) const {
  P_FLT inv, t1, t2, tNear, tFar;

  tNear = -P_FLT_MAX;
  tFar = P_FLT_MAX;

  if (fIsZero(ray.dir.x)) {
    if (ray.orig.x < minExt.x || ray.orig.x > maxExt.x) {
      return false;
    }
  } else {
    inv = 1.0f / ray.dir.x;
    t1 = (minExt.x - ray.orig.x) * inv;
    t2 = (maxExt.x - ray.orig.x) * inv;
    if (t1 > t2) {
      swap(t1, t2);
    }
    tNear = max(t1, tNear);
    tFar = min(t2, tFar);
    if (tNear > tFar || tFar < 0.0f) {
      return false;
    }
  }

  if (fIsZero(ray.dir.y)) {
    if (ray.orig.y < minExt.y || ray.orig.y > maxExt.y) {
      return false;
    }
  } else {
    inv = 1.0f / ray.dir.y;
    t1 = (minExt.y - ray.orig.y) * inv;
    t2 = (maxExt.y - ray.orig.y) * inv;
    if (t1 > t2) {
      swap(t1, t2);
    }
    tNear = max(t1, tNear);
    tFar = min(t2, tFar);
    if (tNear > tFar || tFar < 0.0f) {
      return false;
    }
  }

  if (fIsZero(ray.dir.z)) {
    if (ray.orig.z < minExt.z || ray.orig.z > maxExt.z) {
      return false;
    }
  } else {
    inv = 1.0f / ray.dir.z;
    t1 = (minExt.z - ray.orig.z) * inv;
    t2 = (maxExt.z - ray.orig.z) * inv;
    if (t1 > t2) {
      swap(t1, t2);
    }
    tNear = max(t1, tNear);
    tFar = min(t2, tFar);
    if (tNear > tFar || tFar < 0.0f) {
      return false;
    }
  }

  return true;
}


END_RAYTRACER
