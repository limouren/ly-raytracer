#include <math.h>
#include <stdlib.h>
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
  unsigned char * mRGBPtr;
  int pixelCount;

  strncpy(_filename, filename, strlen(filename) + 1);
  ::Texture * bartTexture = ::viReadPPM(_filename);

  height = bartTexture->mHeight;
  width = bartTexture->mWidth;

  pixelCount = height * width;
  mRGBPtr = bartTexture->mRGB;
  pixels = new RGBColor[pixelCount];
  for (int i = 0; i < pixelCount; i++) {
    pixels[i] = RGBColor(mRGBPtr);
    mRGBPtr += 3;
  }

  free(bartTexture->mRGB);
}


const Color Texture::colorAt(const Vector2D &textureCoord) {
  int x, y;
  P_FLT xInteger, xFraction, yInteger, yFraction;

  xFraction = modf(textureCoord.x * width, &xInteger);
  x = static_cast<int>(xInteger);
  yFraction = modf(textureCoord.y * height, &yInteger);
  y = static_cast<int>(yInteger);

  return pixels[y * width + x].toColor();
}



END_RAYTRACER
