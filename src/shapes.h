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

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const;
    virtual void getIntersect(const Point3D &point, Vector3D * normal,
                              std::vector<P_FLT> * mapping) const {
      *normal = this->normal;
    }
};


class Polygon: public Plane {
  public:
    int vertexNum;
    Point3D * vertex;

    Polygon(Material * material, int pointNum, Point3D * points):
      Plane(material), vertexNum(pointNum) {
      vertex = new Point3D[vertexNum];
      for (int i = 0; i < vertexNum; i++) {
        vertex[i] = points[i];
      }

      normal = crossProduct(points[1] - points[0],
                            points[2] - points[0]);
      normal.normalize();

      d = -dotProduct(vertex[0], normal);

      buildBoundingVolume();
    };

    ~Polygon() {
      delete [] vertex;
    }

    inline void buildBoundingVolume();
    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
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
};


// Quadrics
class Sphere: public Primitive {
  private:
    bool insideOut;
    P_FLT radius;
    Point3D center;

  public:
    Sphere(Material * material, const Point3D &center, P_FLT radius):
      Primitive(material), insideOut(radius < 0.0), center(center),
      radius(fabs(radius)) {
      buildBoundingVolume();
    }

    inline void buildBoundingVolume();
    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    void getIntersect(const Point3D &point, Vector3D * normal,
                      std::vector<P_FLT> * mapping) const;
};


END_RAYTRACER


#endif
