#ifndef RAY_H
#define RAY_H

#include "config.h"

#include "point.h"
#include "vector.h"
#include "material.h"


BEGIN_RAYTRACER


class Ray {
  public:
    Material * medium;
    Point3D orig;
    Vector3D dir, invDir;

    Ray() {}
    Ray(Point3D orig, Vector3D dir, Material * medium):
      orig(orig), dir(dir), medium(medium) {
      // Precompute dir component inverses for ray-box intersection
      if (!fIsZero(dir.x)) {
          invDir.x = 1.0f / dir.x;
      }
      if (!fIsZero(dir.y)) {
          invDir.y = 1.0f / dir.y;
      }
      if (!fIsZero(dir.x)) {
          invDir.z = 1.0f / dir.z;
      }
    }

    inline Point3D rayPoint(P_FLT t) const {
      return orig + (dir * t);
    }
};


END_RAYTRACER


#endif
