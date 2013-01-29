#include <math.h>

#include "config.h"

#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "surface.h"


BEGIN_RAYTRACER


const int Plane::intersect(const Ray &ray, Intercept intercepts[]) {
  P_FLT v_d, v_o, t;

  v_d = dotProduct(normal, ray.dir);

  if (fIsZero(v_d)) {
    return 0;
  }

  v_o = -(dotProduct(normal, ray.orig) + d);
  t = v_o / v_d;

  if (fLessThan(t, 0.0)) {
    return 0;
  }

  intercepts[0] = Intercept(t, v_d < 0.0);
  return 1;
}

// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
const int Polygon::intersect(const Ray &ray, Intercept intercepts[]) {
  Point3D intercept3D = ray.rayPoint(intercepts[0].t);
  int dominantIndex = intercept3D.dominantIndex();

  Vector2D intercept2D = intercept3D.dropIndex(dominantIndex);
  Vector2D * vertex2D = new Point2D[vertex_num];
  for (int i = 0; i < vertex_num; i++) {
    vertex2D[i] = vertex[i].dropIndex(dominantIndex) - intercept2D;
  }

  int windingNumber = 0,
      first = vertex_num - 1;
  bool firstYPositive = (vertex2D[first].y < 0),
       secondYPositive;

  for (int second = 0; second < vertex_num; second++) {
    secondYPositive = (vertex2D[second].y > 0);

    if (firstYPositive != secondYPositive) {
      if (vertex2D[first].x > 0 && vertex2D[second].x > 0) {
        windingNumber++;
      } else if ((vertex2D[first].x > 0 || vertex2D[second].x > 0) &&
                 fGreaterThan(((vertex2D[first].x - vertex2D[first].y) *
                               (vertex2D[second].x - vertex2D[first].x) /
                               (vertex2D[second].y - vertex2D[first].y)),
                              0.0)) {
        windingNumber++;
      }
    }

    first = second;
    firstYPositive = secondYPositive;
  }

  delete [] vertex2D;
  return !(windingNumber % 2);
}


const int Sphere::intersect(const Ray &ray, Intercept intercepts[]) {
  Vector3D origin_to_center;
  P_FLT oc_sqr, ray_closest_approach, half_chord_squared, half_chord, t;

  origin_to_center = center - ray.orig;
  ray_closest_approach = dotProduct(origin_to_center, ray.dir);

  if (fLessThan(ray_closest_approach, 0.0)) {
    return 0;
  }

  oc_sqr = origin_to_center.lengthSqr();

  half_chord_squared = (radius * radius) - oc_sqr +
    (ray_closest_approach * ray_closest_approach);

  if (fLessThan(half_chord_squared, 0.0)) {
    return 0;
  }

  half_chord = sqrt(half_chord_squared);

  if (fGreaterThan(ray_closest_approach, half_chord)) {
    intercepts[0] = Intercept(ray_closest_approach - half_chord, true);
    intercepts[1] = Intercept(ray_closest_approach + half_chord, false);
    return 2;
  } else {
    intercepts[0] = Intercept(ray_closest_approach + half_chord, false);
    return 1;
  }
}

const Vector3D Sphere::normalAt(const Point3D &point) {
  Vector3D center_to_point = point - center;
  center_to_point.normalize();

  return center_to_point;
}


END_RAYTRACER
