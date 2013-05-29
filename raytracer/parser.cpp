// TODO(kent): Finish up the stubs for animation
#include <math.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "config.h"

#include "material.h"
#include "mesh.h"
#include "point.h"
#include "parser.h"
#include "scene.h"
#include "shapes.h"
#include "texture.h"
#include "transform.h"
#include "vector.h"

#define PI_DIV_180 0.01745329251994329576923690768489f


using namespace std;


BEGIN_RAYTRACER


char directoryPath[1024];
int globalDetailLevel = 0;


void fullFilePath(char * filepath, char * filename) {
  strncpy(filepath, directoryPath, strlen(directoryPath) + 1);
  strncat(filepath, filename, strlen(filename));
}

P_FLT degreesToRadians(const P_FLT degrees) {
  return degrees * PI_DIV_180;
}


void parseComment(FILE *file) {
  char str[1000];
  char * dummy = fgets(str, 1000, file);
}


void parseViewpoint(FILE *file, Scene * scene) {
  Point3D target, viewpoint;
  Vector3D up;
  P_FLT angle, hither;
  int imageWidth;
  int imageHeight;

  if ((fscanf(file, " from %f %f %f",
              &(viewpoint.x), &(viewpoint.y), &(viewpoint.z)) != 3) ||
      (fscanf(file, " at %f %f %f",
             &(target.x), &(target.y), &(target.z)) != 3) ||
      (fscanf(file, " up %f %f %f",
             &(up.x), &(up.y), &(up.z)) != 3) ||
      (fscanf(file, " angle %f", &angle) != 1) ||
      (fscanf(file, " hither %f", &hither) !=1) ||
      (fscanf(file, " resolution %d %d",
              &imageWidth, &imageHeight) !=2)) {
    printf("Viewpoint syntax error\n");
    exit(1);
  }

  scene->camera = new Camera(viewpoint, target, up, angle, hither, imageWidth,
                             imageHeight);
}


void parseLight(FILE * file, Scene * scene) {
  Point3D orig;
  Color color;
  int animated,
      dummy,
      num;
  char name[100];
  snprintf(name, sizeof(name), "");

  animated = getc(file);
  if (animated != 'a') {
     ungetc(animated, file);
     animated = 0;
  } else {
     dummy = fscanf(file, "%s", name);
  }

  if (fscanf(file, "%f %f %f ", &(orig.x), &(orig.y), &(orig.z)) != 3) {
     printf("Light source position syntax error");
     exit(1);
  }

  num = fscanf(file, "%f %f %f ", &(color.r), &(color.g), &(color.b));
  if (num == 0) {
    color = Color(1.0f);
  } else if (num != 3) {
     printf("Light source color syntax error");
     exit(1);
  }

  scene->lights.push_back(new Light(orig, color));
}


void parseBackground(FILE * file, Scene * scene) {
  Color bgColor;

  if (fscanf(file, "%f %f %f",
      &(bgColor.r), &(bgColor.g), &(bgColor.b)) != 3) {
     printf("background color syntax error");
     exit(1);
  }

  scene->backgroundColor = bgColor;
}


