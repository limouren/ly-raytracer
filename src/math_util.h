#ifndef MATH_UTIL_H
#define MATH_UTIL_H


#include <math.h>


#include "config.h"


BEGIN_RAYTRACER


const bool fEqual(P_FLT &pFltA, P_FLT &pFltB, P_FLT epsilon) {
  return fabs(pFltA - pFltB) < epsilon * pFltA;
}

const bool fEqual(P_FLT pFltA, P_FLT pFltB) {
  return fEqual(pFltA, pFltB, P_FLT_EPSILON);
}

const bool fIsZero(P_FLT &pFlt, P_FLT epsilon) {
  return fabs(pFlt) < (pFlt * epsilon);
}

const bool fIsZero(P_FLT pFlt) {
  return fIsZero(pFlt, P_FLT_EPSILON);
}

const bool fGreaterThan(P_FLT &pFltA, P_FLT &pFltB, P_FLT epsilon) {
  return pFltA - pFltB > fabs(pFltA * P_FLT_EPSILON);
}

const bool fGreaterThan(P_FLT pFltA, P_FLT pFltB) {
  return fGreaterThan(pFltA, pFltB, P_FLT_EPSILON);
}

const bool fLessThan(P_FLT &pFltA, P_FLT &pFltB, P_FLT epsilon) {
  return pFltA - pFltB < fabs(pFltA * P_FLT_EPSILON);
}

const bool fLessThan(P_FLT pFltA, P_FLT pFltB) {
  return fLessThan(pFltA, pFltB, P_FLT_EPSILON);
}


END_RAYTRACER


#endif
