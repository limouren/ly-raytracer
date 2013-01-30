#include <math.h>

#include "config.h"

#include "math_util.h"


BEGIN_RAYTRACER


inline const bool fGreaterZero(const P_FLT pFlt) {
  return pFlt > P_FLT_EPSILON;
}


inline const bool fIsZero(const P_FLT pFlt) {
  P_FLT absPFlt = fabs(pFlt);
  return absPFlt < (absPFlt * P_FLT_EPSILON);
}


inline const bool fLessZero(const P_FLT pFlt) {
  return pFlt < P_FLT_EPSILON;
}


inline const bool fEqual(const P_FLT pFltA, const P_FLT pFltB) {
  return fIsZero(pFltA - pFltB);
}


inline const bool fGreater(const P_FLT pFltA, const P_FLT pFltB) {
  return pFltA - pFltB > fabs(fMean(pFltA, pFltB) * P_FLT_EPSILON);
}


inline const bool fLess(const P_FLT pFltA, const P_FLT pFltB) {
  return pFltB - pFltA > fabs(fMean(pFltA, pFltB) * P_FLT_EPSILON);
}


inline const P_FLT fMean(const P_FLT pFltA, const P_FLT pFltB) {
  return 0.5 * (pFltA + pFltB);
}


END_RAYTRACER
