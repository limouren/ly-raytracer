#ifndef CAMERA_H
#define CAMERA_H


#include <math.h>

#include "config.h"


class Point3D;
class Vector3D;

BEGIN_RAYTRACER


class Camera {
  public:
    int imageHeight, imageWidth;
    P_FLT angle,  // horizontal view angle
          aspectRatio,
          hither;
    Point3D target,
            viewpoint;
    Vector3D up;

    Camera(const Point3D &viewpoint, const Point3D &target,
           const Vector3D &up, const P_FLT angle, const P_FLT hither,
           const int imageWidth, const int imageHeight):
      imageHeight(imageHeight), imageWidth(imageWidth),
      angle(angle), aspectRatio(static_cast<P_FLT>(imageWidth) /
                                static_cast<P_FLT>(imageHeight)),
      hither(hither), target(target), viewpoint(viewpoint), up(up) {
      this->up.normalize();
    }
};


END_RAYTRACER


#endif
