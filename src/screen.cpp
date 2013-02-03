#include <math.h>
#include <stdio.h>
#include <vector>

#include "bitmap_image.hpp"
#include "pthread.h"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


class Camera;
class Scene;
class Vector3D;


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


void Screen::saveBmp(char * outputFilename) const {
  int new_i, new_j,
      imageHeight = height / INT_RES_FACTOR,
      imageWidth = width / INT_RES_FACTOR,
      pixel_count = imageHeight * imageWidth;

  P_FLT factor = 1.0 / static_cast<P_FLT>(INT_RES_FACTOR * INT_RES_FACTOR);
  C_FLT * red_channel = new C_FLT[pixel_count];
  C_FLT * green_channel = new C_FLT[pixel_count];
  C_FLT * blue_channel = new C_FLT[pixel_count];

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
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

  char outputPathname[1024];
  int size = snprintf(outputPathname, sizeof(outputFilename) + 6, "out/%s",
                      outputFilename);
  if (size > 1024) {
    printf("Output filename too long!\n");
    exit(1);
  }

  bmp_image.save_image(outputPathname);

  delete [] red_channel;
  delete [] green_channel;
  delete [] blue_channel;
}


void * runPixelTasks(void * context) {
  PixelTasks * pixelTasks = static_cast<PixelTasks *>(context);
  pixelTasks->run();
}


void rayTrace(const Scene &scene, const Camera &camera, Screen &screen) {
  Vector3D dir = camera.target - camera.viewpoint;
  dir.normalize();

  P_FLT horizontalMag = sin(camera.angle * 0.5);
  P_FLT verticalMag = horizontalMag / camera.aspectRatio;
  Vector3D top = camera.up * verticalMag;

  Vector3D right = crossProduct(dir, camera.up);
  right.normalize();
  right *= horizontalMag;

  Point3D center = camera.viewpoint + dir;
  Point3D top_left = center + top - right;

  Vector3D i_step = right / static_cast<P_FLT>(screen.width / 2);
  Vector3D j_step = (top / static_cast<P_FLT>(screen.height / 2));
  j_step.negate();

  Vector3D top_left_pixel = top_left - camera.viewpoint +
                            (i_step * 0.5) + (j_step * 0.5);

  PixelTasks * pixelTasks = new PixelTasks(screen.width * screen.height);
  for (int i = 0; i < screen.width; i++) {
    for (int j = 0; j < screen.height; j++) {
      Vector3D ray_dir = top_left_pixel +
                         i_step * static_cast<P_FLT>(i) +
                         j_step * static_cast<P_FLT>(j);
      ray_dir.normalize();
      Ray ray(camera.viewpoint, ray_dir);
      PixelTask task(&(screen.pixels[j*screen.width + i]), ray);

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
         pixelTasks->totalHits(), screen.width * screen.height);
  delete pixelTasks;

  screen.calibrate();
}


END_RAYTRACER
