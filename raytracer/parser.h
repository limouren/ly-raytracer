#ifndef PARSE_H
#define PARSE_H


#include "config.h"


BEGIN_RAYTRACER


class Camera;
class Material;
class Scene;
class Transform;

int parseFile(const char * filename, Scene * scene);


END_RAYTRACER


#endif
