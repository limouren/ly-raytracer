#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "intersection.h"
#include "ray.h"
#include "solid.h"


BEGIN_RAYTRACER


int intersect(Ray ray, Solid * solid, Intersection * intercept);

int intersectMerge(int op, int hit_left, Intersection * intercept_left,
                   int hit_right, Intersection * intercept_right);


C_FLT Shadow(Ray ray, P_FLT t);

int trace(int level, C_FLT weight, Ray ray, Color * color);


END_RAYTRACER


#endif
