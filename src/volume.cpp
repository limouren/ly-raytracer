#include "config.h"

#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "vector.h"
#include "volume.h"



BEGIN_RAYTRACER


const int Box::intersect(const Ray &ray, Intercept intercepts[]) {
  // TODO(kent): fill this in
}


const Vector3D Box::normalAt(const Point3D &point) {
  P_FLT x = (fEqual(point.x, minExt.x))? -1.0: 1.0;
  P_FLT y = (fEqual(point.y, minExt.y))? -1.0: 1.0;
  P_FLT z = (fEqual(point.z, minExt.z))? -1.0: 1.0;

  return Vector3D(x, y, z);
}


END_RAYTRACER
