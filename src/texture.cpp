#include <string>

#include "bart/src/texture.c"

#include "config.h"

#include "color.h"
#include "texture.h"
#include "vector.h"


BEGIN_RAYTRACER


Texture * Texture::loadFromFile(std::string filename) {
  // TODO(kent): Handle other texture file types
  return loadFromPpm(filename);
}


Texture * Texture::loadFromPpm(std::string _filename) {
  char filename[1024];
  strncpy(filename,  _filename.c_str(), _filename.length());
  ::Texture * bartTexture = ::viReadPPM(filename);

  return NULL;
}


const Color Texture::getColor(const Vector2D &textureCoord) {
  return Color();
}



END_RAYTRACER
