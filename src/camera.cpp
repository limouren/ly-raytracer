#include "config.h"

#include "camera.h"


BEGIN_RAYTRACER


class Point3D;
class Vector3D;


void Camera::init(const Point3D &_viewpoint, const Point3D &_target,
          const Vector3D &_up, const P_FLT &_angle, const P_FLT &_hither,
          const int resolutionWidth, const int resolutionHeight) {
  angle = _angle;
  hither = _hither;
  target = _target;
  up = _up;
  viewpoint = _viewpoint;

  aspectRatio = static_cast<P_FLT>(resolutionWidth) /
                static_cast<P_FLT>(resolutionHeight);

  up.normalize();
  initForward();
}


END_RAYTRACER
