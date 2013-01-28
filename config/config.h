#ifndef CONFIG_H
#define CONFIG_H


#define DEBUG 1

#define THREAD_NUM 8


#define RAYTRACER_NAMESPACE raytracer
#define BEGIN_RAYTRACER namespace RAYTRACER_NAMESPACE {
#define END_RAYTRACER }


#define PI (3.141592653589793)

// Camera Settings
#define DEFAULT_ASPECT_RATIO 1.33

// Minimum ray weght threshold
#define MIN_WEIGHT 0.01

// Max intersections per ray against solid
#define MAX_INTERSECTIONS 4

// Max raytrace recurse depth
#define MAX_LEVEL 8

// Float comparison epsilon
#define P_FLT_EPSILON 1.0e-8


#define MODEL_CLS Model
// Internal resolution factor for sampling
#define INT_RES_FACTOR 3


BEGIN_RAYTRACER


// Single precision color
typedef float C_FLT;

// Double precision position
typedef double P_FLT;


const int image_height = 600;
const int image_width = 800;

END_RAYTRACER

#endif
