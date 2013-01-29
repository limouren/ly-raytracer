#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"


BEGIN_RAYTRACER


class Vector3D {
  public:
    P_FLT x, y, z;

    Vector3D(): x(0.0), y(0.0), z(0.0) {}

    Vector3D(const P_FLT x, const P_FLT y, const P_FLT z): x(x), y(y), z(z) {}

    const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index];  // Referenced from yafaray
    }

    Vector3D& operator =(const Vector3D &vector);

    const Vector3D operator +(const Vector3D &vector) const;
    const Vector3D operator -(const Vector3D &vector) const;
    Vector3D& operator +=(const Vector3D &vector);
    Vector3D& operator -=(const Vector3D &vector);

    const Vector3D operator *(const P_FLT scalar) const;
    const Vector3D operator /(const P_FLT scalar) const;
    Vector3D& operator *=(const P_FLT scalar);
    Vector3D& operator /=(const P_FLT scalar);

    const int dominantIndex() const;
    const P_FLT length() const;
    const P_FLT lengthSqr() const;
    const P_FLT normalize();
    const Vector3D negate();
};


const Vector3D operator -(const Vector3D &vector) {
  return Vector3D(vector.x, vector.y, vector.z);
}


const Vector3D crossProduct(const Vector3D &u, const Vector3D &v) {
  return Vector3D(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}


const P_FLT dotProduct(const Vector3D &u, const Vector3D &v) {
  return (u.x * v.x) +
         (u.y * v.y) +
         (u.z * v.z);
}


END_RAYTRACER


#endif
