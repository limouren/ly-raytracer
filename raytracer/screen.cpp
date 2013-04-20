#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include "bitmap/bitmap/bitmap_image.hpp"
#include "lodepng/lodepng.cpp"

#include "config.h"

#include "screen.h"
#include "utils.h"


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
  clock_t startTimer, endTimer;

  printf("Saving file to \"%s\"...", outputFilename);
  fflush(stdout);
  startTimer = clock();
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
  endTimer = clock();
  printf("completed (%.3f seconds).\n", clockTime(startTimer, endTimer));
}


void Screen::savePng(char * outputFilename) const {
  clock_t startTimer, endTimer;

  printf("Saving file to \"%s\"...", outputFilename);
  fflush(stdout);
  startTimer = clock();

  int pixelCount = height * width;
  std::vector<unsigned char> png;
  for (int i = pixelCount - 1; i >=0; i--) {
    RGBColor rgbColor(pixels[i]);
    png.push_back(rgbColor.r);
    png.push_back(rgbColor.g);
    png.push_back(rgbColor.b);
    png.push_back(255);
  }
  lodepng::encode(outputFilename, png, width, height);

  endTimer = clock();
  printf("completed (%.3f seconds).\n", clockTime(startTimer, endTimer));
}


void Screen::rayTrace(const P_FLT time) {
  clock_t startTimer, endTimer;

  scene->buildNodeTree(time);

  startTimer = clock();
  Vector3D dir = scene->camera->target - scene->camera->viewpoint;
  dir.normalize();

  P_FLT horizontal = sin(scene->camera->angle * 0.5f),
        vertical = horizontal / scene->camera->aspectRatio;

  Vector3D top = scene->camera->up - dir * dotProduct(scene->camera->up, dir),
           left = crossProduct(scene->camera->up, dir);
  top.normalize();
  top *= vertical;
  left.normalize();
  left *= horizontal;

  Point3D center = scene->camera->viewpoint + dir,
          topLeft = center + top + left;

  Vector3D pixelHor = -left / static_cast<P_FLT>(width / 2),
           pixelVert = -top / static_cast<P_FLT>(height / 2);

  Vector3D topLeftPixel = topLeft - scene->camera->viewpoint +
                          (pixelHor * 0.5f) + (pixelVert * 0.5f);

  ScreenTracer * screenTracer = new ScreenTracer(scene);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      Vector3D rayDir = topLeftPixel +
                        pixelHor * static_cast<P_FLT>(i) +
                        pixelVert * static_cast<P_FLT>(j);
      rayDir.normalize();

      pixels[j * width + i] = Color();
      screenTracer->addTask(&pixels[j * width + i], rayDir);
    }
  }
  screenTracer->init(scene->camera->viewpoint, pixelHor, pixelVert);

  pthread_t * threads = new pthread_t[threadNum];
  for (int i = 0; i < threadNum; i++) {
    pthread_create(&threads[i], NULL, &runScreenTracer,
                   static_cast<void *>(screenTracer));
  }
  for (int i = 0; i < threadNum; i++) {
    pthread_join(threads[i], NULL);
  }
  endTimer = clock();
  printf("\rTracing image...100.0%% completed (%.3f seconds).\n",
         clockTimeMT(startTimer, endTimer));

  delete screenTracer;
}


END_RAYTRACER
