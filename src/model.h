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
    BoundingBox * boundingBox;

    explicit Model(const unsigned char type):
      type(type), boundingBox(NULL) {}

    virtual ~Model() {}
};


class Primitive: public Model {
  public:
    Material * material;
    Texture * texture;

    Primitive():
      Model(0), material(NULL), texture(NULL) {}
    explicit Primitive(Material * material):
      Model(0), material(material), texture(NULL) {}
    Primitive(Material * material, Texture * texture):
      Model(0), material(material), texture(texture) {}

    virtual ~Primitive() {
      delete boundingBox;
    }

    virtual void buildBoundingBox();
    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const;
    virtual Color getTextureColor(const std::vector<P_FLT> mapping) const;
    virtual int intersect(const Ray &ray, Intercept intercepts[]) const;
    virtual std::vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform);
};


class Composite: public Model {
  public:
    Model * left,
              * right;

    Composite(): Model(1) {}
    Composite(Model * left, Model * right):
      Model(1), left(left), right(right) {}

    ~Composite();
};


class BVHNode: public Model {
  public:
    Model * left,
              * right;

    explicit BVHNode(Model * left, Model * right);

    ~BVHNode();
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


Model * buildBVHNode(vector<Primitive *> modelVector);
Model * buildBVHTree(vector<Primitive *> modelVector);
Model * buildKDNode(vector<Primitive *> modelVector, const int depth);
Model * buildKDTree(vector<Primitive *> modelVector);
BoundingBox * boundingBoxBuilder(int length, Model * modelArray[]);
BoundingBox * boundingBoxBuilder(vector<Model *> modelVector);

void findBVHSplit(vector<Primitive *> modelVector,
                  int * minCostIndex, P_FLT * minCost);
void sahBVHSplit(vector<Primitive *> * modelVector,
                 int * axis, int * minCostIndex);
bool findKDSplit(vector<Primitive *> modelVector, const int axis,
                 int * splitValue, P_FLT * minCost);
bool sahKDSplit(vector<Primitive *> modelVector,
                int * axis, P_FLT * splitValue,
                vector<Primitive *> * leftVector,
                vector<Primitive *> * rightVector);


END_RAYTRACER


#endif
