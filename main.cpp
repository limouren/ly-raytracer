#include "config.h"

#include "camera.cpp"
#include "color.cpp"
#include "light.cpp"
#include "model.cpp"
#include "point.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "shade.cpp"
#include "surface.cpp"
#include "trace.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


void ReadScene() {
  // TODO: read Scene and Camera from file
  char file_name[80] = "models.txt";
}


int main() {
  ReadScene();

  Screen screen = Screen();
  screen.rayTrace();
  screen.save();

  return 0;
}


END_RAYTRACER


using namespace std;


int main() {
  RAYTRACER_NAMESPACE::main();
  return 0;
}
