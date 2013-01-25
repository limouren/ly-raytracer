#ifndef SCREEN_H
#define SCREEN_H


#include <stdio.h>
#include "bitmap_image.hpp"

#include "config.h"

#include "camera.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "model.h"
#include "trace.h"
#include "vector.h"


BEGIN_RAYTRACER


class Screen {
  public:
    Color *** pixels;
    int height, width;

    Screen() {
      height = image_height * INT_RES_FACTOR;
      width = image_width * INT_RES_FACTOR;
      pixels = new Color ** [width];
      for (int i = 0;i < width;i++) {
        pixels[i] = new Color * [height];
      }

      Vector dir = camera.target - camera.viewpoint;
      dir.normalize();

      P_FLT horizontal_mag = sin(camera.angle * 0.5);
      P_FLT vertical_mag = horizontal_mag / camera.aspect_ratio;
      Vector top = camera.up * vertical_mag;

      Vector right = crossProduct(dir, camera.up);
      right.normalize();
      right *= horizontal_mag;

      Point center = camera.viewpoint + dir;
      Point top_left = center + top - right;

      Vector i_step = right / (float)(width / 2);
      Vector j_step = (top / (float)(height / 2));
      j_step.negate();

      Vector top_left_pixel = top_left - camera.viewpoint +
                              (i_step * 0.5) + (j_step * 0.5);

      int hits = 0;
      for (int i = 0;i < width;i++) {
        for (int j = 0;j < height;j++) {
          Vector ray_dir = top_left_pixel +
                           (i_step * (P_FLT)i) + (j_step * (P_FLT)j);
          ray_dir.normalize();
          Ray ray(camera.viewpoint, ray_dir);
          pixels[i][j] = new Color();

          hits += trace(0, 1.0, ray, pixels[i][j]);
        }
      }

      printf("Tracing complete, %d hits out of %d total\n", hits,
             width * height);
      calibrate();
    }

    void calibrate();
    void save();
};


END_RAYTRACER


#endif
