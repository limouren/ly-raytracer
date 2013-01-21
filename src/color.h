#ifndef COLOR_H
#define COLOR_H

#include "config.h"

BEGIN_RAYTRACER

class Color {
  public:
    C_FLT r;
    C_FLT g;
    C_FLT b;


    Color () {
      r = 0.0;
      g = 0.0;
      b = 0.0;
    }

    Color (C_FLT r_, C_FLT g_, C_FLT b_) {
      r = r_;
      g = g_;
      b = b_;
    }

    const Color operator +(const Color color) const;
    Color operator +=(const Color color);

    const Color operator *(const C_FLT scalar) const;
    Color operator *=(const C_FLT scalar);

    const Color operator *(const Color color) const;
    Color operator *=(const Color color);

    void copyFrom(const Color color);
};

END_RAYTRACER


#endif
