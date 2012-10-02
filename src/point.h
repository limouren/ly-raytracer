#ifndef POINT_H
#define POINT_H

#include "config.h"
#include "vector.h"


BEGIN_RAYTRACER

class Point {
  public:
    P_FLT x, y, z;

    Point(): x(0.0), y(0.0), z(0.0) {}

    Point(P_FLT x, P_FLT y, P_FLT z): x(x), y(y), z(z) {}

    Point(Vector vector): x(vector.x), y(vector.y), z(vector.z) {}

    P_FLT const operator[](const unsigned int index) const {
      return (&x)[index]; // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index]; // Referenced from yafaray
    }

    Point& operator =(const Point &point);

    const Vector operator -(const Point &point);

    const Point operator +(const Vector &vector);
    const Point operator -(const Vector &vector);
    Point& operator +=(const Vector &vector);
    Point& operator -=(const Vector &vector);
};


END_RAYTRACER


#endif
