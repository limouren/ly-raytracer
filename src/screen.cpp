#include <stdio.h>
#include <vector>

#include "bitmap_image.hpp"
#include "pthread.h"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


void Screen::calibrate() {
  C_FLT factor = 0.0;
  for (int i = 0;i < width;i++) {
    for (int j = 0;j < height;j++) {
      if (pixels[j*width + i].r > factor) {
        factor = pixels[j*width + i].r;
      }
      if (pixels[j*width + i].g > factor) {
        factor = pixels[j*width + i].g;
      }
      if (pixels[j*width + i].b > factor) {
        factor = pixels[j*width + i].b;
      }
    }
  }

  if (factor > 1.0) {
    factor = 1.0 / factor;
    for (int i = 0;i < width;i++) {
      for (int j = 0;j < height;j++) {
        pixels[j*width + i] *= factor;
      }
    }
    printf("Calbiration complete.\n");
  }
  else {
    printf("Calbiration not required.\n");
  }
}


void * runPixelTasks(void * context) {
  pixelTasks->run();
}


void Screen::rayTrace() {
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

  pixelTasks = new PixelTasks(width * height);
  for (int i = 0;i < width;i++) {
    for (int j = 0;j < height;j++) {
      Vector ray_dir = top_left_pixel +
                       (i_step * (P_FLT)i) + (j_step * (P_FLT)j);
      ray_dir.normalize();
      Ray ray(camera.viewpoint, ray_dir);
      PixelTask task(pixels[j*width + i], ray);

      pixelTasks->insertTask(task);
    }
  }

  pthread_t threads[THREAD_NUM];
  for (int i = 0;i < THREAD_NUM;i++) {
    pthread_create(&threads[i], NULL, &runPixelTasks, NULL);
  }
  for (int i = 0;i < THREAD_NUM;i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tracing complete, %d hits out of %d total\n",
         pixelTasks->totalHits(), width * height);
  calibrate();
}


void Screen::saveBmp() {
  bitmap_image bmp_image(image_width, image_height);

  int new_i, new_j;
  for (int i = 0;i < image_width;i++) {
    for (int j = 0;j < image_height;j++) {
      Color color(0.0, 0.0, 0.0);

      for (int m = 0;m < INT_RES_FACTOR;m++) {
        for (int n = 0;n < INT_RES_FACTOR;n++) {
          new_i = i * INT_RES_FACTOR + m;
          new_j = j * INT_RES_FACTOR + n;

          color += pixels[new_j*width + new_i];
        }
      }
      color *= 1.0 / (float)(INT_RES_FACTOR * INT_RES_FACTOR);

      int r = int(color.r * 255 + 0.5),
          g = int(color.g * 255 + 0.5),
          b = int(color.b * 255 + 0.5);

      bmp_image.set_pixel(i, j, r, g, b);
    }
  }
  bmp_image.save_image("out/output.bmp");
}


END_RAYTRACER
