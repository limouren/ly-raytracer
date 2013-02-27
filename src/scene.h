#ifndef SCENE_H
#define SCENE_H


#include <math.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "config.h"

#include "material.h"
#include "mesh.h"
#include "primitive.h"
#include "tree.h"
#include "utils.h"


BEGIN_RAYTRACER


class Camera;
class Color;
class Light;
class Texture;
class Transform;


class Scene {
  public:
    Node * sceneRoot;
    Camera * camera;
    Color ambience, backgroundColor;
    Material * medium;
    std::vector<Light *> lights;
    std::vector<Material *> materials;
    // Keep track of meshes for later initialization
    std::vector<TriangleMesh *> meshes;
    std::vector<Primitive *> primitives;
    std::vector<Texture *> textures;
    std::vector<Transform *> transforms;

    Scene(): sceneRoot(NULL) {
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
      if (sceneRoot != NULL && sceneRoot->type != 0) {
        delete sceneRoot;
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

    void buildNodeTree() {
      clock_t startTimer, endTimer;

      printf("Building node tree...");
      fflush(stdout);
      startTimer = clock();
      for (std::vector<TriangleMesh *>::iterator itr = meshes.begin();
           itr != meshes.end(); itr++) {
        (*itr)->init();
      }
      sceneRoot = buildBVHTree(primitives.begin(), primitives.end());
      endTimer = clock();
      printf("completed (%.3f seconds).\n", clockTime(startTimer, endTimer));
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
