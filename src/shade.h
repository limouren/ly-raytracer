#ifndef SHADE_H
#define SHADE_H

#include "config.h"

#include "color.h"
#include "intersection.h"
#include "light.h"
#include "material.h"
#include "point.h"
#include "scene.h"
#include "trace.h"
#include "vector.h"

#include <stdio.h>

BEGIN_RAYTRACER


void shade(int level, C_FLT weight, Point point, Vector normal,
           Vector incident, Intersection * intercepts, Color * color) {
  Material * material = intercepts[0].material;

  for (int i = 0;i < scene.lights.size();i++) {
    Light * light = scene.lights[i];
    Vector point_to_light = light->orig - point;
    P_FLT distance_to_light = point_to_light.normalize();
    Ray ray_to_light(point, point_to_light);

    P_FLT ray_dot_normal = dotProduct(point_to_light, normal);
    if (ray_dot_normal < 0 || Shadow(ray_to_light, distance_to_light) < 1.0 ) {
      continue;
    }
    *color += (light->color * material->color * ray_dot_normal);
  }

  if (level >= MAX_LEVEL || weight < MIN_WEIGHT) {
    return;
  }

  P_FLT specWeight = material->kspec * weight;
  if (specWeight > MIN_WEIGHT) {
    Ray specRay(point, incident - (normal * dotProduct(normal, incident) * 2));
    Color * specColor;
    int flag = trace(level + 1, specWeight, specRay, specColor);
    *color += (*specColor) * material->kspec;
  }

  return;
}


END_RAYTRACER


#endif
