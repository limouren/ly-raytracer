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
  printf("ERROR: Unimplemented Primitive::transform stub invoked\n");
  exit(1);
}


inline bool compareBoxX(BoundedModel * modelA, BoundedModel * modelB) {
  return modelA->boundingBox->center.x < modelB->boundingBox->center.x;
}
inline bool compareBoxY(BoundedModel * modelA, BoundedModel * modelB) {
  return modelA->boundingBox->center.y < modelB->boundingBox->center.y;
}
inline bool compareBoxZ(BoundedModel * modelA, BoundedModel * modelB) {
  return modelA->boundingBox->center.z < modelB->boundingBox->center.z;
}


Leaf::Leaf(vector<Primitive *>::iterator begin,
           vector<Primitive *>::iterator end)
  : BoundedModel(10), primNum(end - begin) {
  primitives = new Primitive * [primNum];
  copy(begin, end, primitives);

  Point3D minExt = primitives[0]->boundingBox->minExt,
          maxExt = primitives[0]->boundingBox->maxExt;
  for (int i = 1; i < primNum; i++) {
    minExt = pointMin(minExt, primitives[i]->boundingBox->minExt);
    maxExt = pointMax(maxExt, primitives[i]->boundingBox->maxExt);
  }

  boundingBox = new BoundingBox(minExt, maxExt);
}


Leaf::~Leaf() {
  delete primitives;
}


BVHNode::BVHNode(BoundedModel * left, BoundedModel * right)
  : BoundedModel(20), left(left), right(right) {
  boundingBox = new BoundingBox(pointMin(left->boundingBox->minExt,
                                         right->boundingBox->minExt),
                                pointMax(left->boundingBox->maxExt,
                                         right->boundingBox->maxExt));
}


