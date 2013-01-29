#ifndef POINT_H
#define POINT_H


#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


class Point2D {
  public:
    P_FLT x, y, z;

    Point2D(): x(0.0), y(0.0) {}

    Point2D(P_FLT x, P_FLT y): x(x), y(y) {}

    explicit Point2D(Vector2D vector): x(vector.x), y(vector.y) {}

    const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index];  // Referenced from yafaray
    }

    Point2D& operator =(const Point2D &point);

    const Vector2D operator -(const Point2D &point) const;

    const Point2D operator +(const Vector2D &vector) const;
    const Point2D operator -(const Vector2D &vector) const;
    Point2D& operator +=(const Vector2D &vector);
    Point2D& operator -=(const Vector2D &vector);
};


const P_FLT dotProduct(const Point2D &a, const Point2D &b) {
  return (a.x * b.x) +
         (a.y * b.y);
}
const P_FLT dotProduct(const Point2D &a, const Vector2D &v) {
  return (a.x * v.x) +
         (a.y * v.y);
}
const P_FLT dotProduct(const Vector2D &u, const Point2D &b) {
  return (u.x * b.x) +
         (u.y * b.y);
}


class Point3D {
  public:
    P_FLT x, y, z;

    Point3D(): x(0.0), y(0.0), z(0.0) {}

    Point3D(P_FLT x, P_FLT y, P_FLT z): x(x), y(y), z(z) {}

    explicit Point3D(Vector3D vector): x(vector.x), y(vector.y), z(vector.z) {}

    const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index];  // Referenced from yafaray
    }

    Point3D& operator =(const Point3D &point);

    const Vector3D operator -(const Point3D &point) const;

    const Point3D operator +(const Vector3D &vector) const;
    const Point3D operator -(const Vector3D &vector) const;
    Point3D& operator +=(const Vector3D &vector);
    Point3D& operator -=(const Vector3D &vector);
};


const P_FLT dotProduct(const Point3D &a, const Point3D &b) {
  return (a.x * b.x) +
         (a.y * b.y) +
         (a.z * b.z);
}
const P_FLT dotProduct(const Point3D &a, const Vector3D &v) {
  return (a.x * v.x) +
         (a.y * v.y) +
         (a.z * v.z);
}
const P_FLT dotProduct(const Vector3D &u, const Point3D &b) {
  return (u.x * b.x) +
         (u.y * b.y) +
         (u.z * b.z);
}


END_RAYTRACER


#endif
