#include <math.h>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "math_util.h"
#include "model.h"
#include "point.h"
#include "shapes.h"


BEGIN_RAYTRACER


const int Plane::intersect(const Ray &ray, Intercept intercepts[],
                           Material * entryMat) const {
  P_FLT lightDotNorm, t;

  lightDotNorm = dotProduct(normal, ray.dir);

  if (fIsZero(lightDotNorm)) {
    return 0;
  }

  t = -(dotProduct(normal, ray.orig) + d) / lightDotNorm;

  if (fGreaterZero(t)) {
    intercepts[0] = Intercept(t, lightDotNorm < 0.0f, entryMat, this);
    return 1;
  }

  return 0;
}


inline void Polygon::buildBoundingVolume() {
  Point3D maxExt = vertex[0],
          minExt = vertex[0];
  for (int i = 1; i < vertexNum; i++) {
    maxExt = max(maxExt, vertex[i]);
    minExt = min(minExt, vertex[i]);
  }

  boundingVolume = new Box(minExt, maxExt);
}


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
const int Polygon::intersect(const Ray &ray, Intercept intercepts[],
                             Material * entryMat) const {
  if (!boundingVolume->intersect(ray) ||
      Plane::intersect(ray, intercepts, entryMat) == 0) {
    return 0;
  }

  Point3D intercept3D = ray.rayPoint(intercepts[0].t);
  int dominantIndex = normal.dominantIndex();

  Vector2D intercept2D = intercept3D.dropIndex(dominantIndex);
  Vector2D * vertex2D = new Vector2D[vertexNum];
  for (int i = 0; i < vertexNum; i++) {
    vertex2D[i] = vertex[i].dropIndex(dominantIndex) - intercept2D;
  }

  int windingNumber = 0,
      first = vertexNum - 1;
  bool firstYPositive = (vertex2D[first].y >= 0.0f),
       secondYPositive;

  for (int second = 0; second < vertexNum; second++) {
    secondYPositive = (vertex2D[second].y >= 0.0f);

    if (firstYPositive != secondYPositive) {
      if (vertex2D[first].x >= 0.0f && vertex2D[second].x >= 0.0f) {
        windingNumber++;
      } else if ((vertex2D[first].x >= 0.0f || vertex2D[second].x >= 0.0f) &&
                 vertex2D[first].x - (vertex2D[first].y *
                 (vertex2D[second].x - vertex2D[first].x) /
                 (vertex2D[second].y - vertex2D[first].y)) > 0.0f) {
        windingNumber++;
      }
    }

    first = second;
    firstYPositive = secondYPositive;
  }

  delete [] vertex2D;
  return windingNumber & 1;
}


void PolygonPatch::getIntersect(const Point3D &point, Vector3D * normal,
                                std::vector<P_FLT> * mapping) const {
  // TODO(kent): True interpolation
  P_FLT * weights = new P_FLT[vertexNum];
  *normal = Vector3D(0.0f);

  for (int i = 0; i < vertexNum; i++) {
    if (point == vertex[i]) {
      *normal = vertexNormal[i];
      return;
    }
  }
  for (int i = 0; i < vertexNum; i++) {
    weights[i] = 1 / (vertex[i] - point).length();
    *normal += vertexNormal[i] * weights[i];
  }

  delete [] weights;
  normal->normalize();
}


inline void Sphere::buildBoundingVolume() {
  Vector3D radiusVector(radius);
  boundingVolume = new Box(center + radiusVector, center - radiusVector);
}


void Sphere::getIntersect(const Point3D &point, Vector3D * normal,
                          std::vector<P_FLT> * mapping) const {
  // TODO(kent): Inverse sphere mapping
  if (insideOut) {
    *normal = point - center;
  } else {
    *normal = center - point;
  }

  normal->normalize();
}


const int Sphere::intersect(const Ray &ray, Intercept intercepts[],
                            Material * entryMat) const {
  if (!boundingVolume->intersect(ray)) {
    return 0;
  }

  P_FLT halfChordSqr, halfChord, ocSqr, radiusSqr, rayClosest;
  Vector3D originToCenter = center - ray.orig;

  ocSqr = originToCenter.lengthSqr();
  radiusSqr = radius * radius;
  rayClosest = dotProduct(originToCenter, ray.dir);

  if (fLessZero(rayClosest) and !fLess(ocSqr, radiusSqr)) {
    return 0;
  }


  halfChordSqr = radiusSqr - ocSqr + (rayClosest * rayClosest);
  if (fLessZero(halfChordSqr)) {
    return 0;
  }

  halfChord = sqrt(halfChordSqr);
  if (fGreater(rayClosest, halfChord)) {
    if (insideOut) {
      return 0;
    }
    intercepts[0] = Intercept(rayClosest - halfChord, true, entryMat, this);
    intercepts[1] = Intercept(rayClosest + halfChord, false, this->material,
                              this);
    return 2;
  } else {
    intercepts[0] = Intercept(rayClosest + halfChord, insideOut, entryMat,
                              this);
    return 1;
  }
}


END_RAYTRACER
