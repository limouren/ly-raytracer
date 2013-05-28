#include <math.h>
#include <vector>

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


C_FLT shadow(const Scene * scene, const Ray &ray, P_FLT t) {
  Intercept intercepts[MAX_INTERSECTIONS];

  int hits = intersect(ray, scene->sceneRoot, intercepts);
  if (hits == 0 || intercepts[0].t > t) {
    return 1.0f;
  }

  return 0.0f;
}


inline void specularDirection(const Vector3D &incident, const Vector3D &normal,
                              Vector3D * result) {
  *result = incident - (normal * dotProduct(normal, incident) * 2);
}


inline bool transmissionDirection(const Material * entryMat,
                                  const Material * hitMat,
                                  const Vector3D &incident,
                                  const Vector3D &normal,
                                  Vector3D * result) {
  P_FLT refrRatio,
        cosEntry,
        cosExitSqr;

  // Compute refr direction, return 0 if total internal reflection
  refrRatio = entryMat->refraction / hitMat->refraction;

  cosEntry = -dotProduct(incident, normal);
  cosExitSqr = 1.0f - refrRatio * refrRatio * (1.0f - (cosEntry * cosEntry));
  if (cosExitSqr < 0.0f) {
    return false;  // Total internal reflection
  } else {
    *result = incident * refrRatio + normal *
              (refrRatio * cosEntry - sqrt(cosExitSqr));
    return true;
  }
}


void shade(const Scene * scene, const int level, const C_FLT weight,
           const Ray &ray, Intercept * intercepts, Color * color) {
  Material * entryMat = intercepts[0].material,
           * hitMat = intercepts[0].enter?
                      intercepts[0].primitive->material: ray.medium;

  C_FLT specWeight = hitMat->specular.magnitude() * weight,
        transWeight = hitMat->transmission.magnitude() * weight;

  Vector3D specDir, transDir, normal;
  std::vector<P_FLT> mapping;

  Point3D interceptPoint = ray.rayPoint(intercepts[0].t);

  intercepts[0].primitive->getIntersect(interceptPoint, &normal, &mapping);
  if (dotProduct(ray.dir, normal) > 0.0f) {
    normal.negate();
  }
  specularDirection(ray.dir, normal, &specDir);
  bool transmission = transmissionDirection(entryMat, hitMat, ray.dir, normal,
                                            &transDir);

  *color += scene->ambience * hitMat->ambience;

  for (std::vector<Light *>::const_iterator itr = scene->lights.begin();
       itr != scene->lights.end(); itr++) {
    Vector3D pointToLight = (*itr)->orig - interceptPoint;
    P_FLT distanceToLight = pointToLight.normalize();
    Ray rayToLight(interceptPoint, pointToLight, NULL);

    P_FLT lightDotNormal = dotProduct(pointToLight, normal);
    if (fGreaterZero(lightDotNormal) &&
        fGreaterZero(shadow(scene, rayToLight, distanceToLight))) {
      // Light source diffuse reflection
      *color += (*itr)->color * hitMat->diffuse * lightDotNormal;

      // Light source specular reflection
      Vector3D h = pointToLight - ray.dir;
      h.normalize();
      P_FLT specDot = dotProduct(normal, h);
      if (specDot > 0.0f) {
        *color += (*itr)->color * hitMat->specular *
                  pow(specDot, hitMat->shine);
      }
    } 
    // else if (transmission && fLessZero(lightDotNormal) &&
    //            fLessZero(shadow(scene, rayToLight, distanceToLight))) {
    //   // Light source specular transmission
    //   C_FLT refrRatio = hitMat->refraction / entryMat->refraction;
    //   if (!fEqual(refrRatio, 1.0f)) {
    //     Vector3D h_j = (-ray.dir - pointToLight * refrRatio) /
    //                    (refrRatio - 1);
    //     h_j.normalize();

    //     // TODO(kent): Define transmission highlight coefficient
    //     *color += (*itr)->color * hitMat->transmission *
    //               pow(dotProduct(-normal, h_j), hitMat->shine);
    //   }
    // }
  }

  if (level < MAX_LEVEL) {
    // Other body specular reflection
    if (specWeight > MIN_WEIGHT) {
      Ray specRay(interceptPoint, specDir, entryMat);
      Color specColor;

      trace(scene, level + 1, specWeight, specRay, &specColor);
      *color += specColor * hitMat->specular;
    }

    // Other body specular transmission
    if (transWeight > MIN_WEIGHT) {
      if (transmission) {
        Ray transRay(interceptPoint, transDir, hitMat);
        Color transColor;

        trace(scene, level + 1, transWeight, transRay, &transColor);
        *color += transColor * hitMat->transmission;
      } else {
        // TODO(kent): Handle total internal reflection
      }
    }
  }

  if (intercepts[0].enter && intercepts[0].primitive->texture != NULL) {
    *color *= intercepts[0].primitive->getTexColor(mapping);
  }
}


inline void shadeBackground(const Scene * scene, const Ray &ray,
                            Color * color) {
  // TODO(kent): Shade background map according to ray.dir
  *color += scene->backgroundColor;
}


END_RAYTRACER
