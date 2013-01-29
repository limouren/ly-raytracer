#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <math.h>
#include <string>
#include <vector>

#include "bitmap_image.hpp"

#include "config.h"

#include "camera.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "surface.h"


BEGIN_RAYTRACER


class Background {
  public:
    int height, width,
        orig_x, orig_y;

    P_FLT angle;

    Color * pixels;

    Background(std::string fileName, int orig_x, int orig_y, P_FLT angle):
      orig_x(orig_x), orig_y(orig_y), angle(angle) {
      bitmap_image image(fileName);

      height = image.height();
      width = image.width();

      int pixel_count = height * width;

      pixels = new Color[pixel_count];
      C_FLT * red_channel = new C_FLT[pixel_count];
      C_FLT * green_channel = new C_FLT[pixel_count];
      C_FLT * blue_channel = new C_FLT[pixel_count];

      image.export_rgb(red_channel, green_channel, blue_channel);
      for (int i = 0; i < pixel_count; i++) {
        pixels[i] = Color(red_channel[i], green_channel[i], blue_channel[i]);
      }

      delete [] red_channel;
      delete [] green_channel;
      delete [] blue_channel;
    }


    const Color interpolateBackground(const P_FLT x, const P_FLT y) const {
      P_FLT ratio_x, ratio_y,
            x_floor = floor(x),
            y_floor = floor(y);

      int l = static_cast<int>(x_floor),
          r = l + 1,
          t = static_cast<int>(y_floor),
          b = t + 1;

      ratio_x = x - x_floor;
      ratio_y = y - y_floor;

      Color result = pixels[t * width + l] * ratio_x * ratio_y +
                     pixels[t * width + r] * (1 - ratio_x) * ratio_y +
                     pixels[b * width + l] * ratio_x * (1 - ratio_y) +
                     pixels[b * width + r] * (1 - ratio_x) * (1 - ratio_y);
      return result;
    }


    const Color colorInDirection(const Vector3D &direction) const {
      P_FLT latitude, longitude, x, y;

      latitude = acos(-dotProduct(camera.up, direction));
      longitude = acos(dotProduct(camera.forward, direction) / sin(latitude));
      if (dotProduct(crossProduct(direction, camera.forward), camera.up) > 0) {
        longitude = 1 - longitude;
      }

      x = longitude * (P_FLT) width;
      y = latitude * (P_FLT) height / (2 * PI);

      return interpolateBackground(x, y);
    }


    ~Background() {
      delete [] pixels;
    }
};


END_RAYTRACER


#endif
