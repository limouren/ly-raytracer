#ifndef SURFACE_H
#define SURFACE_H


#include "config.h"

#include "intercept.h"
#include "model.h"
#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


// Planes
class Plane: public Primitive {
  public:
    // A plane is denoted by Ax+By+Cz+D = 0
    P_FLT d;  // D
    Vector3D normal;  // {A, B, C}

    explicit Plane(Material * material): Primitive(material) {}
    Plane(Material * material, Texture * texture):
      Primitive(material, texture) {}

    Plane(Material * material, const Point3D &point, const Vector3D &vector):
      Primitive(material) {
      normal = vector;
      normal.normalize();

      d = - dotProduct(point, normal);
    }

    virtual ~Plane() {}

    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const {
      *normal = this->normal;
    }
    virtual int intersect(const Ray &ray, Intercept intercepts[],
                          Material * entryMat) const;
    void transform(Transform * transform);
};


/* class Circle: public Plane {
  public:
    P_FLT radius;
    Point3D center;

    Circle(Material * material, const Point3D &center, const P_FLT radius,
           const Vector3D &normal):
      Plane(material), normal(normal), radius(radius), center(center) {}

    int intersect(const Ray &ray, Intercept intercepts[],
                  Material * entrymat) const;
} */


class Polygon: public Plane {
  public:
    int dominantIndex, vertexNum;
    Point3D * vertex;

    Polygon(Material * material, int pointNum, Point3D * points):
      Plane(material), vertexNum(pointNum) {
      vertex = new Point3D[vertexNum];
      for (int i = 0; i < vertexNum; i++) {
        vertex[i] = points[i];
      }

      normal = crossProduct(vertex[1] - vertex[0], vertex[2] - vertex[0]);
      normal.normalize();
      dominantIndex = normal.dominantIndex();

      d = -dotProduct(vertex[0], normal);

      buildBoundingVolume();
    };

    virtual ~Polygon() {
      delete [] vertex;
      delete boundingVolume;
    }

    void buildBoundingVolume();
    int intersect(const Ray &ray, Intercept intercepts[],
                  Material * entryMat) const;
    void transform(Transform * transform);
};


class PolygonPatch: public Polygon {
  public:
    Vector3D * vertexNormal;

    PolygonPatch(Material * material, int pointNum, Point3D * points,
                 Vector3D * normals):
      Polygon(material, pointNum, points) {
      vertexNormal = new Vector3D[vertexNum];
      for (int i = 0; i < vertexNum; i++) {
        vertexNormal[i] = normals[i];
      }
    }

    ~PolygonPatch() {
      delete[] vertexNormal;
    }

    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const;
    void transform(Transform * transform);
};


// Quadrics
class Cylinder: public Primitive {
  private:
    bool insideOut;
    P_FLT height, radius, radiusSqr, apexD, baseD;
    Point3D apexCenter, baseCenter;
    Vector3D axisNormal;

  public:
    // NOTE: -ve radius means the cylinder is inward facing
    Cylinder(Material * material, const Point3D &baseCenter,
             const Point3D &apexCenter, const P_FLT radius):
      Primitive(material), insideOut(radius < 0.0f),
      radius(fabs(radius)), radiusSqr(radius * radius),
      apexCenter(apexCenter), baseCenter(baseCenter),
      axisNormal(apexCenter - baseCenter) {
      height = axisNormal.normalize();
      apexD = -dotProduct(axisNormal, apexCenter);
      baseD = -dotProduct(axisNormal, baseCenter);

      buildBoundingVolume();
    }

    ~Cylinder() {
      delete boundingVolume;
    }

    inline void buildBoundingVolume();
    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    int intersect(const Ray &ray, Intercept intercepts[],
                  Material * entryMat) const;
    void transform(Transform * transform);
};


class Sphere: public Primitive {
  private:
    bool insideOut;
    P_FLT radius;
    Point3D center;

  public:
    Sphere(Material * material, const Point3D &center, P_FLT radius):
      Primitive(material), insideOut(radius < 0.0),
      radius(fabs(radius)), center(center) {
      buildBoundingVolume();
    }

    ~Sphere() {
      delete boundingVolume;
    }

    inline void buildBoundingVolume();
    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
    int intersect(const Ray &ray, Intercept intercepts[],
                  Material * entryMat) const;
    void transform(Transform * transform);
};


END_RAYTRACER


#endif
