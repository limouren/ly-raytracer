#ifndef COLOR_H
#define COLOR_H

#include "config.h"


BEGIN_RAYTRACER


class Coeff {
  public:
    C_FLT r;
    C_FLT g;
    C_FLT b;

    Coeff(): r(0.0), g(0.0), b(0.0) {}
    Coeff(C_FLT r, C_FLT g, C_FLT b): r(r), g(g), b(b) {}

    C_FLT magnitude() {
      return (r * 0.299)  + (g * 0.587) + (b * 0.114);
    }
};


class Color: public Coeff {
  public:
    Color(): Coeff() {}
    Color(C_FLT r, C_FLT g, C_FLT b): Coeff(r, g, b) {}

    Color& operator =(const Color &color);

    const Color operator +(const Color color) const;
    Color operator +=(const Color color);

    const Color operator *(const Color color) const;
    Color operator *=(const Color color);

    const Color operator *(const C_FLT scalar) const;
    Color operator *=(const C_FLT scalar);

    const Color operator *(const Coeff coeff) const;
    Color operator *=(const Coeff coeff);

    void scale();
};


END_RAYTRACER


#endif
