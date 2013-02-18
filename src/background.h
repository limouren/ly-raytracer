#ifndef BACKGROUND_H
#define BACKGROUND_H


#include <math.h>

#include "config.h"

#include "color.h"
#include "vector.h"


BEGIN_RAYTRACER


class Background {
  public:
    int height, width,
        orig_x, orig_y;

    P_FLT angle;

    Color * pixels;

    Background() {}


    Color interpolateBackground(const P_FLT x, const P_FLT y) const {
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


    ~Background() {}
};


END_RAYTRACER


#endif
