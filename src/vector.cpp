#include <math.h>

#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


Vector3D& Vector3D::operator =(const Vector3D &vector) {
  if (this == &vector) {
    return *this;
  }

  x = vector.x;
  y = vector.y;
  z = vector.z;

  return *this;
}


const Vector3D Vector3D::operator +(const Vector3D &vector) const {
  Vector3D result = *this;
  result += vector;

  return result;
}


const Vector3D Vector3D::operator -(const Vector3D &vector) const {
  Vector3D result = *this;
  result -= vector;

  return result;
}


Vector3D& Vector3D::operator +=(const Vector3D &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


Vector3D& Vector3D::operator -=(const Vector3D &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


const Vector3D Vector3D::operator *(const P_FLT scalar) const {
  Vector3D result = *this;
  result *= scalar;

  return result;
}


const Vector3D Vector3D::operator /(const P_FLT scalar) const {
  // TODO(kent): Divison by zero?
  Vector3D result = *this;
  result /= scalar;

  return result;
}


Vector3D& Vector3D::operator *=(const P_FLT scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}


Vector3D& Vector3D::operator /=(const P_FLT scalar) {
  // TODO(kent): Divison by zero?
  x /= scalar;
  y /= scalar;
  z /= scalar;

  return *this;
}


const int Vector3D::dominantIndex() const {
  P_FLT abs_x = fabs(x),
        abs_y = fabs(y),
        abs_z = fabs(z);
  if (abs_x > abs_y) {
    if (abs_x > abs_z) {
      return 0;
    } else {
      return 2;
    }
  } else {
    if (abs_y > abs_z) {
      return 1;
    } else {
      return 2;
    }
  }
}


const P_FLT Vector3D::length() const {
  return sqrt(lengthSqr());
}


const P_FLT Vector3D::lengthSqr() const {
  return x * x + y * y + z * z;
}


const P_FLT Vector3D::normalize() {
  // TODO(kent): See if we can use inv. sqrt hack
  P_FLT len = length();
  if (len != 0.0) {
    *this *= (1.0 / len);
  }

  return len;
}


const Vector3D Vector3D::negate() {
  x = -x;
  y = -y;
  z = -z;
}


END_RAYTRACER
