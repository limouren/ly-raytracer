#include <boost/test/included/unit_test.hpp>

#include "config.h"

#include "surface.cpp"


BEGIN_RAYTRACER


BOOST_AUTO_TEST_SUITE(SphereTest)

BOOST_AUTO_TEST_CASE(Constructor) {
  Point p1(2.0, 3.0, 4.0);
  P_FLT r1 = 3.0;
  Sphere s1(p1, r1);
  BOOST_CHECK_CLOSE(s1.center->x, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s1.center->y, 3.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s1.center->z, 4.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s1.radius, 3.0, P_FLT_TOLERANCE);


  Sphere s2(2.0, 3.0, 4.0, 3.0);
  BOOST_CHECK_CLOSE(s2.center->x, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s2.center->y, 3.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s2.center->z, 4.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(s2.radius, 3.0, P_FLT_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(Intersect) {
  Point ps1(0.0, 0.0, 0.0);
  P_FLT rs1 = 1.0, *t_values1;
  Sphere s1(ps1, rs1);

  Point pr1(2.0, 0.0, 0.0);
  Vector vr1(-1.0, 0.0, 0.0);
  Ray r1(pr1, vr1);

  int intersection_num1 = s1.intersect(r1, &t_values1);
  BOOST_CHECK_EQUAL(intersection_num1, 2);
  BOOST_CHECK_CLOSE(t_values1[0], 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(t_values1[1], 3.0, P_FLT_TOLERANCE);
  delete [] t_values1;


  Point ps2(1.0, 1.0, 1.0);
  P_FLT rs2 = 1.0, *t_values2;
  Sphere s2(ps2, rs2);

  Point pr2(3.0, 3.0, 3.0);
  Vector vr2(-1.0, -1.0, -1.0);
  vr2.normalize();
  Ray r2(pr2, vr2);

  int intersection_num2 = s2.intersect(r2, &t_values2);
  BOOST_CHECK_EQUAL(intersection_num2, 2);
  BOOST_CHECK_CLOSE(t_values2[0], 2.464102, P_FLT_TOLERANCE); // sqrt(12) - 1
  BOOST_CHECK_CLOSE(t_values2[1], 4.464102, P_FLT_TOLERANCE); // sqrt(12) + 1
  delete [] t_values2;


  // Case: Wrong direction
  Point ps3(0.0, 0.0, 0.0);
  P_FLT rs3 = 1.0, *t_values3;
  Sphere s3(ps3, rs3);

  Point pr3(2.0, 0.0, 0.0);
  Vector vr3(1.0, 0.0, 0.0);
  Ray r3(pr3, vr3);

  int intersection_num3 = s3.intersect(r3, &t_values3);
  BOOST_CHECK_EQUAL(intersection_num3, 0);


  // Case: Near miss
  Point ps4(0.0, 0.0, 0.0);
  P_FLT rs4 = 1.0, *t_values4;
  Sphere s4(ps4, rs4);

  Point pr4(1.1, 1.1, 1.1);
  Vector vr4(-1.0, 0.0, 0.0);
  Ray r4(pr4, vr4);

  int intersection_num4 = s4.intersect(r4, &t_values4);
  BOOST_CHECK_EQUAL(intersection_num4, 0);
}

BOOST_AUTO_TEST_SUITE_END()


END_RAYTRACER
