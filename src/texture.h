#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_


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
    void loadFromFile(const char * filename);
    void loadFromPpm(const char * filename);

    inline const Color getColor(const Vector2D &textureCoord);

    ~Texture() {
      delete [] pixels;
    }
};


END_RAYTRACER


#endif  // SRC_TEXTURE_H_
