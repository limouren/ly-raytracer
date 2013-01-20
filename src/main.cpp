#include "config.h"

#include "camera.cpp"
#include "point.cpp"
#include "solid.cpp"
#include "screen.cpp"
#include "surface.cpp"
#include "vector.cpp"


using namespace std;

BEGIN_RAYTRACER


Solid * ReadScene() {
  // TODO: read Scene and Camera from file
  string file_name = "models.txt";

  Primitive * ball = new Primitive();
  ball->material = new Material(0.3, 0.3, 0.0, Color(1.0, 0.0, 0.0), 0.0);
  Point center(0.0, 0.0, 0.0);
  Sphere * sphere = new Sphere(center, 1.0);
  ball->surface = (Surface *)(sphere);

  Solid * scene = (Solid *)ball;
  return scene;
}


int main() {
  Solid * scene = ReadScene();

  P_FLT angle = 1.570796;
  P_FLT aspect_ratio = 1.333;
  Point target(0.0, 0.0, 0.0);
  Point viewpoint(0.0, 0.0, 2.0);
  Vector up(0.0, 1.0, 0.0);

  Camera camera(viewpoint, target, angle, aspect_ratio, up);

  Screen screen(scene, camera);
}


END_RAYTRACER


using namespace std;

int main() {
  RAYTRACER_NAMESPACE::main();
}
