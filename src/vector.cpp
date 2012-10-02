#include <math.h>

#include "vector.h"


BEGIN_RAYTRACER


Vector& Vector::operator =(const Vector &vector) {
  if (this == &vector) {
    return *this;
  }

  x = vector.x;
  y = vector.y;
  z = vector.z;

  return *this;
}


const Vector Vector::operator +(const Vector &vector) {
  Vector result = *this;
  result += vector;

  return result;
}


const Vector Vector::operator -(const Vector &vector) {
  Vector result = *this;
  result -= vector;

  return result;
}


Vector& Vector::operator +=(const Vector &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


Vector& Vector::operator -=(const Vector &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


const Vector Vector::operator *(const P_FLT scalar) {
  Vector result = *this;
  result *= scalar;

  return result;
}


const Vector Vector::operator /(const P_FLT scalar) {
  // TODO: Divison by zero?
  Vector result = *this;
  result /= scalar;

  return result;
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
  return sqrt(length_sqr());
}


P_FLT Vector::length_sqr() {
  return x * x + y * y + z * z;
}


P_FLT Vector::normalize() {
  // TODO: See if we can use inv. sqrt hack
  P_FLT len = length();
  if (len != 0.0) {
    *this *= (1.0 / len);
  }

  return len;
}


END_RAYTRACER
