#ifndef PARSE_H
#define PARSE_H


#include <string>

#include "config.h"


BEGIN_RAYTRACER


class Camera;
class Material;
class Scene;
class Screen;


int parseFile(char * filename, Scene * scene, Camera * camera,
              Screen * screen, Material * currentMaterial);


END_RAYTRACER


#endif
