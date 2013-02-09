#include "config.h"

#include "color.h"
#include "intercept.h"
#include "model.h"
#include "point.h"
#include "ray.h"
#include "scene.h"
#include "shade.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
              Material * entryMat) {
  if (model->composite_flag) {
    Composite * composite = static_cast<Composite *>(model);

    int hitsLeft, hitsRight;
    Intercept interceptsLeft[MAX_INTERSECTIONS],
              interceptsRight[MAX_INTERSECTIONS];

    hitsLeft = intersect(ray, composite->left, interceptsLeft, entryMat);
    hitsRight = intersect(ray, composite->right, interceptsRight, entryMat);

    return intersectMerge(composite->op, hitsLeft, interceptsLeft,
                              hitsRight, interceptsRight, intercepts);
  } else {
    Primitive * prim = static_cast<Primitive *>(model);

    return (prim->intersect)(ray, intercepts, entryMat);
  }
}


inline int intersectMerge(int op, int hitsLeft, Intercept interceptsLeft[],
                          int hitsRight, Intercept interceptsRight[],
                          Intercept merged[]) {
  // Assume union only for now
  // TODO(kent): Handle non unions
  // TODO(kent): Change intercept lists to vectors

  int index = 0,
      leftIndex = 0,
      rightIndex = 0;
  while (leftIndex < hitsLeft && rightIndex < hitsRight) {
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

  while (index < MAX_INTERSECTIONS && leftIndex < hitsLeft) {
    merged[index] = interceptsLeft[leftIndex];
    leftIndex++;
    index++;
  }
  while (index < MAX_INTERSECTIONS && rightIndex < hitsRight) {
    merged[index] = interceptsRight[rightIndex];
    rightIndex++;
    index++;
  }

  return index;
}


int trace(int level, C_FLT weight, const Ray &ray, Color * color,
          Material * entryMat) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts, entryMat);
  if (hits > 0) {
    Point3D interceptPoint = ray.rayPoint(intercepts[0].t);
    shade(level, weight, interceptPoint, ray.dir, intercepts, color);
    return hits;
  } else {
    shadeBackground(ray, color);
    return 0;
  }
}


END_RAYTRACER
