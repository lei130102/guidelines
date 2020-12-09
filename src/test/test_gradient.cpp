#include <iostream>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "gradient.h"

#include <vector>
#include <iostream>

BOOST_AUTO_TEST_SUITE(s_gradient)

BOOST_AUTO_TEST_CASE(c_gradient)
{
    std::vector<double> value{103, 223, 333, 538, 234};
    std::vector<double> depth{432, 205, 116, 310, 810};

    std::vector<double> tmp;

    gradient::compute_gradient(value.begin(), value.end(), depth.begin(), depth.end(), std::back_inserter(tmp));

    std::copy(tmp.begin(), tmp.end(), std::ostream_iterator<double>(std::cout, " "));

    std::cin.get();
}

BOOST_AUTO_TEST_SUITE_END()
