#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H


#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "config.h"

#include "bounding_volume.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "intercept.cpp"
#include "light.cpp"
#include "material.cpp"
#include "math_util.cpp"
#include "mesh.cpp"
#include "mesh_triangle.cpp"
#include "parser.cpp"
#include "point.cpp"
#include "primitive.cpp"
#include "ray.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "shapes.cpp"
#include "texture.cpp"
#include "trace.cpp"
#include "transform.cpp"
#include "tree.cpp"
#include "triangle.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


class RayTracer {
  public:
    Scene scene;
    Screen * screen;

    // TODO(kent): Expose aaLevel, threadNum
    RayTracer(char * inputFilename) {
      scene.init(inputFilename);
      // TODO(kent): Don't hardcode time
      scene.buildNodeTree(0.0f);

      screen = new Screen(&scene);
    }

    ~RayTracer() {
      delete screen;
    }

    unsigned char * rayTrace() {
      // TODO(kent): Don't hardcode time
      screen->rayTrace(0.0f);

      int screenSize = screen->height * screen->width;
      unsigned char * data =
        (unsigned char *)(malloc(sizeof(unsigned char) * 3 * screenSize));

      unsigned char * dataPtr = &data[screenSize * 3];
      Color * colorPtr = screen->pixels;

      for (int i = 0; i < screenSize; i++, colorPtr++) {
        RGBColor rgbColor(*colorPtr);
        *(--dataPtr) = rgbColor.b;
        *(--dataPtr) = rgbColor.g;
        *(--dataPtr) = rgbColor.r;
      }

      return data;
    }
};


END_RAYTRACER


#endif
