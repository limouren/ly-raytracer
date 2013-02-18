#ifndef POINT_H
#define POINT_H


#include <algorithm>

#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


class Point2D: public Vector2D {
  public:
    inline Point2D(): Vector2D() {}

    inline Point2D(P_FLT x, P_FLT y): Vector2D(x, y) {}

    inline Point2D& operator =(const Point2D &point);

    inline Vector2D operator -(const Point2D &point) const;

    inline Point2D operator +(const Vector2D &vector) const;
    inline Point2D operator -(const Vector2D &vector) const;
    inline Point2D& operator +=(const Vector2D &vector);
    inline Point2D& operator -=(const Vector2D &vector);
};


inline P_FLT dotProduct(const Point2D &a, const Point2D &b) {
  return dotProduct(static_cast<Vector2D>(a), static_cast<Vector2D>(b));
}
inline P_FLT dotProduct(const Point2D &a, const Vector2D &b) {
  return dotProduct(static_cast<Vector2D>(a), b);
}
inline P_FLT dotProduct(const Vector2D &a, const Point2D &b) {
  return dotProduct(a, static_cast<Vector2D>(b));
}


class Point3D: public Vector3D {
  public:
    inline Point3D(): Vector3D() {}

    inline Point3D(P_FLT x, P_FLT y, P_FLT z): Vector3D(x, y, z) {}

    inline Point3D& operator =(const Point3D &point);

    inline Vector3D operator -(const Point3D &point) const;

    inline Point3D operator +(const Vector3D &vector) const;
    inline Point3D operator -(const Vector3D &vector) const;
    inline Point3D& operator +=(const Vector3D &vector);
    inline Point3D& operator -=(const Vector3D &vector);
};


inline P_FLT dotProduct(const Point3D &a, const Point3D &b) {
  return dotProduct(static_cast<Vector3D>(a), static_cast<Vector3D>(b));
}
inline P_FLT dotProduct(const Point3D &a, const Vector3D &b) {
  return dotProduct(static_cast<Vector3D>(a), b);
}
inline P_FLT dotProduct(const Vector3D &a, const Point3D &b) {
  return dotProduct(a, static_cast<Vector3D>(b));
}

inline Point3D pointMax(const Point3D &a, const Point3D &b) {
  return Point3D(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

inline Point3D pointMin(const Point3D &a, const Point3D &b) {
  return Point3D(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}


END_RAYTRACER


#endif
