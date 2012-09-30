#ifndef POINT_H
#define POINT_H

#include "config.h"
#include "vector.h"


BEGIN_RAYTRACER

class Point::Vector {
  public:
    Point(P_FLT x, P_FLT y, P_FLT z):Vector(x, y, z);

    Point(Vector vec):Point(vec);

    Point& operator =(const Point &vec);
    Point& operator +(const Vector &vec);
    Point& operator -(const Vector &vec);
    Point& operator +=(const Vector &vec);
    Point& operator -=(const Vector &vec);

    Point& copy();
}:


END_RAYTRACER


#endif
