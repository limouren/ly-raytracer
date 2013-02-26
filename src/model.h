#ifndef SRC_MODEL_H
#define SRC_MODEL_H


#include <algorithm>
#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "material.h"
#include "texture.h"


BEGIN_RAYTRACER


class Transform;


using namespace std;


class Model {
  public:
    unsigned char type;

    explicit Model(const unsigned char type):
      type(type) {}

    virtual ~Model() {}
};


class BoundedModel: public Model {
  public:
    BoundingBox * boundingBox;

    explicit BoundedModel(const unsigned char type):
      Model(type), boundingBox(NULL) {}

    virtual ~BoundedModel() {
      if (boundingBox) {
        delete boundingBox;
      }
    }
};


class Primitive: public BoundedModel {
  public:
    Material * material;
    Texture * texture;

    Primitive():
      BoundedModel(0), material(NULL), texture(NULL) {}
    explicit Primitive(Material * material):
      BoundedModel(0), material(material), texture(NULL) {}
    Primitive(Material * material, Texture * texture):
      BoundedModel(0), material(material), texture(texture) {}

    virtual ~Primitive() {}

    virtual void buildBoundingBox();
    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              vector<P_FLT> * mapping) const;
    virtual Color getTextureColor(const vector<P_FLT> mapping) const;
    virtual int intersect(const Ray &ray, Intercept intercepts[]) const;
    virtual vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform);
};


class BVHNode: public BoundedModel {
  public:
    BoundedModel * left,
                 * right;

    explicit BVHNode(BoundedModel * left, BoundedModel * right);

    ~BVHNode();
};


class Leaf: public BoundedModel {
  public:
    Primitive ** primitives;
    unsigned int primNum;

    explicit Leaf(vector<Primitive *>::iterator begin,
                  vector<Primitive *>::iterator end);

    ~Leaf();
};


class KDNode: public Model {
  public:
    unsigned char axis;
    P_FLT value;
    Model * left,
          * right;

    explicit KDNode(unsigned char axis, const P_FLT value,
                    Model * left, Model * right);

    ~KDNode();
};


BoundedModel * buildBVHNode(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end);
BoundedModel * buildBVHTree(vector<Primitive *>::iterator begin,
                            vector<Primitive *>::iterator end);
Model * buildKDNode(vector<Primitive *>::iterator begin,
                    vector<Primitive *>::iterator end, const int depth);
Model * buildKDTree(vector<Primitive *>::iterator begin,
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
