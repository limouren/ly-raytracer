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

  // First modf removes texture coordinates > 1, second for color interpolation
  xFraction = modf(textureCoord.x, &xInteger);
  xFraction = modf(xFraction * width, &xInteger);
  x = std::min(width - 2, static_cast<int>(xInteger));

  yFraction = modf(textureCoord.y, &yInteger);
  yFraction = modf(yFraction * height, &yInteger);
  y = std::min(height - 2, static_cast<int>(yInteger));

  return pixels[y * width + x].toColor() * (1 - xFraction) * (1 - yFraction) +
         pixels[y * width + x + 1].toColor() * xFraction * (1 - yFraction) +
         pixels[(y + 1) * width + x].toColor() * (1 - xFraction) * yFraction +
         pixels[(y + 1) * width + x + 1].toColor() * xFraction * yFraction;
}


END_RAYTRACER
