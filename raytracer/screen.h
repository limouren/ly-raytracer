#ifndef SCREEN_H
#define SCREEN_H


#include <algorithm>
#include <vector>
#include <pthread.h>

#include "config.h"

#include "camera.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "scene.h"
#include "trace.h"
#include "tree.h"


BEGIN_RAYTRACER


class PixelTracer {
  private:
    Color * pixel;
    Vector3D rayDir;

  public:
    PixelTracer(Color * pixel, const Vector3D &rayDir):
      pixel(pixel), rayDir(rayDir) {}

    inline void alias(const Scene * scene, const int level, Color * center,
                      const Point3D &rayOrig, const Vector3D &rayDir,
                      const Vector3D &pixelHor, const Vector3D &pixelVert) {
      Color samples[4];
      Vector3D halfHor = pixelHor * 0.5,
               halfVert = pixelVert * 0.5;
      Vector3D sampleDirs[4] = {rayDir - halfHor - halfVert,
                                rayDir + halfHor - halfVert,
                                rayDir - halfHor + halfVert,
                                rayDir + halfHor + halfVert};
      sampleDirs[0].normalize();
      sampleDirs[1].normalize();
      sampleDirs[2].normalize();
      sampleDirs[3].normalize();
      Ray sampleRays[4] = {Ray(rayOrig, sampleDirs[0], scene->medium),
                           Ray(rayOrig, sampleDirs[1], scene->medium),
                           Ray(rayOrig, sampleDirs[2], scene->medium),
                           Ray(rayOrig, sampleDirs[3], scene->medium)};

      for (int i = 0; i < 4; i++) {
        trace(scene, 0, 1.0f, sampleRays[i], &samples[i]);
      }

      if (level < aaLevel) {
        C_FLT centerMagnitude = center->magnitude();
        for (int i = 0; i < 4; i++) {
          if (fabs(samples[i].magnitude() - centerMagnitude) >
              AA_COLOR_THRESHOLD) {
            alias(scene, level + 1, &samples[i], rayOrig, sampleDirs[i],
                  halfHor, halfVert);
          }
        }
      }

      *center = (samples[0] + samples[1] + samples[2] + samples[3]) * 0.25f;
    }

    inline void run(const Scene * scene, const Point3D &rayOrig,
                    const Vector3D &pixelHor, const Vector3D &pixelVert) {
      Ray ray(rayOrig, rayDir, scene->medium);
      trace(scene, 0, 1.0f, ray, pixel);

      if (aaLevel > 0) {
        alias(scene, 1, pixel, rayOrig, rayDir, pixelHor, pixelVert);
      }
    }
};


class ScreenTracer {
  private:
    const Scene * scene;
    int taskCount;
    std::vector<PixelTracer *> tasks;
    pthread_mutex_t tasksMutex;
    Point3D rayOrig;
    Vector3D pixelHor, pixelVert;

  public:
    explicit ScreenTracer(const Scene * scene): scene(scene) {
      pthread_mutex_init(&tasksMutex, NULL);
    }

    void addTask(Color * pixel, const Vector3D &rayDir) {
      tasks.push_back(new PixelTracer(pixel, rayDir));
    }

    void init(const Point3D &rayOrig, const Vector3D &pixelHor,
              const Vector3D &pixelVert) {
      this->rayOrig = rayOrig;
      this->pixelHor = pixelHor * 0.5f;
      this->pixelVert = pixelVert * 0.5f;

      taskCount = tasks.size();
    }

    inline void run() {
      int index,
          thousandth = taskCount / 1000;
      float progress,
            progressPercent = 100.0f / static_cast<float>(taskCount);
      PixelTracer * pixelTracer;

      pthread_mutex_lock(&tasksMutex);
      index = tasks.size();
      while (index > 0) {
        pixelTracer = tasks.back();
        tasks.pop_back();
        pthread_mutex_unlock(&tasksMutex);

        //if (index % thousandth == 0) {
        //  progress = static_cast<float>(taskCount - index) * progressPercent;
        //  printf("\rTracing image...%.1f%% completed.", progress);
        //  fflush(stdout);
        //}
        pixelTracer->run(scene, rayOrig, pixelHor, pixelVert);
        delete pixelTracer;

        pthread_mutex_lock(&tasksMutex);
        index = tasks.size();
      }
      pthread_mutex_unlock(&tasksMutex);
    }
};


// This is a proxy function for pthreads to run ScreenTracer->trace()
void * runScreenTracer(void * context) {
  static_cast<ScreenTracer *>(context)->run();
  return NULL;
}


class Screen {
  public:
    int height, width;
    Color * pixels;
    Scene * scene;

    explicit Screen(Scene * scene):
      height(scene->camera->imageHeight), width(scene->camera->imageWidth),
      pixels(new Color[height * width]), scene(scene) {}

    ~Screen() {
      delete [] pixels;
    }

    void calibrate();
    void rayTrace(const P_FLT time);
    void saveBmp(char * outputFilename) const;
    void savePng(char * outputFilename) const;
};


END_RAYTRACER


#endif
