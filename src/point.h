#ifndef POINT_H
#define POINT_H

#include "config.h"
#include "vector.h"


BEGIN_RAYTRACER

class Point {
  public:
    P_FLT x, y, z

    Point(): x(0.0), y(0.0), z(0.0) {}
    Point(P_FLT x, P_FLT y, P_FLT z): x(x), y(y), z(z) {}

    Point(Vector vector): x(vector.x), y(vector.y), z(vector.z) {}

    Point& operator =(const Point &vec);

    Point operator +(const Vector &vec);
    Point operator -(const Vector &vec);
    Point& operator +=(const Vector &vec);
    Point& operator -=(const Vector &vec);

    Point copy();
}:


END_RAYTRACER


#endif
