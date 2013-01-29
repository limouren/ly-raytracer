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

    explicit Point(Vector3D vector): x(vector.x), y(vector.y), z(vector.z) {}

    const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index];  // Referenced from yafaray
    }

    Point& operator =(const Point &point);

    const Vector3D operator -(const Point &point) const;

    const Point operator +(const Vector3D &vector) const;
    const Point operator -(const Vector3D &vector) const;
    Point& operator +=(const Vector3D &vector);
    Point& operator -=(const Vector3D &vector);
};


const P_FLT dotProduct(const Point &a, const Point &b) {
  return (a.x * b.x) +
         (a.y * b.y) +
         (a.z * b.z);
}
const P_FLT dotProduct(const Point &a, const Vector3D &v) {
  return (a.x * v.x) +
         (a.y * v.y) +
         (a.z * v.z);
}
const P_FLT dotProduct(const Vector3D &u, const Point &b) {
  return (u.x * b.x) +
         (u.y * b.y) +
         (u.z * b.z);
}


END_RAYTRACER


#endif
