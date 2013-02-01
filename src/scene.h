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
      ambience = Color(0.4, 0.4, 0.4);
    }

    void init_background() {
      std::string fileName = "img/background.bmp";
      background = new Background(fileName, 0, 0, 0.0);
    }

    void init_lights() {
      Light * l1 = new Light(Point3D(-4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * l2 = new Light(Point3D(4.5, 2.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * l3 = new Light(Point3D(2.0, 2.0, 0.0), Color(1.0, 1.0, 1.0));
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
                                    Coeff(0.1, 0.1, 0.1),
                                    Coeff(0.6, 0.1, 0.1),
                                    Coeff(0.6, 0.1, 0.1),
                                    Coeff(0.0, 0.0, 0.0),
                                    1.5,
                                    25),
               * green = new Material(std::string("Green"),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.6, 0.1),
                                      Coeff(0.1, 0.6, 0.1),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      25),
               * blue = new Material(std::string("Blue"),
                                     Coeff(0.1, 0.1, 0.1),
                                     Coeff(0.1, 0.1, 0.6),
                                     Coeff(0.1, 0.1, 0.6),
                                     Coeff(0.0, 0.0, 0.0),
                                     1.5,
                                     25),
               * roughWhite = new Material(std::string("Rough white"),
                                      Coeff(0.3, 0.3, 0.3),
                                      Coeff(0.4, 0.4, 0.4),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      10),
               * black = new Material(std::string("Black"),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.1, 0.1, 0.1),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.5,
                                      25),
               * glass = new Material(std::string("Glass"),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(1.0, 1.0, 1.0),
                                      1.5,
                                      2.5),
               * mirror = new Material(std::string("Mirror"),
                                       Coeff(0.05, 0.05, 0.05),
                                       Coeff(0.05, 0.05, 0.05),
                                       Coeff(0.9, 0.9, 0.9),
                                       Coeff(0.0, 0.0, 0.0),
                                       1.0,
                                       20000.0);

      Surface * sp1 = static_cast<Surface *>(new Sphere(-1.5, 0.0, 5.0, 0.5)),
              * sp2 = static_cast<Surface *>(new Sphere(1.5, 0.0, 5.0, 0.5)),
              * sp3 = static_cast<Surface *>(new Sphere(0.0, 1.0, 5.0, 0.5)),
              * sp4 = static_cast<Surface *>(new Sphere(0.0, 0.0, 6.0, 0.5)),
              * sp5 = static_cast<Surface *>(new Sphere(0.0, -0.3, 3.5, 0.3));

      Point3D octagon[8] = {Point3D(-1.0, -0.5, 5.0),
                            Point3D(-1.0, 0.5, 5.0),
                            Point3D(-0.5, 1.0, 5.0),
                            Point3D(0.5, 1.0, 5.0),
                            Point3D(1.0, 0.5, 5.0),
                            Point3D(1.0, -0.5, 5.0),
                            Point3D(0.5, -1.0, 5.0),
                            Point3D(-0.5, -1.0, 5.0)};
      Surface * o1 = static_cast<Surface *>(new Polygon(8, octagon));

      Point3D leftWall[4] = {Point3D(4.0, 1.5, 15.0),
                             Point3D(4.0, 1.5, 0.0),
                             Point3D(4.0, -1.5, 0.0),
                             Point3D(4.0, -1.5, 15.0)},
              rightWall[4] = {Point3D(-4.0, -1.5, 15.0),
                              Point3D(-4.0, -1.5, 0.0),
                              Point3D(-4.0, 1.5, 0.0),
                              Point3D(-4.0, 1.5, 15.0)},
              backWall[4] = {Point3D(-4.0, 1.5, 15.0),
                             Point3D(4.0, 1.5, 15.0),
                             Point3D(4.0, -1.5, 15.0),
                             Point3D(-4.0, -1.5, 15.0)},
              floor[4] = {Point3D(-4.0, -1.5, 0.0),
                          Point3D(-4.0, -1.5, 15.0),
                          Point3D(4.0, -1.5, 15.0),
                          Point3D(4.0, 1.5, 0.0)};
      Surface * p1 = static_cast<Surface *>(new Polygon(4, leftWall)),
              * p2 = static_cast<Surface *>(new Polygon(4, rightWall)),
              * p3 = static_cast<Surface *>(new Polygon(4, backWall)),
              * p4 = static_cast<Surface *>(new Polygon(4, floor));

      // Balls
      MODEL_CLS * b1 = static_cast<MODEL_CLS *>(new Primitive(red, sp1)),
                * b2 = static_cast<MODEL_CLS *>(new Primitive(green, sp2)),
                * b3 = static_cast<MODEL_CLS *>(new Primitive(blue, sp3)),
                * b4 = static_cast<MODEL_CLS *>(new Primitive(mirror, sp4)),
                * b5 = static_cast<MODEL_CLS *>(new Primitive(glass, sp5));

      // Walls
      MODEL_CLS * w1 = static_cast<MODEL_CLS *>(new Primitive(roughWhite, p1)),
                * w2 = static_cast<MODEL_CLS *>(new Primitive(roughWhite, p2)),
                * w3 = static_cast<MODEL_CLS *>(new Primitive(roughWhite, p3));

      // Floor
      MODEL_CLS * f1 = static_cast<MODEL_CLS *>(new Primitive(roughWhite, p4));

      MODEL_CLS * backNFloor = static_cast<MODEL_CLS *>(new Composite(w3, f1)),
                * sideWalls = static_cast<MODEL_CLS *>(new Composite(w1, w2)),
                * walls = static_cast<MODEL_CLS *>(new Composite(backNFloor,
                                                                 sideWalls)),
                * balls1 = static_cast<MODEL_CLS *>(new Composite(b1, b2)),
                * balls2 = static_cast<MODEL_CLS *>(new Composite(b3, b4)),
                * balls3 = static_cast<MODEL_CLS *>(new Composite(balls1, b5)),
                * balls = static_cast<MODEL_CLS *>(new Composite(balls3,
                                                                 balls2)),
                * root = static_cast<MODEL_CLS *>(new Composite(walls,
                                                                balls));

      modelRoot = root;
    }

    Scene(): background(NULL) {
      init_ambience();
      // init_background();
      init_lights();
      init_medium();
      init_models_and_materials();
    }
} scene;


END_RAYTRACER


#endif
