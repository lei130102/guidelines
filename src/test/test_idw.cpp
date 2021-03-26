#include <iostream>

#include "interpolation.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(s_idw)

BOOST_AUTO_TEST_CASE(c_idw_2_discrete_point)
{
	std::map<std::pair<double, double>, double> values;
	values.insert(std::make_pair(std::make_pair(0, 0), 0));
	values.insert(std::make_pair(std::make_pair(0, 1), 1));

	//
	std::pair<std::pair<double, double> const, double> point = std::make_pair(std::make_pair(0, 0.5), 0);
	interpolation::idw(values, 99999, point, 2);

    BOOST_CHECK((point.second == 0.5));
}

BOOST_AUTO_TEST_CASE(c_idw_3_discrete_point)
{
	std::map<std::pair<double, double>, double> values;
	values.insert(std::make_pair(std::make_pair(0, 0), 0));
	values.insert(std::make_pair(std::make_pair(0, 1), 1));
	values.insert(std::make_pair(std::make_pair(0, 2), 2));

	//
	std::pair<std::pair<double, double> const, double> point = std::make_pair(std::make_pair(0, 0.5), 0);
	interpolation::idw(values, 99999, point, 2);

	BOOST_CHECK((point.second > 0.5));
}

BOOST_AUTO_TEST_CASE(c_idw)
{
    std::map<std::pair<double, double>, double> values;
    values.insert(std::make_pair(std::make_pair(0, 0), 0));
    values.insert(std::make_pair(std::make_pair(0, 1), 1));

    std::map<std::pair<double, double>, double> some_point;
    some_point.insert(std::make_pair(std::make_pair(0, -0.5), 0));
    some_point.insert(std::make_pair(std::make_pair(0, 0.5), 0));
    some_point.insert(std::make_pair(std::make_pair(0, 1.5), 0));
    interpolation::idw(values, 99999, some_point, 2);
}

BOOST_AUTO_TEST_SUITE_END()