void parseFill(FILE * file, Scene * scene) {
  C_FLT shine, transmittance, refraction;
  Color ambience, diffuse, specular;
  int moreparams;
  Material * newMaterial;

  moreparams = getc(file);
  if (moreparams != 'm') {
     ungetc(moreparams, file);
     moreparams = 0;
  }

  if (moreparams) {
    if (fscanf(file, "%f %f %f",
        &(ambience.r), &(ambience.g), &(ambience.b)) != 3) {
      printf("fill material ambient syntax error");
      exit(1);
    }
    if (fscanf(file, "%f %f %f",
        &(diffuse.r), &(diffuse.g), &(diffuse.b)) != 3) {
      printf("fill material diffuse syntax error");
      exit(1);
    }
    if (fscanf(file, "%f %f %f",
        &(specular.r), &(specular.g), &(specular.b)) != 3) {
      printf("fill material specular syntax error");
      exit(1);
    }
    if (fscanf(file, "%f %f %f",
        &shine, &transmittance, &refraction) != 3) {
      printf("fill material (phong, transp, IOR) syntax error");
      exit(1);
    }

    newMaterial = new Material("Untitled",
                               ambience,
                               diffuse,
                               specular,
                               Color(transmittance,
                                     transmittance,
                                     transmittance),
                               refraction,
                               shine);
    scene->materials.push_back(newMaterial);
  } else {
    C_FLT kdiff, kspec;
    Color color;
    if (fscanf(file, "%f %f %f",
        &(color.r), &(color.g), &(color.b)) != 3) {
       printf("fill color syntax error");
       exit(1);
    }
    if (fscanf(file, "%f %f %f %f %f",
        &kdiff, &kspec, &shine, &transmittance, &refraction) != 5) {
       printf("fill material syntax error");
       exit(1);
    }

    newMaterial = new Material("Untitled",
                               color,
                               color * kdiff,
                               color * kspec,
                               Color(transmittance),
                               refraction,
                               shine);
    scene->materials.push_back(newMaterial);
  }
}


void parseCone(FILE * file, Scene * scene) {
  Point3D apexCenter, baseCenter;
  P_FLT apexRadius, baseRadius;
  if (fscanf(file, " %f %f %f %f %f %f %f %f",
             &baseCenter.x, &baseCenter.y, &baseCenter.z, &baseRadius,
             &apexCenter.x, &apexCenter.y, &apexCenter.z, &apexRadius) != 8) {
    printf("cylinder or cone syntax error\n");
    exit(1);
  }

  if (apexRadius == baseRadius) {
    scene->addStaticPrimitive(new Cylinder(scene->latestMat(), baseCenter,
                                           apexCenter, apexRadius));
  } else {
    printf("WARNING: Skipped unimplemented cone\n");
  }
}


void parseSphere(FILE * file, Scene * scene) {
  P_FLT radius;
  Point3D center;

  if (fscanf(file, "%f %f %f %f",
      &(center.x), &(center.y), &(center.z), &radius) != 4) {
    printf("sphere syntax error");
    exit(1);
  }

  scene->addStaticPrimitive(new Sphere(scene->latestMat(), center, radius));
}


void parsePoly(FILE * file, Scene * scene) {
  int patch,
      vertexNum;
  Point3D  * vertex;
  Vector3D * normal;

  patch = getc(file);
  if (patch != 'p') {
     ungetc(patch, file);
     patch = 0;
  }

  if (fscanf(file, "%d", &vertexNum) != 1) {
    printf("polygon or patch syntax error\n");
    exit(1);
  }

  vertex = new Point3D[vertexNum];
  if (vertex == NULL) {
    printf("can't allocate memory for polygon or patch\n");
    exit(1);
  }

  if (patch) {
    normal = new Vector3D[vertexNum];
    if (normal == NULL) {
      printf("can't allocate memory for polygon or patch\n");
      exit(1);
    }

    for (int i = 0; i < vertexNum; i++) {
      if (fscanf(file, " %f %f %f %f %f %f",
                 &vertex[i].x, &vertex[i].y, &vertex[i].z,
                 &normal[i].x, &normal[i].y, &normal[i].z) != 6) {
        printf("polygon or patch syntax error\n");
        exit(1);
      }
    }

    scene->addStaticPrimitive(new PolygonPatch(scene->latestMat(), vertexNum,
                                               vertex, normal));
  } else {
    for (int i = 0; i < vertexNum; i++) {
      if (fscanf(file, " %f %f %f",
                 &vertex[i].x, &vertex[i].y, &vertex[i].z) != 3) {
        printf("polygon or patch syntax error\n");
        exit(1);
      }
    }

    scene->addStaticPrimitive(new Polygon(scene->latestMat(), vertexNum,
                                          vertex));
  }
}

