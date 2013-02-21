#ifndef MODEL_H
#define MODEL_H


#include <algorithm>
#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "material.h"
#include "texture.h"
#include "transform.h"


BEGIN_RAYTRACER


// Ref: An Introduction to Ray Tracing; A.S. Glassner (1989)
class MODEL_CLS {
  public:
    unsigned char type;
    BoundingBox * boundingBox;

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
      Point3D minExtLeft = left->boundingBox->minExt,
              maxExtLeft = left->boundingBox->maxExt,
              minExtRight = right->boundingBox->minExt,
              maxExtRight = right->boundingBox->maxExt;

      boundingBox = new BoundingBox(pointMin(minExtLeft, minExtRight),
                                    pointMax(maxExtLeft, maxExtRight));
    }

    ~BVHNode() {
      delete boundingBox;

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

    virtual ~Primitive() {}

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

    virtual int intersect(const Ray &ray, Intercept intercepts[]) const {
      printf("ERROR: Unimplemented Primitive::intersect stub invoked\n");
      exit(1);
    }

    virtual std::vector<P_FLT> inverseMap(const Point3D &point) const {
      printf("ERROR: Unimplemented Primitive::inverseMap stub invoked\n");
      exit(1);
    }

    virtual void transform(Transform * transform) {
      // TODO(kent): Implement individual transform methods
    }
};


bool compareX(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->minExt.x < modelB->boundingBox->minExt.x;
}
bool compareY(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->minExt.y < modelB->boundingBox->minExt.y;
}
bool compareZ(MODEL_CLS * modelA, MODEL_CLS * modelB) {
  return modelA->boundingBox->minExt.z < modelB->boundingBox->minExt.z;
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


BoundingBox * boundingBoxBuilder(std::vector<MODEL_CLS *> primitives) {
  Point3D minExt(-P_FLT_MAX), maxExt(P_FLT_MAX);

  for (std::vector<MODEL_CLS *>::iterator itr = primitives.begin();
       itr != primitives.end(); itr++) {
    minExt = pointMin((*itr)->boundingBox->minExt, minExt);
    minExt = pointMin((*itr)->boundingBox->maxExt, maxExt);
  }

  return new BoundingBox(minExt, maxExt);
}


END_RAYTRACER


#endif
