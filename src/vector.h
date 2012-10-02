#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"


BEGIN_RAYTRACER


class Vector {
  public:
    P_FLT x, y, z;

    Vector(): x(0.0), y(0.0), z(0.0) {}

    Vector(const P_FLT x, const P_FLT y, const P_FLT z): x(x), y(y), z(z) {}

    P_FLT const operator[](const unsigned int index) const {
      return (&x)[index]; // Referenced from yafaray
    }

    P_FLT& operator[](const unsigned int index) {
      return (&x)[index]; // Referenced from yafaray
    }

    Vector& operator =(const Vector &vector);

    const Vector operator +(const Vector &vector);
    const Vector operator -(const Vector &vector);
    Vector& operator +=(const Vector &vector);
    Vector& operator -=(const Vector &vector);

    const Vector operator *(const P_FLT scalar);
    const Vector operator /(const P_FLT scalar);
    Vector& operator *=(const P_FLT scalar);
    Vector& operator /=(const P_FLT scalar);

    P_FLT dot(const Vector &vec);
    P_FLT length();
    P_FLT normalize();
};


END_RAYTRACER


#endif
