#ifndef SHADE_H
#define SHADE_H


#include "config.h"


BEGIN_RAYTRACER


class Color;
class Intercept;
class Point3D;
class Ray;
class Scene;
class Vector;


void shade(const Scene * scene, const int level, const C_FLT weight,
           const Point3D &point, const Vector3D &normal,
           const Vector3D &incident, Intercept * intercepts, Color * color);

void shadeBackground(const Scene * scene, const Ray &ray, Color * color);


END_RAYTRACER


#endif
