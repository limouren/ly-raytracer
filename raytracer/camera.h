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
    Point3D viewpoint;
    Vector3D forward, up, left;

    Camera(const Point3D &viewpoint, const Point3D &target,
           const Vector3D &up, const P_FLT angle, const P_FLT hither,
           const int imageWidth, const int imageHeight):
      imageHeight(imageHeight), imageWidth(imageWidth),
      angle(angle), aspectRatio(static_cast<P_FLT>(imageWidth) /
                                static_cast<P_FLT>(imageHeight)),
      hither(hither), viewpoint(viewpoint), up(up) {
      this->up.normalize();

      forward = target - viewpoint;
      forward.normalize();

      left = crossProduct(up, forward);
      left.normalize();
    }

    void move(char direction, P_FLT distance) {
      switch (direction) {
        case 'w':  // forward
          viewpoint += forward * distance;
          break;
        case 'a':  // leftward
          viewpoint += left * distance;
          break;
        case 's':  // backward
          viewpoint -= forward * distance;
          break;
        case 'r':  // rightward
          viewpoint -= left * distance;
          break;
      }
    }

    void turn(char direction, P_FLT angle) {
      P_FLT cosine = cos(angle),
            sine = sin(angle);
      switch (direction) {
        case '8':  // up
          sine = -sine;
        case '2':  // down
          forward = forward * cosine + crossProduct(left, forward) * sine +
                    left * dotProduct(left, forward) * (1.0f - cosine);
          forward.normalize();
          break;
        case '6':  // right
          sine = -sine;
        case '4':  // left
          forward = forward * cosine + crossProduct(up, forward) * sine +
                    up * dotProduct(up, forward) * (1.0f - cosine);
          forward.normalize();

          left = crossProduct(up, forward);
          left.normalize();
          break;
      }
    }
};


END_RAYTRACER


#endif
