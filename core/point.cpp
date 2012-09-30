#include "point.h"


BEGIN_RAYTRACER


Point& Point::operator =(const Point &point) {
  if (this == &point) {
    return *this;
  }

  this[0] = point[0];
  this[1] = point[1];
  this[2] = point[2];

  return *this;
}


Point& Point::operator +(const Vector &vec) {
  P_FLT x = this[0] + vec[0];
  P_FLT y = this[1] + vec[1];
  P_FLT z = this[2] + vec[2];

  return Point(x, y, z);
}


Point& Point::operator -(const Vector &vec) {
  P_FLT x = this[0] - vec[0];
  P_FLT y = this[1] - vec[1];
  P_FLT z = this[2] - vec[2];

  return Point(x, y, z);
}


Point& Point::operator +=(const Vector &vec) {
  this[0] += vec[0];
  this[1] += vec[1];
  this[2] += vec[2];
  return *this;
}


Point& Point::operator -=(const Vector &vec) {
  this[0] -= vec[0];
  this[1] -= vec[1];
  this[2] -= vec[2];
  return *this;
}


Point Point::copy() {
  return Point(this[0], this[1], this[2]);
}


END_RAYTRACER
