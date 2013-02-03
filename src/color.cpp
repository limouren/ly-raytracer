#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


inline Color& Color::operator =(const Color &color) {
  if (this == &color) {
    return *this;
  }

  r = color.r;
  g = color.g;
  b = color.b;

  return *this;
}


inline const Color Color::operator +(const Color color) const {
  Color result = *this;
  result += color;

  return result;
}


inline Color Color::operator +=(const Color color) {
  r += color.r;
  g += color.g;
  b += color.b;

  return *this;
}


inline const Color Color::operator *(const Color color) const {
  Color result = *this;
  result *= color;

  return result;
}


inline Color Color::operator *=(const Color color) {
  r *= color.r;
  g *= color.g;
  b *= color.b;

  return *this;
}


inline const Color Color::operator *(const C_FLT scalar) const {
  Color result = *this;
  result *= scalar;

  return result;
}


inline Color Color::operator *=(const C_FLT scalar) {
  r *= scalar;
  g *= scalar;
  b *= scalar;

  return *this;
}


END_RAYTRACER
