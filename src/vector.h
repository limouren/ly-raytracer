#ifndef VECTOR_H
#define VECTOR_H

#include "config.h"


BEGIN_RAYTRACER


class Vector {
  private:
    P_FLT vector_[3];

  public:
    Vector() {
      vector_[0] = 0.0;
      vector_[1] = 0.0;
      vector_[2] = 0.0;
    }

    Vector(const P_FLT x, const P_FLT y, const P_FLT z) {
      vector_[0] = x;
      vector_[1] = y;
      vector_[2] = z;
    }

    P_FLT operator[](const unsigned int index) const {
      return vector_[index];
    }

    P_FLT& operator[](const unsigned int index) {
      return vector_[index];
    }

    Vector& operator =(const Vector &vec);
    Vector& operator +(const Vector &vec);
    Vector& operator -(const Vector &vec);
    Vector& operator +=(const Vector &vec);
    Vector& operator -=(const Vector &vec);

    Vector& copy();

    P_FLT dot(const Vector &vec);
    P_FLT length();
    void mult(const P_FLT scalar);
    P_FLT normalize();
};


END_RAYTRACER


#endif
