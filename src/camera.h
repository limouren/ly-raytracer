#ifndef CAMERA_H
#define CAMERA_H


#include "config.h"

#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


class Camera {
  public:
    P_FLT angle;
    P_FLT aspect_ratio;
    Point target;
    Point viewpoint;
    Vector up;

    Camera(Point viewpoint, Point target, P_FLT angle):
      angle(angle),
      aspect_ratio(DEFAULT_ASPECT_RATIO),
      target(target),
      viewpoint(viewpoint),
      up(Vector(0.0, 0.0, 1.0))
      {}
};


END_RAYTRACER


#endif
