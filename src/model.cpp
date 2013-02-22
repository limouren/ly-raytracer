#include <algorithm>
#include <set>
#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "model.h"
#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


using namespace std;


void Primitive::buildBoundingBox() {
  printf("ERROR: Unimplemented Primitive::buildBoundingBox stub "
         "invoked\n");
  exit(1);
}


void Primitive::getIntersect(const Point3D &point, Vector3D * normal,
                             std::vector<P_FLT> * mapping) const {
  printf("ERROR: Unimplemented Primitive::getIntersect stub invoked\n");
  exit(1);
}


Color Primitive::getTextureColor(const std::vector<P_FLT> mapping)
  const {
  printf("ERROR: Unimplemented Primitive::getTextureCoord stub invoked\n");
  exit(1);
}


int Primitive::intersect(const Ray &ray, Intercept intercepts[]) const {
  printf("ERROR: Unimplemented Primitive::intersect stub invoked\n");
  exit(1);
}


std::vector<P_FLT> Primitive::inverseMap(const Point3D &point) const {
  printf("ERROR: Unimplemented Primitive::inverseMap stub invoked\n");
  exit(1);
}


void Primitive::transform(Transform * transform) {
  // TODO(kent): Implement individual transform methods
}


Composite::~Composite() {
  if (left->type != 0) {
    delete left;
  }
  if (right->type != 0) {
    delete right;
  }
}


inline bool compareBoxX(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->center.x < modelB->boundingBox->center.x;
}
inline bool compareBoxY(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->center.y < modelB->boundingBox->center.y;
}
inline bool compareBoxZ(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->center.z < modelB->boundingBox->center.z;
}


BVHNode::BVHNode(MODEL_CLS * left, MODEL_CLS * right)
  : MODEL_CLS(20), left(left), right(right) {
  boundingBox = new BoundingBox(pointMin(left->boundingBox->minExt,
                                         right->boundingBox->minExt),
                                pointMax(left->boundingBox->maxExt,
                                         right->boundingBox->maxExt));
}


BVHNode::~BVHNode() {
  delete boundingBox;
  if (left->type != 0) {
    delete left;
  }
  if (right->type != 0) {
    delete right;
  }
}


MODEL_CLS * buildBVHNode(vector<Primitive *> modelVector) {
  int axis,
      size = modelVector.size();
  if (size == 1) {
    return modelVector[0];
  } else if (size <= 4) {
    axis = size / 2;
  } else {
    axis = sahSplitVector(&modelVector);
  }

  vector<Primitive *>::iterator mid = modelVector.begin() + axis;
  vector<Primitive *> leftVector(modelVector.begin(), mid),
                      rightVector(mid, modelVector.end());
  return new BVHNode(buildBVHNode(leftVector),
                     buildBVHNode(rightVector));
}


MODEL_CLS * buildBVHTree(vector<Primitive *> modelVector) {
  for (vector<Primitive *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    static_cast<Primitive *>(*itr)->buildBoundingBox();
  }

  if (modelVector.empty()) {
    printf("ERROR: Attempted to construct tree with no primitives\n");
    exit(1);
    return NULL;
  } else {
    return buildBVHNode(modelVector);
  }
}


BoundingBox * boundingBoxBuilder(vector<MODEL_CLS *> modelVector) {
  Point3D minExt(P_FLT_MAX), maxExt(-P_FLT_MAX);

  for (vector<MODEL_CLS *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    minExt = pointMin((*itr)->boundingBox->minExt, minExt);
    maxExt = pointMax((*itr)->boundingBox->maxExt, maxExt);
  }

  return new BoundingBox(minExt, maxExt);
}


void findSAHSplit(vector<Primitive *> modelVector,
                  int * minCostIndex, P_FLT * minCost) {
  int size = modelVector.size();
  P_FLT * leftSA = new P_FLT[size],
        * rightSA = new P_FLT[size];

  Point3D leftMinExt(P_FLT_MAX),
          leftMaxExt(-P_FLT_MAX);
  for (int i = 1; i < size; i++) {
    leftMinExt = pointMin(leftMinExt, modelVector[i]->boundingBox->minExt);
    leftMaxExt = pointMax(leftMaxExt, modelVector[i]->boundingBox->maxExt);
    leftSA[i] = (leftMaxExt - leftMinExt).boxArea();
  }

  Point3D rightMinExt(P_FLT_MAX),
          rightMaxExt(-P_FLT_MAX);
  for (int i = size - 1; i > 0; i--) {
    rightMinExt = pointMin(rightMinExt, modelVector[i]->boundingBox->minExt);
    rightMaxExt = pointMax(rightMaxExt, modelVector[i]->boundingBox->maxExt);
    rightSA[i] = (rightMaxExt - rightMinExt).boxArea();
  }

  P_FLT cost,
        invTotalSA = 1.0f / ((rightMaxExt - leftMinExt).boxArea() * size);
  *minCostIndex = 0;
  *minCost = P_FLT_MAX;
  for (int leftPrims = 1; leftPrims < size; leftPrims++) {
    cost = 0.1f + ((leftSA[leftPrims] * leftPrims) +
                   (rightSA[leftPrims] * (size - leftPrims))) * invTotalSA;
    if (cost <= *minCost) {
      *minCost = cost;
      *minCostIndex = leftPrims;
    }
  }
  delete [] leftSA;
  delete [] rightSA;
}


int sahSplitVector(vector<Primitive *> * modelVector) {
  int xSplit, ySplit, zSplit;
  P_FLT xSplitCost, ySplitCost, zSplitCost;
  vector<Primitive *> xVector(*modelVector),
                      yVector(*modelVector),
                      zVector(*modelVector);
  sort(xVector.begin(), xVector.end(), compareBoxX);
  findSAHSplit(xVector, &xSplit, &xSplitCost);
  sort(yVector.begin(), yVector.end(), compareBoxY);
  findSAHSplit(yVector, &ySplit, &ySplitCost);
  sort(zVector.begin(), zVector.end(), compareBoxZ);
  findSAHSplit(zVector, &zSplit, &zSplitCost);
  if (xSplitCost < ySplitCost) {
    if (xSplitCost < zSplitCost) {
      *modelVector = xVector;
      return xSplit;
    } else {
      *modelVector = zVector;
      return zSplit;
    }
  } else {
    if (ySplitCost < zSplitCost) {
      *modelVector = yVector;
      return ySplit;
    } else {
      *modelVector = zVector;
      return zSplit;
    }
  }
}


END_RAYTRACER
