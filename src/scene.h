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
      Light * l1 = new Light(Point3D(-4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * l2 = new Light(Point3D(4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * l3 = new Light(Point3D(1.0, 1.0, 0.0), Color(1.0, 1.0, 1.0));
      Light * l4 = new Light(Point3D(3.0, 3.0, 0.0), Color(1.0, 1.0, 1.0));

      // lights.push_back(l1);
      // lights.push_back(l2);
      lights.push_back(l3);
      // lights.push_back(l4);
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
                                    Coeff(0.6, 0.2, 0.2),
                                    Coeff(0.6, 0.2, 0.2),
                                    Coeff(0.0, 0.0, 0.0),
                                    1.5,
                                    25);
      Material * blue = new Material(std::string("Blue"),
                                     Coeff(0.2, 0.2, 0.2),
                                     Coeff(0.2, 0.2, 0.6),
                                     Coeff(0.2, 0.2, 0.6),
                                     Coeff(0.0, 0.0, 0.0),
                                     1.5,
                                     25);
      Material * white = new Material(std::string("White"),
                                      Coeff(0.2, 0.2, 0.2),
                                      Coeff(0.6, 0.6, 0.6),
                                      Coeff(0.6, 0.6, 0.6),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      25);
      Material * black = new Material(std::string("Black"),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      25);
      Material * glass = new Material(std::string("Glass"),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(1.0, 1.0, 1.0),
                                      1.5,
                                      2.5);
      Material * mirror = new Material(std::string("Mirror"),
                                       Coeff(0.05, 0.05, 0.05),
                                       Coeff(0.05, 0.05, 0.05),
                                       Coeff(0.9, 0.9, 0.9),
                                       Coeff(0.0, 0.0, 0.0),
                                       1.0,
                                       20000.0);

      Surface * sp1 = static_cast<Surface *>(new Sphere(-1.5, 0.0, 5.0, 1.0)),
              * sp2 = static_cast<Surface *>(new Sphere(1.5, 0.0, 5.0, 1.0)),
              * sp3 = static_cast<Surface *>(new Sphere(0.0, 0.0, 3.5, 1.0)),
              * sp4 = static_cast<Surface *>(new Sphere(0.0, 0.0, 12.0, 0.5));

      Point3D octagon[8] = {Point3D(-1.0, -0.5, 5.0),
                            Point3D(-1.0, 0.5, 5.0),
                            Point3D(-0.5, 1.0, 5.0),
                            Point3D(0.5, 1.0, 5.0),
                            Point3D(1.0, 0.5, 5.0),
                            Point3D(1.0, -0.5, 5.0),
                            Point3D(0.5, -1.0, 5.0),
                            Point3D(-0.5, -1.0, 5.0)};
      Surface * p1 = static_cast<Surface *>(new Polygon(8, octagon));

      Point3D leftWall[4] = {Point3D(1.0, 1.0, 20.0),
                             Point3D(1.0, 1.0, 3.0),
                             Point3D(0.99, -1.1, 3.0),
                             Point3D(0.99, -1.1, 20.0)};
      Point3D rightWall[4] = {Point3D(-1.0, -1.1, 20.0),
                              Point3D(-1.0, -1.1, 3.0),
                              Point3D(-0.99, 1.0, 3.0),
                              Point3D(-0.99, 1.0, 20.0)};
      Point3D backWall[4] = {Point3D(-1.0, 1.0, 20.0),
                             Point3D(1.0, 1.0, 20.0),
                             Point3D(1.0, -1.0, 20.1),
                             Point3D(-1.0, -1.0, 20.1)};
      Surface * p2 = static_cast<Surface *>(new Polygon(4, leftWall));
      Surface * p3 = static_cast<Surface *>(new Polygon(4, rightWall));
      Surface * p4 = static_cast<Surface *>(new Polygon(4, backWall));

      MODEL_CLS * b1 = static_cast<MODEL_CLS *>(new Primitive(red, sp1));
      MODEL_CLS * b2 = static_cast<MODEL_CLS *>(new Primitive(blue, sp2));
      MODEL_CLS * b3 = static_cast<MODEL_CLS *>(new Primitive(white, sp3));
      MODEL_CLS * b4 = static_cast<MODEL_CLS *>(new Primitive(glass, sp4));
      MODEL_CLS * b5 = static_cast<MODEL_CLS *>(new Primitive(red, sp4));

      MODEL_CLS * w1 = static_cast<MODEL_CLS *>(new Primitive(mirror, p2));
      MODEL_CLS * w2 = static_cast<MODEL_CLS *>(new Primitive(mirror, p3));
      MODEL_CLS * w3 = static_cast<MODEL_CLS *>(new Primitive(white, p4));

      MODEL_CLS * mirrors = static_cast<MODEL_CLS *>(new Composite(w1, w2));
      MODEL_CLS * solids = static_cast<MODEL_CLS *>(new Composite(w3, b5));
      MODEL_CLS * root = static_cast<MODEL_CLS *>(new Composite(solids,
                                                                mirrors));

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
