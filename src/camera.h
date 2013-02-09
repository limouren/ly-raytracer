#ifndef CAMERA_H
#define CAMERA_H


#include <math.h>

#include "config.h"

#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


class Camera {
  private:
    void initForward() {
      Vector3D dir = target - viewpoint;
      dir.normalize();
      P_FLT dirDotUp = dotProduct(dir, up);

      P_FLT root = 1.0f / sqrt(1.0f - dirDotUp * dirDotUp);
      forward = up * dirDotUp * root + dir * root;
    }

  public:
    int imageHeight,
        imageWidth;
    P_FLT angle,  // y-direction view angle
          aspectRatio,
          hither;
    Point3D target,
            viewpoint;
    Vector3D forward,
             up;

    Camera() {}

    void init(const Point3D &_viewpoint, const Point3D &_target,
              const Vector3D &_up, const P_FLT &_angle, const P_FLT &_hither,
              const int imageWidth, const int resolutionHeight);
};


END_RAYTRACER


#endif
