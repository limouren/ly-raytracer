#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "bart/src/texture.c"

#include "config.h"

#include "color.h"
#include "texture.h"
#include "vector.h"


BEGIN_RAYTRACER


void Texture::loadFromFile() {
  // TODO(kent): Handle other texture file types
  loadFromPpm();
  printf("Loaded %s\n", filepath);
}


void Texture::loadFromPpm() {
  unsigned char * mRGBPtr;
  int pixelCount;

  ::Texture * bartTexture = ::viReadPPM(filepath);

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
  x = static_cast<int>(xFraction * width);
  yFraction = modf(textureCoord.y, &yInteger);
  y = static_cast<int>(yFraction * height);

  return pixels[y * width + x].toColor();
}


END_RAYTRACER
