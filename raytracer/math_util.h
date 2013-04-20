#ifndef MATH_UTIL_H
#define MATH_UTIL_H


#include "config.h"


BEGIN_RAYTRACER


inline bool fGreaterZero(const P_FLT pFlt);
inline bool fIsZero(const P_FLT pFlt);
inline bool fLessZero(const P_FLT pFlt);

// NOTE: Do not use these float comparisons for zero checking
inline bool fEqual(const P_FLT pFltA, const P_FLT pFltB);
inline bool fGreater(const P_FLT pFltA, const P_FLT pFltB);
inline bool fLess(const P_FLT pFltA, const P_FLT pFltB);

inline P_FLT fMean(const P_FLT pFltA, const P_FLT pFltB);


END_RAYTRACER


#endif
