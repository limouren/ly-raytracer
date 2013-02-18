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
#include "model.cpp"
#include "parser.cpp"
#include "point.cpp"
#include "ray.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "shapes.cpp"
#include "texture.cpp"
#include "trace.cpp"
#include "transform.cpp"
#include "triangle.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


int main(char * inputFilename, char * outputFilename) {
  Camera * camera;

  printf("Parsing file \"%s\"...", inputFilename);
  fflush(stdout);
  parseFile(inputFilename, &scene, &camera, NULL);
  printf("completed.\n");
  scene.buildModelTree();
  scene.loadTextures();

  Screen screen(camera);
  screen.rayTrace(scene, camera);

  printf("Saving file to \"%s\"...", outputFilename);
  fflush(stdout);
  screen.saveBmp(outputFilename);
  printf("completed.\n");

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
