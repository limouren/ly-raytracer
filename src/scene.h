#ifndef SCENE_H
#define SCENE_H


#include <vector>

#include "config.h"

#include "color.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "surface.h"


BEGIN_RAYTRACER


class Scene {
  public:
    Color ambience;
    std::vector<Light * > lights;
    std::vector<Material * > materials;
    MODEL_CLS * modelRoot;

    void init_ambience() {
      ambience = Color(0.3, 0.3, 0.3);
    }

    void init_lights() {
      Light * light1 = new Light(Point(-1.5, 3.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light2 = new Light(Point(1.5, 3.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light3 = new Light(Point(0.0, 5.0, 0.0), Color(1.0, 1.0, 1.0));

      //lights.push_back(light1);
      //lights.push_back(light2);
      lights.push_back(light3);
    }

    void init_models_and_materials() {
      Material * red_plastic = new Material(Coeff(0.2, 0.2, 0.2),
                                            Coeff(0.8, 0.2, 0.2),
                                            Coeff(0.4, 0.4, 0.4),
                                            3.0);
      Material * blue_plastic = new Material(Coeff(0.2, 0.2, 0.2),
                                             Coeff(0.2, 0.2, 0.8),
                                             Coeff(0.4, 0.4, 0.4),
                                             3.0);
      Material * white_plastic = new Material(Coeff(0.2, 0.2, 0.2),
                                               Coeff(0.8, 0.8, 0.8),
                                               Coeff(0.4, 0.4, 0.4),
                                               3.0);
      materials.push_back(red_plastic);
      materials.push_back(blue_plastic);
      materials.push_back(white_plastic);

      Surface * sphere1 = (Surface *) new Sphere(-1.5, 0.0, 4.0, 1.0);
      Surface * sphere2 = (Surface *) new Sphere(1.5, 0.0, 4.0, 1.0);
      Surface * sphere3 = (Surface *) new Sphere(0.0, 0.0, 6.0, 1.0);

      MODEL_CLS * ball1 = (MODEL_CLS *) new Primitive(red_plastic, sphere1);
      MODEL_CLS * ball2 = (MODEL_CLS *) new Primitive(blue_plastic, sphere2);
      MODEL_CLS * ball3 = (MODEL_CLS *) new Primitive(white_plastic, sphere3);

      MODEL_CLS * node = (MODEL_CLS *) new Composite(ball1, ball2);
      MODEL_CLS * root = (MODEL_CLS *) new Composite(node, ball3);

      modelRoot = root;
    }

    Scene() {
      init_ambience();
      init_lights();
      init_models_and_materials();
    }
} scene;


END_RAYTRACER


#endif
