#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_


#include <string.h>

#include "config.h"

#include "color.h"
#include "vector.h"


BEGIN_RAYTRACER


class Texture {
  private:
    int height, width;

    RGBColor * pixels;

  public:
    char filepath[1024];

    explicit Texture(char * _filepath) {
      strncpy(filepath, _filepath, strlen(_filepath) + 1);
    }

    void loadFromFile();
    void loadFromPpm();

    inline Color colorAt(const Vector2D &textureCoord);

    ~Texture() {
      delete [] pixels;
    }
};


END_RAYTRACER


#endif  // SRC_TEXTURE_H_
