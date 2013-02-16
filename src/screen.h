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
#include "model.h"
#include "scene.h"
#include "trace.h"


BEGIN_RAYTRACER


class PixelTracer {
  private:
    Color * pixel;
    Vector3D rayDir;
  public:
    PixelTracer(Color * pixel, const Vector3D &rayDir):
      pixel(pixel), rayDir(rayDir) {}

    inline int run(const Point3D &rayOrig) {
      Ray ray(rayOrig, rayDir);
      return trace(0, 1.0f, ray, pixel, scene.medium);
    }
};


class ScreenTracer {
  private:
    int taskCount;
    std::vector<PixelTracer *> tasks;
    pthread_mutex_t tasksMutex;
    Point3D rayOrig;

  public:
    ScreenTracer() {
      pthread_mutex_init(&tasksMutex, NULL);
    }

    void addTask(Color * pixel, const Vector3D &rayDir) {
      tasks.push_back(new PixelTracer(pixel, rayDir));
    }

    void init(const Point3D &rayOrig) {
      this->rayOrig = rayOrig;
      taskCount = tasks.size();
      // The magic behind steady progress printing...
      std::random_shuffle(tasks.begin(), tasks.end());
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

        if (index % thousandth == 0) {
          progress = static_cast<float>(taskCount - index) * progressPercent;
          printf("\rTracing image...%.1f%% completed.", progress);
          fflush(stdout);
        }
        pixelTracer->run(rayOrig);
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
}


class Screen {
  public:
    int height, width;
    Color * pixels;

    explicit Screen(const Camera * camera):
      height(camera->imageHeight), width(camera->imageWidth) {
      pixels = new Color[height * width];
    }

    ~Screen() {
      delete [] pixels;
    }

    void calibrate();
    void rayTrace(const Scene &scene, const Camera * camera);
    void saveBmp(char * outputFilename) const;
};


END_RAYTRACER


#endif
