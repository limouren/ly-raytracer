#ifndef MATERIAL_H
#define MATERIAL_H


#include "config.h"
#include "color.h"


BEGIN_RAYTRACER


class Material {
  public:
    C_FLT kdiff;
    C_FLT kspec;
    C_FLT ktran;
    Color color;
    C_FLT refrindex;

    Material(C_FLT kdiff, C_FLT kspec, C_FLT ktran, Color color,
             C_FLT refrindex): kdiff(kdiff), kspec(kspec), ktran(ktran),
                               color(color), refrindex(refrindex) {}
};


END_RAYTRACER


#endif
