#ifndef SCREEN_H
#define SCREEN_H


#include <algorithm>
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


class PixelTask {
  public:
    Color * color;
    Ray ray;

    PixelTask() {}
    PixelTask(Color * color, const Ray &ray): color(color), ray(ray) {}

    int run() {
      return trace(0, 1.0f, ray, color, scene.medium);  // Assume outside
    }
};


class PixelTasks {
  private:
    int totalTasks;
    pthread_mutex_t tasksMutex;
    std::vector<PixelTask> tasks;

  public:
    PixelTasks() {
      pthread_mutex_init(&tasksMutex, NULL);
    }

    void insertTask(const PixelTask &pixelTask) {
      tasks.push_back(pixelTask);
    }

    void prepare() {
      totalTasks = tasks.size();
      std::random_shuffle(tasks.begin(), tasks.end());
    }

    void run() {
      int taskNum,
          thousandth = totalTasks / 10000;
      double progress;
      PixelTask currentTask;

      pthread_mutex_lock(&tasksMutex);
      taskNum = tasks.size();
      while (taskNum > 0) {
        currentTask = tasks.back();
        tasks.pop_back();
        pthread_mutex_unlock(&tasksMutex);

        if (taskNum % thousandth == 0) {
          progress = 100.0f * static_cast<double>(totalTasks - taskNum) /
                     static_cast<double>(totalTasks);
          printf("\rTracing...%.2f%% complete", progress);
        }
        currentTask.run();

        pthread_mutex_lock(&tasksMutex);
        taskNum = tasks.size();
      }
      pthread_mutex_unlock(&tasksMutex);
    }
};


class Screen {
  public:
    int height, width;

    Color * pixels;

    Screen() {}

    ~Screen() {
      delete [] pixels;
    }

    void calibrate();
    void saveBmp(char * outputFilename) const;
};


void rayTrace(const Scene &scene, const Camera &camera, Screen &screen);


END_RAYTRACER


#endif
