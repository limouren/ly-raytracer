#ifndef MATERIAL_H
#define MATERIAL_H


#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


class Material {
  public:
    Coeff ambience;
    Coeff diffuse;
    Coeff specular;
    Coeff ktran;
    Color emittance;

    C_FLT attenuation;
    C_FLT refraction;
    C_FLT roughness;

    Material(Coeff ambience, Coeff diffuse, Coeff specular, Color emittance,
             P_FLT roughness):
             ambience(ambience), diffuse(diffuse), specular(specular),
             emittance(emittance), roughness(roughness) {
    }
};


END_RAYTRACER


#endif
