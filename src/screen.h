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
      return trace(0, 1.0, ray, color, scene.medium);  // Assume outside
    }
};


class PixelTasks {
  private:
    int hits, taskIndex;
    pthread_mutex_t hitsMutex, tasksMutex;
    PixelTask * tasks;

  public:
    explicit PixelTasks(int total_tasks) {
      hits = 0;
      taskIndex = 0;
      tasks = new PixelTask[total_tasks];

      hitsMutex = PTHREAD_MUTEX_INITIALIZER;
      tasksMutex = PTHREAD_MUTEX_INITIALIZER;
    }

    ~PixelTasks() {
      delete [] tasks;
    }

    void insertTask(const PixelTask &pixelTask) {
      tasks[taskIndex] = pixelTask;
      taskIndex++;
    }

    void run() {
      int newHits, currentTask;

      pthread_mutex_lock(&tasksMutex);
      while (taskIndex > 0) {
        currentTask = taskIndex - 1;
        taskIndex--;
        pthread_mutex_unlock(&tasksMutex);

        newHits = tasks[currentTask].run();
        pthread_mutex_lock(&hitsMutex);
        hits += newHits;
        pthread_mutex_unlock(&hitsMutex);

        pthread_mutex_lock(&tasksMutex);
      }
      pthread_mutex_unlock(&tasksMutex);
    }

    int totalHits() {
      return hits;
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

    void initSize(const Camera &camera) {
      height = camera.imageHeight * INT_RES_FACTOR;
      width = camera.imageWidth * INT_RES_FACTOR;
      pixels = new Color[height * width];
    }

    void calibrate();
    void saveBmp(char * outputFilename) const;
};


void rayTrace(const Scene &scene, const Camera &camera, Screen &screen);


END_RAYTRACER


#endif
