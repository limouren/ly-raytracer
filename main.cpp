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


int main(std::string inputFilename, std::string outputFilename) {
  Camera camera;
  camera.init(Point3D(0.0, 0.0, 0.0),
              Point3D(0.0, 0.0, 1.0),
              Vector3D(0.0, 1.0, 0.0),
              PI * 0.25,
              0.0,
              800,
              600);

  Screen screen;
  screen.init(camera);

  // parseFile(inputFilename, &scene, &camera, &screen);

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

  string inputFilename(argv[1]);
  string outputFilename(argv[2]);

  RAYTRACER_NAMESPACE::main(inputFilename, outputFilename);
  return 0;
}