void parseInclude(FILE * file, Scene * scene) {
  char filename[80],
       filepath[1024];
  FILE * includeFile;
  int detailLevel;

  if (fscanf(file, "%d %s", &detailLevel, filename) != 2) {
    printf("could not parse include file: %s\n", filename);
    exit(0);
  }

  if (detailLevel <= globalDetailLevel) {
    fullFilePath(filepath, filename);
    parseFile(filepath, scene);
  } else {
    printf("Skipping include file: %s\n", filename);
  }
}


void parseDetailLevel(FILE * file) {
  if (fscanf(file, "%d", &globalDetailLevel) != 1) {
    printf("Error: could not parse detail level.\n");
    exit(1);
  }
}


void parseTexturedTriangle(FILE *file, Scene * scene) {
  char filepath[1024],
       textureName[200];
  int patch;
  P_FLT x, y, z;
  Point3D vertex[3];
  Vector2D texCoord[3];
  Vector3D normal[3];

  patch = getc(file);
  if (patch != 'p') {
     ungetc(patch, file);
     patch = 0;
  }

  if (fscanf(file, "%s", textureName) != 1) {
    printf("Error: could not parse texture name of textured triangle\n");
    exit(1);
  }

  fullFilePath(filepath, textureName);
  Texture * texture = scene->getOrCreateTexture(filepath);

  for (int i = 0; i < 3; i++) {
    if (fscanf(file, " %f %f %f",
               &vertex[i].x, &vertex[i].y, &vertex[i].z) != 3) {
      printf("Error: could not parse textured triangle\n");
      exit(1);
    }

    if (patch) {
      if (fscanf(file, " %f %f %f",
                 &normal[i].x, &normal[i].y, &normal[i].z) != 3) {
        printf("Error: could not parse textured triangle\n");
        exit(1);
      }
    }

    if (fscanf(file, " %f %f ",
        &texCoord[i].x, &texCoord[i].y) != 2) {
      printf("Error: could not parse textured triangle\n");
      exit(1);
    }
  }

  if (patch) {
    scene->addStaticPrimitive(
      new PhongTriangle(scene->latestMat(), texture,
                        vertex[0], vertex[1], vertex[2],
                        normal[0], normal[1], normal[2],
                        texCoord[0], texCoord[1],
                        texCoord[2]));
  } else {
    scene->addStaticPrimitive(
      new TexturedTriangle(scene->latestMat(), texture,
                           vertex[0], vertex[1], vertex[2],
                           texCoord[0], texCoord[1],
                           texCoord[2]));
  }
}


void parseAnimatedTriangle(FILE *file, Scene * scene) {
  int frameTimeNum;
  if (fscanf(file, "%d", &frameTimeNum) != 1) {
    printf("Error: could not parse animated triangle (tpa)\n");
    exit(1);
  };

  P_FLT time;
  Point3D vertex[3];
  Vector3D normal[3];

  TimedPrimitive * timedPrim = new TimedPrimitive();

  for (int i = 0; i < frameTimeNum; i++) {
    if (fscanf(file, " %f", &time) != 1) {
      printf("Error: could not parse animated triangle (tpa)\n");
      exit(1);
    }

    for (int j = 0; j < 3; j++) {
      if (fscanf(file, " %f %f %f",
                 &vertex[j].x, &vertex[j].y, &vertex[j].z) != 3) {
        printf("Error: could not parse animated triangle (tpa)\n");
        exit(1);
      }

      if (fscanf(file, " %f %f %f",
                 &normal[j].x, &normal[j].y, &normal[j].z) != 3) {
        printf("Error: could not parse animated triangle (tpa)\n");
        exit(1);
      }
    }

    timedPrim->addFrame(time,
                        new TrianglePatch(scene->latestMat(),
                                          vertex[0], vertex[1], vertex[2],
                                          normal[0], normal[1], normal[2]));
  }

  scene->addTimedPrimitive(timedPrim);
}


void parseTextureStuff(FILE * file, Scene * scene) {
  char inputChar;

  inputChar = getc(file);
  if (inputChar =='t') {
    parseTexturedTriangle(file, scene);
  } else if (inputChar == 'p') {
    inputChar = getc(file);
    if (inputChar == 'a') {
      parseAnimatedTriangle(file, scene);
    }
  } else {
    printf("Error: tt and ttp are valid codes (not t%c).\n", inputChar);
    exit(1);
  }
}


