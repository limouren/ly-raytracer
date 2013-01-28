#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"


BEGIN_RAYTRACER


class Vector {
  public:
    P_FLT x, y, z;

    Vector(): x(0.0), y(0.0), z(0.0) {}

    Vector(const P_FLT x, const P_FLT y, const P_FLT z): x(x), y(y), z(z) {}

    const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index];  // Referenced from yafaray
    }

    Vector& operator =(const Vector &vector);

    const Vector operator +(const Vector &vector) const;
    const Vector operator -(const Vector &vector) const;
    Vector& operator +=(const Vector &vector);
    Vector& operator -=(const Vector &vector);

    const Vector operator *(const P_FLT scalar) const;
    const Vector operator /(const P_FLT scalar) const;
    Vector& operator *=(const P_FLT scalar);
    Vector& operator /=(const P_FLT scalar);

    const P_FLT length() const;
    const P_FLT lengthSqr() const;
    const P_FLT normalize();
    const Vector negate();
};


const Vector operator -(const Vector &vector) {
  return Vector(vector.x, vector.y, vector.z);
}


const Vector crossProduct(const Vector &u, const Vector &v) {
  return Vector(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}


const P_FLT dotProduct(const Vector &u, const Vector &v) {
  return (u.x * v.x) +
         (u.y * v.y) +
         (u.z * v.z);
}


END_RAYTRACER


#endif
