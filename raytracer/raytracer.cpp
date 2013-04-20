#include <stdio.h>
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

    // TODO(kent): Expose aaLevel, threadNum
    RayTracer(char * inputFilename) {
      scene.init(inputFilename);
      Screen screen(&scene);

      // TODO(kent): Build scene for some other time
      scene.buildNodeTree(0.0f);
    }
};


END_RAYTRACER