void eatWhitespace(FILE * file) {
  char ch = getc(file);
  while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\f' || ch == '\r') {
    ch = getc(file);
  }
  ungetc(ch, file);
}


void parseKeyFrames(FILE *file) {
  char name[200];
  char motion[200];
  char ch;
  int  c;
  int visibility;
  int  ret, i, nKeyFrames;
  float time, x, y, z, angle, te, co, bi;

  if (fscanf(file, "%s", name) != 1) {
     printf("Error: could not read name of animation.\n");
     exit(1);
  }
  eatWhitespace(file);
  ch = getc(file);
  if (ch != '{') {
     printf("Error: could not find a { in animation %s.\n", name);
     exit(1);
  }

  eatWhitespace(file);
  while ( (c = getc(file)) != '}' ) {
    ungetc(c, file);
    if (fscanf(file, "%s %d", motion, &nKeyFrames) != 2) {
      printf("Error: could not read name of motion or"
             " number of keyframes for animation.\n");
      exit(1);
    }

    if (nKeyFrames < 4 && strcmp("visibility", motion)) {
      printf("Error: there must be at least 4 keyframes for %s.\n", name);
      exit(1);
    }

    if (!strcmp(motion, "transl")) {
      for (i = 0; i < nKeyFrames; i++) {
          ret = fscanf(file, " %f %f %f %f %f %f %f",
                       &time, &x, &y, &z, &te, &co, &bi);
          if (ret != 7) {
             printf("error in parsing translation keyframes for %s\n", "");
             exit(1);
          }
       }
    } else if (!strcmp(motion, "rot")) {
      for (i = 0; i < nKeyFrames; i++) {
         ret = fscanf(file, " %f %f %f %f %f %f %f %f", &time, &x, &y, &z,
              &angle, &te, &co, &bi);
         if (ret != 8) {
            printf("error in parsing rotation keyframes for %s\n", "");
            exit(1);
         }
      }
    } else if (!strcmp(motion, "scale")) {
      for (i = 0; i < nKeyFrames; i++) {
         ret = fscanf(file, " %f %f %f %f %f %f %f", &time, &x, &y, &z,
              &te, &co, &bi);
         if (ret != 7) {
            printf("error in parsing scale keyframes for %s\n", "");
            exit(1);
         }
      }
    } else if (!strcmp(motion, "visibility")) {
      for (i = 0; i < nKeyFrames; i++) {
         ret = fscanf(file, " %f %d", &time, &visibility);
         if (ret != 2) {
            printf("error in parsing visibility keyframes for %s\n", "");
            exit(1);
         }
      }
    } else {
      printf("Error: unknown keyframe type (%s)."
             " Must be transl, rot, or scale.\n",
             motion);
      exit(1);
    }
    eatWhitespace(file);
  }
}


void parseXform(FILE * f, Scene * scene) {
  char name[100];
  char ch;

  ch = getc(f);
  if (ch != 's') {
    // TODO(kent): Implement animation transforms
    ungetc(ch, f);

    if (fscanf(f, "%s", name) != 1) {
      printf("Error: could not read transform name.\n");
      exit(1);
    }
    eatWhitespace(f);
    ch = getc(f);
    if (ch != '{') {
      printf("Error: { expected.\n");
      exit(1);
    }

  } else {
    // Static transform
    P_FLT xScale, yScale, zScale, degrees;
    Vector3D rotateAxis, translate;
    Transform* newTransform;

    if (fscanf(f, " %f %f %f %f %f %f %f %f %f %f",
               &xScale, &yScale, &zScale,
               &rotateAxis.x, &rotateAxis.y, &rotateAxis.z, &degrees,
               &translate.x, &translate.y, &translate.z) != 10) {
     printf("Error: could not read static transform.\n");
     exit(1);
    }

    eatWhitespace(f);
    ch = getc(f);
    if (ch != '{') {
      printf("Error: { expected.\n");
      exit(1);
    }

    newTransform = new Transform(xScale, yScale, zScale,
                                 degreesToRadians(degrees),
                                 rotateAxis, translate);
    if (!scene->staticTransforms.empty()) {
      *newTransform = (*scene->staticTransforms.top()) * (*newTransform);
    }
    scene->staticTransforms.push(newTransform);
  }
}


