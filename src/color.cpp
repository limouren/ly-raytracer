#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


Color& Color::operator =(const Color &color) {
  if (this == &color) {
    return *this;
  }

  r = color.r;
  g = color.g;
  b = color.b;

  return *this;
}


const Color Color::operator +(const Color color) const {
  Color result = *this;
  result += color;

  return result;
}


Color Color::operator +=(const Color color) {
  r += color.r;
  g += color.g;
  b += color.b;

  return *this;
}


const Color Color::operator *(const C_FLT scalar) const {
  Color result = *this;
  result *= scalar;

  return result;
}


Color Color::operator *=(const C_FLT scalar) {
  r *= scalar;
  g *= scalar;
  b *= scalar;

  return *this;
}


const Color Color::operator *(const Color color) const {
  Color result = *this;
  result *= color;

  return result;
}


Color Color::operator *=(const Color color) {
  r *= color.r;
  g *= color.g;
  b *= color.b;

  return *this;
}


void Color::scale() {
  C_FLT factor;
  if (r > g) {
    if (b > r) {
      factor = 1.0 / b;
    } else {
      factor = 1.0 / r;
    }
  } else if (b > g) {
    factor = 1.0 / b;
  } else {
    factor = 1.0 / g;
  }

  r *= factor;
  g *= factor;
  b *= factor;
}


END_RAYTRACER
