#include <math.h>
#include <boost/test/included/unit_test.hpp>

#include "config.h"

#include "ray.h"
#include "point.h"
#include "vector.h"


BEGIN_RAYTRACER


BOOST_AUTO_TEST_SUITE(RayTest)

BOOST_AUTO_TEST_CASE(Constructor) {
  Ray r1;
  BOOST_CHECK_CLOSE(r1.orig.x, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r1.orig.y, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r1.orig.z, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r1.dir.x, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r1.dir.y, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r1.dir.z, 0.0, P_FLT_TOLERANCE);

  Vector orig2(1.0, 2.0, -3.0), dir2(1.0, 1.0, 1.0);
  Ray r2(orig2, dir2);
  BOOST_CHECK_CLOSE(r2.orig.x, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r2.orig.y, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r2.orig.z, -3.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r2.dir.x, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r2.dir.y, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(r2.dir.z, 1.0, P_FLT_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(Methods) {
  Vector orig1(1.0, 2.0, -3.0), dir1(1.0, 1.0, 1.0);
  Ray r1(orig1, dir1);

  Point p = r1.ray_point(2.0);
  BOOST_CHECK_CLOSE(p.x, 3.0, P_FLT_TOLERANCE); // 1/sqrt(14)
  BOOST_CHECK_CLOSE(p.y, 4.0, P_FLT_TOLERANCE); // 2/sqrt(14)
  BOOST_CHECK_CLOSE(p.z, -1.0, P_FLT_TOLERANCE); // -3/sqrt(14)
}

BOOST_AUTO_TEST_SUITE_END()


END_RAYTRACER
