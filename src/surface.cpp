#include "math.h"

#include "math_util.h"
#include "surface.h"


BEGIN_RAYTRACER


void Plane::computeD(const Point &point) {
  d = -dotProduct(norm, point);
}


const int Plane::intersect(Ray &ray, P_FLT ** t_values) {
  P_FLT v_d, v_o, t;
  *t_values = new P_FLT[1];

  v_d = dotProduct(norm, ray.dir);

  if (fIsZero(v_d)) {
    return 0;
  }

  v_o = -(dotProduct(norm, ray.orig) + d);
  t = v_o / v_d;

  if (t < 0) {
    return 0;
  }

  (*t_values)[0] = t;
  return 1;
}


const int Sphere::intersect(Ray &ray, P_FLT ** t_values) {
  Vector origin_to_center;
  P_FLT oc_sqr, ray_closest_approach, half_chord_squared, half_chord;

  *t_values = new P_FLT[2];

  origin_to_center = (*center) - ray.orig;
  oc_sqr = origin_to_center.lengthSqr();
  ray_closest_approach = dotProduct(origin_to_center, ray.dir);

  if (ray_closest_approach < 0) {
    return 0;
  }

  half_chord_squared = (radius * radius) - oc_sqr +
    (ray_closest_approach * ray_closest_approach);

  if (half_chord_squared < 0) {
    return 0;
  }

  half_chord = sqrt(half_chord_squared);

  (*t_values)[0] = ray_closest_approach - half_chord;
  (*t_values)[1] = ray_closest_approach + half_chord;

  return 2;
}

const Vector Sphere::normalAt(Point &point) {
  Vector center_to_point = point - (*center);
  center_to_point.normalize();

  return center_to_point;
}

END_RAYTRACER
