#include <math.h>
#include <boost/test/included/unit_test.hpp>

#include "config.h"

#include "vector.cpp"


BEGIN_RAYTRACER


BOOST_AUTO_TEST_SUITE(VectorTest)

BOOST_AUTO_TEST_CASE(Constructor) {
  Vector v1;
  BOOST_CHECK_CLOSE(v1.x, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(v1.y, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(v1.z, 0.0, P_FLT_TOLERANCE);

  Vector v2(1.0, 2.0, -3.0);
  BOOST_CHECK_CLOSE(v2.x, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(v2.y, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(v2.z, -3.0, P_FLT_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(Operators) {
  Vector u(1.0, 1.0, 1.0), v(1.0, 2.0, -3.0);

  Vector x = v;
  BOOST_CHECK_CLOSE(x.x, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -3.0, P_FLT_TOLERANCE);


  x = u + v;
  BOOST_CHECK_CLOSE(x.x, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 3.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -2.0, P_FLT_TOLERANCE);

  x = u - v;
  BOOST_CHECK_CLOSE(x.x, 0.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, -1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, 4.0, P_FLT_TOLERANCE);

  x = Vector();
  x += v;
  BOOST_CHECK_CLOSE(x.x, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -3.0, P_FLT_TOLERANCE);

  x = Vector();
  x -= v;
  BOOST_CHECK_CLOSE(x.x, -1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, -2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, 3.0, P_FLT_TOLERANCE);


  x = v * 2.0;
  BOOST_CHECK_CLOSE(x.x, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 4.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -6.0, P_FLT_TOLERANCE);

  x = v / 2.0;
  BOOST_CHECK_CLOSE(x.x, 0.5, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -1.5, P_FLT_TOLERANCE);

  x = v;
  x *= 2.0;
  BOOST_CHECK_CLOSE(x.x, 2.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 4.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -6.0, P_FLT_TOLERANCE);

  x = v;
  x /= 2.0;
  BOOST_CHECK_CLOSE(x.x, 0.5, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.y, 1.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(x.z, -1.5, P_FLT_TOLERANCE);
}

BOOST_AUTO_TEST_CASE(methods) {
  Vector u(1.0, 1.0, 1.0), v(1.0, 2.0, -3.0);

  P_FLT length = u.length();
  BOOST_CHECK_CLOSE(length, 1.732051, P_FLT_TOLERANCE); // sqrt(3)

  P_FLT lengthSqr = u.lengthSqr();
  BOOST_CHECK_CLOSE(lengthSqr, 3.0, P_FLT_TOLERANCE);

  Vector x = v;
  x.normalize();
  BOOST_CHECK_CLOSE(x.x, 0.267261, P_FLT_TOLERANCE); // 1/sqrt(14)
  BOOST_CHECK_CLOSE(x.y, 0.534522, P_FLT_TOLERANCE); // 2/sqrt(14)
  BOOST_CHECK_CLOSE(x.z, -0.801784, P_FLT_TOLERANCE); // -3/sqrt(14)
}

BOOST_AUTO_TEST_CASE(products) {
  Vector u(1.0, 1.0, 1.0), v(1.0, 2.0, -3.0);

  P_FLT dot_product1 = dotProduct(u, v);
  BOOST_CHECK_CLOSE(dot_product1, 0.0, P_FLT_TOLERANCE);

  P_FLT dot_product2 = dotProduct(v, u);
  BOOST_CHECK_CLOSE(dot_product2, 0.0, P_FLT_TOLERANCE);

  Vector w = crossProduct(u, v);
  BOOST_CHECK_CLOSE(w.x, -5.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(w.y, 4.0, P_FLT_TOLERANCE);
  BOOST_CHECK_CLOSE(w.z, 1.0, P_FLT_TOLERANCE);
}

BOOST_AUTO_TEST_SUITE_END()


END_RAYTRACER
