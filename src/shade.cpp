#ifndef SHADE_H
#define SHADE_H


#include <math.h>

#include "config.h"

#include "color.h"
#include "intercept.h"
#include "light.h"
#include "material.h"
#include "math_util.h"
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


void specularDirection(const Vector3D &incident, const Vector3D &normal,
                       Vector3D &result) {
  result = incident - (normal * dotProduct(normal, incident) * 2);
}


bool transmissionDirection(Material * entryMaterial, Material * exitMaterial,
                           const Vector3D &incident, const Vector3D &normal,
                           Vector3D &result) {
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


void shade(int level, C_FLT weight, const Point3D &point,
           const Vector3D &normal, const Vector3D &incident,
           Intercept * intercepts, Color * color) {
  Material * entryMaterial = intercepts[0].material,
           * exitMaterial = intercepts[0].enter?
                            intercepts[0].primitive->material: scene.medium;

  C_FLT specWeight = exitMaterial->specular.magnitude() * weight,
        transWeight = exitMaterial->transmission.magnitude() * weight;

  Vector3D specDir, transDir;

  specularDirection(incident, normal, specDir);
  bool transmission = transmissionDirection(entryMaterial, exitMaterial,
                                            incident, normal, transDir);

  *color += scene.ambience * exitMaterial->ambience;

  for (int i = 0; i < scene.lights.size(); i++) {
    Light * light = scene.lights[i];

    Vector3D pointToLight = light->orig - point;
    P_FLT distanceToLight = pointToLight.normalize();
    Ray rayToLight(point, pointToLight);

    P_FLT lightDotNormal = dotProduct(pointToLight, normal);
    if (fGreaterZero(lightDotNormal) &&
        fGreaterZero(shadow(rayToLight, distanceToLight))) {
      // Light source diffuse reflection
      *color += light->color * exitMaterial->diffuse * lightDotNormal;

      // Light source specular reflection
      Vector3D h = pointToLight - incident;
      h.normalize();
      P_FLT specDot = dotProduct(normal, h);
      if (specDot > 0.0) {
        *color += light->color * exitMaterial->specular *
                  pow(specDot, exitMaterial->roughness);
      }
    } else if (fLessZero(transmission && lightDotNormal) &&
               fLessZero(shadow(rayToLight, distanceToLight))) {
      // Light source specular transmission
      C_FLT refrRatio = exitMaterial->refraction / entryMaterial->refraction;
      if (!fEqual(refrRatio, 1.0)) {
        Vector3D h_j = (-incident - pointToLight * refrRatio) /
                       (refrRatio - 1);
        h_j.normalize();

        // TODO(kent): Define transmission highlight coefficient
        *color += light->color * exitMaterial->transmission *
                  pow(dotProduct(-normal, h_j), exitMaterial->roughness);
      }
    }
  }

  if (level < MAX_LEVEL) {
    // Other body specular reflection
    if (specWeight > MIN_WEIGHT) {
      Ray specRay(point, specDir);
      Color specColor;

      trace(level + 1, specWeight, specRay, &specColor, entryMaterial);
      *color += specColor * exitMaterial->specular;
    }

    // Other body specular transmission
    if (transWeight > MIN_WEIGHT) {
      if (transmission) {
        Ray transRay(point, transDir);
        Color transColor;

        trace(level + 1, transWeight, transRay, &transColor, exitMaterial);
        *color += transColor * exitMaterial->transmission;
      } else {
        // TODO(kent): Handle total internal reflection
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
