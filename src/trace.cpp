#include "config.h"

#include "color.h"
#include "intercept.h"
#include "model.h"
#include "point.h"
#include "ray.h"
#include "scene.h"
#include "shade.h"
#include "surface.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


int intersect(Ray ray, MODEL_CLS * model, Intercept intercepts[]) {
  if (model->composite_flag) {
    Composite * composite = (Composite *) model;

    int hits_left, hits_right;
    Intercept intercepts_left[MAX_INTERSECTIONS],
                 intercepts_right[MAX_INTERSECTIONS];

    hits_left = intersect(ray, composite->left, intercepts_left);
    if (hits_left == 0 && composite->op != '|') {
      return 0;
    }
    else {
      hits_right = intersect(ray, composite->right, intercepts_right);

      int hits = intersectMerge(composite->op, hits_left, intercepts_left,
                                hits_right, intercepts_right, intercepts);
      return hits;
    }
  }
  else {
    Primitive * prim = (Primitive *) model;

    // TODO: Handle non spheres...
    int hits = (prim->surface->intersect)(ray, intercepts);

    for (int i = 0;i < hits;i++) {
      intercepts[i].primitive = prim;
    }
    return hits;
  }
}

int intersectMerge(int op, int hit_left, Intercept intercepts_left[],
                   int hit_right, Intercept intercepts_right[],
                   Intercept merged[]) {
  // Assume union only for now
  // TODO: Handle non unions

  int left_index = 0, right_index = 0, index = 0;
  while (left_index != hit_left &&
         right_index != hit_right) {
    if (intercepts_left[left_index].t < intercepts_right[right_index].t) {
      merged[index] = intercepts_left[left_index];
      left_index++;
      index++;
    } else {
      merged[index] = intercepts_right[right_index];
      right_index++;
      index++;
    }

    if (index == MAX_INTERSECTIONS) {
      return MAX_INTERSECTIONS;
    }
  }

  if (left_index != hit_left) {
    while (left_index != hit_left && index < MAX_INTERSECTIONS) {
      merged[index] = intercepts_left[left_index];
      left_index++;
      index++;
    }
  }
  else {
    while (right_index != hit_right) {
      merged[index] = intercepts_right[right_index];
      right_index++;
      index++;
    }
  }

  return index;
}


C_FLT shadow(Ray ray, P_FLT t) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts);
  if (hits == 0 || intercepts[0].t > t - P_FLT_EPSILON) {
    return 1.0;
  }

  return 0.0;
}


int trace(int level, C_FLT weight, Ray ray, Color * color) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts);
  if (hits != 0) {
    Point first_intercept = ray.rayPoint(intercepts[0].t);
    Primitive * hit_prim = intercepts[0].primitive;
    Vector normal = (hit_prim->surface->normalAt)(first_intercept);
    if (dotProduct(ray.dir, normal) > 0.0) {
      normal.negate();
    }

    shade(level, weight, first_intercept, normal, ray.dir, intercepts, color);
    return 1;
  }

  if (level > MAX_LEVEL || weight < MIN_WEIGHT) {
    return 0;
  }
  return 0;
}


END_RAYTRACER
