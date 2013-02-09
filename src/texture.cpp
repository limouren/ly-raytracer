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

  strncpy(_filename, filename, strlen(filename));
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

  xFraction = modf(textureCoord.x, &xInteger);
  x = static_cast<int>(xInteger);
  yFraction = modf(textureCoord.y, &yInteger);
  y = static_cast<int>(yInteger);

  return pixels[y * width + x].toColor() * (1 - xFraction) * (1 - yFraction) +
         pixels[y * width + x + 1].toColor() * xFraction * (1 - yFraction) +
         pixels[(y + 1) * width + x].toColor() * (1 - xFraction) * yFraction +
         pixels[(y + 1) * width + x + 1].toColor() * xFraction * yFraction;
}



END_RAYTRACER
