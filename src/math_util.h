#ifndef MATH_UTIL_H
#define MATH_UTIL_H


#include <math.h>


#include "config.h"


BEGIN_RAYTRACER


const bool fEqual(P_FLT float_a, P_FLT float_b, P_FLT epsilon) {
  P_FLT margin = 0.5 * epsilon * (float_a + float_b);
  return fabs(float_a - float_b) < margin;
}

const bool fEqual(P_FLT float_a, P_FLT float_b) {
  return fEqual(float_a, float_b, P_FLT_EPSILON);
}

const bool fIsZero(P_FLT p_flt, P_FLT epsilon) {
  return fabs(p_flt) < (p_flt * epsilon);
}

const bool fIsZero(P_FLT p_flt) {
  return fIsZero(p_flt, P_FLT_EPSILON);
}


END_RAYTRACER


#endif
