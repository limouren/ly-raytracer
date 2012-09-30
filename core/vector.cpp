#include <math.h>

#include "vector.h"


BEGIN_RAYTRACER


Vector& Vector::operator =(const Vector &vec) {
  if (this == &vec) {
    return *this;
  }

  this[0] = vec[0];
  this[1] = vec[1];
  this[2] = vec[2];

  return *this;
}


Vector& Vector::operator +(const Vector &vec) {
  P_FLT x = this[0] + vec[0];
  P_FLT y = this[1] + vec[1];
  P_FLT z = this[2] + vec[2];

  return Vector(x, y, z);
}


Vector& Vector::operator -(const Vector &vec) {
  P_FLT x = this[0] - vec[0];
  P_FLT y = this[1] - vec[1];
  P_FLT z = this[2] - vec[2];

  return Vector(x, y, z);
}


Vector& Vector::operator +=(const Vector &vec) {
  this[0] += vec[0];
  this[1] += vec[1];
  this[2] += vec[2];
  return *this;
}


Vector& Vector::operator -=(const Vector &vec) {
  this[0] -= vec[0];
  this[1] -= vec[1];
  this[2] -= vec[2];
  return *this;
}


Vector Vector::copy() {
  return Vector(this[0], this[1], this[2]);
}


P_FLT Vector::dot(const Vector &vec) {
  return (this[0] * vec[0]) + (this[1] * vec[1]) + (this[2] * vec[2]);
}


P_FLT Vector::length() {
  return sqrt(this.dot(this));
}


void Vector:mult(const P_FLT scalar) {
  this[0] *= scalar;
  this[1] *= scalar;
  this[2] *= scalar;
  return *this;
}


P_FLT Vector::normalize() {
  // TODO: See if we can use inv. sqrt hack
  P_FLT length = this.length();
  P_FLT length_inv = 1.0 / length;
  this.mult(length_inv);

  return length;
}


END_RAYTRACER
