#include "config.h"

#include "camera.cpp"
#include "color.cpp"
#include "geometry.cpp"
#include "light.cpp"
#include "math_util.cpp"
#include "model.cpp"
#include "point.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "trace.cpp"
#include "vector.cpp"
#include "volume.cpp"


BEGIN_RAYTRACER


void ReadScene() {
  // TODO: read Scene and Camera from file
  char file_name[80] = "models.txt";
}


int main() {
  ReadScene();

  Screen screen = Screen();
  screen.rayTrace();
  screen.saveBmp();

  return 0;
}


END_RAYTRACER


using namespace std;


int main() {
  RAYTRACER_NAMESPACE::main();
  return 0;
}
