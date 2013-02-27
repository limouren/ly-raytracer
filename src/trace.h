#ifndef TRACE_H
#define TRACE_H


#include "config.h"


BEGIN_RAYTRACER


class Color;
class Intercept;
class Material;
class Node;
class Ray;
class Scene;


int intersect(const Ray &ray, Node * node, Intercept intercepts[]);
int intersect(const Ray &ray, Node * node, Intercept intercepts[],
              P_FLT * tCeil);

inline int intersectMergeTwo(const int hitsLeft, Intercept interceptLeft[],
                             const int hitsRight, Intercept interceptRight[],
                             Intercept merged[]);

inline int intersectMergeMulti(const int listNum, int hits[],
                               Intercept interceptLists[], Intercept merged[]);

int trace(const Scene * scene, const int level, const C_FLT weight,
          const Ray &ray, Color * color);


END_RAYTRACER


#endif
