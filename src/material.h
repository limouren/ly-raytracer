#ifndef MATERIAL_H
#define MATERIAL_H


#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


class Material {
  public:
    const char * name;

    C_FLT attenuation,
          refraction,
          shine;

    Color ambience,
          diffuse,
          specular,
          transmission;

    Material(const char * name, const Color &ambience, const Color &diffuse,
             const Color &specular, const Color transmission,
             const C_FLT refraction, const C_FLT shine):
             name(name), ambience(ambience), diffuse(diffuse),
             specular(specular), transmission(transmission),
             refraction(refraction), shine(shine) {
    }
};


END_RAYTRACER


#endif
