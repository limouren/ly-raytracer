#ifndef SHADE_H
#define SHADE_H


#include <math.h>
#include <stdio.h>

#include "config.h"

#include "color.h"
#include "intercept.h"
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
  return result;
}


void shade(int level, C_FLT weight, Point point, Vector normal,
           Vector incident, Intercept * intercepts, Color * color) {
  Material * material = intercepts[0].primitive->material;

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

      Vector refLight = refDir(-pointToLight, normal);
      P_FLT specDot = dotProduct(refLight, -incident);

      if (specDot > 0.0) {
        *color += light->color * material->specular *
                  pow(specDot, material->roughness);
      }
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
