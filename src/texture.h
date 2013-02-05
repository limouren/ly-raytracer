#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_


#include <string>

#include "config.h"

#include "color.h"
#include "vector.h"


BEGIN_RAYTRACER


class Texture {
  private:
    bool continuous;
    int height, width;

    RGBColor * pixels;

  public:
    Texture * loadFromFile(const std::string filename);
    Texture * loadFromPpm(const std::string filename);

    inline const Color getColor(const Vector2D &textureCoord);
};


END_RAYTRACER


#endif  // SRC_TEXTURE_H_
