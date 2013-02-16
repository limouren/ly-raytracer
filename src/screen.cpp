#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <vector>

#include "bitmap/bitmap/bitmap_image.hpp"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


class Camera;
class Scene;
class Vector3D;


void Screen::calibrate() {
  C_FLT factor = 0.0f;
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

  if (factor > 1.0f) {
    factor = 1.0f / factor;
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        pixels[j*width + i] *= factor;
      }
    }
  }
}


void Screen::saveBmp(char * outputFilename) const {
  int pixelCount = height * width;
  C_FLT * redChannel = new C_FLT[pixelCount],
        * greenChannel = new C_FLT[pixelCount],
        * blueChannel = new C_FLT[pixelCount];

  for (int i = 0, j = pixelCount - 1; i < pixelCount; i++, j--) {
    redChannel[i] = pixels[j].r;
    greenChannel[i] = pixels[j].g;
    blueChannel[i] = pixels[j].b;
  }

  bitmap_image bmpImage(width, height);
  bmpImage.import_rgb(redChannel, greenChannel, blueChannel);


  bmpImage.save_image(outputFilename);

  delete [] redChannel;
  delete [] greenChannel;
  delete [] blueChannel;
}


void Screen::rayTrace(const Scene &scene, const Camera * camera) {
  Vector3D dir = camera->target - camera->viewpoint;
  dir.normalize();

  P_FLT horizontal = sin(camera->angle * 0.5f),
        vertical = horizontal / camera->aspectRatio;

  Vector3D top = camera->up - dir * dotProduct(camera->up, dir),
           left = crossProduct(camera->up, dir);
  top.normalize();
  top *= vertical;
  left.normalize();
  left *= horizontal;

  Point3D center = camera->viewpoint + dir,
          topLeft = center + top + left;

  Vector3D pixelHor = -left / static_cast<P_FLT>(width / 2),
           pixelVert = -top / static_cast<P_FLT>(height / 2);

  Vector3D topLeftPixel = topLeft - camera->viewpoint +
                          (pixelHor * 0.5f) + (pixelVert * 0.5f);

  ScreenTracer * screenTracer = new ScreenTracer();
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      Vector3D rayDir = topLeftPixel +
                        pixelHor * static_cast<P_FLT>(i) +
                        pixelVert * static_cast<P_FLT>(j);
      rayDir.normalize();

      screenTracer->addTask(&pixels[j * width + i], rayDir);
    }
  }
  screenTracer->init(camera->viewpoint, pixelHor, pixelVert);

  pthread_t * threads = new pthread_t[threadNum];
  for (int i = 0; i < threadNum; i++) {
    pthread_create(&threads[i], NULL, &runScreenTracer,
                   static_cast<void *>(screenTracer));
  }
  for (int i = 0; i < threadNum; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("\rTracing image...100.0%% completed.\n");

  delete screenTracer;
}


END_RAYTRACER
