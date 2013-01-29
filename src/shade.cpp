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


C_FLT shadow(const Ray &ray, P_FLT t) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene.modelRoot, intercepts, NULL);
  if (hits == 0 || intercepts[0].t > t) {
    return 1.0;
  }

  return 0.0;
}


void specularDirection(const Vector &incident, const Vector &normal,
                      Vector &result) {
  result = incident - (normal * dotProduct(normal, incident) * 2);
}


bool transmissionDirection(Material * entryMaterial, Material * exitMaterial,
                          const Vector &incident, const Vector &normal,
                          Vector &result) {
  P_FLT refrRatio,
        cosEntry,
        cosExitSqr;

  // Compute refr direction, return 0 if total internal reflection
  refrRatio = entryMaterial->refraction / exitMaterial->refraction;

  cosEntry = -dotProduct(incident, normal);
  cosExitSqr = 1.0 - refrRatio * refrRatio * (1.0 - (cosEntry * cosEntry));
  if (cosExitSqr < 0.0) {
    return false;  // Total internal reflection
  } else {
    result = incident * refrRatio + normal *
             (refrRatio * cosEntry - sqrt(cosExitSqr));
    return true;
  }
}


void shade(int level, C_FLT weight, const Point &point, const Vector &normal,
           const Vector &incident, Intercept * intercepts, Color * color) {
  Material * entryMaterial = intercepts[0].material,
           * exitMaterial = intercepts[0].enter?
                            intercepts[0].primitive->material: scene.medium;

  *color += scene.ambience * exitMaterial->ambience;

  C_FLT specWeight = exitMaterial->specular.magnitude() * weight;
  Vector specDir;
  specularDirection(incident, normal, specDir);

  Vector transDir;
  bool transmission = transmissionDirection(entryMaterial, exitMaterial,
                                            incident, normal, transDir);
  C_FLT transWeight = exitMaterial->transmission.magnitude() * weight;

  for (int i = 0; i < scene.lights.size(); i++) {
    Light * light = scene.lights[i];

    Vector pointToLight = light->orig - point;
    P_FLT distanceToLight = pointToLight.normalize();
    Ray rayToLight(point, pointToLight);

    P_FLT rayDotNormal = dotProduct(pointToLight, normal);
    if (rayDotNormal > 0.0 &&
        shadow(rayToLight, distanceToLight) > 0.0) {
      // Light source diffuse reflection
      *color += light->color * exitMaterial->diffuse * rayDotNormal;

      // Light source specular reflection
      Vector h = pointToLight - incident;
      h.normalize();
      P_FLT specDot = dotProduct(normal, h);
      if (specDot > 0.0) {
        *color += light->color * exitMaterial->specular *
                  pow(specDot, exitMaterial->roughness);
      }
    } else if (rayDotNormal < 0.0 && transmission &&
               shadow(rayToLight, distanceToLight) > 0.0) {
      // Light source specular transmission
      C_FLT refrRatio = exitMaterial->refraction / entryMaterial->refraction;
      Vector h_j = (-incident - pointToLight * refrRatio) / (refrRatio - 1);
      h_j.normalize();

      // TODO(kent): Define transmission highlight coefficient
      Color specTrans = light->color * exitMaterial->transmission *
                pow(dotProduct(-normal, h_j), exitMaterial->roughness);
      *color += specTrans;
    }
  }

  if (level < MAX_LEVEL) {
    // Other body specular reflection
    if (specWeight > MIN_WEIGHT) {
      Ray specRay(point, specDir);
      Color specColor;

      if (trace(level + 1, specWeight, specRay, &specColor, entryMaterial)) {
        *color += specColor * exitMaterial->specular;
      }
    }

    // Other body specular transmission
    if (transWeight > MIN_WEIGHT) {
      Vector transDir;
      if (transmissionDirection(entryMaterial, exitMaterial,
                                incident, normal, transDir)) {
        Ray transRay(point, transDir);
        Color transColor;
        trace(level + 1, transWeight, transRay, &transColor, exitMaterial);
        transColor *= exitMaterial->transmission;
        *color += transColor;
      }
    }
  }
}


void shadeBackground(const Ray &ray, Color * color) {
  if (scene.background) {
    *color += scene.background->colorInDirection(ray.dir);
  }
}


END_RAYTRACER


#endif
