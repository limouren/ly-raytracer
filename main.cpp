#include <stdio.h>
#include <string>

#include "config.h"

#include "bounding_volume.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "light.cpp"
#include "math_util.cpp"
#include "mesh.cpp"
#include "model.cpp"
#include "parser.cpp"
#include "point.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "shapes.cpp"
#include "trace.cpp"
#include "triangle.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


int main(char * inputFilename, char * outputFilename) {
  Camera camera;
  Screen screen;

  parseFile(inputFilename, &scene, &camera, &screen, NULL);

  rayTrace(scene, camera, screen);

  screen.saveBmp(outputFilename);

  return 0;
}


END_RAYTRACER


using namespace std;


int main(int argc, char * argv[]) {
  if (argc != 3) {
    printf("Usage: raytracer <input filename> <output filename>\n");
    return 1;
  }

  RAYTRACER_NAMESPACE::main(argv[1], argv[2]);
  return 0;
}
