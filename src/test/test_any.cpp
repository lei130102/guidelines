#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "test_any.h"

#include <string>
#include <iostream>

BOOST_AUTO_TEST_SUITE(s_any)

//BOOST_AUTO_TEST_CASE(c_any_void_pointer)
//{
//	int* pi = new int{5};
//	void* pv = pi;
//
//	std::any ai = pi;
//	std::cout << ai.type().name() << "\n";
//	int* pi2 = std::any_cast<int*>(ai);
//	std::cout << *pi2 << "\n";
//
//	std::any av = pv;
//	std::cout << av.type().name() << "\n";
//	void* pv2 = std::any_cast<void*>(av);
//	std::cout << *static_cast<int*>(pv2) << "\n";
//
//	delete pi;
//}

BOOST_AUTO_TEST_CASE(c_any_string_type)
{
	using namespace test_any_string_type;

	typeinfo ti{ "int", []() { return std::any(new int); }, [](std::any object) { delete std::any_cast<int*>(object); } };
	
	std::any ai = ti.type_creator_function();
	std::cout << ai.type().name() << "\n";

	*std::any_cast<int*>(ai) = 5;

	std::cout << *std::any_cast<int*>(ai) << "\n";

	ti.type_deleter_function(ai);


	//不要用void*，擦除了类型信息
}

BOOST_AUTO_TEST_CASE(c_any_movable)
{
	using namespace test_any_movable;



}

BOOST_AUTO_TEST_SUITE_END()