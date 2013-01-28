#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <string>
#include <vector>

#include "bitmap_image.hpp"

#include "config.h"

#include "camera.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "surface.h"


BEGIN_RAYTRACER


class Background {
  public:
    int height, width,
        orig_x, orig_y;

    P_FLT angle;

    Color * pixels;

    Background(std::string fileName, int orig_x, int orig_y, P_FLT angle):
      orig_x(orig_x), orig_y(orig_y), angle(angle) {
      bitmap_image image(fileName);

      height = image.height();
      width = image.width();

      int pixel_count = height * width;

      pixels = new Color[pixel_count];
      C_FLT * red_channel = new C_FLT[pixel_count];
      C_FLT * green_channel = new C_FLT[pixel_count];
      C_FLT * blue_channel = new C_FLT[pixel_count];

      image.export_rgb(red_channel, green_channel, blue_channel);
      for (int i = 0; i < pixel_count; i++) {
        pixels[i] = Color(red_channel[i], green_channel[i], blue_channel[i]);
      }

      delete [] red_channel;
      delete [] green_channel;
      delete [] blue_channel;
    }


    const Color interpolateBackground(const P_FLT x, const P_FLT y) const {
      P_FLT ratio_x, ratio_y,
            x_floor = floor(x),
            y_floor = floor(y);

      int l = (int) x_floor,
          r = l + 1,
          t = (int) y_floor,
          b = t + 1;

      ratio_x = x - x_floor;
      ratio_y = y - y_floor;

      Color result = pixels[t * width + l] * ratio_x * ratio_y +
                     pixels[t * width + r] * (1 - ratio_x) * ratio_y +
                     pixels[b * width + l] * ratio_x * (1 - ratio_y) +
                     pixels[b * width + r] * (1 - ratio_x) * (1 - ratio_y);
      return result;
    }


    const Color colorInDirection(const Vector &direction) const {
      P_FLT latitude, longitude, x, y;

      latitude = acos(-dotProduct(camera.up, direction));
      longitude = acos(dotProduct(camera.forward, direction) / sin(latitude));
      if (dotProduct(crossProduct(direction, camera.forward), camera.up) > 0) {
        longitude = 1 - longitude;
      }

      x = longitude * (P_FLT) width;
      y = latitude * (P_FLT) height / (2 * PI);

      return interpolateBackground(x, y);
    }


    ~Background() {
      delete [] pixels;
    }
};


class Scene {
  public:
    Color ambience;
    Background * background;
    std::vector<Light * > lights;
    MODEL_CLS * modelRoot;

    // TODO: Change these to parsing instead of hardcoded tests
    void init_ambience() {
      ambience = Color(0.2, 0.2, 0.2);
    }

    void init_background() {
      std::string fileName = "img/background.bmp";
      background = new Background(fileName, 0, 0, 0.0);
    }

    void init_lights() {
      Light * light1 = new Light(Point(-5.5, 5.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light2 = new Light(Point(5.5, 5.5, 0.0), Color(1.0, 1.0, 1.0));
      Light * light3 = new Light(Point(0.0, 5.0, 0.0), Color(0.5, 0.5, 0.5));
      Light * light4 = new Light(Point(3.0, 3.0, 0.0), Color(1.0, 1.0, 1.0));

      lights.push_back(light1);
      lights.push_back(light2);
      //lights.push_back(light3);
      //lights.push_back(light4);
    }

    void init_models_and_materials() {
      Material * red = new Material(std::string("Red Plastic"),
                                    Coeff(0.2, 0.2, 0.2),
                                    Coeff(0.8, 0.2, 0.2),
                                    Coeff(0.8, 0.2, 0.2),
                                    Coeff(0.0, 0.0, 0.0),
                                    1.0,
                                    20);
      Material * blue = new Material(std::string("Blue Plastic"),
                                     Coeff(0.2, 0.2, 0.2),
                                     Coeff(0.2, 0.2, 0.8),
                                     Coeff(0.2, 0.2, 0.8),
                                     Coeff(0.0, 0.0, 0.0),
                                     1.0,
                                     20);
      Material * white = new Material(std::string("White Plastic"),
                                      Coeff(0.2, 0.2, 0.2),
                                      Coeff(0.8, 0.8, 0.8),
                                      Coeff(0.8, 0.8, 0.8),
                                      Coeff(0.0, 0.0, 0.0),
                                      1.0,
                                      20);
      Material * glass = new Material(std::string("Glass"),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(0.0, 0.0, 0.0),
                                      Coeff(1.0, 1.0, 1.0),
                                      2.0,
                                      2.5);

      Surface * sphere1 = (Surface *) new Sphere(-1.5, 0.0, 7.0, 1.0);
      Surface * sphere2 = (Surface *) new Sphere(1.5, 0.0, 7.0, 1.0);
      Surface * sphere3 = (Surface *) new Sphere(0.0, 0.0, 0.0, 1.0);
      Surface * sphere4 = (Surface *) new Sphere(0.0, 0.0, 3.0, 0.5);

      MODEL_CLS * ball1 = (MODEL_CLS *) new Primitive(red, sphere1);
      MODEL_CLS * ball2 = (MODEL_CLS *) new Primitive(blue, sphere2);
      MODEL_CLS * ball3 = (MODEL_CLS *) new Primitive(white, sphere3);
      MODEL_CLS * ball4 = (MODEL_CLS *) new Primitive(glass, sphere4);

      MODEL_CLS * b12 = (MODEL_CLS *) new Composite(ball1, ball2);
      MODEL_CLS * root = (MODEL_CLS *) new Composite(b12, ball4);

      modelRoot = root;
    }

    Scene() {
      init_ambience();
      init_background();
      init_lights();
      init_models_and_materials();
    }
} scene;


END_RAYTRACER


#endif
