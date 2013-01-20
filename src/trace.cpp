#ifndef TRACE_CPP
#define TRACE_CPP

#include "config.h"

#include "color.h"
#include "intersection.h"
#include "point.h"
#include "ray.h"
#include "solid.h"
#include "surface.h"
#include "vector.h"


BEGIN_RAYTRACER


int intersect(Ray ray, Solid * solid, Intersection * intercept) {
  P_FLT * t_values;
  Primitive * prim = (Primitive *) solid;

//  std::printf("Intersecting ray %f, %f, %f\n",
//              ray.dir.x, ray.dir.y, ray.dir.z);
  int intercept_num = (prim->surface->intersect)(ray, &t_values);

  if (intercept_num == 0) {
    return 0;
  }

  for (int i = 0;i < intercept_num;i++) {
    intercept[i] = Intersection();
    intercept[i].t = t_values[i];
    intercept[i].prim = prim;
    intercept[i].material = prim->material;
  }
  delete t_values;

  intercept[0].enter = true;
  intercept[1].enter = false;
  return intercept_num;
}


int trace(int level, C_FLT weight, Solid * solid, Ray ray, Color * color) {
  Intersection intercept[MAX_INTERSECTIONS];

  int intercept_num = intersect(ray, solid, intercept);
  if (intercept_num != 0) {
    Primitive * hit_prim = (intercept[0].prim);

    Point first_intercept = ray.rayPoint(intercept[0].t);
    Vector normal = (hit_prim->surface->normalAt)(first_intercept);
    if (dotProduct(ray.dir, normal) > 0.0) {
      normal *= -1;
    }

    color->r = 1.0;
    return 1;
  }

  if (level > MAX_LEVEL || weight < MIN_WEIGHT) {
    return 2;
  }
  return 0;
}


END_RAYTRACER


#endif
