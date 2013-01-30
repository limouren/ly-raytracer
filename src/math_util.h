#ifndef MATH_UTIL_H
#define MATH_UTIL_H


#include "config.h"


BEGIN_RAYTRACER


inline const bool fGreaterZero(const P_FLT pFlt);
inline const bool fIsZero(const P_FLT pFlt);
inline const bool fLessZero(const P_FLT pFlt);

// NOTE: Do not use these float comparisons for zero checking
inline const bool fEqual(const P_FLT pFltA, const P_FLT pFltB);
inline const bool fGreater(const P_FLT pFltA, const P_FLT pFltB);
inline const bool fLess(const P_FLT pFltA, const P_FLT pFltB);

inline const P_FLT fMean(const P_FLT pFltA, const P_FLT pFltB);


END_RAYTRACER


#endif
