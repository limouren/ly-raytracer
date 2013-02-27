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
                              std::vector<P_FLT> * mapping) const;
    virtual Color getTextureColor(const std::vector<P_FLT> mapping) const;
    virtual int intersect(const Ray &ray, Intercept intercepts[]) const;
    virtual std::vector<P_FLT> inverseMap(const Point3D &point) const;
    virtual void transform(Transform * transform);
};


END_RAYTRACER


#endif
