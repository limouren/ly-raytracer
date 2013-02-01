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
#include "geometry.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "triangle.h"


BEGIN_RAYTRACER


class Scene {
  public:
    Color ambience, backgroundColor;
    Background * background;
    Material * medium;
    std::vector<Light * > lights;
    MODEL_CLS * modelRoot;

    // TODO(kent): Change these to parsing instead of hardcoded tests
    void init_ambience() {
      ambience = Color(0.4, 0.4, 0.4);
    }

    void init_background() {
      // std::string fileName = "img/background.bmp";
      // background = new Background(fileName, 0, 0, 0.0);

      backgroundColor = Color(0.6, 0.95, 0.95);
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
               * roughGray = new Material(std::string("Rough gray"),
                                      Coeff(0.15, 0.15, 0.15),
                                      Coeff(0.2, 0.2, 0.2),
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

      Geometry * sp1 = new Sphere(-1.5, 0.0, 5.0, 0.5),
               * sp2 = new Sphere(1.5, 0.0, 5.0, 0.5),
               * sp3 = new Sphere(0.0, 1.0, 5.0, 0.5),
               * sp4 = new Sphere(0.0, 0.0, 6.0, 0.5),
               * sp5 = new Sphere(0.0, -0.3, 3.5, 0.3),
               * sp6 = new Sphere(0.0, 0.0, 0.0, -12.0);

      Point3D octagon[8] = {Point3D(-1.0, -0.5, 5.0),
                            Point3D(-1.0, 0.5, 5.0),
                            Point3D(-0.5, 1.0, 5.0),
                            Point3D(0.5, 1.0, 5.0),
                            Point3D(1.0, 0.5, 5.0),
                            Point3D(1.0, -0.5, 5.0),
                            Point3D(0.5, -1.0, 5.0),
                            Point3D(-0.5, -1.0, 5.0)};
      Geometry * o1 = static_cast<Geometry *>(new Polygon(8, octagon));

      P_FLT piThird = PI * 0.33333;
      std::vector<Point3D> trianglePoints;
      trianglePoints.push_back(Point3D(0.0, 0.0, 5.0));
      for (int i = 0; i < 6; i++) {
        P_FLT angle = static_cast<P_FLT>(i) * piThird;
        trianglePoints.push_back(Point3D(cos(angle), sin(angle), 5.0));
      }
      std::vector<Triangle *> triangles;
      for (int i = 0; i < 6; i++) {
        triangles.push_back(new Triangle(trianglePoints[0],
                                         trianglePoints[i%6+1],
                                         trianglePoints[(i+1)%6+1]));
      }

      Point3D leftWall[4] = {Point3D(2.5, 1.5, 10.0),
                             Point3D(2.5, 1.5, 0.0),
                             Point3D(2.5, -1.5, 0.0),
                             Point3D(2.5, -1.5, 10.0)},
              rightWall[4] = {Point3D(-2.5, -1.5, 10.0),
                              Point3D(-2.5, -1.5, 0.0),
                              Point3D(-2.5, 1.5, 0.0),
                              Point3D(-2.5, 1.5, 10.0)},
              backWall[4] = {Point3D(-2.5, 1.5, 10.0),
                             Point3D(2.5, 1.5, 10.0),
                             Point3D(2.5, -1.5, 10.0),
                             Point3D(-2.5, -1.5, 10.0)},
              floor[4] = {Point3D(-2.5, -1.5, 0.0),
                          Point3D(-2.5, -1.5, 10.0),
                          Point3D(2.5, -1.5, 10.0),
                          Point3D(2.5, 1.5, 0.0)};
      Geometry * p1 = new Polygon(4, leftWall),
               * p2 = new Polygon(4, rightWall),
               * p3 = new Polygon(4, backWall),
               * p4 = new Polygon(4, floor);

      // Balls
      std::vector<MODEL_CLS *> balls;
      balls.push_back(new Primitive(red, sp1)),
      balls.push_back(new Primitive(green, sp2)),
      balls.push_back(new Primitive(blue, sp3)),
      balls.push_back(new Primitive(mirror, sp4)),
      balls.push_back(new Primitive(glass, sp5)),
      balls.push_back(new Primitive(roughGray, sp6));

      std::vector<MODEL_CLS *> pseudoMesh;
      pseudoMesh.push_back(new Primitive(red, triangles[0]));
      pseudoMesh.push_back(new Primitive(green, triangles[1]));
      pseudoMesh.push_back(new Primitive(blue, triangles[2]));
      pseudoMesh.push_back(new Primitive(red, triangles[3]));
      pseudoMesh.push_back(new Primitive(green, triangles[4]));
      pseudoMesh.push_back(new Primitive(blue, triangles[5]));

      // Walls
      std::vector<MODEL_CLS *> walls;
      walls.push_back(new Primitive(roughWhite, p1)),
      walls.push_back(new Primitive(roughWhite, p2)),
      walls.push_back(new Primitive(roughWhite, p3));
      // Floor
      walls.push_back(new Primitive(roughWhite, p4));

      // Model list
      std::vector<MODEL_CLS *> models;
      // models.push_back(buildModelTree(balls));
      models.push_back(buildModelTree(pseudoMesh));
      models.push_back(buildModelTree(walls));

      modelRoot = buildModelTree(models);
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
