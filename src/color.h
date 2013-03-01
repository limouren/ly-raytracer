#ifndef COLOR_H
#define COLOR_H


#include <math.h>
#include <stdio.h>

#include "config.h"


BEGIN_RAYTRACER


class Color {
  public:
    C_FLT r;
    C_FLT g;
    C_FLT b;

    inline Color(): r(0.0f), g(0.0f), b(0.0f) {}
    inline explicit Color(const C_FLT intensity):
      r(intensity), g(intensity), b(intensity) {}
    inline Color(const C_FLT r, const C_FLT g, const C_FLT b):
      r(r), g(g), b(b) {}

    inline Color& operator =(const Color &color);

    inline Color operator +(const Color color) const;
    inline Color operator +=(const Color color);

    inline Color operator *(const Color color) const;
    inline Color operator *=(const Color color);

    inline Color operator *(const C_FLT scalar) const;
    inline Color operator *=(const C_FLT scalar);

    inline Color exp(P_FLT exponent) {
      Color result(pow(r, exponent),
                   pow(g, exponent),
                   pow(b, exponent));
      return result;
    }

    inline C_FLT magnitude() {
      return (r * 0.299f)  + (g * 0.587f) + (b * 0.114f);
    }

    void print(char * id) {
      printf("Color %s: %f, %f, %f\n", id, r, g, b);
    }
};


class RGBColor {
  public:
    unsigned char r, g, b;

    inline RGBColor() {}
    explicit inline RGBColor(const Color &color) {
      r = static_cast<unsigned char>(256.0f * color.r);
      g = static_cast<unsigned char>(256.0f * color.g);
      b = static_cast<unsigned char>(256.0f * color.b);
    }

    inline explicit RGBColor(unsigned char * mRGB) {
      r = *mRGB;
      g = *(mRGB + 1);
      b = *(mRGB + 2);
    }

    inline Color toColor() const;
};


END_RAYTRACER


#endif
