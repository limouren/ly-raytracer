#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RAYTRACER_NAMESPACE raytracer
#define BEGIN_RAYTRACER namespace RAYTRACER_NAMESPACE {
#define END_RAYTRACER }


BEGIN_RAYTRACER


// Single precision color
typedef float C_FLT;

// Double precision position
typedef double P_FLT;

// Max raytrace recurse depth
int MAX_LEVEL = 2;

// Minimum ray weght threshold
P_FLT MIN_WEIGHT = 0.01;

// Float comparison epsilon
P_FLT P_FLT_EPSILON = 1.0e-8;

// Max intersections per ray against solid
int MAX_INTERSECTIONS = 4;

// Camera Settings
P_FLT DEFAULT_ASPECT_RATIO = 1.33;


END_RAYTRACER

#endif
