#include <math.h>

#include "vector.h"


BEGIN_RAYTRACER


Vector& Vector::operator =(const Vector &vec) {
  if (this == &vec) {
    return *this;
  }

  x = vec.x;
  y = vec.y;
  z = vec.z;

  return *this;
}


Vector Vector::operator +(const Vector &vec) {
  return Vector(x + vec.x, y + vec.y, z + vec.z);
}


Vector Vector::operator -(const Vector &vec) {
  return Vector(x - vec.x, y - vec.y, z - vec.z);
}


Vector& Vector::operator +=(const Vector &vec) {
  x += vec.x;
  y += vec.y;
  z += vec.z;
  return *this;
}


Vector& Vector::operator -=(const Vector &vec) {
  x -= vec.x;
  y -= vec.y;
  z -= vec.z;
  return *this;
}


Vector Vector::operator *(const P_FLT scalar) {
  return Vector(x * scalar, y * scalar, z * scalar);
}


Vector Vector::operator /(const P_FLT scalar) {
  // TODO: Divison by zero?
  return Vector(x / scalar, y / scalar, z / scalar);
}


Vector& Vector::operator *=(const P_FLT scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}


Vector& Vector::operator /=(const P_FLT scalar) {
  // TODO: Divison by zero?
  x /= scalar;
  y /= scalar;
  z /= scalar;
  return *this;
}


P_FLT Vector::dot(const Vector &vec) {
  return (x * vec.x) + (y * vec.y) + (z * vec.z);
}


P_FLT Vector::length() {
  return sqrt(x * x + y * y + z * z);
}


P_FLT Vector::normalize() {
  // TODO: See if we can use inv. sqrt hack
  P_FLT len = this->length();
  if (len != 0.0) {
    *this *= (1.0 / len);
  }

  return len;
}


END_RAYTRACER
