#ifndef COLOR_H
#define COLOR_H


#include <math.h>
#ifdef DEBUG
#include <stdio.h>
#endif


#include "config.h"


BEGIN_RAYTRACER


class Color {
  public:
    C_FLT r;
    C_FLT g;
    C_FLT b;

    inline Color(): r(0.0), g(0.0), b(0.0) {}
    inline explicit Color(const C_FLT intensity):
      r(intensity), g(intensity), b(intensity) {}
    inline Color(const C_FLT r, const C_FLT g, const C_FLT b):
      r(r), g(g), b(b) {}

    inline Color& operator =(const Color &color);

    inline const Color operator +(const Color color) const;
    inline Color operator +=(const Color color);

    inline const Color operator *(const Color color) const;
    inline Color operator *=(const Color color);

    inline const Color operator *(const C_FLT scalar) const;
    inline Color operator *=(const C_FLT scalar);

    inline const Color exp(P_FLT exponent) {
      Color result(pow(r, exponent),
                   pow(g, exponent),
                   pow(b, exponent));
      return result;
    }

    inline C_FLT magnitude() {
      return (r * 0.299)  + (g * 0.587) + (b * 0.114);
    }

    #ifdef DEBUG
    void print(char * id) {
      printf("Color %s: %f, %f, %f\n", id, r, g, b);
    }
    #endif
};


class RGBColor {
  public:
    int r, g, b;

    RGBColor() {}

    explicit RGBColor(unsigned char * mRGB) {
      r = static_cast<int>(*mRGB);
      g = static_cast<int>(*(mRGB + 1));
      b = static_cast<int>(*(mRGB + 2));
    }

    inline const Color toColor() const;
};


END_RAYTRACER


#endif
