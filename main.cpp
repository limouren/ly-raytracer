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
#include "point.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "shapes.cpp"
#include "trace.cpp"
#include "triangle.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


int main(std::string outputFilename) {
  Screen screen = Screen(outputFilename);
  screen.imageHeight = 600;
  screen.imageWidth = 800;

  screen.init();
  screen.rayTrace();
  screen.saveBmp();

  return 0;
}


END_RAYTRACER


using namespace std;


int main(int argc, char * argv[]) {
  if (argc != 2) {
    printf("Usage: raytracer <output filename>\n");
    return 1;
  }

  string outputFilename(argv[1]);

  RAYTRACER_NAMESPACE::main(outputFilename);
  return 0;
}
