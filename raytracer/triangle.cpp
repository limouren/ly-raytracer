#include <vector>

#include "config.h"

#include "bounding_volume.h"
#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "shapes.h"
#include "triangle.h"
#include "vector.h"


BEGIN_RAYTRACER


void Triangle::buildBoundingBox() {
  Point3D minExt = pointMin(pointMin(vertex1, vertex2), vertex3),
          maxExt = pointMax(pointMax(vertex1, vertex2), vertex3);
  boundingBox = new BoundingBox(minExt, maxExt);
}


Primitive * Triangle::interpolate(Primitive * primitive,
                                  const P_FLT ratio) const {
  Triangle * triangle = static_cast<Triangle *>(primitive);
  P_FLT ratioSub = 1.0f - ratio;

  Point3D newVertex1 = vertex1 * ratioSub + triangle->vertex1 * ratio,
          newVertex2 = vertex2 * ratioSub + triangle->vertex2 * ratio,
          newVertex3 = vertex3 * ratioSub + triangle->vertex3 * ratio;

  return new Triangle(material, newVertex1, newVertex2, newVertex3);
}


int Triangle::intersect(const Ray &ray, Intercept intercepts[]) const {
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

      if ((u - vertex1.y) * (vertex2.z - vertex1.z) >
          (v - vertex1.z) * (vertex2.y - vertex1.y)) {
        return 0;
      }
      if ((u - vertex2.y) * (vertex3.z - vertex2.z) >
          (v - vertex2.z) * (vertex3.y - vertex2.y)) {
        return 0;
      }
      if ((u - vertex3.y) * (vertex1.z - vertex3.z) >
          (v - vertex3.z) * (vertex1.y - vertex3.y)) {
        return 0;
      }

      break;

    case 1:  // y dominant, normal.y > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - vertex1.x) * (vertex2.z - vertex1.z) <
          (v - vertex1.z) * (vertex2.x - vertex1.x)) {
        return 0;
      }
      if ((u - vertex2.x) * (vertex3.z - vertex2.z) <
          (v - vertex2.z) * (vertex3.x - vertex2.x)) {
        return 0;
      }
      if ((u - vertex3.x) * (vertex1.z - vertex3.z) <
          (v - vertex3.z) * (vertex1.x - vertex3.x)) {
        return 0;
      }

      break;

    case 2:  // z dominant, normal.z > 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - vertex1.x) * (vertex2.y - vertex1.y) >
          (v - vertex1.y) * (vertex2.x - vertex1.x)) {
        return 0;
      }
      if ((u - vertex2.x) * (vertex3.y - vertex2.y) >
          (v - vertex2.y) * (vertex3.x - vertex2.x)) {
        return 0;
      }
      if ((u - vertex3.x) * (vertex1.y - vertex3.y) >
          (v - vertex3.y) * (vertex1.x - vertex3.x)) {
        return 0;
      }

      break;

    case 3:  // x dominant, normal.x < 0
      u = ray.orig.y + t * ray.dir.y;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - vertex1.y) * (vertex2.z - vertex1.z) <
          (v - vertex1.z) * (vertex2.y - vertex1.y)) {
        return 0;
      }
      if ((u - vertex2.y) * (vertex3.z - vertex2.z) <
          (v - vertex2.z) * (vertex3.y - vertex2.y)) {
        return 0;
      }
      if ((u - vertex3.y) * (vertex1.z - vertex3.z) <
          (v - vertex3.z) * (vertex1.y - vertex3.y)) {
        return 0;
      }

      break;

    case 4:  // y dominant, normal.y < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.z + t * ray.dir.z;

      if ((u - vertex1.x) * (vertex2.z - vertex1.z) >
          (v - vertex1.z) * (vertex2.x - vertex1.x)) {
        return 0;
      }
      if ((u - vertex2.x) * (vertex3.z - vertex2.z) >
          (v - vertex2.z) * (vertex3.x - vertex2.x)) {
        return 0;
      }
      if ((u - vertex3.x) * (vertex1.z - vertex3.z) >
          (v - vertex3.z) * (vertex1.x - vertex3.x)) {
        return 0;
      }

      break;

    case 5:  // z dominant, normal.z < 0
      u = ray.orig.x + t * ray.dir.x;
      v = ray.orig.y + t * ray.dir.y;

      if ((u - vertex1.x) * (vertex2.y - vertex1.y) <
          (v - vertex1.y) * (vertex2.x - vertex1.x)) {
        return 0;
      }
      if ((u - vertex2.x) * (vertex3.y - vertex2.y) <
          (v - vertex2.y) * (vertex3.x - vertex2.x)) {
        return 0;
      }
      if ((u - vertex3.x) * (vertex1.y - vertex3.y) <
          (v - vertex3.y) * (vertex1.x - vertex3.x)) {
        return 0;
      }

      break;
  }

  intercepts[0] = Intercept(t, normalDotDir < 0, ray.medium, this);
  return 1;
}


