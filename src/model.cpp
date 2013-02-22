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


int findSAHSplit(vector<Primitive *> modelVector, const int axis) {
  switch (axis) {
    case 0:
      sort(modelVector.begin(), modelVector.end(), compareBoxX);
      break;
    case 1:
      sort(modelVector.begin(), modelVector.end(), compareBoxY);
      break;
    case 2:
      sort(modelVector.begin(), modelVector.end(), compareBoxZ);
      break;
  }
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

  int minCostIndex = 1;
  P_FLT cost,
        minCost = 0.1f + leftSA[1] + (rightSA[1] * (size - 1));
  for (int leftPrims = 2; leftPrims < size; leftPrims++) {
    cost = 0.1f + (leftSA[leftPrims] * leftPrims)
                + (rightSA[leftPrims] * (size - leftPrims));
    if (cost <= minCost) {
      minCost = cost;
      minCostIndex = leftPrims;
    }
  }
  delete [] leftSA;
  delete [] rightSA;

  if ((minCost > (rightMaxExt - leftMinExt).boxArea() * size) &&
      (size <= 8)) {
    return 0;
  } else {
    return minCostIndex;
  }
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
  delete left;
  delete right;
}


BVHLeaf::BVHLeaf(vector<Primitive *> modelVector)
  :MODEL_CLS(21), primNum(modelVector.size()) {
  if (primNum > 8) {
    printf("ERROR: Create BVHLeaf with >8 primitives: (%d)\n", primNum);
    throw 20;
    exit(1);
  }

  copy(modelVector.begin(), modelVector.end(), primitives);
  MODEL_CLS * bounders[8];
  copy(primitives, primitives + primNum, bounders);
  boundingBox = boundingBoxBuilder(primNum, bounders);
}


MODEL_CLS * buildBVHNode(vector<Primitive *> modelVector, const int depth) {
  if (modelVector.size() < 9) {
    return new BVHLeaf(modelVector);
  }

  int axis = findSAHSplit(modelVector, depth % 3);
  if (axis == 0) {
    return new BVHLeaf(modelVector);
  }

  vector<Primitive *>::iterator mid = modelVector.begin() + axis;
  vector<Primitive *> leftVector(modelVector.begin(), mid),
                      rightVector(mid, modelVector.end());
  return new BVHNode(buildBVHNode(leftVector, depth + 1),
                     buildBVHNode(rightVector, depth + 1));
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
    return buildBVHNode(modelVector, 0);
  }
}


BoundingBox * boundingBoxBuilder(const int length, MODEL_CLS * modelArray[]) {
  Point3D minExt(P_FLT_MAX), maxExt(-P_FLT_MAX);

  for (int i = 0; i < length; i++) {
    minExt = pointMin(modelArray[i]->boundingBox->minExt, minExt);
    maxExt = pointMax(modelArray[i]->boundingBox->maxExt, maxExt);
  }

  return new BoundingBox(minExt, maxExt);
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


END_RAYTRACER
