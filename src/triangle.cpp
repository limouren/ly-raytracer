#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


void Triangle::buildBoundingBox() {
  Point3D minExt = pointMin(pointMin(*vertex1, *vertex2), *vertex3),
          maxExt = pointMax(pointMax(*vertex1, *vertex2), *vertex3);
  boundingBox = new BoundingBox(minExt, maxExt);
}


// Ref: Glassner -An Introduction to Ray Tracing - P.53-59
int Triangle::intersect(const Ray &ray, Intercept intercepts[],
                              Material * entryMat) const {
  P_FLT normalDotOrig, normalDotDir,
        t, u, v;

  normalDotDir = dotProduct(normal, ray.dir);
  if (fIsZero(normalDotDir)) {
    return 0;
  }

  normalDotOrig = dotProduct(normal, ray.orig);

  t = -(d + normalDotOrig) / normalDotDir;

  if (!fGreaterZero(t)) {
    return 0;
  }

  // inspired by povray triangle intersection
  // idea: cross product each vector after projecting to YZ/XZ/XY plane
  switch (dominantIndex) {
    case 0:  // x dominant, normal.x > 0
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex1)->y) * ((vertex2)->z - (vertex1)->z) >
          (v - (vertex1)->z) * ((vertex2)->y - (vertex1)->y)) {
        return 0;
      }
      if ((u - (vertex2)->y) * ((vertex3)->z - (vertex2)->z) >
          (v - (vertex2)->z) * ((vertex3)->y - (vertex2)->y)) {
        return 0;
      }
      if ((u - (vertex3)->y) * ((vertex1)->z - (vertex3)->z) >
          (v - (vertex3)->z) * ((vertex1)->y - (vertex3)->y)) {
        return 0;
      }

      break;

    case 1:  // y dominant, normal.y > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex1)->x) * ((vertex2)->z - (vertex1)->z) <
          (v - (vertex1)->z) * ((vertex2)->x - (vertex1)->x)) {
        return 0;
      }
      if ((u - (vertex2)->x) * ((vertex3)->z - (vertex2)->z) <
          (v - (vertex2)->z) * ((vertex3)->x - (vertex2)->x)) {
        return 0;
      }
      if ((u - (vertex3)->x) * ((vertex1)->z - (vertex3)->z) <
          (v - (vertex3)->z) * ((vertex1)->x - (vertex3)->x)) {
        return 0;
      }

      break;

    case 2:  // z dominant, normal.z > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - (vertex1)->x) * ((vertex2)->y - (vertex1)->y) >
          (v - (vertex1)->y) * ((vertex2)->x - (vertex1)->x)) {
        return 0;
      }
      if ((u - (vertex2)->x) * ((vertex3)->y - (vertex2)->y) >
          (v - (vertex2)->y) * ((vertex3)->x - (vertex2)->x)) {
        return 0;
      }
      if ((u - (vertex3)->x) * ((vertex1)->y - (vertex3)->y) >
          (v - (vertex3)->y) * ((vertex1)->x - (vertex3)->x)) {
        return 0;
      }

      break;

    case 3:  // x dominant, normal.x < 0
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex1)->y) * ((vertex2)->z - (vertex1)->z) <
          (v - (vertex1)->z) * ((vertex2)->y - (vertex1)->y)) {
        return 0;
      }
      if ((u - (vertex2)->y) * ((vertex3)->z - (vertex2)->z) <
          (v - (vertex2)->z) * ((vertex3)->y - (vertex2)->y)) {
        return 0;
      }
      if ((u - (vertex3)->y) * ((vertex1)->z - (vertex3)->z) <
          (v - (vertex3)->z) * ((vertex1)->y - (vertex3)->y)) {
        return 0;
      }

      break;

    case 4:  // y dominant, normal.y < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - (vertex1)->x) * ((vertex2)->z - (vertex1)->z) >
          (v - (vertex1)->z) * ((vertex2)->x - (vertex1)->x)) {
        return 0;
      }
      if ((u - (vertex2)->x) * ((vertex3)->z - (vertex2)->z) >
          (v - (vertex2)->z) * ((vertex3)->x - (vertex2)->x)) {
        return 0;
      }
      if ((u - (vertex3)->x) * ((vertex1)->z - (vertex3)->z) >
          (v - (vertex3)->z) * ((vertex1)->x - (vertex3)->x)) {
        return 0;
      }

      break;

    case 5:  // z dominant, normal.z < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - (vertex1)->x) * ((vertex2)->y - (vertex1)->y) <
          (v - (vertex1)->y) * ((vertex2)->x - (vertex1)->x)) {
        return 0;
      }
      if ((u - (vertex2)->x) * ((vertex3)->y - (vertex2)->y) <
          (v - (vertex2)->y) * ((vertex3)->x - (vertex2)->x)) {
        return 0;
      }
      if ((u - (vertex3)->x) * ((vertex1)->y - (vertex3)->y) <
          (v - (vertex3)->y) * ((vertex1)->x - (vertex3)->x)) {
        return 0;
      }

      break;
  }

  intercepts[0] = Intercept(t, normalDotDir < 0, entryMat, this);
  return 1;
}


std::vector<P_FLT> Triangle::inverseMap(const Point3D &point) const {
  std::vector<P_FLT> mapping(3, 0.0);
  Vector3D v12 = *vertex2 - *vertex1,
           v13 = *vertex3 - *vertex1,
           vP1 = point - *vertex1;
  P_FLT d00 = dotProduct(v12, v12),
        d01 = dotProduct(v12, v13),
        d11 = dotProduct(v13, v13),
        d20 = dotProduct(vP1, v12),
        d21 = dotProduct(vP1, v13);
  P_FLT denom =  1.0f / (d00 * d11 - d01 * d01);
  mapping[1] = (d11 * d20 - d01 * d21) * denom,
  mapping[2] = (d00 * d21 - d01 * d20) * denom,
  mapping[0] = 1.0f - mapping[1] - mapping[2];
  return mapping;
}


void TexturedTriangle::getIntersect(const Point3D &point, Vector3D * normal,
                                    std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = this->normal;
}


Color TexturedTriangle::getTextureColor(
  const std::vector<P_FLT> mapping) const {
  Vector2D textureCoord = *vertexTextureCoord1 * mapping[0] +
                          *vertexTextureCoord2 * mapping[1] +
                          *vertexTextureCoord3 * mapping[2];

  return texture->colorAt(textureCoord);
}


void TrianglePatch::getIntersect(const Point3D &point, Vector3D * normal,
                                 std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = *vertexNormal1 * (*mapping)[0] +
            *vertexNormal2 * (*mapping)[1] +
            *vertexNormal3 * (*mapping)[2];
}


void PhongTriangle::getIntersect(const Point3D &point, Vector3D * normal,
                                 std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = *vertexNormal1 * (*mapping)[0] +
            *vertexNormal2 * (*mapping)[1] +
            *vertexNormal3 * (*mapping)[2];
}


Color PhongTriangle::getTextureColor(
  const std::vector<P_FLT> mapping) const {
  Vector2D textureCoord = *vertexTextureCoord1 * mapping[0] +
                          *vertexTextureCoord2 * mapping[1] +
                          *vertexTextureCoord3 * mapping[2];

  return texture->colorAt(textureCoord);
}


END_RAYTRACER
