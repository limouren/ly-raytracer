#ifndef SHADE_H
#define SHADE_H


#include <math.h>

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


C_FLT shadow(const Ray &ray, P_FLT t, Material * material) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts, material);
  if (hits == 0 || intercepts[0].t > t) {
    return 1.0;
  }

  return 0.0;
}


int specularDirection(const Vector &incident, const Vector &normal,
                      Vector &result) {
  result = incident - (normal * dotProduct(normal, incident) * 2);
  return 0;
}


int transmissionDirection(Material * entryMaterial, Material * exitMaterial,
                          const Vector &incident, const Vector &normal,
                          Vector &result) {
  P_FLT refrEntry,
        refrExit,
        refrRatio,
        cosEntry,
        cosExitSqr;

  // Compute refr direction, return 0 if total internal reflection
  refrEntry = entryMaterial? entryMaterial->refraction: 1.0;
  refrExit = exitMaterial? exitMaterial->refraction: 1.0;
  refrRatio = refrEntry / refrExit;

  cosEntry = -dotProduct(incident, normal);
  cosExitSqr = 1.0 - refrRatio * refrRatio * (1.0 - (cosEntry * cosEntry));
  if (cosExitSqr < 0.0) {
    return 0; // Total internal reflection
  }
  else {
    result = incident * refrRatio +
             normal * (refrRatio * cosEntry - sqrt(cosExitSqr));
    return 1;
  }
}


void shade(int &level, C_FLT weight, const Point &point, const Vector &normal,
           const Vector &incident, Intercept * intercepts, Color * color) {
  Material * material = intercepts[0].primitive->material;

  *color += scene.ambience * material->ambience;

  for (int i = 0;i < scene.lights.size();i++) {
    Light * light = scene.lights[i];

    Vector pointToLight = light->orig - point;

    P_FLT distanceToLight = pointToLight.normalize();
    Ray rayToLight(point, pointToLight);

    P_FLT rayDotNormal = dotProduct(pointToLight, normal);
    if (rayDotNormal > 0.0 &&
        shadow(rayToLight, distanceToLight, material) > 0.0) {

      // Light source diffuse reflection
      *color += light->color * material->diffuse * rayDotNormal;

      Vector specDir;
      specularDirection(-pointToLight, normal, specDir);
      P_FLT specDot = -dotProduct(specDir, incident);

      // Light source specular reflection
      if (specDot > 0.0) {
        *color += light->color * material->specular *
                  pow(specDot, material->roughness);
      }
    }
  }

  if (level < MAX_LEVEL) {
    // Other body specular reflection
    C_FLT specWeight = material->specular.magnitude() * weight;
    if (specWeight > MIN_WEIGHT) {
      Vector specDir;
      specularDirection(incident, normal, specDir);
      Ray specRay(point, specDir);
      Color specColor;

      if (trace(level + 1, specWeight, specRay, &specColor)) {
        *color += specColor * material->specular;
      }
    }

    // Other body specular transmission
    C_FLT transWeight = material->transmission.magnitude() * weight;
    if (transWeight > MIN_WEIGHT) {
      Vector transDir;
      if (transmissionDirection(intercepts[0].material,
                                intercepts[1].material,
                                incident, normal, transDir)) {
        Ray transRay(point, transDir);
        Color transColor;

        if (trace(level + 1, transWeight, transRay, &transColor)) {
          transColor *= material->transmission;
          *color += transColor;
        }
      }
    }
  }
}


void shadeBackground(const Ray &ray, Color * color) {
}


END_RAYTRACER


#endif
