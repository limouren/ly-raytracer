#ifndef MODEL_H
#define MODEL_H


#include <vector>

#include "config.h"

#include "material.h"
#include "texture.h"


BEGIN_RAYTRACER


// Ref: An Introduction to Ray Tracing; A.S. Glassner (1989)
class MODEL_CLS {
  public:
    int composite_flag;

    explicit MODEL_CLS(int composite_flag): composite_flag(composite_flag) {}
};


class Composite: public MODEL_CLS {
  public:
    char op;
    int depth;
    MODEL_CLS * left;
    MODEL_CLS * right;

    Composite(): MODEL_CLS(1) {}

    Composite(MODEL_CLS * left, MODEL_CLS * right, int depth):
      MODEL_CLS(1), op('|'), left(left), right(right), depth(depth) {}

    ~Composite() {
      delete left;
      delete right;
    }
};


class Primitive: public MODEL_CLS {
  public:
    BoundingVolume * boundingVolume;
    Material * material;
    Texture * texture;

    Primitive():
      boundingVolume(NULL), material(NULL), texture(NULL), MODEL_CLS(0) {}

    explicit Primitive(Material * material):
      boundingVolume(NULL), material(material), texture(NULL), MODEL_CLS(0) {}

    Primitive(Material * material, Texture * texture):
      boundingVolume(NULL), material(material), texture(texture),
      MODEL_CLS(0) {}

    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const {
      printf("ERROR: Unimplemented Primitive::getIntersect stub invoked\n");
      exit(1);
    }

    virtual const Color getTextureColor(const std::vector<P_FLT> mapping)
      const {
      printf("ERROR: Unimplemented Primitive::getTextureCoord stub invoked\n");
      exit(1);
    }

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const {
      printf("ERROR: Unimplemented Primitive::intersect stub invoked\n");
      exit(1);
    }

    virtual std::vector<P_FLT> inverseMap(const Point3D &point) const {
      printf("ERROR: Unimplemented Primitive::inverseMap stub invoked\n");
      exit(1);
    }

    virtual ~Primitive() {}
};


MODEL_CLS * buildModelTreeNode(std::vector<MODEL_CLS *> modelVector,
                               int depth) {
  int size = modelVector.size();

  switch (size) {
    case 0:
      return NULL;
    case 1:
      return modelVector[0];
    case 2:
      return new Composite(modelVector[0], modelVector[1], depth);
    default:
      std::vector<MODEL_CLS *>::iterator midpoint = modelVector.begin() +
                                                    (size / 2);
      std::vector<MODEL_CLS *> firstHalf(modelVector.begin(), midpoint),
                               secondHalf(midpoint, modelVector.end());

      return new Composite(buildModelTreeNode(firstHalf, depth + 1),
                           buildModelTreeNode(secondHalf, depth + 1),
                           depth);
  }
}


END_RAYTRACER


#endif
