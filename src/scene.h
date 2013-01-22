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
      Light * light1 = new Light(Point(0.5, 2.0, 0.0), Color(1.0, 1.0, 1.0), 1.0);
      //Light * light2 = new Light(Point(-0.5, 2.0, 0.0), Color(1.0, 1.0, 1.0), 1.0);
      lights.push_back(light1);
      //lights.push_back(light2);
    }

    void init_solids_and_materials() {
      Primitive * ball = new Primitive();

      Material * red_plastic = new Material(0.5, 0.5, 0.0,
                                            Color(1.0, 0.0, 0.0), 0.0);
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
