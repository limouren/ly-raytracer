#ifndef TRACE_H
#define TRACE_H


#include "config.h"

#include "color.h"
#include "ray.h"


BEGIN_RAYTRACER


Color trace(Ray ray) {
  return Color(0.5, 0.5, 0.5);
}


END_RAYTRACER


#endif
