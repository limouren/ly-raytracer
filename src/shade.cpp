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


bool transmissionDirection(Material * entryMat, Material * hitMat,
                           const Vector3D &incident, const Vector3D &normal,
                           Vector3D &result) {
  P_FLT refrRatio,
        cosEntry,
        cosExitSqr;

  // Compute refr direction, return 0 if total internal reflection
  refrRatio = entryMat->refraction / hitMat->refraction;

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


void shade(int level, C_FLT weight, const Point3D &interceptPoint,
           const Vector3D &incident, Intercept * intercepts, Color * color) {
  Material * entryMat = intercepts[0].material,
           * hitMat = intercepts[0].enter?
                      intercepts[0].primitive->material: scene.medium;

  C_FLT specWeight = hitMat->specular.magnitude() * weight,
        transWeight = hitMat->transmission.magnitude() * weight;

  Vector3D specDir, transDir, normal;

  normal = intercepts[0].primitive->normalAt(interceptPoint);
  if (dotProduct(incident, normal) > 0.0) {
    normal.negate();
  }
  specularDirection(incident, normal, specDir);
  bool transmission = transmissionDirection(entryMat, hitMat, incident, normal,
                                            transDir);

  *color += scene.ambience * hitMat->ambience;

  for (int i = 0; i < scene.lights.size(); i++) {
    Light * light = scene.lights[i];

    Vector3D pointToLight = light->orig - interceptPoint;
    P_FLT distanceToLight = pointToLight.normalize();
    Ray rayToLight(interceptPoint, pointToLight);

    P_FLT lightDotNormal = dotProduct(pointToLight, normal);
    if (fGreaterZero(lightDotNormal) &&
        fGreaterZero(shadow(rayToLight, distanceToLight))) {
      // Light source diffuse reflection
      *color += light->color * hitMat->diffuse * lightDotNormal;

      // Light source specular reflection
      Vector3D h = pointToLight - incident;
      h.normalize();
      P_FLT specDot = dotProduct(normal, h);
      if (specDot > 0.0) {
        *color += light->color * hitMat->specular *
                  pow(specDot, hitMat->shine);
      }
    } else if (fLessZero(transmission && lightDotNormal) &&
               fLessZero(shadow(rayToLight, distanceToLight))) {
      // Light source specular transmission
      C_FLT refrRatio = hitMat->refraction / entryMat->refraction;
      if (!fEqual(refrRatio, 1.0)) {
        Vector3D h_j = (-incident - pointToLight * refrRatio) /
                       (refrRatio - 1);
        h_j.normalize();

        // TODO(kent): Define transmission highlight coefficient
        *color += light->color * hitMat->transmission *
                  pow(dotProduct(-normal, h_j), hitMat->shine);
      }
    }
  }

  if (level < MAX_LEVEL) {
    // Other body specular reflection
    if (specWeight > MIN_WEIGHT) {
      Ray specRay(interceptPoint, specDir);
      Color specColor;

      trace(level + 1, specWeight, specRay, &specColor, entryMat);
      *color += specColor * hitMat->specular;
    }

    // Other body specular transmission
    if (transWeight > MIN_WEIGHT) {
      if (transmission) {
        Ray transRay(interceptPoint, transDir);
        Color transColor;

        trace(level + 1, transWeight, transRay, &transColor, hitMat);
        *color += transColor * hitMat->transmission;
      } else {
        // TODO(kent): Handle total internal reflection
      }
    }
  }

  if (intercepts[0].enter && intercepts[0].primitive->texture != NULL) {
    *color *= intercepts[0].primitive->getTextureColor(
      intercepts[0].primitive->inverseMap(interceptPoint));
  }
}


void shadeBackground(const Ray &ray, Color * color) {
  *color += scene.backgroundColor;
}


END_RAYTRACER
