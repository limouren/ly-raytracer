#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RAYTRACER_NAMESPACE raytracer
#define BEGIN_RAYTRACER namespace RAYTRACER_NAMESPACE {
#define END_RAYTRACER }

// Camera Settings
#define DEFAULT_ASPECT_RATIO 1.33

// Minimum ray weght threshold
#define MIN_WEIGHT 0.01

// Max intersections per ray against solid
#define MAX_INTERSECTIONS 4

// Max raytrace recurse depth
#define MAX_LEVEL 2

// Float comparison epsilon
#define P_FLT_EPSILON 1.0e-8

BEGIN_RAYTRACER


// Single precision color
typedef float C_FLT;

// Double precision position
typedef double P_FLT;


const int height = 600;
const int width = 800;


END_RAYTRACER

#endif
