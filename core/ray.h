#ifndef RAY_H
#define RAY_H

#include "config.h"
#include "vector.h"


BEGIN_RAYTRACER


class Ray {
  public:
    Vector orig, dir;

    Ray() {
      orig = Vector();
      dir = Vector();
    }

    Ray(Vector origin, Vector direction) {
      orig = origin;
      dir = direction;
    }

    Ray& copy() {
      return Ray(orig.copy(), dir.copy());
    }

    P_FLT dot(const Vector &vec) {
      return dir.dot(&vec);
    }

    P_FLT normalize() {
      return dir.normalize();
    };
};


END_RAYTRACER


#endif
