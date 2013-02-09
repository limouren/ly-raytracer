#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <string.h>
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
    std::vector<Texture *> textures;
    MODEL_CLS * modelRoot;

    Scene(): modelRoot(NULL) {
      medium = new Material("Vacuum",
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(0.0, 0.0, 0.0),
                            Color(1.0, 1.0, 1.0),
                            1.0,
                            0.0);
    }

    void loadTextures() {
      for (std::vector<Texture *>::iterator itr = textures.begin();
           itr != textures.end(); itr++) {
        (*itr)->loadFromFile();
      }
      printf("%d textures loaded\n", textures.size());
    }

    // TODO(kent): Fix this eventually
    // ~Scene() {
    //   delete background;
    //   delete medium;
    //   while (!lights.empty()) {
    //     delete lights.back();
    //     lights.pop_back();
    //   }
    //   while (!materials.empty()) {
    //     delete materials.back();
    //     materials.pop_back();
    //   }
    //   for (std::map<const char *, Texture *>::iterator itr =
    //          textures.begin();
    //        itr != textures.end(); itr++) {
    //     delete itr->first;
    //     delete itr->second;
    //   }
    //   textures.clear();
    //   delete modelRoot;
    // }
} scene;


END_RAYTRACER


#endif
