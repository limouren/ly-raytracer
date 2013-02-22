#include "config.h"

#include "point.h"


BEGIN_RAYTRACER


inline Point2D& Point2D::operator =(const Point2D &point) {
  if (this == &point) {
    return *this;
  }

  x = point.x;
  y = point.y;

  return *this;
}


inline Vector2D Point2D::operator -(const Point2D &point) const {
  return Vector2D(x - point.x, y - point.y);
}


inline Point2D Point2D::operator +(const Vector2D &vector) const {
  return Point2D(x + vector.x, y + vector.y);
}


inline Point2D Point2D::operator -(const Vector2D &vector) const {
  return Point2D(x - vector.x, y - vector.y);
}


inline Point2D& Point2D::operator +=(const Vector2D &vector) {
  x += vector.x;
  y += vector.y;

  return *this;
}


inline Point2D& Point2D::operator -=(const Vector2D &vector) {
  x -= vector.x;
  y -= vector.y;

  return *this;
}




inline Point3D& Point3D::operator =(const Point3D &point) {
  if (this == &point) {
    return *this;
  }

  x = point.x;
  y = point.y;
  z = point.z;

  return *this;
}


inline Vector3D Point3D::operator -(const Point3D &point) const {
  return Vector3D(x - point.x, y - point.y, z - point.z);
}


inline Point3D Point3D::operator +(const Vector3D &vector) const {
  return Point3D(x + vector.x, y + vector.y, z + vector.z);
}


inline Point3D Point3D::operator -(const Vector3D &vector) const {
  return Point3D(x - vector.x, y - vector.y, z - vector.z);
}


inline Point3D& Point3D::operator +=(const Vector3D &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


inline Point3D& Point3D::operator -=(const Vector3D &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


inline Point3D Point3D::operator *(const P_FLT scalar) const {
  return Point3D(x * scalar, y * scalar, z * scalar);
}


inline Point3D& Point3D::operator *=(const P_FLT scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}


END_RAYTRACER
