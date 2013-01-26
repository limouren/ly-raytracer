#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "intercept.h"
#include "model.h"
#include "ray.h"


BEGIN_RAYTRACER


int intersect(const Ray &ray, const MODEL_CLS * model, Intercept intercepts[]);

int intersectMerge(int op, int hit_left, Intercept intercept_left[],
                   int hit_right, Intercept intercept_right[],
                   Intercept merged[]);


C_FLT shadow(const Ray &ray, P_FLT t);

int trace(int level, C_FLT weight, const Ray &ray, Color * color);


END_RAYTRACER


#endif
