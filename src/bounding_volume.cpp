#include <algorithm>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "vector.h"



BEGIN_RAYTRACER


// Ref: Glassner -An Introduction to Ray Tracing - P.65-66
const bool Box::intersect(const Ray &ray) const {
  P_FLT t1, t2,
        invX, invY, invZ,
        tNear = -P_FLT_MAX,
        tFar = P_FLT_MAX;

  if ((fIsZero(ray.dir.x) &&
       (ray.orig.x < minExt.x || ray.orig.x > maxExt.x)) ||
      (fIsZero(ray.dir.y) &&
       (ray.orig.y < minExt.y || ray.orig.y > maxExt.y)) ||
      (fIsZero(ray.dir.z) &&
       (ray.orig.z < minExt.z || ray.orig.z > maxExt.z))) {
      return false;
  }

  invX = 1.0f / ray.dir.x,
  t1 = (minExt.x - ray.orig.x) * invX;
  t2 = (maxExt.x - ray.orig.x) * invX;
  if (t1 > t2) {
    std::swap(t1, t2);
  }
  tNear = std::max(t1, tNear);
  tFar = std::min(t2, tFar);
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  invY = 1.0f / ray.dir.y,
  t1 = (minExt.y - ray.orig.y) * invY;
  t2 = (maxExt.y - ray.orig.y) * invY;
  if (t1 > t2) {
    std::swap(t1, t2);
  }
  tNear = std::max(t1, tNear);
  tFar = std::min(t2, tFar);
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  invZ = 1.0f / ray.dir.z;
  t1 = (minExt.z - ray.orig.z) * invZ;
  t2 = (maxExt.z - ray.orig.z) * invZ;
  if (t1 > t2) {
    std::swap(t1, t2);
  }
  tNear = std::max(t1, tNear);
  tFar = std::min(t2, tFar);
  if (tNear > tFar || tFar < 0.0f) {
    return false;
  }

  return true;
}


END_RAYTRACER
