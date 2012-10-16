#ifndef SURFACE_H
#define SURFACE_H

#include "config.h"

#include "point.h"
#include "ray.h"
#include "vector.h"


BEGIN_RAYTRACER


class Surface {
  public:
    virtual const int intersect(Ray &ray, P_FLT ** t_values) = 0;
    virtual const Vector normalAt(Point &point) = 0;
};


// Plane surfaces
class Plane: Surface {
  protected:
    void computeD(const Point &point);

  public:
    // A plane is denoted by Ax+By+Cz+D = 0
    P_FLT d; // D
    Vector norm; // {A, B, C}

    Plane() {}

    Plane(const Point &point, const Vector &vector) {
      norm = vector;
      norm.normalize();

      computeD(point);
    }

    virtual const int intersect(Ray &ray, P_FLT ** t_values);
    virtual const Vector normalAt(Point &point) {
      return norm;
    }
};


// TODO: Add test cases for polygons
class Polygon: Plane {
  public:
    int vertex_num;
    Point ** vertices;

    Polygon(Point ** points, int point_num): vertex_num(point_num) {
      if (point_num < 3) {
        // Raise exception
      }

      // Compute norm
      norm = crossProduct(*points[1] - *points[0], *points[2] - *points[0]);
      norm.normalize();

      vertices = new Point*[vertex_num];
      for(int i = 0;i < vertex_num;i++) {
        vertices[i] = points[i];
      }

      computeD(*vertices[0]);
    };

    ~Polygon() {
      delete vertices;
    }

    virtual const int intersect(Ray &ray, P_FLT ** t_values);
};


// Quadric surfaces
class Sphere: Surface {
  public:
    P_FLT radius;
    Point * center;

    Sphere(P_FLT x, P_FLT y, P_FLT z, P_FLT radius): radius(radius) {
      // TODO: Clean up this new point in destructor
      center = new Point(x, y, z);
    }
    Sphere(Point &center, P_FLT radius): center(&center), radius(radius) {}

    const int intersect(Ray &ray, P_FLT ** t_values);
    const Vector normalAt(Point &point);
};


END_RAYTRACER


#endif
