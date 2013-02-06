#include <string.h>

#include "bart/src/texture.c"

#include "config.h"

#include "color.h"
#include "texture.h"
#include "vector.h"


BEGIN_RAYTRACER


void Texture::loadFromFile(const char * filename) {
  // TODO(kent): Handle other texture file types
  return loadFromPpm(filename);
}


void Texture::loadFromPpm(const char * filename) {
  char _filename[1024];
  strncpy(_filename, filename, strlen(filename));
  ::Texture * bartTexture = ::viReadPPM(_filename);
}


const Color Texture::getColor(const Vector2D &textureCoord) {
  return Color();
}



END_RAYTRACER
