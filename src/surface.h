#ifndef SURFACE_H
#define SURFACE_H


#include "config.h"

#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


class Surface {
  public:
    virtual const int intersect(const Ray &ray, Intercept intercepts[]) {}
    virtual const Vector3D normalAt(const Point3D &point) {}

    virtual ~Surface() {}
};


// Plane surfaces
class Plane: public Surface {
  public:
    // A plane is denoted by Ax+By+Cz+D = 0
    P_FLT d;  // D
    Vector3D normal;  // {A, B, C}

    Plane() {}

    Plane(const Point3D &point, const Vector3D &vector) {
      normal = vector;
      normal.normalize();

      d = - dotProduct(point, vector);
    }

    virtual const int intersect(const Ray &ray, Intercept intercepts[]);
    virtual const Vector3D normalAt(const Point3D &point) {
      return normal;
    }
};


// TODO(kent): Add test cases for polygons
class Polygon: public Plane {
  public:
    int vertex_num;
    Point3D * vertex;

    Polygon(int point_num, Point3D * points):
      vertex_num(point_num), vertex(points),
      Plane() {
      // if (vertex_num < 3) {
        // Raise exception
      // }

      // Compute norm
      normal = crossProduct(points[1] - points[0],
                          points[2] - points[0]);
      normal.normalize();

      d = - dotProduct(vertex[0], normal);
    };

    ~Polygon() {
      delete [] vertex;
    }

    const int intersect(const Ray &ray, Intercept intercepts[]);
};


// Quadric surfaces
class Sphere: public Surface {
  public:
    P_FLT radius;
    Point3D center;

    Sphere(P_FLT x, P_FLT y, P_FLT z, P_FLT radius): radius(radius) {
      // TODO(kent): Clean up this new point in destructor
      center = Point3D(x, y, z);
    }
    Sphere(Point3D center, P_FLT radius): center(center), radius(radius) {}

    const int intersect(const Ray &ray, Intercept intercepts[]);
    const Vector3D normalAt(const Point3D &point);
};


END_RAYTRACER


#endif
