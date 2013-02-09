#ifndef SCREEN_H
#define SCREEN_H


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
    int taskIndex, totalTasks;
    pthread_mutex_t tasksMutex;
    PixelTask * tasks;

  public:
    explicit PixelTasks(int _totalTasks): totalTasks(_totalTasks) {
      tasks = new PixelTask[totalTasks];
      taskIndex = 0;

      pthread_mutex_init(&tasksMutex, NULL);
    }

    ~PixelTasks() {
      delete [] tasks;
    }

    void insertTask(const PixelTask &pixelTask) {
      tasks[taskIndex] = pixelTask;
      taskIndex++;
    }

    void run() {
      int currentTask,
          thousandth = totalTasks / 10000;
      double progress;

      pthread_mutex_lock(&tasksMutex);
      while (taskIndex > 0) {
        taskIndex--;
        currentTask = taskIndex;
        pthread_mutex_unlock(&tasksMutex);

        if (currentTask % thousandth == 0) {
          progress = 100.0f * static_cast<double>(totalTasks - currentTask) /
                     static_cast<double>(totalTasks);
          printf("\rTracing...%.2f%% complete", progress);
        }
        tasks[currentTask].run();

        pthread_mutex_lock(&tasksMutex);
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
