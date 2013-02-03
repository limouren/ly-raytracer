// TODO(kent): Finish up the stubs for animation
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "config.h"

#include "material.h"
#include "mesh.h"
#include "point.h"
#include "parser.h"
#include "scene.h"
#include "screen.h"
#include "shapes.h"
#include "vector.h"


// TODO(kent): Get rid of these
#define X 0
#define Y 1
#define Z 2
typedef float Vec2f[2];
typedef float Vec3f[3];


using namespace std;


BEGIN_RAYTRACER


int globalDetailLevel = 0;


void parseComment(FILE *file) {
  char str[1000];
  char * dummy = fgets(str, 1000, file);
}


void parseViewpoint(FILE *file, Camera * camera) {
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

  camera->init(viewpoint, target, up, angle, hither, imageWidth, imageHeight);
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


void parseFill(FILE * file, Scene * scene, Material ** currentMaterial) {
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

  *currentMaterial = newMaterial;
}


void parseCone(FILE * file, vector<MODEL_CLS *> &models,
               Material * currentMaterial) {
  Point3D apex, base;
  P_FLT radiusApex, radiusBase;

  if (fscanf(file, " %f %f %f %f %f %f %f %f",
            &base.x, &base.y, &base.z, &radiusBase,
            &apex.x, &apex.y, &apex.z, &radiusApex) !=8) {
    printf("cylinder or cone syntax error\n");
    exit(1);
  }
  // TODO(kent): Handle cones
}


void parseSphere(FILE * file, vector<MODEL_CLS *> &models,
                 Material * currentMaterial) {
  P_FLT radius;
  Point3D center;

  if (fscanf(file, "%f %f %f %f",
      &(center.x), &(center.y), &(center.z), &radius) != 4) {
    printf("sphere syntax error");
    exit(1);
  }

  models.push_back(new Sphere(currentMaterial, center, radius));
}


void parsePoly(FILE * file, vector<MODEL_CLS *> &models,
               Material * currentMaterial) {
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

    // TODO(kent): Implement polygon patch
    models.push_back(new Polygon(currentMaterial, vertexNum, vertex));
  } else {
    for (int i = 0; i < vertexNum; i++) {
      if (fscanf(file, " %f %f %f",
                 &vertex[i].x, &vertex[i].y, &vertex[i].z) != 3) {
        printf("polygon or patch syntax error\n");
        exit(1);
      }
    }

    models.push_back(new Polygon(currentMaterial, vertexNum, vertex));
  }
}


