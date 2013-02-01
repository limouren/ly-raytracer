#include "config.h"

#include "geometry.h"
#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
const int Triangle::intersect(const Ray &ray, Intercept intercepts[]) {
  P_FLT u, v;

  if (Plane::intersect(ray, intercepts) == 0) {
    return 0;
  }

  // ref: povray triangle intersection
  switch (normal.dominantIndex()) {
    case 0:
      u = ray.orig.y + intercepts[0].t * ray.dir.y;
      v = ray.orig.z + intercepts[0].t * ray.dir.z;

      if ((vertex[1].y - u) * (vertex[1].z - vertex[0].z) <
          (vertex[1].z - v) * (vertex[1].y - vertex[0].y)) {
        return 0;
      }
      if ((vertex[2].y - u) * (vertex[2].z - vertex[1].z) <
          (vertex[2].z - v) * (vertex[2].y - vertex[1].y)) {
        return 0;
      }
      if ((vertex[0].y - u) * (vertex[0].z - vertex[2].z) <
          (vertex[0].z - v) * (vertex[0].y - vertex[2].y)) {
        return 0;
      }

      intercepts[0].enter = true;
      return 1;

    case 1:
      u = ray.orig.y + intercepts[0].t * ray.dir.y;
      v = ray.orig.z + intercepts[0].t * ray.dir.z;

      if ((vertex[1].x - u) * (vertex[1].z - vertex[0].z) <
          (vertex[1].z - v) * (vertex[1].x - vertex[0].y)) {
        return 0;
      }
      if ((vertex[2].x - u) * (vertex[2].z - vertex[1].z) <
          (vertex[2].z - v) * (vertex[2].x - vertex[1].y)) {
        return 0;
      }
      if ((vertex[0].x - u) * (vertex[0].z - vertex[2].z) <
          (vertex[0].z - v) * (vertex[0].x - vertex[2].y)) {
        return 0;
      }

      intercepts[0].enter = true;
      return 1;

    case 2:
      u = ray.orig.x + intercepts[0].t * ray.dir.x;
      v = ray.orig.y + intercepts[0].t * ray.dir.y;

      if ((vertex[1].x - u) * (vertex[1].y - vertex[0].y) <
          (vertex[1].y - v) * (vertex[1].x - vertex[0].x)) {
        return 0;
      }
      if ((vertex[2].x - u) * (vertex[2].y - vertex[1].y) <
          (vertex[2].y - v) * (vertex[2].x - vertex[1].x)) {
        return 0;
      }
      if ((vertex[0].x - u) * (vertex[0].y - vertex[2].y) <
          (vertex[0].y - v) * (vertex[0].x - vertex[2].x)) {
        return 0;
      }

      intercepts[0].enter = true;
      return 1;
  }
}


END_RAYTRACER
