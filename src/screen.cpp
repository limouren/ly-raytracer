#include <math.h>
#include <stdio.h>
#include <vector>

#include "bitmap_image.hpp"
#include "pthread.h"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


void * runPixelTasks(void * context) {
  PixelTasks * pixelTasks = static_cast<PixelTasks *>(context);
  pixelTasks->run();
}


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


void Screen::rayTrace() {
  Vector3D dir = camera.target - camera.viewpoint;
  dir.normalize();

  P_FLT horizontal_mag = sin(camera.angle * 0.5);
  P_FLT vertical_mag = horizontal_mag / camera.aspect_ratio;
  Vector3D top = camera.up * vertical_mag;

  Vector3D right = crossProduct(dir, camera.up);
  right.normalize();
  right *= horizontal_mag;

  Point3D center = camera.viewpoint + dir;
  Point3D top_left = center + top - right;

  Vector3D i_step = right / static_cast<P_FLT>(width / 2);
  Vector3D j_step = (top / static_cast<P_FLT>(height / 2));
  j_step.negate();

  Vector3D top_left_pixel = top_left - camera.viewpoint +
                            (i_step * 0.5) + (j_step * 0.5);

  PixelTasks * pixelTasks = new PixelTasks(width * height);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      Vector3D ray_dir = top_left_pixel +
                         (i_step * (P_FLT)i) + (j_step * (P_FLT)j);
      ray_dir.normalize();
      Ray ray(camera.viewpoint, ray_dir);
      PixelTask task(&pixels[j*width + i], ray);

      pixelTasks->insertTask(task);
    }
  }

  pthread_t threads[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_create(&threads[i], NULL, &runPixelTasks,
                   static_cast<void *>(pixelTasks));
  }
  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Tracing complete, %d hits out of %d total\n",
         pixelTasks->totalHits(), width * height);
  delete pixelTasks;

  calibrate();
}


void Screen::saveBmp() {
  int new_i, new_j,
      pixel_count = height * width;

  P_FLT factor = 1.0 / static_cast<P_FLT>(INT_RES_FACTOR * INT_RES_FACTOR);
  C_FLT * red_channel = new C_FLT[pixel_count];
  C_FLT * green_channel = new C_FLT[pixel_count];
  C_FLT * blue_channel = new C_FLT[pixel_count];

  for (int i = 0; i < imageWidth; i++) {
    for (int j = 0; j < imageHeight; j++) {
      for (int m = 0; m < INT_RES_FACTOR; m++) {
        for (int n = 0; n < INT_RES_FACTOR; n++) {
          new_i = i * INT_RES_FACTOR + m;
          new_j = j * INT_RES_FACTOR + n;

          red_channel[j*imageWidth + i] += pixels[new_j*width + new_i].r;
          green_channel[j*imageWidth + i] += pixels[new_j*width + new_i].g;
          blue_channel[j*imageWidth + i] += pixels[new_j*width + new_i].b;
        }
      }
      red_channel[j*imageWidth + i] *= factor;
      green_channel[j*imageWidth + i] *= factor;
      blue_channel[j*imageWidth + i] *= factor;
    }
  }

  bitmap_image bmp_image(imageWidth, imageHeight);
  bmp_image.import_rgb(red_channel, green_channel, blue_channel);

  std::string out("out/");
  out += outputFilename;

  bmp_image.save_image(out);

  delete [] red_channel;
  delete [] green_channel;
  delete [] blue_channel;
}


END_RAYTRACER
