#ifndef SCREEN_H
#define SCREEN_H


#include <fstream>
#include "bitmap_image.hpp"

#include "config.h"

#include "camera.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "solid.h"
#include "trace.cpp"
#include "vector.h"


BEGIN_RAYTRACER


class Screen {
  public:
    Solid * scene;
    Camera camera;


    Screen(Solid * scene, Camera camera): scene(scene), camera(camera) {
      int height = 480;
      int width = 640;
      bitmap_image image(width, height);

      Vector dir = camera.target - camera.viewpoint;
      P_FLT horizontal_mag = dir.length() * cos(camera.angle * 0.5);
      dir.normalize();

      P_FLT vertical_mag = horizontal_mag / camera.aspect_ratio;
      Vector top = camera.up * vertical_mag;

      Vector right = crossProduct(dir, camera.up);
      right.normalize();
      right *= horizontal_mag;

      Point center = camera.viewpoint + dir;
      Point top_left = center + top - right;
      Vector i_step = right / (float)(width / 2);
      Vector j_step = top / (float)(height / 2) * -1.0;
      Vector top_left_pixel = top_left - camera.viewpoint +
                              (i_step * 0.5) + (j_step * 0.5);

      int hits = 0, total = 0;
      for (int i = 0;i < width;i++) {
        for (int j = 0;j < height;j++) {
          Vector ray_dir = top_left_pixel + (i_step * i) + (j_step * j);
          ray_dir.normalize();
          Ray ray(camera.viewpoint, ray_dir);

          Color * color = new Color();

          hits += trace(0, 1.0, scene, ray, color);
          total++;

          int r = int(color->r * 255 + 0.5);
          int g = int(color->g * 255 + 0.5);
          int b = int(color->b * 255 + 0.5);

          delete color;

          image.set_pixel(i, j, r, g, b);
        }
      }

      std::printf("Saving image, %d hits out of %d total\n", hits, total);
      image.save_image("out/output.bmp");
    }
};


END_RAYTRACER


#endif
