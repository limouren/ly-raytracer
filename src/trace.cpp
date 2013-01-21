#include "config.h"

#include "color.h"
#include "intersection.h"
#include "point.h"
#include "ray.h"
#include "scene.h"
#include "shade.h"
#include "solid.h"
#include "surface.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


int intersect(Ray ray, Solid * solid, Intersection * intercept) {
  int hit_left, hit_right;
  Intersection intercept_left[MAX_INTERSECTIONS],
               intercept_right[MAX_INTERSECTIONS];

  Primitive * prim = (Primitive *) solid;
  if (solid->composite_flag) {
    Composite * composite = (Composite *) solid;

    hit_left = intersect(ray, composite->left, intercept_left);
    if (hit_left == 0 && composite->op != '|') {
      return 0;
    }
    else {
      hit_right = intersect(ray, composite->right, intercept_right);

      intercept = intersectMerge(composite->op, hit_left, intercept_left,
                                 hit_right, intercept_right);
      return hit_left + hit_right;
    }
  }
  else {
    // TODO: Handle non spheres...
    P_FLT * t_values;
    int intercept_num = (prim->surface->intersect)(ray, &t_values);

    if (intercept_num == 0) {
      return 0;
    }

    for (int i = 0;i < intercept_num;i++) {
      intercept[i] = Intersection();
      intercept[i].t = t_values[i];
      intercept[i].prim = prim;
      intercept[i].material = prim->material;
      intercept[i].enter = false;
    }
    intercept[0].enter = true;

    return intercept_num;
  }
}

Intersection * intersectMerge(int op, int hit_left,
                              Intersection * intercept_left, int hit_right,
                              Intersection * intercept_right) {
  // Assume union only for now
  // TODO: Handle non unions
  Intersection * merged = new Intersection[hit_left + hit_right];

  int left = 0, right = 0;
  while (left != hit_left && right != hit_right) {
    if (intercept_left[left].t < intercept_right[right].t) {
      merged[left + right] = intercept_left[left];
      left++;
    } else {
      merged[left + right] = intercept_right[right];
      right++;
    }
  }

  if (left != hit_left) {
    while (left != hit_left) {
      merged[left + right] = intercept_left[left];
      left++;
    }
  } else {
    while (right != hit_right) {
      merged[left + right] = intercept_right[right];
      right++;
    }
  }

  return merged;
}


C_FLT Shadow(Ray ray, P_FLT t) {
  Intersection intercepts[MAX_INTERSECTIONS];

  int intercept_num = intersect(ray, scene.modelRoot, intercepts);
  if (intercept_num == 0 || intercepts[0].t > t - P_FLT_EPSILON) {
    return 1.0;
  }

  return 0.0;
}


int trace(int level, C_FLT weight, Ray ray, Color * &color) {
  Intersection intercepts[MAX_INTERSECTIONS];

  int intercept_num = intersect(ray, scene.modelRoot, intercepts);
  if (intercept_num != 0) {
    Primitive * hit_prim = (intercepts[0].prim);

    Point first_intercept = ray.rayPoint(intercepts[0].t);
    Vector normal = (hit_prim->surface->normalAt)(first_intercept);
    if (dotProduct(ray.dir, normal) > 0.0) {
      normal *= -1;
    }

    *color = *shade(level, weight, first_intercept, normal, ray.dir, intercepts);
    return 1;
  }

  if (level > MAX_LEVEL || weight < MIN_WEIGHT) {
    return 0;
  }
  return 0;
}


END_RAYTRACER
