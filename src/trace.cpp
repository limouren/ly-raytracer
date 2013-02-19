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


inline int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
                     Material * entryMat) {
  switch (model->type) {
    case 0:  // Primitive
      return static_cast<Primitive *>(model)->intersect(ray, intercepts,
                                                        entryMat);

    case 1: {  // Composite
      Composite * composite = static_cast<Composite *>(model);
      int hitsLeft, hitsRight;
      Intercept interceptsLeft[MAX_INTERSECTIONS],
                interceptsRight[MAX_INTERSECTIONS];

      hitsLeft = intersect(ray, composite->left, interceptsLeft, entryMat);
      hitsRight = intersect(ray, composite->right, interceptsRight, entryMat);

      return intersectMerge(composite->op, hitsLeft, interceptsLeft, hitsRight,
                            interceptsRight, intercepts);
    }

    case 2: {  // K-D Tree Node
      BVHNode * bvhNode = static_cast<BVHNode *>(model);
      int hitsLeft, hitsRight;
      Intercept interceptsLeft[MAX_INTERSECTIONS],
                interceptsRight[MAX_INTERSECTIONS];

      if (!bvhNode->boundingVolume->intersect(ray)) {
        return 0;
      }

      hitsLeft = intersect(ray, bvhNode->left, interceptsLeft, entryMat);
      hitsRight = intersect(ray, bvhNode->right, interceptsRight, entryMat);

      return intersectMerge('|', hitsLeft, interceptsLeft, hitsRight,
                            interceptsRight, intercepts);
    }
  }

  return 0;
}


inline int intersectMerge(const int op,
                          const int hitsLeft, Intercept interceptsLeft[],
                          const int hitsRight, Intercept interceptsRight[],
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
    } else {
      merged[index] = interceptsRight[rightIndex];
      rightIndex++;
    }
    index++;

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


inline int trace(const Scene * scene, const int level, const C_FLT weight,
                 const Ray &ray, Color * color, Material * entryMat) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene->modelRoot, intercepts, entryMat);
  if (hits > 0) {
    Point3D interceptPoint = ray.rayPoint(intercepts[0].t);
    shade(scene, level, weight, interceptPoint, ray.dir, intercepts, color);
    return hits;
  } else {
    shadeBackground(scene, ray, color);
    return 0;
  }
}


END_RAYTRACER
