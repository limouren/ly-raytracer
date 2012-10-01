#include <boost/test/included/unit_test.hpp>

#include "config.h"

#include "color.h"


BEGIN_RAYTRACER


BOOST_AUTO_TEST_SUITE(ColorConstructorTest)

BOOST_AUTO_TEST_CASE(Case1) {
  // Case 1
  Color black;
  BOOST_CHECK_EQUAL(black.r, 0.0);
  BOOST_CHECK_EQUAL(black.g, 0.0);
  BOOST_CHECK_EQUAL(black.b, 0.0);

  Color white(1.0, 1.0, 1.0);
  BOOST_CHECK_EQUAL(white.r, 1.0);
  BOOST_CHECK_EQUAL(white.g, 1.0);
  BOOST_CHECK_EQUAL(white.b, 1.0);
}


BOOST_AUTO_TEST_SUITE_END()


END_RAYTRACER
