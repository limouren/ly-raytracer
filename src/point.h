#ifndef POINT_H
#define POINT_H


#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


class Point2D: public Vector2D {
  public:
    Point2D(): Vector2D() {}

    Point2D(P_FLT x, P_FLT y): Vector2D(x, y) {}

    Point2D& operator =(const Point2D &point);

    const Vector2D operator -(const Point2D &point) const;

    const Point2D operator +(const Vector2D &vector) const;
    const Point2D operator -(const Vector2D &vector) const;
    Point2D& operator +=(const Vector2D &vector);
    Point2D& operator -=(const Vector2D &vector);
};


const P_FLT dotProduct(const Point2D &a, const Point2D &b) {
  return dotProduct(static_cast<Vector2D>(a), static_cast<Vector2D>(b));
}
const P_FLT dotProduct(const Point2D &a, const Vector2D &b) {
  return dotProduct(static_cast<Vector2D>(a), b);
}
const P_FLT dotProduct(const Vector2D &a, const Point2D &b) {
  return dotProduct(a, static_cast<Vector2D>(b));
}


class Point3D: public Vector3D {
  public:
    Point3D(): Vector3D() {}

    Point3D(P_FLT x, P_FLT y, P_FLT z): Vector3D(x, y, z) {}

    Point3D& operator =(const Point3D &point);

    const Vector3D operator -(const Point3D &point) const;

    const Point3D operator +(const Vector3D &vector) const;
    const Point3D operator -(const Vector3D &vector) const;
    Point3D& operator +=(const Vector3D &vector);
    Point3D& operator -=(const Vector3D &vector);
};


const P_FLT dotProduct(const Point3D &a, const Point3D &b) {
  return dotProduct(static_cast<Vector3D>(a), static_cast<Vector3D>(b));
}
const P_FLT dotProduct(const Point3D &a, const Vector3D &b) {
  return dotProduct(static_cast<Vector3D>(a), b);
}
const P_FLT dotProduct(const Vector3D &a, const Point3D &b) {
  return dotProduct(a, static_cast<Vector3D>(b));
}


END_RAYTRACER


#endif
