#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "intercept.h"
#include "material.h"
#include "model.h"
#include "ray.h"


BEGIN_RAYTRACER


inline int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
                     Material * entryMat);

inline int intersectMerge(const int op,
                          const int hitsLeft, Intercept interceptLeft[],
                          const int hitsRight, Intercept interceptRight[],
                          Intercept merged[]);

inline int trace(const int level, const C_FLT weight, const Ray &ray,
                 Color * color, Material * entryMat);


END_RAYTRACER


#endif
