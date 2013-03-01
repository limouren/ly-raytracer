#include <math.h>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "math_util.h"
#include "point.h"
#include "shapes.h"
#include "transform.h"


BEGIN_RAYTRACER


int Plane::intersect(const Ray &ray, Intercept intercepts[]) const {
  P_FLT rayDotNorm, t;

  rayDotNorm = dotProduct(normal, ray.dir);

  if (fIsZero(rayDotNorm)) {
    return 0;
  }

  t = -(dotProduct(normal, ray.orig) + d) / rayDotNorm;

  if (fGreaterZero(t)) {
    intercepts[0] = Intercept(t, rayDotNorm < 0.0f, ray.medium, this);
    return 1;
  }

  return 0;
}


void Plane::transform(Transform * transform) {
  // TODO(kent): Fill this in later
  printf("ERROR: Unimplemented plane transformation\n");
  exit(1);
}


/* const int Circle::intersect(const Ray &ray, Intercept intercepts[]) const {
  if (Plane::intersect(ray, intercepts, ray.medium) == 0) {
    return 0;
  }

  Point3D interceptPoint = ray.rayPoint(intercepts[0].t);
  if ((interceptPoint - center).lengthSqr < radius * radius) {
    return 1;
  }
} */


inline void Polygon::buildBoundingBox() {
  Point3D maxExt = vertex[0],
          minExt = vertex[0];
  for (int i = 1; i < vertexNum; i++) {
    maxExt = pointMax(maxExt, vertex[i]);
    minExt = pointMin(minExt, vertex[i]);
  }

  boundingBox = new BoundingBox(minExt, maxExt);
}


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
int Polygon::intersect(const Ray &ray, Intercept intercepts[]) const {
  if (Plane::intersect(ray, intercepts) == 0) {
    return 0;
  }

  Point3D intercept3D = ray.rayPoint(intercepts[0].t);

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


void Polygon::transform(Transform * transform) {
  for (int i = 0; i < vertexNum; i++) {
    transform->transformPoint(vertex[i], &vertex[i]);
  }

  transform->transformNormal(normal, &normal);
  dominantIndex = normal.dominantIndex();

  d = -dotProduct(vertex[0], normal);

  buildBoundingBox();
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


void PolygonPatch::transform(Transform * transform) {
  Polygon::transform(transform);
  for (int i = 0; i < vertexNum; i++) {
    transform->transformNormal(vertexNormal[i], &vertexNormal[i]);
  }
}


/* TODO(kent): Use this when Cone is implemented
int Cone::buildBoundingBox() {
  // Square means it doesn't matter which normal is which
  Vector3D apexExtend(sqrt(1 - apex.normal.x * apex.normal.x) * apex.radius,
                      sqrt(1 - apex.normal.y * apex.normal.y) * apex.radius,
                      sqrt(1 - apex.normal.z * apex.normal.z) * apex.radius);
  Vector3D baseExtend(sqrt(1 - base.normal.x * base.normal.x) * base.radius,
                      sqrt(1 - base.normal.y * base.normal.y) * base.radius,
                      sqrt(1 - base.normal.z * base.normal.z) * base.radius);

  Point3D minExt = pointMin(base.center - baseExtend,
                            apex.center - apexExtend);
          maxExt = pointMax(base.center + baseExtend,
                            apex.center + apexExtend);
  boundingBox = new BoundingBox(minExt, maxExt);
}


ref: www.geometrictools.com/LibMathematics/Intersection/Intersection.html
int Cone::intersectQuadric() const {} */


void Cylinder::buildBoundingBox() {
  Vector3D radiusVector(radius);
  Point3D minExt = pointMin(baseCenter - radiusVector,
                            apexCenter - radiusVector),
          maxExt = pointMax(baseCenter + radiusVector,
                            apexCenter + radiusVector);
  boundingBox = new BoundingBox(minExt, maxExt);
}


void Cylinder::getIntersect(const Point3D &point, Vector3D * normal,
                            std::vector<P_FLT> * mapping) const {
  P_FLT radiusSqr = radius * radius,
        pointD = - dotProduct(axisNormal, point);

  if (fEqual(baseD, pointD) &&
      (point - baseCenter).lengthSqr() < radiusSqr) {
    *normal = insideOut? axisNormal: -axisNormal;
  } else if (fEqual(apexD, pointD) &&
             (point - apexCenter).lengthSqr() < radiusSqr) {
    *normal = insideOut? -axisNormal: axisNormal;
  }

  Vector3D baseToPoint = point - baseCenter;
  *normal = baseToPoint - axisNormal * dotProduct(axisNormal, baseToPoint);
  if (insideOut) {
    normal->negate();
  }
  normal->normalize();
}


int Cylinder::intersect(const Ray &ray, Intercept intercepts[]) const {
  P_FLT baseToOrigDotAxis, origDistanceSqr,
        rayDotAxis,
        t;
  Vector3D baseToOrig;

  rayDotAxis = dotProduct(ray.dir, axisNormal);
  baseToOrig = ray.orig - baseCenter;
  baseToOrigDotAxis = dotProduct(baseToOrig, axisNormal);
  origDistanceSqr = (baseToOrig - axisNormal * baseToOrigDotAxis).lengthSqr();

  if (!(fGreaterZero(baseToOrigDotAxis) &&
        fLess(baseToOrigDotAxis, height) &&
        fLess(origDistanceSqr, radiusSqr))) {
    // Ray origin outside cylinder
    if (insideOut) {
      return 0;
    }

    std::vector<P_FLT> tValues;
    if (!fIsZero(rayDotAxis)) {
      P_FLT origDotAxis = dotProduct(ray.orig, axisNormal);

      t = -(origDotAxis + apexD) / rayDotAxis;
      if (fGreaterZero(t)) {
        Point3D interceptPoint = ray.rayPoint(t);
        if ((apexCenter - interceptPoint).lengthSqr() < radiusSqr) {
          tValues.push_back(t);
        }
      }

      t = -(origDotAxis + baseD) / rayDotAxis;
      if (fGreaterZero(t)) {
        Point3D interceptPoint = ray.rayPoint(t);
        if ((baseCenter - interceptPoint).lengthSqr() < radiusSqr) {
          tValues.push_back(t);
        }
      }
    }

    intercepts[0] = Intercept(0.0f, true, ray.medium, this);
    intercepts[1] = Intercept(0.0f, false, material, this);

    if (tValues.size() == 2 || fEqual(rayDotAxis, 1.0f)) {
      std::sort(tValues.begin(), tValues.end());
      for (int i = 0; i < tValues.size(); i++) {
        intercepts[i].t = tValues[i];
      }
      return tValues.size();
    }

    P_FLT halfChord, halfDiagonalChord,
          rayDotHorInv, axisDotPoint, rayAxisDistance, rayClosestT;
    Vector3D distanceVector, rayHorizontal;

    distanceVector = crossProduct(ray.dir, axisNormal);
    distanceVector.normalize();
    rayAxisDistance = dotProduct(baseToOrig, distanceVector);

    if (!fLess(rayAxisDistance, radius)) {
      std::sort(tValues.begin(), tValues.end());
      for (int i = 0; i < tValues.size(); i++) {
        intercepts[i].t = tValues[i];
      }
      return tValues.size();
    }

    rayHorizontal = ray.dir - axisNormal * rayDotAxis;
    rayHorizontal.normalize();
    rayDotHorInv = 1.0f / sqrt(1.0f - rayDotAxis * rayDotAxis);
    rayClosestT = -dotProduct(baseToOrig, rayHorizontal) * rayDotHorInv;
    halfChord = sqrt(radiusSqr - rayAxisDistance * rayAxisDistance);
    halfDiagonalChord = halfChord * rayDotHorInv;

    t = rayClosestT - halfDiagonalChord;
    if (fGreaterZero(t)) {
      axisDotPoint = dotProduct(ray.rayPoint(t) - baseCenter, axisNormal);
      if (fGreaterZero(axisDotPoint) && fLess(axisDotPoint, height)) {
        tValues.push_back(t);
      }
    }

    t = rayClosestT + halfDiagonalChord;
    if (fGreaterZero(t)) {
      axisDotPoint = dotProduct(ray.rayPoint(t) - baseCenter, axisNormal);
      if (fGreaterZero(axisDotPoint) && fLess(axisDotPoint, height)) {
        tValues.push_back(t);
      }
    }

    std::sort(tValues.begin(), tValues.end());
    for (int i = 0; i < tValues.size(); i++) {
      intercepts[i].t = tValues[i];
    }
    return tValues.size();
  } else {
    if (!fIsZero(rayDotAxis)) {
      P_FLT origDotAxis = dotProduct(ray.orig, axisNormal);

      t = -(origDotAxis + apexD) / rayDotAxis;
      if (fGreaterZero(t)) {
        Point3D interceptPoint = ray.rayPoint(t);
        if ((apexCenter - interceptPoint).lengthSqr() < radiusSqr) {
          intercepts[0] = Intercept(t, insideOut, material, this);
          return 1;
        }
      }

      t = -(origDotAxis + baseD) / rayDotAxis;
      if (fGreaterZero(t)) {
        Point3D interceptPoint = ray.rayPoint(t);
        if ((baseCenter - interceptPoint).lengthSqr() < radiusSqr) {
          intercepts[0] = Intercept(t, insideOut, material, this);
          return 1;
        }
      }
    }

    if (fEqual(rayDotAxis, 1.0f)) {
      return 0;
    }

    P_FLT halfChord, halfDiagonalChord,
          rayDotHorInv, axisDotPoint, rayAxisDistance, rayClosestT;
    Vector3D distanceVector, rayHorizontal;

    distanceVector = crossProduct(ray.dir, axisNormal);
    distanceVector.normalize();
    rayAxisDistance = dotProduct(baseToOrig, distanceVector);

    if (!fLess(rayAxisDistance, radius)) {
      return 0;
    }

    rayHorizontal = ray.dir - axisNormal * rayDotAxis;
    rayHorizontal.normalize();
    rayDotHorInv = 1.0f / sqrt(1.0f - rayDotAxis * rayDotAxis);
    rayClosestT = -dotProduct(baseToOrig, rayHorizontal) * rayDotHorInv;
    halfChord = sqrt(radiusSqr - rayAxisDistance * rayAxisDistance);
    halfDiagonalChord = halfChord * rayDotHorInv;

    t = rayClosestT - halfDiagonalChord;
    if (fGreaterZero(t)) {
      axisDotPoint = dotProduct(ray.rayPoint(t) - baseCenter, axisNormal);
      if (fGreaterZero(axisDotPoint) && fLess(axisDotPoint, height)) {
        intercepts[0] = Intercept(t, insideOut, material, this);
        return 1;
      }
    }

    t = rayClosestT + halfDiagonalChord;
    if (fGreaterZero(t)) {
      axisDotPoint = dotProduct(ray.rayPoint(t) - baseCenter, axisNormal);
      if (fGreaterZero(axisDotPoint) && fLess(axisDotPoint, height)) {
        intercepts[0] = Intercept(t, insideOut, material, this);
        return 1;
      }
    }

    return 0;
  }
}


void Cylinder::transform(Transform * transform) {
  transform->transformPoint(apexCenter, &apexCenter);
  transform->transformPoint(baseCenter, &baseCenter);
  transform->transformNormal(axisNormal, &axisNormal);
  apexD = -dotProduct(axisNormal, apexCenter);
  baseD = -dotProduct(axisNormal, baseCenter);

  buildBoundingBox();
}


inline void Sphere::buildBoundingBox() {
  Vector3D radiusVector(radius);
  boundingBox = new BoundingBox(center + radiusVector, center - radiusVector);
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


int Sphere::intersect(const Ray &ray, Intercept intercepts[]) const {
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
    intercepts[0] = Intercept(rayClosest - halfChord, true, ray.medium, this);
    intercepts[1] = Intercept(rayClosest + halfChord, false, this->material,
                              this);
    return 2;
  } else {
    intercepts[0] = Intercept(rayClosest + halfChord, insideOut, ray.medium,
                              this);
    return 1;
  }
}


void Sphere::transform(Transform * transform) {
  transform->transformPoint(center, &center);

  buildBoundingBox();
}


END_RAYTRACER
