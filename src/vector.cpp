#include <math.h>

#include "vector.h"


BEGIN_RAYTRACER


Vector& Vector::operator =(const Vector &vec) {
  if (this == &vec) {
    return *this;
  }

  vector_[0] = vec[0];
  vector_[1] = vec[1];
  vector_[2] = vec[2];

  return *this;
}


Vector& Vector::operator +(const Vector &vec) {
  P_FLT x = vector_[0] + vec[0];
  P_FLT y = vector_[1] + vec[1];
  P_FLT z = vector_[2] + vec[2];

  return *(new Vector(x, y, z));
}


Vector& Vector::operator -(const Vector &vec) {
  P_FLT x = vector_[0] - vec[0];
  P_FLT y = vector_[1] - vec[1];
  P_FLT z = vector_[2] - vec[2];

  return *(new Vector(x, y, z));
}


Vector& Vector::operator +=(const Vector &vec) {
  vector_[0] += vec[0];
  vector_[1] += vec[1];
  vector_[2] += vec[2];
  return *this;
}


Vector& Vector::operator -=(const Vector &vec) {
  vector_[0] -= vec[0];
  vector_[1] -= vec[1];
  vector_[2] -= vec[2];
  return *this;
}


Vector& Vector::copy() {
  return *(new Vector(vector_[0], vector_[1], vector_[2]));
}


P_FLT Vector::dot(const Vector &vec) {
  return (vector_[0] * vec[0]) + (vector_[1] * vec[1]) + (vector_[2] * vec[2]);
}


P_FLT Vector::length() {
  return sqrt(vector_[0] * vector_[0] +
              vector_[1] * vector_[1] +
              vector_[2] * vector_[2]);
}


void Vector::mult(const P_FLT scalar) {
  vector_[0] *= scalar;
  vector_[1] *= scalar;
  vector_[2] *= scalar;
}


P_FLT Vector::normalize() {
  // TODO: See if we can use inv. sqrt hack
  P_FLT len = this->length();
  P_FLT len_inv = 1.0 / len;
  this->mult(len_inv);

  return len;
}


END_RAYTRACER
