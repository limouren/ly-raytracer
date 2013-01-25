#ifndef SHADE_H
#define SHADE_H

#include "config.h"

#include "color.h"
#include "intercept.h"
#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


void shade(int level, C_FLT weight, Point point, Vector normal,
           Vector incident, Intercept * intercepts, Color * color);

END_RAYTRACER


#endif
