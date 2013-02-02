#include <math.h>

#include "config.h"

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
    intercepts[0] = Intercept(t, lightDotNorm < 0.0, entryMat, this);
    return 1;
  }

  return 0;
}


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
const int Polygon::intersect(const Ray &ray, Intercept intercepts[],
                             Material * entryMat) const {
  if (Plane::intersect(ray, intercepts, entryMat) == 0) {
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
  bool firstYPositive = (vertex2D[first].y >= 0.0),
       secondYPositive;

  for (int second = 0; second < vertexNum; second++) {
    secondYPositive = (vertex2D[second].y >= 0.0);

    if (firstYPositive != secondYPositive) {
      if (vertex2D[first].x >= 0.0 && vertex2D[second].x >= 0.0) {
        windingNumber++;
      } else if ((vertex2D[first].x >= 0.0 || vertex2D[second].x >= 0.0) &&
                 vertex2D[first].x - (vertex2D[first].y *
                 (vertex2D[second].x - vertex2D[first].x) /
                 (vertex2D[second].y - vertex2D[first].y)) > 0.0) {
        windingNumber++;
      }
    }

    first = second;
    firstYPositive = secondYPositive;
  }

  delete [] vertex2D;
  return windingNumber % 2;
}


const int Sphere::intersect(const Ray &ray, Intercept intercepts[],
                            Material * entryMat) const {
  bool insideOut = (radius < 0.0);
  P_FLT halfChordSqr, halfChord, ocSqr, rayClosest, t;
  Vector3D originToCenter;

  originToCenter = center - ray.orig;
  ocSqr = originToCenter.lengthSqr();
  rayClosest = dotProduct(originToCenter, ray.dir);

  if (fLessZero(rayClosest) and !fLess(ocSqr, radius * radius)) {
    return 0;
  }


  halfChordSqr = (radius * radius) - ocSqr + (rayClosest * rayClosest);
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


const Vector3D Sphere::normalAt(const Point3D &point) const {
  Vector3D normal;
  if (radius < 0.0) {
    normal = point - center;
  } else {
    normal = center - point;
  }

  normal.normalize();
  return normal;
}


END_RAYTRACER
