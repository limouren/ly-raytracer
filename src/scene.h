#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <string>
#include <vector>

#include "bitmap_image.hpp"

#include "config.h"

#include "background.h"
#include "camera.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "surface.h"


BEGIN_RAYTRACER


class Scene {
  public:
    Color ambience;
    Background * background;
    Material * medium;
    std::vector<Light * > lights;
    MODEL_CLS * modelRoot;

    // TODO(kent): Change these to parsing instead of hardcoded tests
    void init_ambience() {
      ambience = Color(0.2, 0.2, 0.2);
    }

    void init_background() {
      std::string fileName = "img/background.bmp";
      background = new Background(fileName, 0, 0, 0.0);
    }

    void init_lights() {
      Light * light1 = new Light(Point3D(-4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light2 = new Light(Point3D(4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light3 = new Light(Point3D(0.0, 5.0, 0.0), Color(0.5, 0.5, 0.5));
      Light * light4 = new Light(Point3D(3.0, 3.0, 0.0), Color(1.0, 1.0, 1.0));

      lights.push_back(light1);
      lights.push_back(light2);
      // lights.push_back(light3);
      // lights.push_back(light4);
    }

    void init_medium() {
      medium = new Material(std::string("Vacuum"),
                            Coeff(0.0, 0.0, 0.0),
                            Coeff(0.0, 0.0, 0.0),
                            Coeff(0.0, 0.0, 0.0),
                            Coeff(1.0, 1.0, 1.0),
                            1.0,
                            0.0);
    }

    void init_models_and_materials() {
      Material * red = new Material(std::string("Red"),
                                    Coeff(0.2, 0.2, 0.2),
                                    Coeff(0.8, 0.2, 0.2),
                                    Coeff(0.8, 0.2, 0.2),
                                    Coeff(0.0, 0.0, 0.0),
                                    1.5,
                                    100);
      Material * blue = new Material(std::string("Blue"),
                                     Coeff(0.2, 0.2, 0.2),
                                     Coeff(0.2, 0.2, 0.8),
                                     Coeff(0.2, 0.2, 0.8),
                                     Coeff(0.0, 0.0, 0.0),
                                     1.5,
                                     100);
      Material * white = new Material(std::string("White"),
                                      Coeff(0.2, 0.2, 0.2),
                                      Coeff(0.8, 0.8, 0.8),
                                      Coeff(0.8, 0.8, 0.8),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      100);
      Material * black = new Material(std::string("Black"),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      100);
      Material * glass = new Material(std::string("Glass"),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(1.0, 1.0, 1.0),
                                      1.5,
                                      2.5);

      Surface * sp1 = static_cast<Surface *>(new Sphere(-1.5, 0.0, 5.0, 1.0));
      Surface * sp2 = static_cast<Surface *>(new Sphere(1.5, 0.0, 7.0, 1.0));
      Surface * sp3 = static_cast<Surface *>(new Sphere(0.0, 0.0, 0.0, 1.0));
      Surface * sp4 = static_cast<Surface *>(new Sphere(0.0, 0.0, 3.0, 0.5));

      MODEL_CLS * b1 = static_cast<MODEL_CLS *>(new Primitive(black, sp1));
      MODEL_CLS * b2 = static_cast<MODEL_CLS *>(new Primitive(black, sp2));
      MODEL_CLS * b3 = static_cast<MODEL_CLS *>(new Primitive(white, sp3));
      MODEL_CLS * b4 = static_cast<MODEL_CLS *>(new Primitive(glass, sp4));

      MODEL_CLS * b12 = static_cast<MODEL_CLS *>(new Composite(b1, b2));
      MODEL_CLS * root = static_cast<MODEL_CLS *>(new Composite(b12, b4));

      modelRoot = root;
    }

    Scene(): background(NULL) {
      init_ambience();
      init_background();
      init_lights();
      init_medium();
      init_models_and_materials();
    }
} scene;


END_RAYTRACER


#endif
