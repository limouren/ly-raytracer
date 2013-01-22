#include "config.h"

#include "camera.cpp"
#include "color.cpp"
#include "light.cpp"
#include "point.cpp"
#include "scene.cpp"
#include "screen.cpp"
#include "solid.cpp"
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

  P_FLT angle = 1.570796;
  P_FLT aspect_ratio = 1.333;
  Point target(0.0, 0.0, 1.0);
  Point viewpoint(0.0, 0.0, 0.0);
  Vector up(0.0, 1.0, 0.0);

  Camera camera(viewpoint, target, angle, aspect_ratio, up);

  Screen screen(camera);
  screen.save();

  return 0;
}


END_RAYTRACER


using namespace std;


int main() {
  RAYTRACER_NAMESPACE::main();
  return 0;
}
