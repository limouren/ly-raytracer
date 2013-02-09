#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "vector.h"



BEGIN_RAYTRACER


// Ref: Glassner -An Introduction to Ray Tracing - P.65-66
const bool Box::intersect(const Ray &ray) const {
  P_FLT t1, t2, tmp,
        tNear = -P_FLT_MAX,
        tFar = P_FLT_MAX;

  if (fIsZero(ray.dir.x)) {
    if (ray.orig.x < minExt.x || ray.orig.x < maxExt.x) {
      return false;
    }
  } else if (fIsZero(ray.dir.y)) {
    if (ray.orig.y < minExt.y || ray.orig.y < maxExt.y) {
      return false;
    }
  } else if (fIsZero(ray.dir.z)) {
    if (ray.orig.z < minExt.z || ray.orig.z < maxExt.z) {
      return false;
    }
  }

  t1 = (minExt.x - ray.orig.x) / ray.dir.x;
  t2 = (maxExt.x - ray.orig.x) / ray.dir.x;
  if (t1 > t2) {
    tmp = t1;
    t1 = t2;
    t2 = tmp;
  }
  if (t1 > tNear) {
    tNear = t1;
  }
  if (t2 < tFar) {
    tFar = t2;
  }
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  t1 = (minExt.y - ray.orig.y) / ray.dir.y;
  t2 = (maxExt.y - ray.orig.y) / ray.dir.y;
  if (t1 > t2) {
    tmp = t1;
    t1 = t2;
    t2 = tmp;
  }
  if (t1 > tNear) {
    tNear = t1;
  }
  if (t2 < tFar) {
    tFar = t2;
  }
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  t1 = (minExt.z - ray.orig.z) / ray.dir.z;
  t2 = (maxExt.z - ray.orig.z) / ray.dir.z;
  if (t1 > t2) {
    tmp = t1;
    t1 = t2;
    t2 = tmp;
  }
  if (t1 > tNear) {
    tNear = t1;
  }
  if (t2 < tFar) {
    tFar = t2;
  }
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  return true;
}


END_RAYTRACER
