#ifndef SRC_MODEL_H
#define SRC_MODEL_H


#include <algorithm>
#include <vector>

#include "config.h"


BEGIN_RAYTRACER


class BoundingBox;
class Primitive;
class Transform;


using namespace std;


class Node {
  public:
    unsigned char type;

    explicit Node(const unsigned char type):
      type(type) {}

    virtual ~Node() {}
};


class BoundedNode: public Node {
  public:
    BoundingBox * boundingBox;

    explicit BoundedNode(const unsigned char type):
      Node(type), boundingBox(NULL) {}

    virtual ~BoundedNode() {
      if (boundingBox) {
        delete boundingBox;
      }
    }
};


class Leaf: public BoundedNode {
  public:
    Primitive ** primitives;
    unsigned int primNum;

    explicit Leaf(vector<Primitive *>::iterator begin,
                  vector<Primitive *>::iterator end);

    ~Leaf();
};


class BVHNode: public BoundedNode {
  public:
    BoundedNode * left,
                * right;

    explicit BVHNode(BoundedNode * left, BoundedNode * right);

    ~BVHNode();
};


class KDNode: public Node {
  public:
    unsigned char axis;
    P_FLT value;
    Node * left,
         * right;

    explicit KDNode(unsigned char axis, const P_FLT value,
                    Node * left, Node * right);

    ~KDNode();
};


BoundedNode * buildBVHNode(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end);
BoundedNode * buildBVHTree(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end);
Node * buildKDNode(vector<Primitive *>::iterator begin,
                    vector<Primitive *>::iterator end, const int depth);
Node * buildKDTree(vector<Primitive *>::iterator begin,
                    vector<Primitive *>::iterator end);

bool findBVHSplit(vector<Primitive *>::iterator begin,
                  vector<Primitive *>::iterator end,
                  int * minCostIndex, P_FLT * minCost);
bool sahBVHSplit(vector<Primitive *>::iterator begin,
                 vector<Primitive *>::iterator end,
                 int * axis, int * minCostIndex);
bool findKDSplit(vector<Primitive *>::iterator begin,
                 vector<Primitive *>::iterator end,
                 const int axis, int * splitValue, P_FLT * minCost);
bool sahKDSplit(vector<Primitive *>::iterator begin,
                vector<Primitive *>::iterator end,
                int * axis, P_FLT * splitValue);


END_RAYTRACER


#endif
