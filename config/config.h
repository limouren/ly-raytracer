#ifndef CONFIG_H
#define CONFIG_H


#define DEBUG (1)


#define RAYTRACER_NAMESPACE raytracer
#define BEGIN_RAYTRACER namespace RAYTRACER_NAMESPACE {
#define END_RAYTRACER }


#define PI (3.141592653589793f)

// Camera Settings
#define DEFAULT_ASPECT_RATIO (1.33333f)

// Max intersections per ray against solid
#define MAX_INTERSECTIONS (4)

// Max raytrace recurse depth
#define MAX_LEVEL (8)

// Minimum ray weght threshold
#define MIN_WEIGHT (0.01f)

// Float comparison epsilon
#define P_FLT_EPSILON (1.0E-5f)

// Maximum size for positional values
#define P_FLT_MAX (1.0E+10f)


// Levels of recursion in adaptive supersampling
#define DEFAULT_AA_LEVEL (0)
#define AA_COLOR_THRESHOLD (0.1f)


BEGIN_RAYTRACER


// Single precision color
typedef float C_FLT;

// Double precision position
typedef float P_FLT;


int aaLevel = DEFAULT_AA_LEVEL;

// Detect number of threads, ref: http://goo.gl/q9GsB
#if Win32
SYSTEM_INFO sysinfo;
GetSystemInfo(&sysinfo);

int threadNum = sysinfo.dwNumberOfProcessors;
#else
int threadNum = sysconf(_SC_NPROCESSORS_ONLN);
#endif

END_RAYTRACER

#endif
