#ifndef SHADE_H
#define SHADE_H

#include "config.h"

#include "color.h"
#include "intersection.h"
#include "light.h"
#include "material.h"
#include "point.h"
#include "scene.h"
#include "vector.h"


BEGIN_RAYTRACER


Color Shade(Point point, Vector normal, Vector incident,
            Intersection * intercepts) {
  return Color();
}


END_RAYTRACER


#endif
