#ifndef SHADE_H
#define SHADE_H

#include "config.h"

#include "color.h"
#include "intersection.h"
#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


void shade(int level, C_FLT weight, Point point, Vector normal,
           Vector incident, Intersection * intercepts, Color * color);

END_RAYTRACER


#endif
