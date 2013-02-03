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

    Plane(Material * material, const Point3D &point, const Vector3D &vector):
      Primitive(material) {
      normal = vector;
      normal.normalize();

      d = - dotProduct(point, normal);
    }

    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const;
    virtual const Vector3D normalAt(const Point3D &point) const {
      return normal;
    }
};


class Polygon: public Plane {
  public:
    int vertexNum;
    Point3D * vertex;

    Polygon(Material * material, int pointNum, Point3D * points):
      Plane(material), vertexNum(pointNum) {
      vertex = new Point3D[pointNum];
      for (int i = 0; i < vertexNum; i++) {
        vertex[i] = points[i];
      }

      normal = crossProduct(points[1] - points[0],
                            points[2] - points[0]);
      normal.normalize();

      d = -dotProduct(vertex[0], normal);
    };

    ~Polygon() {
      delete [] vertex;
    }

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
};


// Quadrics
class Sphere: public Primitive {
  public:
    P_FLT radius;
    Point3D center;

    Sphere(Material * material, Point3D center, P_FLT radius):
      Primitive(material), center(center), radius(radius) {}

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    const Vector3D normalAt(const Point3D &point) const;
};


END_RAYTRACER


#endif
