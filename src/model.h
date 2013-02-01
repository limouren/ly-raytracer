#ifndef MODEL_H
#define MODEL_H


#include <vector>

#include "config.h"

#include "geometry.h"
#include "material.h"


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
      left(left), right(right), MODEL_CLS(1) {
      op = '|';
    }

    ~Composite() {
      delete left;
      delete right;
    }
};


class Primitive: public MODEL_CLS {
  public:
    Material * material;
    Geometry * geometry;

    Primitive(): MODEL_CLS(0) {}
    Primitive(Material * material, Geometry * geometry):
      material(material), geometry(geometry), MODEL_CLS(0) {
    }
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
