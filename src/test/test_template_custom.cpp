
#include <test_template_custom.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <type_traits>

BOOST_AUTO_TEST_SUITE(s_template_custom)

BOOST_AUTO_TEST_CASE(c_template_custom_loop)
{
	{
		using namespace test_template_custom_loop::loop_typelist;

		BOOST_CHECK((std::is_same_v<loop<0, typelist<int, double, long>>::type, int> == true));
		BOOST_CHECK((std::is_same_v<loop<1, typelist<int, double, long>>::type, double> == true));
		BOOST_CHECK((std::is_same_v<loop<2, typelist<int, double, long>>::type, long> == true));
	}

	{
		using namespace test_template_custom_loop::loop_intlist;

		BOOST_CHECK((std::is_same_v<loop<0>::type, intlist<>> == true));
		BOOST_CHECK((std::is_same_v<loop<1>::type, intlist<0>> == true));
		BOOST_CHECK((std::is_same_v<loop<2>::type, intlist<0, 1>> == true));
		BOOST_CHECK((std::is_same_v<loop<3>::type, intlist<0, 1, 2>> == true));

		func(loop<2>::type{});
	}

	{
		using namespace test_template_custom_loop::intlist;

		BOOST_CHECK((std::is_same_v<std::make_integer_sequence<int, 0>, intlist<>> == true));
		BOOST_CHECK((std::is_same_v<std::make_integer_sequence<int, 1>, intlist<0>> == true));
		BOOST_CHECK((std::is_same_v<std::make_integer_sequence<int, 2>, intlist<0, 1>> == true));
		BOOST_CHECK((std::is_same_v<std::make_integer_sequence<int, 3>, intlist<0, 1, 2>> == true));

		func(std::make_integer_sequence<int, 2>{});
	}
}


BOOST_AUTO_TEST_SUITE_END()