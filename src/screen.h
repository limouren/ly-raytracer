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
    PixelTask(Color * color, const Ray &ray): color(color), ray(ray) {}

    int run() {
      return trace(0, 1.0, ray, color, NULL);
    }
};


class PixelTasks {
  private:
    int hits, task_index;
    pthread_mutex_t hits_mutex, tasks_mutex;
    PixelTask * tasks;

  public:
    explicit PixelTasks(int total_tasks) {
      hits = 0;
      task_index = 0;
      tasks = new PixelTask[total_tasks];

      hits_mutex = PTHREAD_MUTEX_INITIALIZER;
      tasks_mutex = PTHREAD_MUTEX_INITIALIZER;
    }

    ~PixelTasks() {
      delete [] tasks;
    }

    void insertTask(const PixelTask &pixelTask) {
      tasks[task_index] = pixelTask;
      task_index++;
    }

    void run() {
      int new_hits, current_task;

      pthread_mutex_lock(&tasks_mutex);
      while (task_index > 0) {
        current_task = task_index - 1;
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
    Color * pixels;
    int height, width;

    Screen() {
      height = image_height * INT_RES_FACTOR;
      width = image_width * INT_RES_FACTOR;
      pixels = new Color[height * width];
    }

    ~Screen() {
      delete [] pixels;
    }

    void calibrate();
    void rayTrace();
    void saveBmp();
    void saveJpg();
    void savePng();
};


END_RAYTRACER


#endif
