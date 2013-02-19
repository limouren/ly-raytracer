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

  inline InterceptMerger(int hits, Intercept * begin):
    remain(hits), ptr(begin) {}
};


inline bool compareT(InterceptMerger im1, InterceptMerger im2) {
  return im1.ptr->t < im2.ptr->t;
}


inline int intersectMerge(int listNum, int * hits,
                          Intercept interceptsLists[][MAX_INTERSECTIONS],
                          Intercept merged[]) {
  std::list<InterceptMerger> mergers;
  for (int i = 0; i < listNum; i++) {
    if (hits[i]) {
      mergers.push_front(InterceptMerger(hits[i], &interceptsLists[i][0]));
    }
  }

  std::list<InterceptMerger>::iterator itr;
  for (unsigned int index = 0; index < MAX_INTERSECTIONS; index++) {
    if (mergers.empty()) {
      return index;
    }
    itr = min_element(mergers.begin(), mergers.end(), compareT);

    merged[index] = *(itr->ptr);
    itr->remain--;
    switch (itr->remain) {
      case 0:
        mergers.erase(itr);
        break;
      default:
        itr->ptr++;
        break;
    }
  }

  return MAX_INTERSECTIONS;
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
