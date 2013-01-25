#ifndef MATERIAL_H
#define MATERIAL_H


#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


class Material {
  public:
    Coeff ambience,
          diffuse,
          specular,
          transmission;

    C_FLT attenuation,
          refraction,
          roughness;

    Material(Coeff ambience, Coeff diffuse, Coeff specular, P_FLT roughness):
             ambience(ambience), diffuse(diffuse), specular(specular),
             roughness(roughness) {
    }
};


END_RAYTRACER


#endif
