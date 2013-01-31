#ifndef COLOR_H
#define COLOR_H


#include <math.h>
#ifdef DEBUG
#include <stdio.h>
#endif


#include "config.h"


BEGIN_RAYTRACER


class Coeff {
  public:
    C_FLT r;
    C_FLT g;
    C_FLT b;

    inline Coeff(): r(0.0), g(0.0), b(0.0) {}

    inline Coeff(C_FLT r, C_FLT g, C_FLT b): r(r), g(g), b(b) {}

    inline C_FLT magnitude() {
      return (r * 0.299)  + (g * 0.587) + (b * 0.114);
    }

    inline const Coeff exp(P_FLT exponent) {
      Coeff result(pow(r, exponent),
                   pow(g, exponent),
                   pow(b, exponent));
      return result;
    }

    #ifdef DEBUG
    void print(char * id) {
      printf("Color %s: %f, %f, %f\n", id, r, g, b);
    }
    #endif
};


class Color: public Coeff {
  public:
    inline Color(): Coeff() {}

    inline Color(C_FLT r, C_FLT g, C_FLT b): Coeff(r, g, b) {}

    inline Color& operator =(const Color &color);

    inline const Color operator +(const Color color) const;
    inline Color operator +=(const Color color);

    inline const Color operator *(const Color color) const;
    inline Color operator *=(const Color color);

    inline const Color operator *(const C_FLT scalar) const;
    inline Color operator *=(const C_FLT scalar);

    inline const Color operator *(const Coeff coeff) const;
    inline Color operator *=(const Coeff coeff);
};


END_RAYTRACER


#endif
