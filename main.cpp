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


int main(char * inputFilename, char * outputFilename) {
  Scene scene;
  scene.init(inputFilename);

  Screen screen(&scene);
  P_FLT time;
  char filename[1024];
  for (int i = 1; i <= 150; i++) {
    time = (i - 1) * (1.0f / 30.0f);
    screen.rayTrace(time);

    snprintf(filename, strlen(outputFilename) + 10,
             "%s%.3d.bmp", outputFilename, i);
    screen.saveBmp(filename);
  }

  return 0;
}


END_RAYTRACER


using namespace std;


int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("Usage: raytracer <input filename> <output filename>\n");
    return 1;
  }

  // TODO(kent): Modify aaLevel, threadNum with command line params

  RAYTRACER_NAMESPACE::main(argv[1], argv[2]);
  return 0;
}
