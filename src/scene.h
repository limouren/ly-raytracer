#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <vector>

#include "bitmap_image.hpp"

#include "config.h"

#include "background.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "shapes.h"
#include "triangle.h"


BEGIN_RAYTRACER


class Scene {
  public:
    Color ambience, backgroundColor;
    Background * background;
    Material * medium;
    std::vector<Light * > lights;
    std::vector<Material *> materials;
    MODEL_CLS * modelRoot;

    // TODO(kent): Change these to parsing instead of hardcoded tests
    void init_ambience() {
      ambience = Color(0.4, 0.4, 0.4);
    }

    void init_background() {
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
      medium = new Material("Vacuum",
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(1.0, 1.0, 1.0),
                            1.0,
                            0.0);
    }

    void init_models_and_materials() {
      Material * red = new Material("Red",
                                    Color(0.1, 0.1, 0.1),
                                    Color(0.6, 0.1, 0.1),
                                    Color(0.6, 0.1, 0.1),
                                    Color(0.0, 0.0, 0.0),
                                    1.5,
                                    25),
               * green = new Material("Green",
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.1, 0.6, 0.1),
                                      Color(0.1, 0.6, 0.1),
                                      Color(0.0, 0.0, 0.0),
                                      1.5,
                                      25),
               * blue = new Material("Blue",
                                     Color(0.1, 0.1, 0.1),
                                     Color(0.1, 0.1, 0.6),
                                     Color(0.1, 0.1, 0.6),
                                     Color(0.0, 0.0, 0.0),
                                     1.5,
                                     25),
               * roughWhite = new Material("Rough white",
                                      Color(0.3, 0.3, 0.3),
                                      Color(0.4, 0.4, 0.4),
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.0, 0.0, 0.0),
                                      1.5,
                                      10),
               * roughGray = new Material("Rough gray",
                                      Color(0.15, 0.15, 0.15),
                                      Color(0.2, 0.2, 0.2),
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.0, 0.0, 0.0),
                                      1.5,
                                      10),
               * black = new Material("Black",
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.1, 0.1, 0.1),
                                      Color(0.0, 0.0, 0.0),
                                      1.5,
                                      25),
               * glass = new Material("Glass",
                                      Color(0.0, 0.0, 0.0),
                                      Color(0.0, 0.0, 0.0),
                                      Color(0.0, 0.0, 0.0),
                                      Color(1.0, 1.0, 1.0),
                                      1.5,
                                      2.5),
               * mirror = new Material("Mirror",
                                       Color(0.05, 0.05, 0.05),
                                       Color(0.05, 0.05, 0.05),
                                       Color(0.9, 0.9, 0.9),
                                       Color(0.0, 0.0, 0.0),
                                       1.0,
                                       20000.0);
      materials.push_back(red);
      materials.push_back(green);
      materials.push_back(blue);
      materials.push_back(roughWhite);
      materials.push_back(roughGray);
      materials.push_back(glass);
      materials.push_back(mirror);

      MODEL_CLS * sp1 = new Sphere(red, -1.5, 0.0, 5.0, 0.5),
                * sp2 = new Sphere(green, 1.5, 0.0, 5.0, 0.5),
                * sp3 = new Sphere(blue, 0.0, 1.0, 5.0, 0.5),
                * sp4 = new Sphere(mirror, 0.0, 0.0, 6.0, 0.5),
                * sp5 = new Sphere(glass, 0.0, -0.3, 3.5, 0.3),
                * sp6 = new Sphere(roughGray, 0.0, 0.0, 0.0, -12.0);

      std::vector<Point3D> trianglePoints;
      trianglePoints.push_back(Point3D(0.0, 0.0, 5.0));
      trianglePoints.push_back(Point3D(-1.0, -0.5, 5.0));
      trianglePoints.push_back(Point3D(-1.0, 0.5, 5.0));
      trianglePoints.push_back(Point3D(-0.5, 1.0, 5.0));
      trianglePoints.push_back(Point3D(0.5, 1.0, 5.0));
      trianglePoints.push_back(Point3D(1.0, 0.5, 5.0));
      trianglePoints.push_back(Point3D(1.0, -0.5, 5.0));
      trianglePoints.push_back(Point3D(0.5, -1.0, 5.0));
      trianglePoints.push_back(Point3D(-0.5, -1.0, 5.0));
      std::vector<Vector3D> normals;
      std::vector<Vector2D> textureCoords;
      std::vector<int *> triangleDefs;
      for (int i = 0; i < 8; i++) {
        int * triangleDef = new int[3];
        triangleDef[0] = 0;
        triangleDef[1] = (i)%8+1;
        triangleDef[2] = (i+1)%8+1;
        triangleDefs.push_back(triangleDef);
      }

      MODEL_CLS * mesh = new TriangleMesh(red, trianglePoints, normals,
                                          textureCoords, triangleDefs);


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
      MODEL_CLS * p1 = new Polygon(roughWhite, 4, leftWall),
                * p2 = new Polygon(roughWhite, 4, rightWall),
                * p3 = new Polygon(roughWhite, 4, backWall),
                * p4 = new Polygon(roughWhite, 4, floor);

      // Balls
      std::vector<MODEL_CLS *> balls;
      balls.push_back(sp1),
      balls.push_back(sp2),
      balls.push_back(sp3),
      balls.push_back(sp4),
      balls.push_back(sp5),
      balls.push_back(sp6);

      std::vector<MODEL_CLS *> meshes;
      meshes.push_back(mesh);

      // Walls
      std::vector<MODEL_CLS *> walls;
      walls.push_back(p1),
      walls.push_back(p2),
      walls.push_back(p3);
      // Floor
      walls.push_back(p4);

      // Model list
      std::vector<MODEL_CLS *> models;
      // models.push_back(buildModelTree(balls));
      models.push_back(buildModelTree(meshes));
      models.push_back(buildModelTree(walls));

      modelRoot = buildModelTree(models);
    }

    Scene() {
      // init_ambience();
      // init_background();
      // init_lights();
      init_medium();
      // init_models_and_materials();
    }
} scene;


END_RAYTRACER


#endif
