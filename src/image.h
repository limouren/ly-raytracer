#ifndef IMAGE_H
#define IMAGE_H


#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


class Image {
  public:
    int height, width;
    Color * pixels;

    Image(int height, int width) {
      pixels = new Color[height * width];
    }

    Image(int height, int width, Color * pixels_) {
      int total_pixels = height * width;

      pixels = new Color[total_pixels];
      for (int i = 0; i < total_pixels; i++) {
        pixels[i] = pixels_[i];
      }
    }
};


END_RAYTRACER


#endif
