#include "point.h"


BEGIN_RAYTRACER


Point& Point::operator =(const Point &point) {
  if (this == &point) {
    return *this;
  }

  x = point.x;
  y = point.y;
  z = point.z;

  return *this;
}


const Vector3D Point::operator -(const Point &point) const {
  return Vector3D(x - point.x, y - point.y, z - point.z);
}


const Point Point::operator +(const Vector3D &vector) const {
  Point result = *this;
  result += vector;

  return result;
}


const Point Point::operator -(const Vector3D &vector) const {
  Point result = *this;
  result -= vector;

  return result;
}


Point& Point::operator +=(const Vector3D &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


Point& Point::operator -=(const Vector3D &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


END_RAYTRACER
