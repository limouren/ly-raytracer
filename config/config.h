#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RAYTRACER_NAMESPACE raytracer
#define BEGIN_RAYTRACER namespace RAYTRACER {
#define END_RAYTRACER }


BEGIN_RAYTRACER


// Single precision color
typedef float C_FLT;

// Double precision position
typedef double P_FLT;

// Float comparison epsilon
P_FLT P_FLT_EPSILON = 1.0e-8;


// Camera Settings
P_FLT DEFAULT_ASPECT_RATIO = 1.33;


END_RAYTRACER

#endif
