#include <math.h>

#include "config.h"

#include "math_util.h"
#include "surface.h"


BEGIN_RAYTRACER


const int Plane::intersect(Ray &ray, Intercept intercepts[]) {
  P_FLT v_d, v_o, t;

  v_d = dotProduct(norm, ray.dir);

  if (fIsZero(v_d)) {
    return 0;
  }

  v_o = -(dotProduct(norm, ray.orig) + d);
  t = v_o / v_d;

  if (t < 0) {
    return 0;
  }

  intercepts[0] = Intercept(t, v_d < 0.0);
  return 1;
}


const int Sphere::intersect(Ray &ray, Intercept intercepts[]) {
  Vector origin_to_center;
  P_FLT oc_sqr, ray_closest_approach, half_chord_squared, half_chord;

  origin_to_center = center - ray.orig;
  ray_closest_approach = dotProduct(origin_to_center, ray.dir);

  if (ray_closest_approach < 0) {
    return 0;
  }

  oc_sqr = origin_to_center.lengthSqr();
  half_chord_squared = (radius * radius) - oc_sqr +
    (ray_closest_approach * ray_closest_approach);

  if (half_chord_squared < 0) {
    return 0;
  }

  half_chord = sqrt(half_chord_squared);

  if (ray_closest_approach > half_chord) {
    intercepts[0] = Intercept(ray_closest_approach - half_chord, true);
    intercepts[1] = Intercept(ray_closest_approach + half_chord, false);
    return 2;
  }
  else {
    intercepts[0] = Intercept(ray_closest_approach + half_chord, false);
    return 1;
  }
}

const Vector Sphere::normalAt(Point &point) {
  Vector center_to_point = point - center;
  center_to_point.normalize();

  return center_to_point;
}


END_RAYTRACER
