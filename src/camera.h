#ifndef CAMERA_H
#define CAMERA_H


#include <math.h>

#include "config.h"

#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


class Camera {
  private:
    void computeForward() {
      Vector3D dir = target - viewpoint;
      dir.normalize();
      P_FLT dirDotUp = dotProduct(dir, up);

      P_FLT root = 1 / sqrt(1 - dirDotUp * dirDotUp);
      forward = up * dirDotUp * root + dir * root;
    }

  public:
    P_FLT angle;  // Width of view angle
    P_FLT aspect_ratio;
    Point3D target;
    Point3D viewpoint;
    Vector3D forward, up;

#if DEBUG
    Camera() {
      // TODO(kent): Don't hardcode camera
      angle = PI * 0.25;
      aspect_ratio = 1.333;
      target = Point3D(0.0, 0.0, 4.0);
      viewpoint = Point3D(0.0, 1.0, 0.0);
      up = Vector3D(0.0, 1.0, 0.0);
    }
#endif

    Camera(Point3D viewpoint, Point3D target, P_FLT angle):
      angle(angle),
      aspect_ratio(DEFAULT_ASPECT_RATIO),
      target(target),
      viewpoint(viewpoint),
      up(Vector3D(0.0, 1.0, 0.0))
      {}
    Camera(Point3D viewpoint, Point3D target, P_FLT angle, P_FLT aspect_ratio,
           Vector3D up):
      angle(angle),
      aspect_ratio(aspect_ratio),
      target(target),
      viewpoint(viewpoint),
      up(up) {
      up.normalize();
    }
} camera;


END_RAYTRACER


#endif