std::vector<P_FLT> Triangle::inverseMap(const Point3D &point) const {
  std::vector<P_FLT> mapping(3, 0.0f);
  Vector3D v12 = vertex2 - vertex1,
           v13 = vertex3 - vertex1,
           vP1 = point - vertex1;
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


void Triangle::transform(Transform * transform) {
  transform->transformPoint(vertex1, &vertex1);
  transform->transformPoint(vertex2, &vertex2);
  transform->transformPoint(vertex3, &vertex3);
}


void TexturedTriangle::getIntersect(const Point3D &point, Vector3D * normal,
                                    std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = this->normal;
}


Color TexturedTriangle::getTexColor(const std::vector<P_FLT> mapping) const {
  Vector2D texCoord = vertexTexCoord1 * mapping[0] +
                      vertexTexCoord2 * mapping[1] +
                      vertexTexCoord3 * mapping[2];

  return texture->colorAt(texCoord);
}


Primitive * TexturedTriangle::interpolate(Primitive * primitive,
                                          const P_FLT ratio) const {
  TexturedTriangle * triangle = static_cast<TexturedTriangle *>(primitive);
  P_FLT ratioSub = 1.0f - ratio;

  Point3D newVertex1 = vertex1 * ratioSub + triangle->vertex1 * ratio,
          newVertex2 = vertex2 * ratioSub + triangle->vertex2 * ratio,
          newVertex3 = vertex3 * ratioSub + triangle->vertex3 * ratio;
  Vector2D newTexCoord1 = vertexTexCoord1 * ratioSub +
                          triangle->vertexTexCoord1 * ratio,
           newTexCoord2 = vertexTexCoord2 * ratioSub +
                          triangle->vertexTexCoord2 * ratio,
           newTexCoord3 = vertexTexCoord3 * ratioSub +
                          triangle->vertexTexCoord3 * ratio;

  return new TexturedTriangle(material, texture,
                              newVertex1, newVertex2, newVertex3,
                              newTexCoord1, newTexCoord2, newTexCoord3);
}


void TrianglePatch::getIntersect(const Point3D &point, Vector3D * normal,
                                 std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = vertexNormal1 * (*mapping)[0] +
            vertexNormal2 * (*mapping)[1] +
            vertexNormal3 * (*mapping)[2];
}


Primitive * TrianglePatch::interpolate(Primitive * primitive,
                                       const P_FLT ratio) const {
  TrianglePatch * triangle = static_cast<TrianglePatch *>(primitive);
  P_FLT ratioSub = 1.0f - ratio;

  Point3D newVertex1 = vertex1 * ratioSub + triangle->vertex1 * ratio,
          newVertex2 = vertex2 * ratioSub + triangle->vertex2 * ratio,
          newVertex3 = vertex3 * ratioSub + triangle->vertex3 * ratio;
  Vector3D newNormal1 = vertexNormal1 * ratioSub +
                        triangle->vertexNormal1 * ratio,
           newNormal2 = vertexNormal2 * ratioSub +
                        triangle->vertexNormal2 * ratio,
           newNormal3 = vertexNormal3 * ratioSub +
                        triangle->vertexNormal3 * ratio;

  return new TrianglePatch(material, newVertex1, newVertex2, newVertex3,
                           vertexNormal1, vertexNormal2, vertexNormal3);
}


void TrianglePatch::transform(Transform * transform) {
  Triangle::transform(transform);
  transform->transformNormal(vertexNormal1, &vertexNormal1);
  transform->transformNormal(vertexNormal2, &vertexNormal2);
  transform->transformNormal(vertexNormal3, &vertexNormal3);
}


void PhongTriangle::getIntersect(const Point3D &point, Vector3D * normal,
                                 std::vector<P_FLT> * mapping) const {
  *mapping = inverseMap(point);
  *normal = vertexNormal1 * (*mapping)[0] +
            vertexNormal2 * (*mapping)[1] +
            vertexNormal3 * (*mapping)[2];
}


Color PhongTriangle::getTexColor(const std::vector<P_FLT> mapping) const {
  Vector2D texCoord = vertexTexCoord1 * mapping[0] +
                      vertexTexCoord2 * mapping[1] +
                      vertexTexCoord3 * mapping[2];

  return texture->colorAt(texCoord);
}


Primitive * PhongTriangle::interpolate(Primitive * primitive,
                                       const P_FLT ratio) const {
  PhongTriangle * triangle = static_cast<PhongTriangle *>(primitive);
  P_FLT ratioSub = 1.0f - ratio;

  Point3D newVertex1 = vertex1 * ratioSub + triangle->vertex1 * ratio,
          newVertex2 = vertex2 * ratioSub + triangle->vertex2 * ratio,
          newVertex3 = vertex3 * ratioSub + triangle->vertex3 * ratio;
  Vector3D newNormal1 = vertexNormal1 * ratioSub +
                        triangle->vertexNormal1 * ratio,
           newNormal2 = vertexNormal2 * ratioSub +
                        triangle->vertexNormal2 * ratio,
           newNormal3 = vertexNormal3 * ratioSub +
                        triangle->vertexNormal3 * ratio;
  Vector2D newTexCoord1 = vertexTexCoord1 * ratioSub +
                          triangle->vertexTexCoord1 * ratio,
           newTexCoord2 = vertexTexCoord2 * ratioSub +
                          triangle->vertexTexCoord2 * ratio,
           newTexCoord3 = vertexTexCoord3 * ratioSub +
                          triangle->vertexTexCoord3 * ratio;

  return new PhongTriangle(material, texture,
                           newVertex1, newVertex2, newVertex3,
                           vertexNormal1, vertexNormal2, vertexNormal3,
                           newTexCoord1, newTexCoord2, newTexCoord3);
}


void PhongTriangle::transform(Transform * transform) {
  Triangle::transform(transform);
  transform->transformNormal(vertexNormal1, &vertexNormal1);
  transform->transformNormal(vertexNormal2, &vertexNormal2);
  transform->transformNormal(vertexNormal3, &vertexNormal3);
}


END_RAYTRACER
