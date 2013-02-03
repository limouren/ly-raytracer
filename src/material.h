#ifndef MATERIAL_H
#define MATERIAL_H


#include <string>

#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


class Material {
  public:
    std::string name;

    C_FLT attenuation,
          refraction,
          shine;

    Color ambience,
          diffuse,
          specular,
          transmission;

    Material(std::string name, Color ambience, Color diffuse, Color specular,
             Color transmission, C_FLT refraction, C_FLT shine):
             name(name), ambience(ambience), diffuse(diffuse),
             specular(specular), transmission(transmission),
             refraction(refraction), shine(shine) {
    }
};


END_RAYTRACER


#endif