void parseInclude(FILE * file, char * directoryPath, Scene * scene,
                  Camera * camera, Screen * screen, vector<MODEL_CLS *> &models,
                  Material * currentMaterial) {
  char filename[100];
  FILE * includeFile;
  int detailLevel;

  if (fscanf(file, "%d %s", &detailLevel, filename) != 2) {
    printf("could not parse include file\n", filename);
    exit(0);
  }

  if (detailLevel <= globalDetailLevel) {
    strncat(directoryPath, filename, strlen(filename));

    Scene subscene;
    parseFile(directoryPath, &subscene, camera, screen, currentMaterial);

    if (subscene.modelRoot != NULL) {
      models.push_back(subscene.modelRoot);
    }
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


// TODO(kent): Handle textured triangles
void parseTexturedTriangle(FILE *file)
{
   int is_patch;
   int q;
   Vec3f verts[3];
   Vec3f norms[3];
   float tu[3],tv[3];
   char texturename[100];

   is_patch=getc(file);
   if (is_patch!='p')
   {
      ungetc(is_patch,file);
      is_patch=0;
   }

   int dummy = fscanf(file,"%s",texturename);

   for(q=0;q<3;q++)
   {
      if (fscanf(file," %f %f %f",&verts[q][X],&verts[q][Y],&verts[q][Z])!=3)
    goto parseErr;

      if (is_patch)
      {
    if (fscanf(file," %f %f %f",&norms[q][X],&norms[q][Y],&norms[q][Z])!=3)
       goto parseErr;
      }

      if (fscanf(file," %f %f ",&tu[q],&tv[q])!=2)
    goto parseErr;
   }


   if (is_patch)
   {
      /* add a textured triangle patch here
       * e.g., viAddTexturedTriPatch(texturename,verts,norms,tu,tv);
       */
   }
   else
   {
      /* add a textured triangle here
       * e.g.,  viAddTexturedTriangle(texturename,verts,tu,tv);
       */
   }
   return;

 parseErr:
   printf("Error: could not parse textured triangle\n");
   exit(1);

}


void parseAnimatedTriangle(FILE *file)
{
   int q,w;
   int num_times;
   Vec3f *verts;
   Vec3f *norms;
   float *times;

   int dummy = fscanf(file,"%d",&num_times);
   times=(float*)malloc(sizeof(float)*num_times);
   verts=(Vec3f*)malloc(sizeof(Vec3f)*3*num_times);
   norms=(Vec3f*)malloc(sizeof(Vec3f)*3*num_times);

   for(q=0;q<num_times;q++)
   {
      if (fscanf(file," %f",&times[q])!=1)
    goto parseErr;

      for(w=0;w<3;w++)
      {
    if (fscanf(file," %f %f %f",&verts[q*3+w][X],&verts[q*3+w][Y],&verts[q*3+w][Z])!=3)
       goto parseErr;


    if (fscanf(file," %f %f %f",&norms[q*3+w][X],&norms[q*3+w][Y],&norms[q*3+w][Z])!=3)
       goto parseErr;
      }
   }

   /* add a animated triangle here
    * e.g., viAddAnimatedTriangle(num_times,times,verts,norms);
    */
   return;

 parseErr:
   printf("Error: could not parse animated triangle (tpa)\n");
   exit(1);

}


void parseTextureStuff(FILE *file)
{
   int is_triangle;

   is_triangle=getc(file);
   if (is_triangle=='t')
   {
      parseTexturedTriangle(file);
   }
   else if (is_triangle=='p')
   {
      is_triangle=getc(file);
      if (is_triangle=='a')
      {
    parseAnimatedTriangle(file);
      }
   }
   else
   {
      printf("Error: tt and ttp are valid codes (not t%c).\n",(char)is_triangle);
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


void parseKeyFrames(FILE *file)
{
  char name[200];
  char motion[200];
  char ch;
  int  c;
  int visibility;
  int  ret, i, nKeyFrames;
  float time, x, y, z, angle, te, co, bi;

  if (fscanf(file,"%s",name)!=1)
  {
     printf("Error: could not read name of animation.\n");
     exit(1);
  }
  eatWhitespace(file);
  ch=getc(file);
  if (ch!='{')
  {
     printf("Error: could not find a { in animation %s.\n",name);
     exit(1);
  }

  eatWhitespace(file);
  while ( (c = getc(file)) != '}' )
  {
    ungetc(c, file);
    if (fscanf(file, "%s %d", motion, &nKeyFrames)!=2)
    {
      printf("Error: could not read name of motion or number of keyframes for animation.\n");
      exit(1);
    }

    if (nKeyFrames<4 && strcmp("visibility", motion))
    {
      printf("Error: there must be at least 4 keyframes for %s.\n",name);
      exit(1);
    }

    if (!strcmp(motion, "transl")) {
      for( i=0; i<nKeyFrames; i++ ) {
          ret = fscanf(file, " %f %f %f %f %f %f %f", &time, &x, &y, &z,
               &te, &co, &bi);
          if (ret != 7)
          {
             printf("error in parsing translation keyframes for %s\n", "");
             exit(1);
          }
       }
    } else if (!strcmp (motion, "rot")) {
      for( i=0; i<nKeyFrames; i++ ) {
         ret = fscanf(file," %f %f %f %f %f %f %f %f", &time, &x, &y, &z,
              &angle, &te, &co, &bi);
         if (ret != 8) {
            printf("error in parsing rotation keyframes for %s\n", "");
            exit(1);
         }
      }
    } else if (!strcmp(motion, "scale")) {
      for( i=0; i<nKeyFrames; i++ ) {
         ret = fscanf(file, " %f %f %f %f %f %f %f", &time, &x, &y, &z,
              &te, &co, &bi);
         if (ret != 7) {
            printf("error in parsing scale keyframes for %s\n", "");
            exit(1);
         }
      }
    } else if (!strcmp(motion, "visibility")) {
      for( i=0; i<nKeyFrames; i++ ) {
         ret = fscanf(file, " %f %d", &time, &visibility);
         if (ret != 2) {
            printf("error in parsing visibility keyframes for %s\n", "");
            exit(1);
         }
      }
    } else {
      printf("Error: unknown keyframe type (%s). Must be transl, rot, or scale.\n",
             motion);
      exit(1);
    }
    eatWhitespace(file);
  }
}


void parseXform(FILE *f)
{
  char name[100];
  char ch;
  int is_static;

  is_static = getc(f);
  if (is_static != 's')
  {
     ungetc(is_static, f);
     is_static=0;
  }

  if (is_static) {
    Vec3f scale, trans, rot;
    float deg;

    if (fscanf(f," %f %f %f %f %f %f %f %f %f %f",
               &scale[0], &scale[1], &scale[2],
               &rot[0], &rot[1], &rot[2], &deg,
               &trans[0], &trans[1], &trans[2]) != 10) {
     printf("Error: could not read static transform.\n");
     exit(1);
    }
    eatWhitespace(f);
    ch=getc(f);
    if (ch!='{') {
      printf("Error: { expected.\n");
      exit(1);
    }
  } else {
    if (fscanf(f,"%s",name)!=1) {
      printf("Error: could not read transform name.\n");
      exit(1);
    }
    eatWhitespace(f);
    ch=getc(f);
    if (ch!='{') {
      printf("Error: { expected.\n");
      exit(1);
    }
  }
}


void parseAnimParams(FILE *file)
{
  float start,end;
  int num_frames;
  if (fscanf(file,"%f %f %d",&start,&end,&num_frames)!=3)
  {
     printf("Error: could not parse animations parameters.\n");
     exit(1);
  }
  /* add animations parameters here
   * e.g., viSetupAnimParams(start,end,num_frames);
   */
}


void parseA(FILE * file, Scene * scene) {
  char name[100];
  char ch;
  int ambient;

  ambient = getc(file);
  if (ambient != 'm') {
    ungetc(ambient, file);
    parseAnimParams(file);
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


void getPoints(FILE * file, const char * type, vector<Point3D> &points) {
  int pointNum;
  P_FLT x, y, z;

  if (fscanf(file, "%d", &pointNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }

  for (int i = 0; i < pointNum; i++) {
    if (fscanf(file, "%f %f %f ", &x, &y, &z) != 3) {
      printf("Error: could not read %d %ss of mesh.\n", pointNum, type);
      exit(1);
    }

    points.push_back(Point3D(x, y, z));
  }
}


void getVectors(FILE * file, const char * type, vector<Vector3D> &vectors) {
  int vectorNum;
  P_FLT x, y, z;

  if (fscanf(file, "%d", &vectorNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }

  for (int i = 0; i < vectorNum; i++) {
    if (fscanf(file, "%f %f %f ", &x, &y, &z) != 3) {
      printf("Error: could not read %d %ss of mesh.\n", vectorNum, type);
      exit(1);
    }

    vectors.push_back(Vector3D(x, y, z));
  }
}


void getTextureCoords(FILE * file, const char * type,
                      vector<Vector2D> &vectors) {
  int vectorNum;
  P_FLT x, y;

  if (fscanf(file, "%d", &vectorNum) != 1) {
    printf("Error: could not parse mesh (expected '%sNum').\n", type);
    exit(1);
  }

  for (int i = 0; i < vectorNum; i++) {
    if (fscanf(file, "%f %f", &x, &y) != 2) {
      printf("Error: could not read %d %ss of mesh.\n", vectorNum, type);
      exit(1);
    }

    vectors.push_back(Vector2D(x, y));
  }
}


void getTriangleDefs(FILE * file, bool hasNorms,
                     bool hasTextures, vector<int *>& triangleDefs) {
  int definitionSize, index,
      normal1, normal2, normal3,
      texture1, texture2, texture3,
      vertex1, vertex2, vertex3,
      triangleNum;

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
    if (fscanf(file, "%d %d %d ",
               &triangleDef[0], &triangleDef[1], &triangleDef[2]) != 3) {
      printf("Error: could not read vertices of mesh.\n");
      exit(1);
    }
    index = 3;

    if (hasNorms) {
      if (fscanf(file,"%d %d %d",
                 &triangleDef[index],
                 &triangleDef[index + 1],
                 &triangleDef[index + 2]) != 3)
      {
         printf("Error: could not read normal indices of mesh.\n");
         exit(1);
      }
      index = 6;
    }

    if (hasTextures) {
      if (fscanf(file,"%d %d %d",
                 &triangleDef[index],
                 &triangleDef[index + 1],
                 &triangleDef[index + 2]) != 3)
      {
         printf("Error: could not read texturecoord indices of mesh.\n");
         exit(1);
      }
    }

    triangleDefs.push_back(triangleDef);
  }
}


void parseMesh(FILE * file, vector<MODEL_CLS *> &models,
               Material * currentMaterial) {
  char buffer[200],
       texturename[200];
  int flag;
  vector<Point3D> vertex;
  vector<Vector3D> normal;
  vector<Vector2D> textureCoords;
  vector<int *> triangleDefs;


  if (fscanf(file, "%s", buffer) != 1) {
    printf("Error: could not parse mesh (could not find 'vertices').\n");
    exit(1);
  }

  if (strcmp(buffer, "vertices")) {
    printf("%s\n", buffer);
    printf("Error: could not parse mesh (expected 'vertices').\n");
    exit(1);
  }
  getPoints(file, "vertex", vertex);

  flag = fscanf(file, "%s", buffer);
  if (!strcmp(buffer, "normals")) {
    getVectors(file, "normal", normal);
    flag = fscanf(file, "%s", buffer);
  }
  if (!strcmp(buffer, "texturecoords")) {
    getTextureCoords(file, "textureCoord", textureCoords);
    flag = fscanf(file, "%s", buffer);
  }

  if (!strcmp(buffer, "triangles")) {
    getTriangleDefs(file, !normal.empty(), !textureCoords.empty(),
                    triangleDefs);
  } else {
    printf("Error: expected 'triangles' in mesh.\n");
    exit(1);
  }

  models.push_back(new TriangleMesh(currentMaterial, vertex, normal,
                                    textureCoords, triangleDefs));
}


int parseFile(char * filename, Scene * scene, Camera * camera,
              Screen * screen, Material * previousMaterial) {
  FILE * file;

  if (!(file = fopen(filename, "r"))) {
    printf("File %s not found.\n", filename);
    return 1;
  }

  vector<MODEL_CLS *> models;
  Material * currentMaterial = previousMaterial;

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
        parseViewpoint(file, camera);
        break;
      case 'l':
        parseLight(file, scene);
        break;
      case 'b':
        parseBackground(file, scene);
        break;
      case 'f':
        parseFill(file, scene, &currentMaterial);
        break;
      case 'c':
        parseCone(file, models, currentMaterial);
        break;
      case 's':
        parseSphere(file, models, currentMaterial);
        break;
      case 'p':
        parsePoly(file, models, currentMaterial);
        break;
      case 'i':
        char directoryPath[1024];
        strncpy(directoryPath, filename, 1024);
        *(strrchr(directoryPath, '/') + 1)= '\0';

        parseInclude(file, directoryPath, scene, camera, screen, models,
                     currentMaterial);
        break;
      case 'd':
        parseDetailLevel(file);
        break;
      case 't':
        parseTextureStuff(file);
        break;
      case 'x':
        parseXform(file);
        break;
      case '}':
        // TODO(Kent): WTF is this?
        // viEndXform();
        break;
      case 'a':
        parseA(file, scene);
        break;
      case 'k':
        parseKeyFrames(file);
        break;
      case 'm':
        parseMesh(file, models, currentMaterial);
        break;
      default:
        printf("unknown NFF primitive code: %c\n", ch);
        printf("previous was %c\n", last);
        exit(1);
    }
  }

  scene->modelRoot = buildModelTree(models);
  return 0;
}


END_RAYTRACER
