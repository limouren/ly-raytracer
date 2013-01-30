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
      Light * light3 = new Light(Point3D(0.5, 2.0, 0.0), Color(1.0, 1.0, 1.0));
      Light * light4 = new Light(Point3D(3.0, 3.0, 0.0), Color(1.0, 1.0, 1.0));

      // lights.push_back(light1);
      // lights.push_back(light2);
      lights.push_back(light3);
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
      Material * mirror = new Material(std::string("Mirror"),
                                       Coeff(0.0, 0.0, 0.0),
                                       Coeff(0.0, 0.0, 0.0),
                                       Coeff(1.0, 1.0, 1.0),
                                       Coeff(0.0, 0.0, 0.0),
                                       1.5,
                                       20000.0);

      Surface * sp1 = static_cast<Surface *>(new Sphere(-1.5, 0.0, 5.0, 1.0)),
              * sp2 = static_cast<Surface *>(new Sphere(1.5, 0.0, 5.0, 1.0)),
              * sp3 = static_cast<Surface *>(new Sphere(0.0, 0.0, 3.5, 1.0)),
              * sp4 = static_cast<Surface *>(new Sphere(0.0, 0.0, 3.0, 0.5));

      Point3D octagon[8] = {Point3D(-1.0, -0.5, 5.0),
                            Point3D(-1.0, 0.5, 5.0),
                            Point3D(-0.5, 1.0, 5.0),
                            Point3D(0.5, 1.0, 5.0),
                            Point3D(1.0, 0.5, 5.0),
                            Point3D(1.0, -0.5, 5.0),
                            Point3D(0.5, -1.0, 5.0),
                            Point3D(-0.5, -1.0, 5.0)};
      Surface * p1 = static_cast<Surface *>(new Polygon(8, octagon));

      Point3D leftWall[4] = {Point3D(14.5, 15.0, 30.0),
                             Point3D(15.0, 15.0, 0.0),
                             Point3D(15.0, -15.1, 0.0),
                             Point3D(14.5, -15.1, 30.0)};
      Point3D rightWall[4] = {Point3D(-14.5, -15.1, 30.0),
                              Point3D(-15.0, -15.1, 0.0),
                              Point3D(-15.0, 15.0, 0.0),
                              Point3D(-14.5, 15.0, 30.0)};
      Surface * p2 = static_cast<Surface *>(new Polygon(4, leftWall));
      Surface * p3 = static_cast<Surface *>(new Polygon(4, rightWall));

      MODEL_CLS * b1 = static_cast<MODEL_CLS *>(new Primitive(red, sp1));
      MODEL_CLS * b2 = static_cast<MODEL_CLS *>(new Primitive(blue, sp2));
      MODEL_CLS * b3 = static_cast<MODEL_CLS *>(new Primitive(white, sp3));
      MODEL_CLS * b4 = static_cast<MODEL_CLS *>(new Primitive(glass, sp4));

      MODEL_CLS * w1 = static_cast<MODEL_CLS *>(new Primitive(red, p2));
      MODEL_CLS * w2 = static_cast<MODEL_CLS *>(new Primitive(blue, p3));

      MODEL_CLS * root = static_cast<MODEL_CLS *>(new Composite(w1, w2));

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
