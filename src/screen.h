#ifndef SCREEN_H
#define SCREEN_H


#include <pthread.h>

#include "config.h"

#include "camera.h"
#include "color.h"
#include "point.h"
#include "ray.h"
#include "model.h"
#include "trace.h"


BEGIN_RAYTRACER


class PixelTask {
  public:
    Color * color;
    Ray ray;

    PixelTask() {}
    PixelTask(Color * color, Ray &ray): color(color), ray(ray) {}

    int run() {
      return trace(0, 1.0, ray, color);
    }
};


class PixelTasks {
  private:
    int hits, task_index;
    pthread_mutex_t hits_mutex, tasks_mutex;
    PixelTask * tasks;

  public:
    PixelTasks(int total_tasks) {
      hits = 0;
      task_index = 0;
      tasks = new PixelTask[total_tasks];

      hits_mutex = PTHREAD_MUTEX_INITIALIZER;
      tasks_mutex = PTHREAD_MUTEX_INITIALIZER;
    }

    ~PixelTasks() {
      delete tasks;
    }

    void insertTask(PixelTask &pixelTask) {
      tasks[task_index] = pixelTask;
      task_index++;
    }

    void run() {
      int new_hits, current_task;

      pthread_mutex_lock(&tasks_mutex);
      while (task_index > 0) {
        current_task = task_index;
        task_index--;
        pthread_mutex_unlock(&tasks_mutex);

        new_hits = tasks[current_task].run();
        pthread_mutex_lock(&hits_mutex);
        hits += new_hits;
        pthread_mutex_unlock(&hits_mutex);

        pthread_mutex_lock(&tasks_mutex);
      }
      pthread_mutex_unlock(&tasks_mutex);
    }

    int totalHits() {
      return hits;
    }
};

PixelTasks * pixelTasks;


class Screen {
  public:
    Color *** pixels;
    int height, width;

    Screen() {
      height = image_height * INT_RES_FACTOR;
      width = image_width * INT_RES_FACTOR;
      pixels = new Color ** [width];
      for (int i = 0;i < width;i++) {
        pixels[i] = new Color * [height];
      }
    }

    void calibrate();
    void rayTrace();
    void save();
};


END_RAYTRACER


#endif
