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


inline int intersect(const Ray &ray, MODEL_CLS * model, Intercept intercepts[],
                     Material * entryMat);

inline int intersectMerge(const int op,
                          const int hitsLeft, Intercept interceptLeft[],
                          const int hitsRight, Intercept interceptRight[],
                          Intercept merged[]);

inline int intersectMerge(const int listNum, int hits[],
                          Intercept interceptLists[][MAX_INTERSECTIONS],
                          Intercept merged[]);

inline int trace(const Scene * scene, const int level, const C_FLT weight,
                 const Ray &ray, Color * color, Material * entryMat);


END_RAYTRACER


#endif
