#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "intercept.h"
#include "material.h"
#include "model.h"
#include "ray.h"


BEGIN_RAYTRACER


int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
              Material * entryMat);

int intersectMerge(int op, int hit_left, Intercept intercept_left[],
                   int hit_right, Intercept intercept_right[],
                   Intercept merged[]);

int trace(int level, C_FLT weight, const Ray &ray, Color * color,
          Material * entryMat);


END_RAYTRACER


#endif
