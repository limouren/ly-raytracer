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
  }
}


void Screen::saveBmp(char * outputFilename) const {
  if (pixels == NULL) {
    printf("Error: Saving image to file before traced\n");
    exit(1);
  }

  int imagePixelIndex, pixelIndex,
      // Avoid this division somehow
      imageHeight = height / INT_RES_FACTOR,
      imageWidth = width / INT_RES_FACTOR,
      pixelCount = imageHeight * imageWidth;

  P_FLT factor = 1.0 / static_cast<P_FLT>(INT_RES_FACTOR * INT_RES_FACTOR);
  C_FLT * redChannel = new C_FLT[pixelCount];
  C_FLT * greenChannel = new C_FLT[pixelCount];
  C_FLT * blueChannel = new C_FLT[pixelCount];

  for (int i = 0; i < imageWidth; i++) {
    for (int j = 0; j < imageHeight; j++) {
      imagePixelIndex = (imageHeight - j - 1) * imageWidth + i;
      for (int m = 0; m < INT_RES_FACTOR; m++) {
        for (int n = 0; n < INT_RES_FACTOR; n++) {
          pixelIndex = j * INT_RES_FACTOR * width + i * INT_RES_FACTOR;

          redChannel[imagePixelIndex] += pixels[pixelIndex].r;
          greenChannel[imagePixelIndex] += pixels[pixelIndex].g;
          blueChannel[imagePixelIndex] += pixels[pixelIndex].b;
        }
      }
      redChannel[imagePixelIndex] *= factor;
      greenChannel[imagePixelIndex] *= factor;
      blueChannel[imagePixelIndex] *= factor;
    }
  }

  bitmap_image bmpImage(imageWidth, imageHeight);
  bmpImage.import_rgb(redChannel, greenChannel, blueChannel);

  char outputPathname[1024];
  int size = snprintf(outputPathname, sizeof(outputPathname), "out/%s",
                      outputFilename);
  if (size > 1024) {
    printf("Output filename too long!\n");
    exit(1);
  }

  bmpImage.save_image(outputPathname);

  delete [] redChannel;
  delete [] greenChannel;
  delete [] blueChannel;
}


void * runPixelTasks(void * context) {
  PixelTasks * pixelTasks = static_cast<PixelTasks *>(context);
  pixelTasks->run();
}


void rayTrace(const Scene &scene, const Camera &camera, Screen &screen) {
  Vector3D dir = camera.target - camera.viewpoint;
  dir.normalize();

  screen.height = camera.imageHeight * INT_RES_FACTOR;
  screen.width = camera.imageWidth * INT_RES_FACTOR;
  screen.pixels = new Color[screen.width * screen.height];

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

  delete pixelTasks;
  screen.calibrate();
}


END_RAYTRACER