BVHNode::~BVHNode() {
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


BoundedModel * buildBVHNode(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end) {
  int axis, minCostIndex;
  if (end - begin <= 8) {
    return new Leaf(begin, end);
  } else if (sahBVHSplit(begin, end, &axis, &minCostIndex)) {
    return new BVHNode(buildBVHNode(begin, begin + minCostIndex),
                       buildBVHNode(begin + minCostIndex, end));
  } else {
    return new Leaf(begin, end);
  }
}


BoundedModel * buildBVHTree(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end) {
  if (end == begin) {
    printf("ERROR: Attempted to construct tree with no primitives\n");
    exit(1);
    return NULL;
  } else {
    for (vector<Primitive *>::iterator itr = begin; itr != end; itr++) {
      static_cast<Primitive *>(*itr)->buildBoundingBox();
    }

    return buildBVHNode(begin, end);
  }
}


#define KD_MAX_DEPTH 32


Model * buildKDNode(vector<Primitive *>::iterator begin,
                    vector<Primitive *>::iterator end, const int depth) {
  if (depth == KD_MAX_DEPTH || end - begin <= 8) {
    return new Leaf(begin, end);
  } else {
    int axis;
    P_FLT splitValue;
    if (sahKDSplit(begin, end, &axis, &splitValue)) {
      vector<Primitive *> leftPrims, rightPrims;
      for (vector<Primitive *>::iterator itr = begin; itr != end; itr++) {
        if ((*itr)->boundingBox->minExt[axis] < splitValue) {
          leftPrims.push_back(*itr);
        }
        if ((*itr)->boundingBox->maxExt[axis] > splitValue) {
          rightPrims.push_back(*itr);
        }
      }

      return new KDNode(axis, splitValue,
                        buildKDNode(leftPrims.begin(), leftPrims.end(),
                                    depth + 1),
                        buildKDNode(rightPrims.begin(), rightPrims.end(),
                                    depth + 1));
    } else {
      return new Leaf(begin, end);
    }
  }
}


Model * buildKDTree(vector<Primitive *>::iterator begin,
                    vector<Primitive *>::iterator end) {
  if (begin == end) {
    printf("ERROR: Attempted to construct tree with no primitives\n");
    exit(1);
    return NULL;
  } else {
    for (vector<Primitive *>::iterator itr = begin; itr != end; itr++) {
      static_cast<Primitive *>(*itr)->buildBoundingBox();
    }

    return buildKDNode(begin, end, 0);
  }
}


bool findBVHSplit(vector<Primitive *>::iterator begin,
                  vector<Primitive *>::iterator end,
                  int * minCostIndex, P_FLT * minCost) {
  int size = end - begin;
  P_FLT * leftSA = new P_FLT[size],
        * rightSA = new P_FLT[size];

  Point3D leftMinExt(P_FLT_MAX),
          leftMaxExt(-P_FLT_MAX),
          rightMinExt(P_FLT_MAX),
          rightMaxExt(-P_FLT_MAX);
  for (int i = 1 ; i < size; i++) {
    leftMinExt = pointMin(leftMinExt, (*(begin + i))->boundingBox->minExt);
    leftMaxExt = pointMax(leftMaxExt, (*(begin + i))->boundingBox->maxExt);
    leftSA[i] = (leftMaxExt - leftMinExt).boxArea();

    rightMinExt = pointMin(rightMinExt, (*(end - i))->boundingBox->minExt);
    rightMaxExt = pointMax(rightMaxExt, (*(end - i))->boundingBox->maxExt);
    rightSA[size - i] = (rightMaxExt - rightMinExt).boxArea();
  }

  P_FLT cost,
        invTotalSA = 1.0f / (rightMaxExt - leftMinExt).boxArea();
  *minCostIndex = 0;
  *minCost = static_cast<P_FLT>(size);
  for (int leftPrimNum = 1; leftPrimNum < size; leftPrimNum++) {
    cost = 0.1f + invTotalSA * (leftSA[leftPrimNum] * leftPrimNum +
                                rightSA[leftPrimNum] * (size - leftPrimNum));
    if (cost < *minCost) {
      *minCost = cost;
      *minCostIndex = leftPrimNum;
    }
  }
  delete [] leftSA;
  delete [] rightSA;

  return minCostIndex != 0;
}


bool sahBVHSplit(vector<Primitive *>::iterator begin,
                 vector<Primitive *>::iterator end,
                 int * axis, int * minCostIndex) {
  bool xSplitSuccess, ySplitSuccess, zSplitSuccess;
  int xSplit, ySplit, zSplit;
  P_FLT xSplitCost, ySplitCost, zSplitCost;
  vector<Primitive *> xVec(begin, end),
                      yVec(begin, end),
                      zVec(begin, end);
  sort(xVec.begin(), xVec.end(), compareBoxX);
  xSplitSuccess = findBVHSplit(xVec.begin(), xVec.end(), &xSplit, &xSplitCost);
  sort(yVec.begin(), yVec.end(), compareBoxY);
  ySplitSuccess = findBVHSplit(yVec.begin(), yVec.end(), &ySplit, &ySplitCost);
  sort(zVec.begin(), zVec.end(), compareBoxZ);
  zSplitSuccess = findBVHSplit(zVec.begin(), zVec.end(), &zSplit, &zSplitCost);

  if (xSplitCost < ySplitCost) {
    if (xSplitCost < zSplitCost) {
      *axis = 0;
      *minCostIndex = xSplit;
      copy(xVec.begin(), xVec.end(), begin);
    } else {
      *axis = 2;
      *minCostIndex = zSplit;
      copy(zVec.begin(), zVec.end(), begin);
    }
  } else {
    if (ySplitCost < zSplitCost) {
      *axis = 1;
      *minCostIndex = ySplit;
      copy(yVec.begin(), yVec.end(), begin);
    } else {
      *axis = 2;
      *minCostIndex = zSplit;
      copy(zVec.begin(), zVec.end(), begin);
    }
  }

  return xSplitSuccess || ySplitSuccess || zSplitSuccess;
}


struct Edge {
  P_FLT leftSA, rightSA,
        value;
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


bool findKDSplit(vector<Primitive *>::iterator begin,
                 vector<Primitive *>::iterator end,
                 const int axis, P_FLT * splitValue, P_FLT * minCost) {
  vector<Edge> edges;
  for (vector<Primitive *>::iterator itr = begin; itr != end; itr++) {
    edges.push_back(Edge((*itr)->boundingBox->minExt[axis], *itr, true));
    edges.push_back(Edge((*itr)->boundingBox->maxExt[axis], *itr, false));
  }
  sort(edges.begin(), edges.end(), edgeSort);

  int primNum = end - begin,
      edgeNum = edges.size();

  Point3D leftMinExt = edges.front().prim->boundingBox->minExt,
          leftMaxExt = edges.front().prim->boundingBox->maxExt,
          rightMinExt = edges.back().prim->boundingBox->minExt,
          rightMaxExt = edges.back().prim->boundingBox->maxExt;
  for (int i = 0, j = edgeNum - 1; i < edgeNum; i++, j--) {
    if (edges[i].left) {
      leftMinExt = pointMin(leftMinExt, edges[i].prim->boundingBox->minExt);
      leftMaxExt = pointMax(leftMaxExt, edges[i].prim->boundingBox->maxExt);
    }
    edges[i].leftSA = (leftMaxExt - leftMinExt).boxArea();

    if (!edges[j].left) {
      rightMinExt = pointMin(rightMinExt, edges[j].prim->boundingBox->minExt);
      rightMaxExt = pointMax(rightMaxExt, edges[j].prim->boundingBox->maxExt);
    }
    edges[j].rightSA = (rightMaxExt - rightMinExt).boxArea();
  }

  int skipEdge = 0,
      leftPrims = 0,
      rightPrims = primNum;
  P_FLT cost,
        invTotalSA = 1.0f / ((rightMaxExt - leftMinExt).boxArea());

  *minCost = P_FLT_MAX;
  for (int i = 0; i < edgeNum; i++) {
    if (!edges[i].left) {
      rightPrims--;
      // Count number of boxes that share right edge at this value
      for (int j = i + 1;
           j < edgeNum && edges[j].value == edges[i].value && !edges[j].left;
           j++) {
        skipEdge++;
      }
      rightPrims -= skipEdge;
    }

    cost = 0.1f + invTotalSA * (edges[i].leftSA * leftPrims +
                                edges[i].rightSA * rightPrims);
    if (cost < *minCost) {
      *minCost = cost;
      *splitValue = edges[i].value;
    }

    if (edges[i].left) {
      leftPrims++;
      // Count number of boxes that share left edge at this value
      for (int j = i + 1;
           j < edgeNum && edges[j].value == edges[i].value && edges[j].left;
           j++) {
        skipEdge++;
      }
      leftPrims += skipEdge;
    }

    i += skipEdge;
    skipEdge = 0;
  }

  return *minCost < static_cast<P_FLT>(primNum);
}


bool sahKDSplit(vector<Primitive *>::iterator begin,
                vector<Primitive *>::iterator end,
                int * axis, P_FLT * splitValue) {
  bool xSplitSuccess, ySplitSuccess, zSplitSuccess;
  P_FLT xSplit, ySplit, zSplit;
  P_FLT minSplitCost, xSplitCost, ySplitCost, zSplitCost;
  xSplitSuccess = findKDSplit(begin, end, 0, &xSplit, &xSplitCost);
  ySplitSuccess = findKDSplit(begin, end, 1, &ySplit, &ySplitCost);
  zSplitSuccess = findKDSplit(begin, end, 2, &zSplit, &zSplitCost);

  if (!(xSplitSuccess || ySplitSuccess || zSplitSuccess)) {
    return false;
  }

  if (xSplitCost < ySplitCost) {
    if (xSplitCost < zSplitCost) {
      *axis = 0;
      *splitValue = xSplit;
    } else {
      *axis = 2;
      *splitValue = zSplit;
    }
  } else {
    if (ySplitCost < zSplitCost) {
      *axis = 1;
      *splitValue = ySplit;
    } else {
      *axis = 2;
      *splitValue = zSplit;
    }
  }

  return true;
}


END_RAYTRACER
