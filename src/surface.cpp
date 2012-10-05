#include "math.h"

#include "surface.h"


BEGIN_RAYTRACER


void Plane::computeD(const Point &point) {
  d = -dotProduct(norm, point);
}


int Sphere::intersect(Ray &ray, P_FLT ** t_values_ptr) {
  Vector origin_to_center;
  P_FLT oc_sqr, ray_closest_approach, half_chord_squared, half_chord,
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

  t_values[0] = ray_closest_approach - half_chord;
  t_values[1] = ray_closest_approach + half_chord;
  (*t_values_ptr) = t_values;

  return 2;
}

const Vector Sphere::normalAt(Point &point) {
  Vector center_to_point = point - (*center);
  center_to_point.normalize();

  return center_to_point;
}

END_RAYTRACER
