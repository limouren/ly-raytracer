#include <algorithm>
#include <vector>

#include "config.h"

#include "intercept.h"
#include "point.h"
#include "ray.h"
#include "transform.h"
#include "vector.h"


BEGIN_RAYTRACER


using namespace std;


void Primitive::buildBoundingBox() {
  printf("ERROR: Unimplemented Primitive::buildBoundingBox stub "
         "invoked\n");
  exit(1);
}


void Primitive::getIntersect(const Point3D &point, Vector3D * normal,
                             vector<P_FLT> * mapping) const {
  printf("ERROR: Unimplemented Primitive::getIntersect stub invoked\n");
  exit(1);
}


Color Primitive::getTexColor(const vector<P_FLT> mapping)
  const {
  printf("ERROR: Unimplemented Primitive::getTexCoord stub invoked\n");
  exit(1);
}


Primitive * Primitive::interpolate(Primitive * primitive,
                                   const P_FLT ratio) const {
  printf("ERROR: Unimplemented Primitive::interpolate stub invoked\n");
  exit(1);
}


int Primitive::intersect(const Ray &ray, Intercept intercepts[]) const {
  printf("ERROR: Unimplemented Primitive::intersect stub invoked\n");
  exit(1);
}


vector<P_FLT> Primitive::inverseMap(const Point3D &point) const {
  printf("ERROR: Unimplemented Primitive::inverseMap stub invoked\n");
  exit(1);
}


void Primitive::transform(Transform * transform) {
  printf("ERROR: Unimplemented Primitive::transform stub invoked\n");
  exit(1);
}


Primitive * TimedPrimitive::atTime(const P_FLT time) {
  while (currentFrame < frametime.size() &&
         time > frametime[currentFrame]) {
    currentFrame++;
  }

  if (currentFrame == frametime.size()) {
    return frame.back();
  } else if (currentFrame == 0) {
    return frame.front();
  } else {
    P_FLT ratio = (frametime[currentFrame] - time) /
                  (frametime[currentFrame] - frametime[currentFrame -1]);
    return frame[currentFrame]->interpolate(frame[currentFrame - 1], ratio);
  }
}


END_RAYTRACER
