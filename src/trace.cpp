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


int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
              Material * entryMaterial) {
  if (model->composite_flag) {
    Composite * composite = static_cast<Composite *>(model);

    int hits_left, hits_right;
    Intercept intercepts_left[MAX_INTERSECTIONS],
                 intercepts_right[MAX_INTERSECTIONS];

    hits_left = intersect(ray, composite->left, intercepts_left,
                          entryMaterial);
    if (hits_left == 0 && composite->op != '|') {
      return 0;
    } else {
      hits_right = intersect(ray, composite->right, intercepts_right,
                             entryMaterial);

      int hits = intersectMerge(composite->op, hits_left, intercepts_left,
                                hits_right, intercepts_right, intercepts);
      return hits;
    }
  } else {
    Primitive * prim = static_cast<Primitive *>(model);

    // TODO(kent): Handle non spheres...
    int hits = (prim->surface->intersect)(ray, intercepts);
    if (hits) {
      intercepts[0].material = entryMaterial;
      intercepts[0].primitive = prim;
      for (int i = 1; i < hits; i++) {
        intercepts[i].material = prim->material;
        intercepts[i].primitive = prim;
      }
    }
    return hits;
  }
}

int intersectMerge(int op, int hit_left, Intercept intercepts_left[],
                   int hit_right, Intercept intercepts_right[],
                   Intercept merged[]) {
  // Assume union only for now
  // TODO(kent): Handle non unions
  // TODO(kent): Change intercept lists to vectors

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
  } else {
    while (right_index != hit_right && index < MAX_INTERSECTIONS) {
      merged[index] = intercepts_right[right_index];
      right_index++;
      index++;
    }
  }

  return index;
}


int trace(int level, C_FLT weight, const Ray &ray, Color * color,
          Material * entryMaterial) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts, entryMaterial);
  if (hits != 0) {
    Point first_intercept = ray.rayPoint(intercepts[0].t);
    Primitive * hit_prim = intercepts[0].primitive;
    Vector normal = (hit_prim->surface->normalAt)(first_intercept);
    if (dotProduct(ray.dir, normal) > 0.0) {
      normal.negate();
    }

    shade(level, weight, first_intercept, normal, ray.dir, intercepts, color);
    return 1;
  } else {
    shadeBackground(ray, color);
    return 0;
  }
}


END_RAYTRACER
