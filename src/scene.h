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
    Material * medium;
    std::vector<Light * > lights;
    std::vector<Material *> materials;
    std::vector<MODEL_CLS *> primitives;
    std::vector<Texture *> textures;
    MODEL_CLS * modelRoot;

    Scene(): modelRoot(NULL) {
      medium = new Material("Vacuum",
                            Color(0.0f, 0.0f, 0.0f),
                            Color(0.0f, 0.0f, 0.0f),
                            Color(0.0f, 0.0f, 0.0f),
                            Color(1.0f, 1.0f, 1.0f),
                            1.0f,
                            0.0f);
    }

    Texture * getOrCreateTexture(char * filepath) {
      for (std::vector<Texture *>::iterator itr = textures.begin();
           itr != textures.end(); itr++) {
        if (strncmp(filepath, (*itr)->filepath, strlen(filepath) + 1) == 0) {
          return *itr;
        }
      }

      Texture * newTexture = new Texture(filepath);
      textures.push_back(newTexture);
      return newTexture;
    }

    void loadTextures() {
      for (std::vector<Texture *>::iterator itr = textures.begin();
           itr != textures.end(); itr++) {
        (*itr)->loadFromFile();
      }
    }

    void buildModelTree() {
      modelRoot = buildModelTreeNode(primitives, 0);
    }

    ~Scene() {
      delete medium;
      while (!lights.empty()) {
        delete lights.back();
        lights.pop_back();
      }
      while (!materials.empty()) {
        delete materials.back();
        materials.pop_back();
      }
      while (!textures.empty()) {
        delete textures.back();
        textures.pop_back();
      }

      delete modelRoot;
      while (!primitives.empty()) {
        delete primitives.back();
        primitives.pop_back();
      }
    }
} scene;


END_RAYTRACER


#endif
