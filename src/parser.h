#ifndef PARSE_H
#define PARSE_H


#include "config.h"


BEGIN_RAYTRACER


class Camera;
class Material;
class Scene;


int parseFile(const char * filename, Scene * scene, Camera ** camera,
              Material ** currentMaterial);


END_RAYTRACER


#endif
