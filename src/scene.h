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
#include "transform.h"
#include "triangle.h"


BEGIN_RAYTRACER


class Scene {
  public:
    MODEL_CLS * modelRoot;
    Camera * camera;
    Color ambience, backgroundColor;
    Material * medium;
    std::vector<Light *> lights;
    std::vector<Material *> materials;
    std::vector<MODEL_CLS *> primitives;
    std::vector<Texture *> textures;
    std::vector<Transform *> transforms;

    Scene(): modelRoot(NULL) {
      medium = new Material("Vacuum",
                            Color(0.0f, 0.0f, 0.0f),
                            Color(0.0f, 0.0f, 0.0f),
                            Color(0.0f, 0.0f, 0.0f),
                            Color(1.0f, 1.0f, 1.0f),
                            1.0f,
                            0.0f);
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

      // Check for empty or single element scene
      if (modelRoot != NULL && modelRoot->type != 0) {
        delete modelRoot;
      }
      while (!primitives.empty()) {
        delete primitives.back();
        primitives.pop_back();
      }
    }

    void addPrimitive(Primitive * primitive, Transform * staticTransform) {
      if (staticTransform) {
        primitive->transform(staticTransform);
      }

      primitives.push_back(primitive);
    }

    void buildModelTree() {
      modelRoot = buildModelTreeNode(primitives, 0);
    }

    Material * latestMat() {
      if (materials.empty()) {
        printf("ERROR: Cannot create primitive with no material\n");
        exit(1);
      }

      return materials.back();
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
      printf("Loading textures...");
      for (std::vector<Texture *>::iterator itr = textures.begin();
           itr != textures.end(); itr++) {
        (*itr)->loadFromFile();
      }
      printf("completed.\n");
    }

    void init(char * inputFilename) {
      printf("Parsing file \"%s\"...", inputFilename);
      fflush(stdout);
      parseFile(inputFilename, this);
      printf("completed.\n");

      loadTextures();
    }
};


END_RAYTRACER


#endif
