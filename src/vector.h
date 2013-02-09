#ifndef VECTOR_H
#define VECTOR_H


#ifdef DEBUG
#include <stdio.h>
#endif


#include "config.h"


BEGIN_RAYTRACER


class Vector2D {
  public:
    P_FLT x, y;

    inline Vector2D(): x(0.0), y(0.0) {}

    inline Vector2D(const P_FLT x, const P_FLT y): x(x), y(y) {}

    inline const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    inline Vector2D operator =(const Vector2D &vector);

    inline const Vector2D operator +(const Vector2D &vector) const;
    inline const Vector2D operator -(const Vector2D &vector) const;
    inline Vector2D operator +=(const Vector2D &vector);
    inline Vector2D operator -=(const Vector2D &vector);

    inline const Vector2D operator *(const P_FLT scalar) const;
    inline const Vector2D operator /(const P_FLT scalar) const;
    inline Vector2D operator *=(const P_FLT scalar);
    inline Vector2D operator /=(const P_FLT scalar);

    inline const P_FLT length() const;
    inline const P_FLT lengthSqr() const;
    inline const P_FLT normalize();
    inline const Vector2D negate();
};


inline const bool operator ==(const Vector2D &vectorA,
                              const Vector2D &vectorB) {
  return (vectorA.x == vectorB.x &&
          vectorA.y == vectorB.y);
}


inline const Vector2D operator -(const Vector2D &vector) {
  return Vector2D(-vector.x, -vector.y);
}


inline const P_FLT dotProduct(const Vector2D &u, const Vector2D &v) {
  return (u.x * v.x) +
         (u.y * v.y);
}


class Vector3D {
  public:
    P_FLT x, y, z;

    inline Vector3D(): x(0.0), y(0.0), z(0.0) {}
    explicit inline Vector3D(const P_FLT pFlt): x(pFlt), y(pFlt), z(pFlt) {}

    inline Vector3D(const P_FLT x, const P_FLT y, const P_FLT z):
      x(x), y(y), z(z) {}

    inline const P_FLT operator[](const unsigned int index) const {
      return (&x)[index];  // Referenced from yafaray
    }

    inline Vector3D operator =(const Vector3D &vector);

    inline const Vector3D operator +(const Vector3D &vector) const;
    inline const Vector3D operator -(const Vector3D &vector) const;
    inline Vector3D operator +=(const Vector3D &vector);
    inline Vector3D operator -=(const Vector3D &vector);

    inline const Vector3D operator *(const P_FLT scalar) const;
    inline const Vector3D operator /(const P_FLT scalar) const;
    inline Vector3D operator *=(const P_FLT scalar);
    inline Vector3D operator /=(const P_FLT scalar);

    inline const int dominantIndex() const;
    inline const Vector2D dropIndex(const int index) const;
    inline const P_FLT length() const;
    inline const P_FLT lengthSqr() const;
    inline const P_FLT normalize();
    inline const Vector3D negate();

    #ifdef DEBUG
    void print(char * id) const {
      printf("Vector %s: %f, %f, %f\n", id, x, y, z);
    }
    #endif
};


inline const bool operator ==(const Vector3D &vectorA,
                              const Vector3D &vectorB) {
  return (vectorA.x == vectorB.x &&
          vectorA.y == vectorB.y &&
          vectorA.z == vectorB.z);
}


inline const Vector3D operator -(const Vector3D &vector) {
  return Vector3D(-vector.x, -vector.y, -vector.z);
}


inline const Vector3D crossProduct(const Vector3D &u, const Vector3D &v) {
  return Vector3D(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}


inline const P_FLT dotProduct(const Vector3D &u, const Vector3D &v) {
  return (u.x * v.x) +
         (u.y * v.y) +
         (u.z * v.z);
}


END_RAYTRACER


#endif
