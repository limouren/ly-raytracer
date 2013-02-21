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
    case 0: {  // Primitive
      Primitive * primitive = static_cast<Primitive *>(model);
      if (!primitive->boundingBox->intersect(ray)) {
        return 0;
      } else {
        return primitive->intersect(ray, intercepts);
      }
    }

    case 1: {  // Composite
      Composite * composite = static_cast<Composite *>(model);
      int hitsLeft, hitsRight;
      Intercept interceptsLeft[MAX_INTERSECTIONS],
                interceptsRight[MAX_INTERSECTIONS];

      hitsLeft = intersect(ray, composite->left, interceptsLeft);
      hitsRight = intersect(ray, composite->right, interceptsRight);

      return intersectMergeTwo(hitsLeft, interceptsLeft,
                               hitsRight, interceptsRight, intercepts);
    }

    case 20: {  // BVH Node
      BVHNode * bvhNode = static_cast<BVHNode *>(model);

      if (!bvhNode->boundingBox->intersect(ray)) {
        return 0;
      }

      int hits[2];
      Intercept interceptsLists[2][MAX_INTERSECTIONS];

      hits[0] = intersect(ray, bvhNode->left, interceptsLists[0]);
      hits[1] = intersect(ray, bvhNode->right, interceptsLists[1]);

      return intersectMergeTwo(hits[0], interceptsLists[0],
                               hits[1], interceptsLists[1], intercepts);
    }

    case 21: {  // BVH Leaf
      BVHLeaf * bvhLeaf = static_cast<BVHLeaf *>(model);

      if (bvhLeaf->primNum == 0 || (!bvhLeaf->boundingBox->intersect(ray))) {
        return 0;
      }

      switch (bvhLeaf->primNum) {
        case 1: {
          if (!bvhLeaf->primitives[0]->boundingBox->intersect(ray)) {
            return 0;
          }
          return bvhLeaf->primitives[0]->intersect(ray, intercepts);
        }
        case 2: {
          int hits[2];
          Intercept interceptsLists[2][MAX_INTERSECTIONS];
          if (bvhLeaf->primitives[0]->boundingBox->intersect(ray)) {
            hits[0] = bvhLeaf->primitives[0]->intersect(ray,
                                                        interceptsLists[0]);
          } else {
            hits[0] = 0;
          }
          if (bvhLeaf->primitives[1]->boundingBox->intersect(ray)) {
            hits[1] = bvhLeaf->primitives[1]->intersect(ray,
                                                        interceptsLists[1]);
          } else {
            hits[1] = 0;
          }
          return intersectMergeTwo(hits[0], interceptsLists[0],
                                   hits[1], interceptsLists[1], intercepts);
        }
        default: {
          int hits[8];
          Intercept interceptsLists[8][MAX_INTERSECTIONS];
          for (int i = 0; i < bvhLeaf->primNum; i++) {
            if (bvhLeaf->primitives[i]->boundingBox->intersect(ray)) {
              hits[i] = bvhLeaf->primitives[i]->intersect(ray,
                                                          interceptsLists[i]);
            } else {
              hits[i] = 0;
            }
          }
          return intersectMergeMulti(bvhLeaf->primNum, hits, interceptsLists,
                                     intercepts);
        }
      }
    }
  }

  return 0;
}


inline int intersectMergeTwo(int hitsLeft, Intercept interceptsLeft[],
                             int hitsRight, Intercept interceptsRight[],
                             Intercept merged[]) {
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


inline int intersectMergeMulti(int listNum, int * hits,
                               Intercept interceptsLists[][MAX_INTERSECTIONS],
                               Intercept merged[]) {
  // NOTE: Merge at most 8; Designed for use in octree
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
