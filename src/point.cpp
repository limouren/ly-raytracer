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


Point Point::operator +(const Vector &vector) {
  return Point(x + vector.x, y + vector.y, z + vector.z);
}


Point Point::operator -(const Vector &vector) {
  return Point(x - vector.x, y - vector.y, z - vector.z);
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
