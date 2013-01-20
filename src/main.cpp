#include "config.h"

#include "camera.cpp"
#include "point.cpp"
#include "solid.cpp"
#include "screen.cpp"
#include "vector.cpp"


BEGIN_RAYTRACER


Solid * ReadScene() {
  std::string file_name = "models.txt";

  Solid * scene = new Solid(1);

  return scene;
}


int main() {
  // TODO: read Scene and Camera from file
  Solid * scene = ReadScene();

  P_FLT angle = 1.570796;
  P_FLT aspect_ratio = 1.333;
  Point target = Point();
  Point viewport(0.0, 0.0, 5.0);
  Vector up(0.0, 1.0, 0.0);

  Camera camera(viewport, target, angle, aspect_ratio, up);

  Screen screen(*scene, camera);
}


END_RAYTRACER


using namespace std;

int main() {
  RAYTRACER_NAMESPACE::main();
}
