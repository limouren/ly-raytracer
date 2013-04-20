#ifndef SRC_PRIMITIVE_H
#define SRC_PRIMITIVE_H


#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "material.h"
#include "texture.h"
#include "transform.h"


BEGIN_RAYTRACER


using namespace std;


class Primitive {
  public:
    BoundingBox * boundingBox;
    Material * material;
    Texture * texture;

    Primitive():
      boundingBox(NULL), material(NULL), texture(NULL) {}
    explicit Primitive(Material * material):
      boundingBox(NULL), material(material), texture(NULL) {}
    Primitive(Material * material, Texture * texture):
      boundingBox(NULL), material(material), texture(texture) {}

    virtual ~Primitive() {}

    virtual void buildBoundingBox();
    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              vector<P_FLT> * mapping) const;
    virtual Color getTexColor(const vector<P_FLT> mapping) const;
    virtual Primitive * interpolate(Primitive * primitive,
                                    const P_FLT ratio) const;
    virtual int intersect(const Ray &ray, Intercept intercepts[]) const;
    virtual vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform);
};


class TimedPrimitive {
  private:
    unsigned int currentFrame;
    vector<P_FLT> frametime;
    vector<Primitive *> frame;


  public:
    TimedPrimitive(): currentFrame(0) {}

    ~TimedPrimitive() {
      while (!frame.empty()) {
        delete frame.back();
        frame.pop_back();
      }
    }

    void addFrame(const P_FLT time, Primitive * prim) {
      frametime.push_back(time);
      frame.push_back(prim);
    }

    Primitive * atTime(const P_FLT time);
};


END_RAYTRACER


#endif
