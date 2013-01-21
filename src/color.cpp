#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


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


void Color::copyFrom(const Color color) {
  r = color.r;
  g = color.g;
  b = color.b;
}


END_RAYTRACER
