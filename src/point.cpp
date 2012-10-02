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


const Point Point::operator +(const Vector &vector) {
  Point result = *this;
  result += vector;

  return result;
}


const Point Point::operator -(const Vector &vector) {
  Point result = *this;
  result -= vector;

  return result;
}


Point& Point::operator +=(const Vector &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


Point& Point::operator -=(const Vector &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


END_RAYTRACER
