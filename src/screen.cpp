#include "bitmap_image.hpp"

#include "config.h"

#include "screen.h"



BEGIN_RAYTRACER


void Screen::calibrate() {
  C_FLT factor = 0.0;
  for (int i = 0;i < width;i++) {
    for (int j = 0;j < height;j++) {
      if (pixels[i][j]->r > factor) {
        factor = pixels[i][j]->r;
      }
      if (pixels[i][j]->g > factor) {
        factor = pixels[i][j]->g;
      }
      if (pixels[i][j]->b > factor) {
        factor = pixels[i][j]->b;
      }
    }
  }

  if (factor > 1.0) {
    factor = 1.0 / factor;
    for (int i = 0;i < width;i++) {
      for (int j = 0;j < height;j++) {
        (*pixels[i][j]) *= factor;
      }
    }
    printf("Calbiration complete.\n");
  }
  else {
    printf("Calbiration not required.\n");
  }
}


void Screen::save() {
  bitmap_image bmp_image(image_width, image_height);
  for (int i = 0;i < image_width;i++) {
    for (int j = 0;j < image_height;j++) {
      Color color(0.0, 0.0, 0.0);

      for (int m = 0;m < INT_RES_FACTOR;m++) {
        for (int n = 0;n < INT_RES_FACTOR;n++) {
          color += *(pixels[i * INT_RES_FACTOR + m][j * INT_RES_FACTOR + n]);
        }
      }
      color *= 1.0 / (float)(INT_RES_FACTOR * INT_RES_FACTOR);

      int r = int(color.r * 255 + 0.5),
          g = int(color.g * 255 + 0.5),
          b = int(color.b * 255 + 0.5);

      bmp_image.set_pixel(i, j, r, g, b);
    }
  }
  bmp_image.save_image("out/output.bmp");
}


END_RAYTRACER
