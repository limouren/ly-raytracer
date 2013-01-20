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
    std::vector<Solid * > solids;

    void init_lights() {
      Light * light = new Light();

      Point orig(-5.0, 5.0, 0.0);
      light->orig = orig;
      Color color(1.0, 1.0, 1.0);
      light->color = color;
      C_FLT intensity = 0.5;
      light->intensity = intensity;

      lights.push_back(light);
    }

    void init_solids_and_materials() {
      Primitive * ball = new Primitive();

      Material * red_plastic = new Material(0.3, 0.3, 0.0,
                                            Color(1.0, 0.0, 0.0), 0.0);
      materials.push_back(red_plastic);
      ball->material = red_plastic;

      Point center(0.0, 0.0, 0.0);
      ball->surface = (Surface *)(new Sphere(center, 1.0));

      solids.push_back((Solid *)ball);
    }

    Scene() {
      init_lights();
      init_solids_and_materials();
    }
} scene;


END_RAYTRACER


#endif
