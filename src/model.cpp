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
  return modelA->boundingBox->minExt.x + modelA->boundingBox->maxExt.x <
         modelB->boundingBox->minExt.x + modelB->boundingBox->maxExt.x;
}
inline bool compareBoxY(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->minExt.y + modelA->boundingBox->maxExt.y <
         modelB->boundingBox->minExt.y + modelB->boundingBox->maxExt.y;
}
inline bool compareBoxZ(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->minExt.z + modelA->boundingBox->maxExt.z <
         modelB->boundingBox->minExt.z + modelB->boundingBox->maxExt.z;
}


BVHNode::BVHNode(vector<Primitive *> modelVector, const int depth)
  : MODEL_CLS(20) {
  int axis = depth % 3,
      half = modelVector.size() / 2;
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
  vector<Primitive *>::iterator mid = modelVector.begin() + half;

  vector<Primitive *> leftVec(modelVector.begin(), mid);
  vector<Primitive *> rightVec(mid, modelVector.end());

  if (half > 7) {
    left = new BVHNode(leftVec, depth + 1);
    right = new BVHNode(rightVec, depth + 1);
  } else {
    left = new BVHLeaf(leftVec);
    right = new BVHLeaf(rightVec);
  }

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
  copy(modelVector.begin(), modelVector.end(), primitives);
  MODEL_CLS * bounders[8];
  copy(primitives, primitives + primNum, bounders);
  boundingBox = boundingBoxBuilder(primNum, bounders);
}


MODEL_CLS * buildBVHTree(vector<Primitive *> modelVector) {
  for (vector<Primitive *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    static_cast<Primitive *>(*itr)->buildBoundingBox();
  }

  int size = modelVector.size();

  if (size > 8) {
    return new BVHNode(modelVector, 0);
  } else if (size <= 8) {
    return new BVHLeaf(modelVector);
  } else {
    printf("ERROR: Attempted to construct empty model tree\n");
    exit(1);
    return NULL;
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