void parseEndXform(FILE * file, Scene * scene) {
  if (scene->staticTransforms.empty()) {
    // NOTE(kenji): it might hint a malformed AFF file
    // We cannot throw exception here since '}' also closes animated
    // transform, and currently Scene::staticTransforms only handle static transform
    return;
  }

  delete scene->staticTransforms.top();
  scene->staticTransforms.pop();
}


void parseAnimParams(FILE * file, Scene * scene) {
  if (fscanf(file, "%f %f %d",
             &scene->startTime, &scene->endTime, &scene->frameNum) != 3) {
     printf("Error: could not parse animations parameters.\n");
     exit(1);
  }
}


void parseA(FILE * file, Scene * scene) {
  char name[100];
  char ch;
  int ambient;

  ambient = getc(file);
  if (ambient != 'm') {
    ungetc(ambient, file);
    parseAnimParams(file, scene);
  } else {
    Color ambience;
    if (fscanf(file, "%f %f %f",
               &ambience.r, &ambience.g, &ambience.b) != 3) {
      printf("Error: could not parse ambient light (am).\n");
      exit(1);
    }

    scene->ambience = ambience;
  }
}


void getPoints(FILE * file, const char * type, vector<Point3D> * points) {
  int pointNum;
  Point3D point;

  if (fscanf(file, "%d", &pointNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }

  for (int i = 0; i < pointNum; i++) {
    if (fscanf(file, "%f %f %f ", &point.x, &point.y, &point.z) != 3) {
      printf("Error: could not read %d %ss of mesh.\n", pointNum, type);
      exit(1);
    }

    points->push_back(point);
  }
}


void getVectors(FILE * file, const char * type, vector<Vector3D> * vectors) {
  int vectorNum;
  Vector3D vector;

  if (fscanf(file, "%d", &vectorNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }

  for (int i = 0; i < vectorNum; i++) {
    if (fscanf(file, "%f %f %f ", &vector.x, &vector.y, &vector.z) != 3) {
      printf("Error: could not read %d %ss of mesh.\n", vectorNum, type);
      exit(1);
    }

    vectors->push_back(vector);
  }
}


void getTexCoordinates(FILE * file, const char * type, char * textureName,
                       vector<Vector2D> * texCoords) {
  int flag, texCoordNum;
  P_FLT x, y;

  if (fscanf(file, "%d", &texCoordNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }
  if (fscanf(file, "%s", textureName) != 1) {
    printf("Error: could not parse texture name.\n");
    exit(1);
  }

  for (int i = 0; i < texCoordNum; i++) {
    if (fscanf(file, "%f %f", &x, &y) != 2) {
      printf("Error: could not read %d %ss of mesh.\n", texCoordNum, type);
      exit(1);
    }

    texCoords->push_back(Vector2D(x, y));
  }
}


void getTriangleDefs(FILE * file, bool hasNorms, bool hasTextures,
                     vector<int *> * triangleDefs) {
  int definitionSize, index, triangleNum;

  if (fscanf(file, "%d", &triangleNum) != 1) {
    printf("Error: could not parse mesh (expected 'triangleNum').\n");
    exit(1);
  }

  definitionSize = 3;
  if (hasNorms) {
    definitionSize += 3;
  }
  if (hasTextures) {
    definitionSize += 3;
  }

  for (int i = 0; i < triangleNum; i++) {
    int * triangleDef = new int[definitionSize];
    if (fscanf(file, "%d %d %d",
               &triangleDef[0], &triangleDef[1], &triangleDef[2]) != 3) {
      printf("Error: could not read vertices of mesh.\n");
      exit(1);
    }
    index = 3;

    if (hasNorms) {
      if (fscanf(file, "%d %d %d",
                 &triangleDef[index],
                 &triangleDef[index + 1],
                 &triangleDef[index + 2]) != 3) {
         printf("Error: could not read normal indices of mesh.\n");
         exit(1);
      }
      index = 6;
    }

    if (hasTextures) {
      if (fscanf(file, "%d %d %d",
                 &triangleDef[index],
                 &triangleDef[index + 1],
                 &triangleDef[index + 2]) != 3) {
         printf("Error: could not read texturecoord indices of mesh.\n");
         exit(1);
      }
    }

    triangleDefs->push_back(triangleDef);
  }
}


void parseMesh(FILE * file, Scene * scene) {
  char textureName[200],
       buffer[200];
  int flag;
  vector<Point3D> vertex;
  vector<Vector3D> normal;
  vector<Vector2D> texCoords;
  vector<int *> triangleDefs;
  Texture * texture = NULL;


  if (fscanf(file, "%s", buffer) != 1) {
    printf("Error: could not parse mesh (could not find 'vertices').\n");
    exit(1);
  }

  if (strcmp(buffer, "vertices")) {
    printf("%s\n", buffer);
    printf("Error: could not parse mesh (expected 'vertices').\n");
    exit(1);
  }
  getPoints(file, "vertex", &vertex);

  flag = fscanf(file, "%s", buffer);
  if (!strcmp(buffer, "normals")) {
    getVectors(file, "normal", &normal);
    flag = fscanf(file, "%s", buffer);
  }
  if (!strcmp(buffer, "texturecoords")) {
    getTexCoordinates(file, "texCoord", textureName, &texCoords);

    char filepath[1024];
    fullFilePath(filepath, textureName);
    texture = scene->getOrCreateTexture(filepath);

    flag = fscanf(file, "%s", buffer);
  }

  if (!strcmp(buffer, "triangles")) {
    getTriangleDefs(file, normal.size() > 0, texCoords.size() > 0,
                    &triangleDefs);
  } else {
    printf("Error: expected 'triangles' in mesh.\n");
    exit(1);
  }

  TriangleMesh * mesh = new TriangleMesh(scene->latestMat(), texture, vertex,
                                         normal, texCoords, triangleDefs);
  scene->addStaticPrimitive(mesh);
  scene->meshes.push_back(mesh);
}


int parseFile(const char * filename, Scene * scene) {
  FILE * file;

  if (!(file = fopen(filename, "r"))) {
    printf("File %s not found.\n", filename);
    return 1;
  }
  strncpy(directoryPath, filename, 1024);
  *(strrchr(directoryPath, '/') + 1) = '\0';

  char last;
  int ch;
  while ((ch = getc(file)) != EOF) {
    switch (ch) {
      case ' ':
      case '\t':
      case '\n':
      case '\f':
      case '\r':
        continue;
      case '#':
      case '%':
        parseComment(file);
        break;
      case 'v':
        parseViewpoint(file, scene);
        break;
      case 'l':
        parseLight(file, scene);
        break;
      case 'b':
        parseBackground(file, scene);
        break;
      case 'f':
        parseFill(file, scene);
        break;
      case 'c':
        parseCone(file, scene);
        break;
      case 's':
        parseSphere(file, scene);
        break;
      case 'p':
        parsePoly(file, scene);
        break;
      case 'i':
        parseInclude(file, scene);
        break;
      case 'd':
        parseDetailLevel(file);
        break;
      case 't':
        parseTextureStuff(file, scene);
        break;
      case 'x':
        parseXform(file, scene);
        break;
      case '}':
        parseEndXform(file, scene);
        break;
      case 'a':
        parseA(file, scene);
        break;
      case 'k':
        parseKeyFrames(file);
        break;
      case 'm':
        parseMesh(file, scene);
        break;
      default:
        printf("unknown NFF primitive code: %c\n", ch);
        printf("previous was %c\n", last);
        exit(1);
    }
  }

  return 0;
}


END_RAYTRACER
