#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <test_sqlite.h>

#include <iostream>

BOOST_AUTO_TEST_SUITE(s_sqlite)

BOOST_AUTO_TEST_CASE(c_sqlite_threadsafe)
{
	int value = sqlite3_threadsafe();
	std::cout << "sqlite3_threadsafe:" << value << '\n';
}

//返回值的
BOOST_AUTO_TEST_CASE(c_sqlite_select)
{

}

//不返回值的
BOOST_AUTO_TEST_CASE(c_sqlite_insert)
{

}

BOOST_AUTO_TEST_SUITE_END()