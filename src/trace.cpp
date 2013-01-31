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

    int hitsLeft, hitsRight;
    Intercept interceptsLeft[MAX_INTERSECTIONS],
                 interceptsRight[MAX_INTERSECTIONS];

    hitsLeft = intersect(ray, composite->left, interceptsLeft,
                          entryMaterial);
    if (hitsLeft == 0 && composite->op != '|') {
      return 0;
    } else {
      hitsRight = intersect(ray, composite->right, interceptsRight,
                             entryMaterial);

      int hits = intersectMerge(composite->op, hitsLeft, interceptsLeft,
                                hitsRight, interceptsRight, intercepts);
      return hits;
    }
  } else {
    Primitive * prim = static_cast<Primitive *>(model);

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

int intersectMerge(int op, int hitsLeft, Intercept interceptsLeft[],
                   int hitsRight, Intercept interceptsRight[],
                   Intercept merged[]) {
  // Assume union only for now
  // TODO(kent): Handle non unions
  // TODO(kent): Change intercept lists to vectors

  int index = 0,
      leftIndex = 0,
      rightIndex = 0;
  while (leftIndex != hitsLeft &&
         rightIndex != hitsRight) {
    if (interceptsLeft[leftIndex].t < interceptsRight[rightIndex].t) {
      merged[index] = interceptsLeft[leftIndex];
      leftIndex++;
      index++;
    } else {
      merged[index] = interceptsRight[rightIndex];
      rightIndex++;
      index++;
    }

    if (index == MAX_INTERSECTIONS) {
      return MAX_INTERSECTIONS;
    }
  }

  if (leftIndex != hitsLeft) {
    while (leftIndex != hitsLeft && index < MAX_INTERSECTIONS) {
      merged[index] = interceptsLeft[leftIndex];
      leftIndex++;
      index++;
    }
  } else {
    while (rightIndex != hitsRight && index < MAX_INTERSECTIONS) {
      merged[index] = interceptsRight[rightIndex];
      rightIndex++;
      index++;
    }
  }

  return index;
}


int trace(int level, C_FLT weight, const Ray &ray, Color * color,
          Material * entryMaterial) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts, entryMaterial);
  if (hits > 0) {
    Point3D interceptPoint = ray.rayPoint(intercepts[0].t);
    Primitive * primitive = intercepts[0].primitive;
    Vector3D normal = (primitive->surface->normalAt)(interceptPoint);
    if (dotProduct(ray.dir, normal) > 0.0) {
      normal.negate();
    }

    shade(level, weight, interceptPoint, normal, ray.dir, intercepts, color);
    return hits;
  } else {
    shadeBackground(ray, color);
    return 0;
  }
}


END_RAYTRACER
