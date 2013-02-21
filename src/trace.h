#ifndef TRACE_H
#define TRACE_H


#include "config.h"


BEGIN_RAYTRACER


class Color;
class Intercept;
class MODEL_CLS;
class Material;
class Ray;
class Scene;


int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[]);

inline int intersectMergeTwo(const int hitsLeft, Intercept interceptLeft[],
                             const int hitsRight, Intercept interceptRight[],
                             Intercept merged[]);

inline int intersectMergeMulti(const int listNum, int hits[],
                               Intercept interceptLists[][MAX_INTERSECTIONS],
                               Intercept merged[]);

int trace(const Scene * scene, const int level, const C_FLT weight,
          const Ray &ray, Color * color);


END_RAYTRACER


#endif
