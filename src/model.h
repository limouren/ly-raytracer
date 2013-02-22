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


class MODEL_CLS {
  public:
    unsigned char type;
    BoundingBox * boundingBox;

    explicit MODEL_CLS(const unsigned char type):
      type(type), boundingBox(NULL) {}

    virtual ~MODEL_CLS() {}
};


class Primitive: public MODEL_CLS {
  public:
    Material * material;
    Texture * texture;

    Primitive():
      MODEL_CLS(0), material(NULL), texture(NULL) {}
    explicit Primitive(Material * material):
      MODEL_CLS(0), material(material), texture(NULL) {}
    Primitive(Material * material, Texture * texture):
      MODEL_CLS(0), material(material), texture(texture) {}

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


class Composite: public MODEL_CLS {
  public:
    MODEL_CLS * left,
              * right;

    Composite(): MODEL_CLS(1) {}
    Composite(MODEL_CLS * left, MODEL_CLS * right):
      MODEL_CLS(1), left(left), right(right) {}

    ~Composite();
};


class BVHNode: public MODEL_CLS {
  public:
    MODEL_CLS * left,
              * right;

    explicit BVHNode(MODEL_CLS * left, MODEL_CLS * right);

    ~BVHNode();
};


class BVHLeaf: public MODEL_CLS {
  public:
    Primitive * primitives[8];
    int primNum;

    explicit BVHLeaf(vector<Primitive *> modelVector);

    ~BVHLeaf() {}
};

MODEL_CLS * buildBVHNode(vector<Primitive *> modelVector, int depth);
MODEL_CLS * buildBVHTree(std::vector<Primitive *> modelVector);
BoundingBox * boundingBoxBuilder(int length, MODEL_CLS * modelArray[]);
BoundingBox * boundingBoxBuilder(vector<MODEL_CLS *> modelVector);


END_RAYTRACER


#endif
