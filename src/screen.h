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
    Color * pixels[width][height];

    void calibrate() {
      C_FLT factor = 0.0;
      for (int i = 0;i < width;i++) {
        for (int j = 0;j < height;j++) {
          if (pixels[i][j]->r > factor) {
            factor = pixels[i][j]->r;
          }
          if (pixels[i][j]->g > factor) {
            factor = pixels[i][j]->g;
          }
          if (pixels[i][j]->b > factor) {
            factor = pixels[i][j]->b;
          }
        }
      }

      if (factor > 1.0) {
        factor = 1.0 / factor;
        for (int i = 0;i < width;i++) {
          for (int j = 0;j < height;j++) {
            (*pixels[i][j]) *= factor;
          }
        }
        printf("Calbiration complete.\n");
      }
      else {
        printf("Calbiration not required.\n");
      }
    }

    Screen() {
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

    void save() {
      bitmap_image image(width, height);
      for (int i = 0;i < width;i++) {
        for (int j = 0;j < height;j++) {
          int r = int(pixels[i][j]->r * 255 + 0.5);
          int g = int(pixels[i][j]->g * 255 + 0.5);
          int b = int(pixels[i][j]->b * 255 + 0.5);

          image.set_pixel(i, j, r, g, b);
        }
      }
      image.save_image("out/output.bmp");
    }
};


END_RAYTRACER


#endif
