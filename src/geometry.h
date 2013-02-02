#ifndef SURFACE_H
#define SURFACE_H


#include "config.h"

#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


class Geometry {
  public:
    virtual const int intersect(const Ray &ray, Intercept intercepts[],
                                Material * entryMat) const {}
    virtual const Vector3D normalAt(const Point3D &point) const {}

    virtual ~Geometry() {}
};


// Planes
class Plane: public Geometry {
  public:
    // A plane is denoted by Ax+By+Cz+D = 0
    P_FLT d;  // D
    Vector3D normal;  // {A, B, C}

    Plane() {}

    Plane(const Point3D &point, const Vector3D &vector) {
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

    Polygon(int pointNum, Point3D * points):
      vertexNum(pointNum), Plane() {
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
class Sphere: public Geometry {
  public:
    P_FLT radius;
    Point3D center;

    Sphere(P_FLT x, P_FLT y, P_FLT z, P_FLT radius):
      radius(radius), center(Point3D(x, y, z)) { }
    Sphere(Point3D center, P_FLT radius): center(center), radius(radius) {}

    const int intersect(const Ray &ray, Intercept intercepts[],
                        Material * entryMat) const;
    const Vector3D normalAt(const Point3D &point) const;
};


END_RAYTRACER


#endif