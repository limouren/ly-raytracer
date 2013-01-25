#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "intersection.h"
#include "model.h"
#include "ray.h"


BEGIN_RAYTRACER


int intersect(Ray ray, MODEL_CLS * model, Intersection intercepts[]);

int intersectMerge(int op, int hit_left, Intersection intercept_left[],
                   int hit_right, Intersection intercept_right[],
                   Intersection merged[]);


C_FLT shadow(Ray ray, P_FLT t);

int trace(int level, C_FLT weight, Ray ray, Color * color);


END_RAYTRACER


#endif
