#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "bitmap/bitmap/bitmap_image.hpp"

#include "config.h"

#include "camera.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "texture.h"
#include "transform.h"
#include "utils.h"


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
      clock_t startTimer, endTimer;

      printf("Loading textures...");
      fflush(stdout);
      startTimer = clock();
      for (std::vector<Texture *>::iterator itr = textures.begin();
           itr != textures.end(); itr++) {
        (*itr)->loadFromFile();
      }
      endTimer = clock();
      printf("completed (%.3f seconds).\n", clockTime(startTimer, endTimer));
    }

    void init(char * inputFilename) {
      clock_t startTimer, endTimer;

      printf("Parsing scene...");
      fflush(stdout);
      startTimer = clock();
      parseFile(inputFilename, this);
      endTimer = clock();
      printf("completed (%.3f seconds).\n", clockTime(startTimer, endTimer));

      loadTextures();
    }
};


END_RAYTRACER


#endif
