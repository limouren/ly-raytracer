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
    MODEL_CLS * left;
    MODEL_CLS * right;

    Composite(): MODEL_CLS(1) {}
    Composite(MODEL_CLS * left, MODEL_CLS * right):
      op('|'), left(left), right(right), MODEL_CLS(1) {}

    ~Composite() {
      delete left;
      delete right;
    }
};


class Primitive: public MODEL_CLS {
  public:
    Material * material;
    Texture * texture;

    Primitive(): MODEL_CLS(0) {}
    explicit Primitive(Material * material):
      material(material), MODEL_CLS(0) {}
    Primitive(Material * material, Texture * texture):
      material(material), texture(texture), MODEL_CLS(0) {}

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const {
      printf("WARNING: Unimplemented Primitive::intersect stub invoked\n");
    }
    virtual const Vector3D normalAt(const Point3D &point) const {
      printf("WARNING: Unimplemented Primitive::normalAt stub invoked\n");
    }
    virtual const Vector2D textureCoordAt(const Point3D &point) const {
      printf("WARNING: Unimplemented Primitive::textureCoordAt stub "
             "invoked\n");
    }

    virtual ~Primitive() {}
};


MODEL_CLS * buildModelTree(std::vector<MODEL_CLS *> modelVector) {
  int size = modelVector.size();

  switch (size) {
    case 0:
      return NULL;
    case 1:
      return modelVector[0];
    case 2:
      return new Composite(modelVector[0], modelVector[1]);
    default:
      std::vector<MODEL_CLS *>::iterator midpoint = modelVector.begin() +
                                                    (size / 2);
      std::vector<MODEL_CLS *> firstHalf(modelVector.begin(), midpoint),
                               secondHalf(midpoint, modelVector.end());

      return new Composite(buildModelTree(firstHalf),
                           buildModelTree(secondHalf));
  }
}


END_RAYTRACER


#endif
