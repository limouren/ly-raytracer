#include <algorithm>
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


inline bool compareBoxX(Model * modelA, Model * modelB) {
  return modelA->boundingBox->center.x < modelB->boundingBox->center.x;
}
inline bool compareBoxY(Model * modelA, Model * modelB) {
  return modelA->boundingBox->center.y < modelB->boundingBox->center.y;
}
inline bool compareBoxZ(Model * modelA, Model * modelB) {
  return modelA->boundingBox->center.z < modelB->boundingBox->center.z;
}


BVHNode::BVHNode(Model * left, Model * right)
  : Model(20), left(left), right(right) {
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


KDNode::KDNode(const unsigned char axis, const P_FLT value,
                 Model * left, Model * right)
  : Model(30), axis(axis), value(value), left(left), right(right) {}


KDNode::~KDNode() {
  if (left->type != 0) {
    delete left;
  }
  if (right->type != 0) {
    delete right;
  }
}


Model * buildBVHNode(vector<Primitive *> modelVector) {
  int axis, minCostIndex,
      size = modelVector.size();
  if (size == 1) {
    return modelVector[0];
  } else if (size <= 4) {
    minCostIndex = size / 2;
  } else {
    sahBVHSplit(&modelVector, &axis, &minCostIndex);
  }

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

  vector<Primitive *>::iterator mid = modelVector.begin() + minCostIndex;
  vector<Primitive *> leftVector(modelVector.begin(), mid),
                      rightVector(mid, modelVector.end());
  return new BVHNode(buildBVHNode(leftVector),
                     buildBVHNode(rightVector));
}


Model * buildBVHTree(vector<Primitive *> modelVector) {
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


#define KD_MAX_DEPTH 32


Model * buildKDNode(vector<Primitive *> modelVector, const int depth) {
  if (modelVector.size() == 1) {
    return modelVector[0];
  } else if (depth == KD_MAX_DEPTH) {
    return buildBVHNode(modelVector);
  }

  int axis;
  P_FLT value;
  vector<Primitive *> leftVector, rightVector;
  if (sahKDSplit(modelVector, &axis, &value, &leftVector, &rightVector)) {
    Model * leftNode = buildKDNode(leftVector, depth + 1);
    Model * rightNode = buildKDNode(rightVector, depth + 1);

    return new KDNode(axis, value, leftNode, rightNode);
  } else {
    return buildBVHNode(modelVector);
  }
}


Model * buildKDTree(vector<Primitive *> modelVector) {
  for (vector<Primitive *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    static_cast<Primitive *>(*itr)->buildBoundingBox();
  }

  if (modelVector.empty()) {
    printf("ERROR: Attempted to construct tree with no primitives\n");
    exit(1);
    return NULL;
  } else {
    return buildKDNode(modelVector, 0);
  }
}


BoundingBox * boundingBoxBuilder(vector<Model *> modelVector) {
  Point3D minExt(P_FLT_MAX), maxExt(-P_FLT_MAX);

  for (vector<Model *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    minExt = pointMin((*itr)->boundingBox->minExt, minExt);
    maxExt = pointMax((*itr)->boundingBox->maxExt, maxExt);
  }

  return new BoundingBox(minExt, maxExt);
}


void findBVHSplit(vector<Primitive *> modelVector,
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
    if (cost < *minCost) {
      *minCost = cost;
      *minCostIndex = leftPrims;
    }
  }
  delete [] leftSA;
  delete [] rightSA;
}


void sahBVHSplit(vector<Primitive *> * modelVector,
                 int * axis, int * minCostIndex) {
  int xSplit, ySplit, zSplit;
  P_FLT xSplitCost, ySplitCost, zSplitCost;
  vector<Primitive *> xVector(*modelVector),
                      yVector(*modelVector),
                      zVector(*modelVector);
  sort(xVector.begin(), xVector.end(), compareBoxX);
  findBVHSplit(xVector, &xSplit, &xSplitCost);
  sort(yVector.begin(), yVector.end(), compareBoxY);
  findBVHSplit(yVector, &ySplit, &ySplitCost);
  sort(zVector.begin(), zVector.end(), compareBoxZ);
  findBVHSplit(zVector, &zSplit, &zSplitCost);
  if (xSplitCost < ySplitCost) {
    if (xSplitCost < zSplitCost) {
      *axis = 0;
      *minCostIndex = xSplit;
      *modelVector = xVector;
    } else {
      *axis = 2;
      *minCostIndex = zSplit;
      *modelVector = zVector;
    }
  } else {
    if (ySplitCost < zSplitCost) {
      *axis = 1;
      *minCostIndex = ySplit;
      *modelVector = yVector;
    } else {
      *axis = 2;
      *minCostIndex = zSplit;
      *modelVector = zVector;
    }
  }
}


struct Edge {
  P_FLT value;
  Primitive * prim;
  bool left;

  Edge(P_FLT value, Primitive * prim, bool left)
    : value(value), prim(prim), left(left) {}
};


bool edgeSort(const Edge &edgeA, const Edge &edgeB) {
  if (edgeA.value == edgeB.value) {
    if (edgeA.left == edgeB.left) {
      return edgeA.prim < edgeB.prim;
    }

    return edgeA.left;
  }

  return edgeA.value < edgeB.value;
}


bool findKDSplit(vector<Primitive *> modelVector, const int axis,
                 P_FLT * splitValue, P_FLT * minCost) {
  vector<Edge> edges;
  for (vector<Primitive *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    edges.push_back(Edge((*itr)->boundingBox->minExt[axis], *itr, true));
    edges.push_back(Edge((*itr)->boundingBox->maxExt[axis], *itr, false));
  }
  sort(edges.begin(), edges.end(), edgeSort);

  int primNum = modelVector.size(),
      edgeNum = edges.size();
  P_FLT * leftSA = new P_FLT[edgeNum],
        * rightSA = new P_FLT[edgeNum];

  Point3D leftMinExt = edges.front().prim->boundingBox->minExt,
          leftMaxExt = edges.front().prim->boundingBox->maxExt,
          rightMinExt = edges.back().prim->boundingBox->minExt,
          rightMaxExt = edges.back().prim->boundingBox->maxExt;
  for (int i = 0, j = edgeNum - 1; i < edgeNum; i++, j--) {
    if (edges[i].left) {
      leftMinExt = pointMin(leftMinExt, edges[i].prim->boundingBox->minExt);
      leftMaxExt = pointMax(leftMaxExt, edges[i].prim->boundingBox->maxExt);
    }
    leftSA[i] = (leftMaxExt - leftMinExt).boxArea();

    if (!edges[j].left) {
      rightMinExt = pointMin(rightMinExt, edges[j].prim->boundingBox->minExt);
      rightMaxExt = pointMax(rightMaxExt, edges[j].prim->boundingBox->maxExt);
    }
    rightSA[j] = (rightMaxExt - rightMinExt).boxArea();
  }

  bool splitSuccess = false;
  int countedLeftEdge = 0,
      countedRightEdge = 0,
      leftPrims = 0,
      rightPrims = primNum;
  P_FLT cost,
        invTotalSA = 1.0f / ((rightMaxExt - leftMinExt).boxArea());

  *minCost = static_cast<P_FLT>(primNum);
  for (int i = 0; i < edgeNum; i++) {
    if (edges[i].left) {
      if (countedLeftEdge > 0) {
        countedLeftEdge--;
      } else {
        leftPrims++;
        // Count number of boxes that share left edge at this value
        for (int j = i + 1; j < edgeNum; j++) {
          if (edges[j].value == edges[i].value && edges[j].left) {
            countedLeftEdge++;
          } else {
            break;
          };
        }

        leftPrims += countedLeftEdge;
        cost = 0.2f + ((leftSA[i] * leftPrims) +
                       (rightSA[i] * rightPrims)) * invTotalSA;
        if (cost < *minCost) {
          splitSuccess = true;
          *minCost = cost;
          *splitValue = edges[i].value;
        }
      }
    } else {
      if (countedRightEdge > 0) {
        countedRightEdge--;
      } else {
        cost = 0.2f + ((leftSA[i] * leftPrims) +
                       (rightSA[i] * rightPrims)) * invTotalSA;
        if (cost < *minCost) {
          splitSuccess = true;
          *minCost = cost;
          *splitValue = edges[i].value;
        }

        rightPrims--;
        // Count number of boxes that share right edge at this value
        for (int j = i + 1; j < edgeNum; j++) {
          if (edges[j].value == edges[i].value && !edges[j].left) {
            countedRightEdge++;
          } else {
            break;
          };
        }

        rightPrims -= countedRightEdge;
      }
    }
  }
  delete [] leftSA;
  delete [] rightSA;

  return splitSuccess;
}


bool sahKDSplit(vector<Primitive *> modelVector,
                int * axis, P_FLT * splitValue,
                vector<Primitive *> * leftVector,
                vector<Primitive *> * rightVector) {
  bool xSplitSuccess, ySplitSuccess, zSplitSuccess;
  P_FLT xSplit, ySplit, zSplit;
  P_FLT minSplitCost, xSplitCost, ySplitCost, zSplitCost;
  vector<Primitive *> xVector(modelVector),
                      yVector(modelVector),
                      zVector(modelVector);
  xSplitSuccess = findKDSplit(xVector, 0, &xSplit, &xSplitCost);
  ySplitSuccess = findKDSplit(yVector, 1, &ySplit, &ySplitCost);
  zSplitSuccess = findKDSplit(zVector, 2, &zSplit, &zSplitCost);

  if (!(xSplitSuccess || ySplitSuccess || zSplitSuccess)) {
    return false;
  }

  if (xSplitCost < ySplitCost) {
    if (xSplitCost < zSplitCost) {
      *axis = 0;
      *splitValue = xSplit;
      modelVector = xVector;
    } else {
      *axis = 2;
      *splitValue = zSplit;
      modelVector = zVector;
    }
  } else {
    if (ySplitCost < zSplitCost) {
      *axis = 1;
      *splitValue = ySplit;
      modelVector = yVector;
    } else {
      *axis = 2;
      *splitValue = zSplit;
      modelVector = zVector;
    }
  }

  for (vector<Primitive *>::iterator itr = modelVector.begin();
       itr != modelVector.end(); itr++) {
    if ((*itr)->boundingBox->minExt[*axis] <= *splitValue) {
      leftVector->push_back(*itr);
    }
    if ((*itr)->boundingBox->maxExt[*axis] >= *splitValue) {
      rightVector->push_back(*itr);
    }
  }

  /*printf("Split vector of size %d into %d : %d\n",
         modelVector.size(), leftVector->size(), rightVector->size());
  printf("Expansion factor: %.2f%%.\n",
         (leftVector->size() + rightVector->size()) * 100.0f /
          modelVector.size());
  getchar();*/
  return true;
}


END_RAYTRACER
