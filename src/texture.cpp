#include <algorithm>
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


Color Texture::colorAt(const Vector2D &textureCoord) {
  int x, y;
  P_FLT xInteger, xFract, yInteger, yFract;

  // First modf removes texture coordinates > 1, second for color interpolation
  xFract = modf(textureCoord.x, &xInteger);
  xFract = modf(xFract * width, &xInteger);
  x = min(width - 2, static_cast<int>(xInteger));

  yFract = modf(textureCoord.y, &yInteger);
  yFract = modf(yFract * height, &yInteger);
  y = min(height - 2, static_cast<int>(yInteger));

  return pixels[y * width + x].toColor() * (1.0f - xFract) * (1.0f - yFract) +
         pixels[y * width + x + 1].toColor() * xFract * (1.0f - yFract) +
         pixels[(y + 1) * width + x].toColor() * (1.0f - xFract) * yFract +
         pixels[(y + 1) * width + x + 1].toColor() * xFract * yFract;
}


END_RAYTRACER
