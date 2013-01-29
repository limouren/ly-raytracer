#ifndef SHADE_H
#define SHADE_H

#include "config.h"

#include "color.h"
#include "intercept.h"
#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


void shade(int level, C_FLT weight, const Point3D &point,
           const Vector3D &normal, const Vector3D &incident,
           Intercept * intercepts, Color * color);

void shadeBackground(const Ray &ray, Color * color);


END_RAYTRACER


#endif
