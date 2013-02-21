#include <algorithm>
#include <vector>

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


int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[]) {
  switch (model->type) {
    case 0:  // Primitive
      return static_cast<Primitive *>(model)->intersect(ray, intercepts);

    case 1: {  // Composite
      Composite * composite = static_cast<Composite *>(model);
      int hitsLeft, hitsRight;
      Intercept interceptsLeft[MAX_INTERSECTIONS],
                interceptsRight[MAX_INTERSECTIONS];

      hitsLeft = intersect(ray, composite->left, interceptsLeft);
      hitsRight = intersect(ray, composite->right, interceptsRight);

      return intersectMerge(composite->op, hitsLeft, interceptsLeft, hitsRight,
                            interceptsRight, intercepts);
    }

    case 2: {  // K-D Tree Node
      BVHNode * bvhNode = static_cast<BVHNode *>(model);
      int hits[2];
      Intercept interceptsLists[2][MAX_INTERSECTIONS];

      if (!bvhNode->boundingBox->intersect(ray)) {
        return 0;
      }

      hits[0] = intersect(ray, bvhNode->left, interceptsLists[0]);
      hits[1] = intersect(ray, bvhNode->right, interceptsLists[1]);

      return intersectMerge(2, hits, interceptsLists, intercepts);
    }
  }

  return 0;
}


inline int intersectMerge(const int op,
                          int hitsLeft, Intercept interceptsLeft[],
                          int hitsRight, Intercept interceptsRight[],
                          Intercept merged[]) {
  // Assume union only for now
  // TODO(kent): Handle non unions
  unsigned int index = 0;
  Intercept * left = interceptsLeft,
            * right = interceptsRight;

  while (hitsLeft > 0 && hitsRight > 0) {
    if (left->t < right->t) {
      merged[index] = *left;
      hitsLeft--;
    } else {
      merged[index] = * right;
      hitsRight--;
    }
    index++;

    if (index == MAX_INTERSECTIONS) {
      return MAX_INTERSECTIONS;
    }
  }

  while (index < MAX_INTERSECTIONS && hitsLeft > 0) {
    merged[index] = *left;
    hitsLeft--;
    index++;
  }
  while (index < MAX_INTERSECTIONS && hitsRight > 0) {
    merged[index] = *right;
    hitsRight--;
    index++;
  }

  return index;
}


struct InterceptMerger {
  int remain;
  Intercept * ptr;

  inline InterceptMerger() {}
  inline InterceptMerger(int hits, Intercept * begin):
    remain(hits), ptr(begin) {}
};


inline bool compareT(InterceptMerger im1, InterceptMerger im2) {
  return im1.ptr->t < im2.ptr->t;
}


inline int intersectMerge(int listNum, int * hits,
                          Intercept interceptsLists[][MAX_INTERSECTIONS],
                          Intercept merged[]) {
  // Merge at most 8 nodes of oct tree
  InterceptMerger mergers[8];
  InterceptMerger * mergerPtr;
  unsigned int index = 0;
  for (int i = 0; i < listNum; i++) {
    if (hits[i] == 0) {
        continue;
    }

    mergers[index].remain = hits[i];
    mergers[index].ptr =  &interceptsLists[i][0];
    index++;
  }
  listNum = index;

  for (index = 0; index < MAX_INTERSECTIONS; index++) {
    if (listNum == 0) {
      return index;
    }
    mergerPtr = min_element(mergers, mergers + listNum, compareT);

    merged[index] = *(mergerPtr->ptr);
    mergerPtr->remain--;
    switch (mergerPtr->remain) {
      case 0:
        mergerPtr->ptr = mergers[listNum - 1].ptr;
        mergerPtr->remain = mergers[listNum - 1].remain;
        listNum--;
        break;
      default:
        mergerPtr->ptr++;
        break;
    }
  }

  return index;
}


int trace(const Scene * scene, const int level, const C_FLT weight,
          const Ray &ray, Color * color) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene->modelRoot, intercepts);
  if (hits > 0) {
    Point3D interceptPoint = ray.rayPoint(intercepts[0].t);
    shade(scene, level, weight, interceptPoint, ray, intercepts, color);
    return hits;
  } else {
    shadeBackground(scene, ray, color);
    return 0;
  }
}


END_RAYTRACER
