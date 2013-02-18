#include <math.h>

#include "config.h"

#include "vector.h"


BEGIN_RAYTRACER


inline Vector2D Vector2D::operator =(const Vector2D &vector) {
  if (this == &vector) {
    return *this;
  }

  x = vector.x;
  y = vector.y;

  return *this;
}


inline Vector2D Vector2D::operator +(const Vector2D &vector) const {
  Vector2D result = *this;
  result += vector;

  return result;
}


inline Vector2D Vector2D::operator -(const Vector2D &vector) const {
  Vector2D result = *this;
  result -= vector;

  return result;
}


inline Vector2D Vector2D::operator +=(const Vector2D &vector) {
  x += vector.x;
  y += vector.y;

  return *this;
}


inline Vector2D Vector2D::operator -=(const Vector2D &vector) {
  x -= vector.x;
  y -= vector.y;

  return *this;
}


inline Vector2D Vector2D::operator *(const P_FLT scalar) const {
  Vector2D result = *this;
  result *= scalar;

  return result;
}


inline Vector2D Vector2D::operator /(const P_FLT scalar) const {
  Vector2D result = *this;
  result /= scalar;

  return result;
}


inline Vector2D Vector2D::operator *=(const P_FLT scalar) {
  x *= scalar;
  y *= scalar;

  return *this;
}


inline Vector2D Vector2D::operator /=(const P_FLT scalar) {
  x /= scalar;
  y /= scalar;

  return *this;
}


inline P_FLT Vector2D::length() const {
  return sqrt(lengthSqr());
}


inline P_FLT Vector2D::lengthSqr() const {
  return x * x + y * y;
}


inline P_FLT Vector2D::normalize() {
  P_FLT len = length();
  *this /= len;

  return len;
}


inline Vector2D Vector2D::negate() {
  x = -x;
  y = -y;

  return *this;
}


inline Vector3D Vector3D::operator =(const Vector3D &vector) {
  if (this == &vector) {
    return *this;
  }

  x = vector.x;
  y = vector.y;
  z = vector.z;

  return *this;
}


inline Vector3D Vector3D::operator +(const Vector3D &vector) const {
  Vector3D result = *this;
  result += vector;

  return result;
}


inline Vector3D Vector3D::operator -(const Vector3D &vector) const {
  Vector3D result = *this;
  result -= vector;

  return result;
}


inline Vector3D Vector3D::operator +=(const Vector3D &vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;

  return *this;
}


inline Vector3D Vector3D::operator -=(const Vector3D &vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;

  return *this;
}


inline Vector3D Vector3D::operator *(const P_FLT scalar) const {
  Vector3D result = *this;
  result *= scalar;

  return result;
}


inline Vector3D Vector3D::operator /(const P_FLT scalar) const {
  Vector3D result = *this;
  result /= scalar;

  return result;
}


inline Vector3D Vector3D::operator *=(const P_FLT scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;

  return *this;
}


inline Vector3D Vector3D::operator /=(const P_FLT scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;

  return *this;
}


inline Vector2D Vector3D::dropIndex(const int index) const {
  if (index == 0) {
    return Vector2D(y, z);
  } else if (index == 1)  {
    return Vector2D(x, z);
  } else {
    return Vector2D(x, y);
  }
}


inline int Vector3D::dominantIndex() const {
  P_FLT abs_x = fabs(x),
        abs_y = fabs(y),
        abs_z = fabs(z);
  if (abs_x > abs_y) {
    if (abs_x > abs_z) {
      return x > 0? 0: 3;
    } else {
      return z > 0? 2: 5;
    }
  } else {
    if (abs_y > abs_z) {
      return y > 0? 1: 4;
    } else {
      return z > 0? 2: 5;
    }
  }
}


inline P_FLT Vector3D::length() const {
  return sqrt(lengthSqr());
}


inline P_FLT Vector3D::lengthSqr() const {
  return x * x + y * y + z * z;
}


inline P_FLT Vector3D::normalize() {
  P_FLT len = length();
  *this /= len;

  return len;
}


inline Vector3D Vector3D::negate() {
  x = -x;
  y = -y;
  z = -z;

  return *this;
}


END_RAYTRACER
