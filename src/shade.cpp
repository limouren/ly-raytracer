#ifndef SHADE_H
#define SHADE_H


#include <math.h>

#include "config.h"

#include "color.h"
#include "intersection.h"
#include "light.h"
#include "material.h"
#include "point.h"
#include "scene.h"
#include "shade.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


const Vector refDir(Vector incident, Vector normal) {
  Vector result = incident - (normal * dotProduct(normal, incident) * 2);
  result.normalize();
  return result;
}


void shade(int level, C_FLT weight, Point point, Vector normal,
           Vector incident, Intersection * intercepts, Color * color) {
  Material * material = intercepts[0].material;

  *color += scene.ambience * material->ambience;

  for (int i = 0;i < scene.lights.size();i++) {
    Light * light = scene.lights[i];

    Vector pointToLight = light->orig - point;

    P_FLT distance_to_light = pointToLight.normalize();
    Ray rayToLight(point, pointToLight);

    P_FLT rayDotNormal = dotProduct(pointToLight, normal);
    if (rayDotNormal > 0.0 &&
        shadow(rayToLight, distance_to_light) > 0.0) {
      *color += light->color * material->diffuse * rayDotNormal;
    }
  }

  if (level < MAX_LEVEL && weight > MIN_WEIGHT) {
    P_FLT specWeight = material->specular.luminance() * weight;
    if (specWeight > MIN_WEIGHT) {
      Ray specRay(point, refDir(incident, normal));
      Color specColor(0.0, 0.0, 0.0);

      if (trace(level + 1, specWeight, specRay, &specColor)) {
        *color += specColor * material->specular;
      }
    }
  }
}

END_RAYTRACER


#endif
