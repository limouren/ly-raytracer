#ifndef MODEL_H
#define MODEL_H


#include <algorithm>
#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "material.h"
#include "texture.h"


BEGIN_RAYTRACER


// Ref: An Introduction to Ray Tracing; A.S. Glassner (1989)
class MODEL_CLS {
  public:
    unsigned char type;
    BoundingVolume * boundingVolume;

    explicit MODEL_CLS(const unsigned char type): type(type) {}

    virtual ~MODEL_CLS() {}
};


class BVHNode: public MODEL_CLS {
  public:
    MODEL_CLS * left,
              * right;

    BVHNode(): MODEL_CLS(2) {}

    BVHNode(MODEL_CLS * left, MODEL_CLS * right):
      MODEL_CLS(2), left(left), right(right) {
      Point3D minExtLeft, maxExtLeft, minExtRight, maxExtRight;
      left->boundingVolume->getBox(&minExtLeft, &maxExtLeft);
      right->boundingVolume->getBox(&minExtRight, &maxExtRight);

      boundingVolume = new Box(pointMin(minExtLeft, minExtRight),
                               pointMax(maxExtLeft, maxExtRight));
    }

    ~BVHNode() {
      delete boundingVolume;

      if (left->type != 0) {
        delete left;
      }
      if (right->type != 0) {
        delete right;
      }
    }
};


class Composite: public MODEL_CLS {
  public:
    char op;
    MODEL_CLS * left,
              * right;

    Composite(): MODEL_CLS(1) {}

    Composite(MODEL_CLS * left, MODEL_CLS * right):
      MODEL_CLS(1), op('|'), left(left), right(right) {}

    ~Composite() {
      if (left->type != 0) {
        delete left;
      }
      if (right->type != 0) {
        delete right;
      }
    }
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

    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const {
      printf("ERROR: Unimplemented Primitive::getIntersect stub invoked\n");
      exit(1);
    }

    virtual Color getTextureColor(const std::vector<P_FLT> mapping)
      const {
      printf("ERROR: Unimplemented Primitive::getTextureCoord stub invoked\n");
      exit(1);
    }

    virtual int intersect(const Ray &ray, Intercept intercepts[],
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


bool compareX(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  Point3D minExtA, maxExtA, minExtB, maxExtB;
  modelA->boundingVolume->getBox(&minExtA, &maxExtA);
  modelB->boundingVolume->getBox(&minExtB, &maxExtB);
  return maxExtA.x < maxExtB.x;
}
bool compareY(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  Point3D minExtA, maxExtA, minExtB, maxExtB;
  modelA->boundingVolume->getBox(&minExtA, &maxExtA);
  modelB->boundingVolume->getBox(&minExtB, &maxExtB);
  return maxExtA.y < maxExtB.y;
}
bool compareZ(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  Point3D minExtA, maxExtA, minExtB, maxExtB;
  modelA->boundingVolume->getBox(&minExtA, &maxExtA);
  modelB->boundingVolume->getBox(&minExtB, &maxExtB);
  return maxExtA.z < maxExtB.z;
}


MODEL_CLS * buildModelTreeNode(std::vector<MODEL_CLS *> modelVector,
                               const int depth) {
  int size = modelVector.size();

  switch (size) {
    case 0:
      printf("ERROR: Attempted to construct empty model tree\n");
      exit(1);
      return NULL;

    case 1:
      return modelVector[0];

    default:
      switch (depth % 3) {
        case 0:
          std::sort(modelVector.begin(), modelVector.end(), compareX);
          break;
        case 1:
          std::sort(modelVector.begin(), modelVector.end(), compareY);
          break;
        case 2:
          std::sort(modelVector.begin(), modelVector.end(), compareZ);
          break;
      }
      std::vector<MODEL_CLS *>::iterator midpoint = modelVector.begin() +
                                                    (size / 2);
      std::vector<MODEL_CLS *> firstHalf(modelVector.begin(), midpoint),
                               secondHalf(midpoint, modelVector.end());

      MODEL_CLS * left = buildModelTreeNode(firstHalf, depth + 1),
                * right = buildModelTreeNode(secondHalf, depth + 1);

      return new BVHNode(left, right);
  }
}


END_RAYTRACER


#endif
