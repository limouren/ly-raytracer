#ifndef SCENE_H
#define SCENE_H


#include <vector>

#include "config.h"

#include "light.h"
#include "material.h"
#include "surface.h"
#include "solid.h"


BEGIN_RAYTRACER


class Scene {
  public:
    std::vector<Light * > lights;
    std::vector<Material * > materials;
    Solid * modelRoot;

    void init_lights() {
      Light * light1 = new Light();
      light1->orig = Point(1.0, 1.0, 0.0);
      light1->color = Color(1.0, 0.0, 0.0);
      light1->intensity = 1.0;
      lights.push_back(light1);

      Light * light2 = new Light();
      light2->orig = Point(0.0, -1.414, 0.0);
      light2->color = Color(0.0, 0.0, 1.0);
      light2->intensity = 1.0;
      lights.push_back(light2);

      Light * light3 = new Light();
      light3->orig = Point(-1.0, 1.0, 0.0);
      light3->color = Color(0.0, 1.0, 0.0);
      light3->intensity = 1.0;
      lights.push_back(light3);
    }

    void init_solids_and_materials() {
      Primitive * ball = new Primitive();

      Material * red_plastic = new Material(0.7, 0.1, 0.0,
                                            Color(0.0, 0.0, 0.0), 0.0);
      materials.push_back(red_plastic);
      ball->material = red_plastic;

      Sphere * sphere = new Sphere(0.0, 0.0, 5.0, 2.0);
      ball->surface = (Surface *)sphere;

      modelRoot = (Solid *)ball;
    }

    Scene() {
      init_lights();
      init_solids_and_materials();
    }
} scene;


END_RAYTRACER


#endif
