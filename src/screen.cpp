#include <stdio.h>
#include <vector>

#include "bitmap_image.hpp"
#include "pthread.h"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


void Screen::calibrate() {
  C_FLT factor = 0.0;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
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
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        pixels[j*width + i] *= factor;
      }
    }
    printf("Calbiration complete.\n");
  } else {
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

  Vector i_step = right / static_cast<P_FLT>(width / 2);
  Vector j_step = (top / static_cast<P_FLT>(height / 2));
  j_step.negate();

  Vector top_left_pixel = top_left - camera.viewpoint +
                          (i_step * 0.5) + (j_step * 0.5);

  pixelTasks = new PixelTasks(width * height);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      Vector ray_dir = top_left_pixel +
                       (i_step * (P_FLT)i) + (j_step * (P_FLT)j);
      ray_dir.normalize();
      Ray ray(camera.viewpoint, ray_dir);
      PixelTask task(&pixels[j*width + i], ray);

      pixelTasks->insertTask(task);
    }
  }

  pthread_t threads[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_create(&threads[i], NULL, &runPixelTasks, NULL);
  }
  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tracing complete, %d hits out of %d total\n",
         pixelTasks->totalHits(), width * height);
  calibrate();
}


void Screen::saveBmp() {
  int new_i, new_j,
      pixel_count = height * width;

  P_FLT factor = 1.0 / static_cast<P_FLT>(INT_RES_FACTOR * INT_RES_FACTOR);
  C_FLT * red_channel = new C_FLT[pixel_count];
  C_FLT * green_channel = new C_FLT[pixel_count];
  C_FLT * blue_channel = new C_FLT[pixel_count];

  for (int i = 0; i < image_width; i++) {
    for (int j = 0; j < image_height; j++) {
      for (int m = 0; m < INT_RES_FACTOR; m++) {
        for (int n = 0; n < INT_RES_FACTOR; n++) {
          new_i = i * INT_RES_FACTOR + m;
          new_j = j * INT_RES_FACTOR + n;

          red_channel[j*image_width + i] += pixels[new_j*width + new_i].r;
          green_channel[j*image_width + i] += pixels[new_j*width + new_i].g;
          blue_channel[j*image_width + i] += pixels[new_j*width + new_i].b;
        }
      }
      red_channel[j*image_width + i] *= factor;
      green_channel[j*image_width + i] *= factor;
      blue_channel[j*image_width + i] *= factor;
    }
  }

  bitmap_image bmp_image(image_width, image_height);
  bmp_image.import_rgb(red_channel, green_channel, blue_channel);
  bmp_image.save_image("out/output.bmp");

  delete [] red_channel;
  delete [] green_channel;
  delete [] blue_channel;
}


END_RAYTRACER
