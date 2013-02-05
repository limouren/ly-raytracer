#ifndef SCENE_H
#define SCENE_H


#include <map>
#include <math.h>
#include <string>
#include <vector>

#include "bitmap/bitmap/bitmap_image.hpp"

#include "config.h"

#include "background.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "shapes.h"
#include "texture.h"
#include "triangle.h"


BEGIN_RAYTRACER


class Scene {
  public:
    Color ambience, backgroundColor;
    Background * background;
    Material * medium;
    std::vector<Light * > lights;
    std::vector<Material *> materials;
    std::map<std::string, Texture *> textures;
    MODEL_CLS * modelRoot;

    Scene() {
      medium = new Material("Vacuum",
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(1.0, 1.0, 1.0),
                            1.0,
                            0.0);
    }
} scene;


END_RAYTRACER


#endif
