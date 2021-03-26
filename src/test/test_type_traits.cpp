#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <type_traits>
#include <array>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/extent.hpp>
#include <boost/type_traits/rank.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/enable_if.hpp>
#include <boost/type_traits/is_member_object_pointer.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_compound.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/has_virtual_destructor.hpp>
#include <boost/type_traits/is_assignable.hpp>
#include <boost/type_traits/is_copy_assignable.hpp>
#include <boost/type_traits/is_nothrow_move_assignable.hpp>
#include <boost/type_traits/is_constructible.hpp>
#include <boost/type_traits/is_destructible.hpp>
#include <boost/type_traits/is_default_constructible.hpp>
#include <boost/type_traits/is_copy_constructible.hpp>
#include <boost/type_traits/is_nothrow_move_constructible.hpp>

#include <test_type_traits.h>


BOOST_AUTO_TEST_SUITE(s_type_traits)

BOOST_AUTO_TEST_CASE(c_std_integral_constant)
{
	//#include <type_traits>
	//C++11，但部分功能C++14
	//std::integral_constant

	using two_t = std::integral_constant<int, 2>;
	using four_t = std::integral_constant<int, 4>;

	static_assert(!std::is_same<two_t, four_t>::value, "two_t and four_t are equal");

	static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");




	enum class my_e   //枚举可以定义在函数作用域内
	{
		e1,
		e2
	};
	using my_e_e1 = std::integral_constant<my_e, my_e::e1>;
	using my_e_e2 = std::integral_constant<my_e, my_e::e2>;
	
	static_assert(my_e_e1::value != my_e::e2, "my_e_e1::value == my_e::e2");

	static_assert(!std::is_same<my_e_e1, my_e_e2>::value, "my_e_e1 == my_e_e2");
	static_assert(std::is_same<my_e_e1::value_type, my_e_e2::value_type>::value, "my_e_e1::value_type != my_e_e2::value_type");
}

BOOST_AUTO_TEST_CASE(c_boost_integral_constant)
{
	//#include <boost/type_traits/integral_constant.hpp>
	//boost::integral_constant

	using two_t = boost::integral_constant<int, 2>;
	using four_t = boost::integral_constant<int, 4>;

	static_assert(!std::is_same<two_t, four_t>::value, "two_t and four_t are equal");

	static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");




	enum class my_e   //枚举可以定义在函数作用域内
	{
		e1,
		e2
	};
	using my_e_e1 = boost::integral_constant<my_e, my_e::e1>;
	using my_e_e2 = boost::integral_constant<my_e, my_e::e2>;

	static_assert(my_e_e1::value != my_e::e2, "my_e_e1::value == my_e::e2");

	static_assert(!std::is_same<my_e_e1, my_e_e2>::value, "my_e_e1 == my_e_e2");
	static_assert(std::is_same<my_e_e1::value_type, my_e_e2::value_type>::value, "my_e_e1::value_type != my_e_e2::value_type");
}

BOOST_AUTO_TEST_CASE(c_test_integral_constant)
{
	using two_t = test_type_traits::integral_constant<int, 2>;
	using four_t = test_type_traits::integral_constant<int, 4>;

	static_assert(!std::is_same<two_t, four_t>::value, "two_t and four_t are equal");

	static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");




	enum class my_e   //枚举可以定义在函数作用域内
	{
		e1,
		e2
	};
	using my_e_e1 = test_type_traits::integral_constant<my_e, my_e::e1>;
	using my_e_e2 = test_type_traits::integral_constant<my_e, my_e::e2>;

	static_assert(my_e_e1::value != my_e::e2, "my_e_e1::value == my_e::e2");

	static_assert(!std::is_same<my_e_e1, my_e_e2>::value, "my_e_e1 == my_e_e2");
	static_assert(std::is_same<my_e_e1::value_type, my_e_e2::value_type>::value, "my_e_e1::value_type != my_e_e2::value_type");
}

BOOST_AUTO_TEST_CASE(c_std_bool_constant)
{
	//#include <type_traits>
	//C++17
	//std::bool_constant

	//略
}

BOOST_AUTO_TEST_CASE(c_boost_true_type)
{
	//#include <boost/type_traits/integral_constant.hpp>
	//boost::true_type
	//boost::false_type
	//没有boost::bool_constant

	//略
}

BOOST_AUTO_TEST_CASE(c_test_bool_constant)
{
	//略
}


BOOST_AUTO_TEST_CASE(c_std_is_array)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_array;

	//C++17
	//template<class T>
	//inline constexpr bool is_array_v;

	using namespace test_type_traits::c_std_is_array;

	BOOST_CHECK(std::is_array<A>::value == false);
	BOOST_CHECK(std::is_array_v<A> == false);
	BOOST_CHECK(std::is_array<A[]>::value == true);
	BOOST_CHECK(std::is_array_v<A[]> == true);
	BOOST_CHECK(std::is_array<A[3]>::value == true);
	BOOST_CHECK(std::is_array_v<A[3]> == true);
	BOOST_CHECK(std::is_array<float>::value == false);
	BOOST_CHECK(std::is_array_v<float> == false);
	BOOST_CHECK(std::is_array<int>::value == false);
	BOOST_CHECK(std::is_array_v<int> == false);
	BOOST_CHECK(std::is_array<int[]>::value == true);
	BOOST_CHECK(std::is_array_v<int[]> == true);
	BOOST_CHECK(std::is_array<int[3]>::value == true);
	BOOST_CHECK(std::is_array_v<int[3]> == true);
	BOOST_CHECK((std::is_array<std::array<int, 3>>::value == false));
	BOOST_CHECK((std::is_array_v<std::array<int, 3>> == false));
}

BOOST_AUTO_TEST_CASE(c_boost_is_array)
{
	//#include <boost/type_traits/is_array.hpp>
	//template<class T>
	//struct is_array;

	using namespace test_type_traits::c_boost_is_array;

	BOOST_CHECK(boost::is_array<A>::value == false);
	BOOST_CHECK(boost::is_array<A[]>::value == true);
	BOOST_CHECK(boost::is_array<A[3]>::value == true);
	BOOST_CHECK(boost::is_array<float>::value == false);
	BOOST_CHECK(boost::is_array<int>::value == false);
	BOOST_CHECK(boost::is_array<int[]>::value == true);
	BOOST_CHECK(boost::is_array<int[3]>::value == true);
	BOOST_CHECK((boost::is_array<std::array<int, 3>>::value == false));
}

BOOST_AUTO_TEST_CASE(c_test_is_array)
{
	using namespace test_type_traits::c_test_is_array;

	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<A>::value == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<A> == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<A[]>::value == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<A[]> == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<A[3]>::value == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<A[3]> == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<float>::value == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<float> == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<int>::value == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<int> == false);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<int[]>::value == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<int[]> == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array<int[3]>::value == true);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_array_v<int[3]> == true);
	BOOST_CHECK((test_type_traits::primary_type_categories::is_array<std::array<int, 3>>::value == false));
	BOOST_CHECK((test_type_traits::primary_type_categories::is_array_v<std::array<int, 3>> == false));
}

BOOST_AUTO_TEST_CASE(c_std_is_union)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_union;

	//C++17
	//template<class T>
	//inline constexpr bool is_union_v;

	using namespace test_type_traits::c_std_is_union;

	BOOST_CHECK(!std::is_union<A>::value);
	BOOST_CHECK(!std::is_union_v<A>);
	BOOST_CHECK(std::is_union<B>::value);
	BOOST_CHECK(std::is_union_v<B>);
	BOOST_CHECK(!std::is_union<C>::value);
	BOOST_CHECK(!std::is_union_v<C>);
	BOOST_CHECK(!std::is_union<int>::value);
	BOOST_CHECK(!std::is_union_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_union)
{
	//#include <boost/type_traits/is_union.hpp>
	//template<class T>
	//struct is_union;

	using namespace test_type_traits::c_boost_is_union;

	BOOST_CHECK(!boost::is_union<A>::value);
	BOOST_CHECK(boost::is_union<B>::value);
	BOOST_CHECK(!boost::is_union<C>::value);
	BOOST_CHECK(!boost::is_union<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_union)
{
	using namespace test_type_traits::c_test_is_union;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_union_by_is_union::is_union<B>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_union_by_is_union::is_union_v<B>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union<C>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union_v<C>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_union_by_is_union::is_union_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_class)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_class;

	//C++17
	//template<class T>
	//inline constexpr bool is_class_v;

	using namespace test_type_traits::c_std_is_class;

	BOOST_CHECK(std::is_class<A>::value);
	BOOST_CHECK(std::is_class_v<A>);
	BOOST_CHECK(std::is_class<B>::value);
	BOOST_CHECK(std::is_class_v<B>);
	BOOST_CHECK(!std::is_class<C>::value);
	BOOST_CHECK(!std::is_class_v<C>);
	BOOST_CHECK(!std::is_class<int>::value);
	BOOST_CHECK(!std::is_class_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_class)
{
	//#include <boost/type_traits/is_class.hpp>
	//template<class T>
	//struct is_class;

	using namespace test_type_traits::c_boost_is_class;

	BOOST_CHECK(boost::is_class<A>::value);
	BOOST_CHECK(boost::is_class<B>::value);
	BOOST_CHECK(!boost::is_class<C>::value);
	BOOST_CHECK(!boost::is_class<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_class)
{
	using namespace test_type_traits::c_test_is_class;

	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_union::is_class<A>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_union::is_class_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_union::is_class<B>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_union::is_class_v<B>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_union::is_class<C>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_union::is_class_v<C>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_union::is_class<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_union::is_class_v<int>);

	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_class::is_class<A>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_class::is_class_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_class::is_class<B>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_class_by_is_class::is_class_v<B>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_class::is_class<C>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_class::is_class_v<C>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_class::is_class<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_class_by_is_class::is_class_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_enum)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_enum;

	//C++17
	//template<class T>
	//inline constexpr bool is_enum_v;

	using namespace test_type_traits::c_std_is_enum;

	BOOST_CHECK(!std::is_enum<A>::value);
	BOOST_CHECK(!std::is_enum_v<A>);
	BOOST_CHECK(std::is_enum<E>::value);
	BOOST_CHECK(std::is_enum_v<E>);
	BOOST_CHECK(std::is_enum<Ec>::value);
	BOOST_CHECK(std::is_enum_v<Ec>);
	BOOST_CHECK(!std::is_enum<int>::value);
	BOOST_CHECK(!std::is_enum_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_enum)
{
	//#include <boost/type_traits/is_enum.hpp>
	//template<class T>
	//struct is_enum;

	using namespace test_type_traits::c_boost_is_enum;

	BOOST_CHECK(!boost::is_enum<A>::value);
	BOOST_CHECK(boost::is_enum<E>::value);
	BOOST_CHECK(boost::is_enum<Ec>::value);
	BOOST_CHECK(!boost::is_enum<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_enum)
{
	using namespace test_type_traits::c_test_is_enum;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum<E>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum_v<E>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum<Ec>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum_v<Ec>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_enum_by_is_enum::is_enum_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_floating_point)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_floating_point;

	//C++17
	//template<class T>
	//inline constexpr bool is_floating_point_v;

	using namespace test_type_traits::c_std_is_floating_point;

	BOOST_CHECK(!std::is_floating_point<A>::value);
	BOOST_CHECK(!std::is_floating_point_v<A>);
	BOOST_CHECK(std::is_floating_point<float>::value);
	BOOST_CHECK(std::is_floating_point_v<float>);
	BOOST_CHECK(!std::is_floating_point<float&>::value);
	BOOST_CHECK(!std::is_floating_point_v<float&>);
	BOOST_CHECK(std::is_floating_point<double>::value);
	BOOST_CHECK(std::is_floating_point_v<double>);
	BOOST_CHECK(!std::is_floating_point<double&>::value);
	BOOST_CHECK(!std::is_floating_point_v<double&>);
	BOOST_CHECK(!std::is_floating_point<int>::value);
	BOOST_CHECK(!std::is_floating_point_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_floating_point)
{
	//#include <boost/type_traits/is_floating_point.hpp>
	//template<class T>
	//struct is_floating_point;

	using namespace test_type_traits::c_boost_is_floating_point;

	BOOST_CHECK(!boost::is_floating_point<A>::value);
	BOOST_CHECK(boost::is_floating_point<float>::value);
	BOOST_CHECK(!boost::is_floating_point<float&>::value);
	BOOST_CHECK(boost::is_floating_point<double>::value);
	BOOST_CHECK(!boost::is_floating_point<double&>::value);
	BOOST_CHECK(!boost::is_floating_point<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_floating_point)
{
	using namespace test_type_traits::c_test_is_floating_point;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point<float>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_v<float>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point<float&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_v<float&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point<double>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_v<double>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point<double&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_v<double&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_v<int>);

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<float>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<float>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<float&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<float&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<double>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<double>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<double&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<double&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_floating_point_by_is_same_and_remove_cv::is_floating_point_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_function)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_function;

	//C++17
	//template<class T>
	//inline constexpr bool is_function_v;

	using namespace test_type_traits::c_std_is_function;

	BOOST_CHECK(!std::is_function<A>::value);
	BOOST_CHECK(!std::is_function_v<A>);
	BOOST_CHECK(std::is_function<int(int)>::value);
	BOOST_CHECK(std::is_function_v<int(int)>);
	BOOST_CHECK(std::is_function<decltype(f)>::value);
	BOOST_CHECK(std::is_function_v<decltype(f)>);
	BOOST_CHECK(!std::is_function<int>::value);
	BOOST_CHECK(!std::is_function_v<int>);

	using T = PM_traits<decltype(&A::fun)>::member_type;   //T为int() const&
	BOOST_CHECK(std::is_function<T>::value);
	BOOST_CHECK(std::is_function_v<T>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_function)
{
	//#include <boost/type_traits/detail/is_function_cxx_11.hpp>
	//template<class T>
	//struct is_function;

	using namespace test_type_traits::c_boost_is_function;

	BOOST_CHECK(!boost::is_function<A>::value);
	BOOST_CHECK(boost::is_function<int(int)>::value);
	BOOST_CHECK(boost::is_function<decltype(f)>::value);
	BOOST_CHECK(!boost::is_function<int>::value);

	using T = PM_traits<decltype(&A::fun)>::member_type;   //T为int() const&
	BOOST_CHECK(boost::is_function<T>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_function)
{
	using namespace test_type_traits::c_test_is_function;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_function<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_function_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function<int(int)>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function_v<int(int)>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function<decltype(f)>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function_v<decltype(f)>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_function<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_function_v<int>);

	using T = PM_traits<decltype(&A::fun)>::member_type;   //T为int() const&
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function<T>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_function_v<T>);
}

BOOST_AUTO_TEST_CASE(c_std_is_integral)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_integral;

	//C++17
	//template<class T>
	//inline constexpr bool is_integral_v;

	using namespace test_type_traits::c_std_is_integral;

	BOOST_CHECK(!std::is_integral<A>::value);
	BOOST_CHECK(!std::is_integral_v<A>);
	BOOST_CHECK(!std::is_integral<E>::value);
	BOOST_CHECK(!std::is_integral_v<E>);
	BOOST_CHECK(!std::is_integral<float>::value);
	BOOST_CHECK(!std::is_integral_v<float>);
	BOOST_CHECK(std::is_integral<int>::value);
	BOOST_CHECK(std::is_integral_v<int>);
	BOOST_CHECK(std::is_integral<bool>::value);
	BOOST_CHECK(std::is_integral_v<bool>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_integral)
{
	//#include <boost/type_traits/is_integral.hpp>
	//template<class T>
	//struct is_integral;

	using namespace test_type_traits::c_boost_is_integral;

	BOOST_CHECK(!boost::is_integral<A>::value);
	BOOST_CHECK(!boost::is_integral<E>::value);
	BOOST_CHECK(!boost::is_integral<float>::value);
	BOOST_CHECK(boost::is_integral<int>::value);
	BOOST_CHECK(boost::is_integral<bool>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_integral)
{
	using namespace test_type_traits::c_test_is_integral;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral_v<A>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral<E>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral_v<E>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral<float>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_integral_v<float>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_integral<int>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_integral_v<int>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_integral<bool>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_integral_v<bool>);
}

BOOST_AUTO_TEST_CASE(c_std_is_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_pointer;

	//C++17
	//template<class T>
	//inline constexpr bool is_pointer_v;

	using namespace test_type_traits::c_std_is_pointer;

	BOOST_CHECK(!std::is_pointer<A>::value);
	BOOST_CHECK(!std::is_pointer_v<A>);
	BOOST_CHECK(std::is_pointer<A*>::value);
	BOOST_CHECK(std::is_pointer_v<A*>);
	BOOST_CHECK(!std::is_pointer<A&>::value);
	BOOST_CHECK(!std::is_pointer_v<A&>);
	BOOST_CHECK(!std::is_pointer<int>::value);
	BOOST_CHECK(!std::is_pointer_v<int>);
	BOOST_CHECK(std::is_pointer<int*>::value);
	BOOST_CHECK(std::is_pointer_v<int*>);
	BOOST_CHECK(std::is_pointer<int**>::value);
	BOOST_CHECK(std::is_pointer_v<int**>);
	BOOST_CHECK(!std::is_pointer<int[10]>::value);
	BOOST_CHECK(!std::is_pointer_v<int[10]>);
	BOOST_CHECK(!std::is_pointer<std::nullptr_t>::value);
	BOOST_CHECK(!std::is_pointer_v<std::nullptr_t>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_pointer)
{
	//#include <boost/type_traits/is_pointer.hpp>
	//template<class T>
	//struct is_pointer;

	using namespace test_type_traits::c_boost_is_pointer;

	BOOST_CHECK(!boost::is_pointer<A>::value);
	BOOST_CHECK(boost::is_pointer<A*>::value);
	BOOST_CHECK(!boost::is_pointer<A&>::value);
	BOOST_CHECK(!boost::is_pointer<int>::value);
	BOOST_CHECK(boost::is_pointer<int*>::value);
	BOOST_CHECK(boost::is_pointer<int**>::value);
	BOOST_CHECK(!boost::is_pointer<int[10]>::value);
	BOOST_CHECK(!boost::is_pointer<std::nullptr_t>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_pointer)
{
	using namespace test_type_traits::c_test_is_pointer;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer<A*>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_v<A*>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer<A&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_v<A&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_v<int>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer<int*>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_v<int*>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer<int**>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_v<int**>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer<int[10]>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_v<int[10]>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer<std::nullptr_t>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_v<std::nullptr_t>);

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<A*>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<A*>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<A&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<A&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<int>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<int*>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<int*>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<int**>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<int**>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<int[10]>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<int[10]>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer<std::nullptr_t>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_pointer_by_remove_cv::is_pointer_v<std::nullptr_t>);
}

BOOST_AUTO_TEST_CASE(c_std_is_void)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_void;

	//C++17
	//template<class T>
	//inline constexpr bool is_void_v;

	BOOST_CHECK(std::is_void<void>::value);
	BOOST_CHECK(std::is_void_v<void>);
	BOOST_CHECK(!std::is_void<int>::value);
	BOOST_CHECK(!std::is_void_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_void)
{
	//#include <boost/type_traits/is_void.hpp>
	//template<class T>
	//struct is_void;

	BOOST_CHECK(boost::is_void<void>::value);
	BOOST_CHECK(!boost::is_void<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_void)
{
	BOOST_CHECK(test_type_traits::primary_type_categories::is_void<void>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_void_v<void>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_void<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_void_v<int>);

	BOOST_CHECK(test_type_traits::primary_type_categories::is_void_by_is_same_and_remove_cv::is_void<void>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_void_by_is_same_and_remove_cv::is_void_v<void>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_void_by_is_same_and_remove_cv::is_void<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_void_by_is_same_and_remove_cv::is_void_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_lvalue_reference)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_lvalue_reference;

	//C++17
	//template<class T>
	//inline constexpr bool is_lvalue_reference_v;

	using namespace test_type_traits::c_std_is_lvalue_reference;

	BOOST_CHECK(!std::is_lvalue_reference<A>::value);
	BOOST_CHECK(!std::is_lvalue_reference_v<A>);
	BOOST_CHECK(std::is_lvalue_reference<A&>::value);
	BOOST_CHECK(std::is_lvalue_reference_v<A&>);
	BOOST_CHECK(!std::is_lvalue_reference<A&&>::value);
	BOOST_CHECK(!std::is_lvalue_reference_v<A&&>);
	BOOST_CHECK(!std::is_lvalue_reference<int>::value);
	BOOST_CHECK(!std::is_lvalue_reference_v<int>);
	BOOST_CHECK(std::is_lvalue_reference<int&>::value);
	BOOST_CHECK(std::is_lvalue_reference_v<int&>);
	BOOST_CHECK(!std::is_lvalue_reference<int&&>::value);
	BOOST_CHECK(!std::is_lvalue_reference_v<int&&>);

}

BOOST_AUTO_TEST_CASE(c_boost_is_lvalue_reference)
{
	//#include <boost/type_traits/is_lvalue_reference.hpp>
	//template<class T>
	//struct is_lvalue_reference;

	using namespace test_type_traits::c_boost_is_lvalue_reference;

	BOOST_CHECK(!boost::is_lvalue_reference<A>::value);
	BOOST_CHECK(boost::is_lvalue_reference<A&>::value);
	BOOST_CHECK(!boost::is_lvalue_reference<A&&>::value);
	BOOST_CHECK(!boost::is_lvalue_reference<int>::value);
	BOOST_CHECK(boost::is_lvalue_reference<int&>::value);
	BOOST_CHECK(!boost::is_lvalue_reference<int&&>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_lvalue_reference)
{
	using namespace test_type_traits::c_test_is_lvalue_reference;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_lvalue_reference<A&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_lvalue_reference_v<A&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference<A&&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference_v<A&&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference_v<int>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_lvalue_reference<int&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_lvalue_reference_v<int&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference<int&&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_lvalue_reference_v<int&&>);
}

BOOST_AUTO_TEST_CASE(c_std_is_rvalue_reference)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_rvalue_reference;

	//C++17
	//template<class T>
	//inline constexpr bool is_rvalue_reference_v;

	using namespace test_type_traits::c_std_is_rvalue_reference;

	BOOST_CHECK(!std::is_rvalue_reference<A>::value);
	BOOST_CHECK(!std::is_rvalue_reference_v<A>);
	BOOST_CHECK(!std::is_rvalue_reference<A&>::value);
	BOOST_CHECK(!std::is_rvalue_reference_v<A&>);
	BOOST_CHECK(std::is_rvalue_reference<A&&>::value);
	BOOST_CHECK(std::is_rvalue_reference_v<A&&>);
	BOOST_CHECK(!std::is_rvalue_reference<int>::value);
	BOOST_CHECK(!std::is_rvalue_reference_v<int>);
	BOOST_CHECK(!std::is_rvalue_reference<int&>::value);
	BOOST_CHECK(!std::is_rvalue_reference_v<int&>);
	BOOST_CHECK(std::is_rvalue_reference<int&&>::value);
	BOOST_CHECK(std::is_rvalue_reference_v<int&&>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_rvalue_reference)
{
	//#include <boost/type_traits/is_rvalue_reference.hpp>
	//template<class T>
	//struct is_rvalue_reference;

	using namespace test_type_traits::c_boost_is_rvalue_reference;

	BOOST_CHECK(!boost::is_rvalue_reference<A>::value);
	BOOST_CHECK(!boost::is_rvalue_reference<A&>::value);
	BOOST_CHECK(boost::is_rvalue_reference<A&&>::value);
	BOOST_CHECK(!boost::is_rvalue_reference<int>::value);
	BOOST_CHECK(!boost::is_rvalue_reference<int&>::value);
	BOOST_CHECK(boost::is_rvalue_reference<int&&>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_rvalue_reference)
{
	using namespace test_type_traits::c_test_is_rvalue_reference;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference_v<A>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference<A&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference_v<A&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_rvalue_reference<A&&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_rvalue_reference_v<A&&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference_v<int>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference<int&>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_rvalue_reference_v<int&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_rvalue_reference<int&&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_rvalue_reference_v<int&&>);
}

BOOST_AUTO_TEST_CASE(c_std_is_reference)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_reference;

	//C++17
	//template<class T>
	//inline constexpr bool is_reference_v;

	using namespace test_type_traits::c_std_is_reference;

	BOOST_CHECK(!std::is_reference<A>::value);
	BOOST_CHECK(!std::is_reference_v<A>);
	BOOST_CHECK(std::is_reference<A&>::value);
	BOOST_CHECK(std::is_reference_v<A&>);
	BOOST_CHECK(std::is_reference<A&&>::value);
	BOOST_CHECK(std::is_reference_v<A&&>);
	BOOST_CHECK(!std::is_reference<int>::value);
	BOOST_CHECK(!std::is_reference_v<int>);
	BOOST_CHECK(std::is_reference<int&>::value);
	BOOST_CHECK(std::is_reference_v<int&>);
	BOOST_CHECK(std::is_reference<int&&>::value);
	BOOST_CHECK(std::is_reference_v<int&&>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_reference)
{
	//#include <boost/type_traits/is_reference.hpp>
	//template<class T>
	//struct is_reference;

	using namespace test_type_traits::c_boost_is_reference;

	BOOST_CHECK(!boost::is_reference<A>::value);
	BOOST_CHECK(boost::is_reference<A&>::value);
	BOOST_CHECK(boost::is_reference<A&&>::value);
	BOOST_CHECK(!boost::is_reference<int>::value);
	BOOST_CHECK(boost::is_reference<int&>::value);
	BOOST_CHECK(boost::is_reference<int&&>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_reference)
{
	using namespace test_type_traits::c_test_is_reference;

	BOOST_CHECK(!test_type_traits::primary_type_categories::is_reference<A>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_reference_v<A>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference<A&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference_v<A&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference<A&&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference_v<A&&>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_reference<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_reference_v<int>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference<int&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference_v<int&>);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference<int&&>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_reference_v<int&&>);
}

BOOST_AUTO_TEST_CASE(c_std_is_member_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_member_pointer;

	//C++17
	//template<class T>
	//inline constexpr bool is_member_pointer_v;

	using namespace test_type_traits::c_std_is_member_pointer;

	BOOST_CHECK(std::is_member_pointer<int(cls::*)>::value);
	BOOST_CHECK(std::is_member_pointer_v<int(cls::*)>);
	BOOST_CHECK(!std::is_member_pointer<int>::value);
	BOOST_CHECK(!std::is_member_pointer_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_member_pointer)
{
	//#include <boost/type_traits/is_member_pointer.hpp>
	//template<class T>
	//struct is_member_pointer;

	using namespace test_type_traits::c_boost_is_member_pointer;

	BOOST_CHECK(boost::is_member_pointer<int(cls::*)>::value);
	BOOST_CHECK(!boost::is_member_pointer<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_member_pointer)
{
	using namespace test_type_traits::c_test_is_member_pointer;

	BOOST_CHECK(test_type_traits::primary_type_categories::is_member_pointer<int(cls::*)>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_member_pointer_v<int(cls::*)>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_member_pointer<int>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_member_pointer_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_member_function_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_member_function_pointer;

	//C++17
	//template<class T>
	//inline constexpr bool is_member_function_pointer_v;

	using namespace test_type_traits::c_std_is_member_function_pointer;

	//若A::member是数据成员而非函数，则在编译时失败
	static_assert(std::is_member_function_pointer<decltype(&A::member)>::value, "A::member is not a member function.");
	static_assert(std::is_member_function_pointer_v<decltype(&A::member)>, "A::member is not a member function.");
}

BOOST_AUTO_TEST_CASE(c_boost_is_member_function_pointer)
{
	//#include <boost/type_traits/detail/is_member_function_pointer_cxx_11.hpp>
	//template<class T>
	//struct is_rvalue_reference;
	using namespace test_type_traits::c_boost_is_member_function_pointer;

	//若A::member是数据成员而非函数，则在编译时失败
	static_assert(boost::is_member_function_pointer<decltype(&A::member)>::value, "A::member is not a member function.");
}

BOOST_AUTO_TEST_CASE(c_test_is_member_function_pointer)
{
	using namespace test_type_traits::c_test_is_member_function_pointer;

	//若A::member是数据成员而非函数，则在编译时失败
	static_assert(test_type_traits::primary_type_categories::is_member_function_pointer_helper_by_is_function::is_member_function_pointer<decltype(&A::member)>::value, "A::member is not a member function.");
	static_assert(test_type_traits::primary_type_categories::is_member_function_pointer_helper_by_is_function::is_member_function_pointer_v<decltype(&A::member)>, "A::member is not a member function.");
}

BOOST_AUTO_TEST_CASE(c_std_is_null_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_null_pointer;

	//C++17
	//template<class T>
	//inline constexpr bool is_null_pointer_v;

	BOOST_CHECK(std::is_null_pointer<decltype(nullptr)>::value);
	BOOST_CHECK(std::is_null_pointer_v<decltype(nullptr)>);
	BOOST_CHECK(!std::is_null_pointer<int*>::value);
	BOOST_CHECK(!std::is_null_pointer_v<int*>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_null_pointer)
{
	//boost没有
}

BOOST_AUTO_TEST_CASE(c_test_is_null_pointer)
{
	BOOST_CHECK(test_type_traits::primary_type_categories::is_null_pointer<decltype(nullptr)>::value);
	BOOST_CHECK(test_type_traits::primary_type_categories::is_null_pointer_v<decltype(nullptr)>);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_null_pointer<int*>::value);
	BOOST_CHECK(!test_type_traits::primary_type_categories::is_null_pointer_v<int*>);
}

BOOST_AUTO_TEST_CASE(c_std_is_base_of)
{
	//#include <type_traits>
	//C++11
	//template<class Base, class Derived>
	//struct is_base_of;

	//C++17
	//template<class Base, class Derived>
	//inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

	using namespace test_type_traits::c_std_is_base_of;

	BOOST_CHECK((std::is_base_of<A, B>::value));
	BOOST_CHECK((std::is_base_of_v<A, B>));
	BOOST_CHECK(!(std::is_base_of<B, A>::value));
	BOOST_CHECK(!(std::is_base_of_v<B, A>));
	BOOST_CHECK(!(std::is_base_of<C, B>::value));
	BOOST_CHECK(!(std::is_base_of_v<C, B>));
}

BOOST_AUTO_TEST_CASE(c_boost_is_base_of)
{
	//#include <boost/type_traits/is_base_of.hpp>
	//template<class T>
	//struct is_base_of;

	using namespace test_type_traits::c_boost_is_base_of;

	BOOST_CHECK((boost::is_base_of<A, B>::value));
	BOOST_CHECK(!(boost::is_base_of<B, A>::value));
	BOOST_CHECK(!(boost::is_base_of<C, B>::value));
}

BOOST_AUTO_TEST_CASE(c_test_is_base_of)
{
	using namespace test_type_traits::c_test_is_base_of;

	BOOST_CHECK((test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of<A, B>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of_v<A, B>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of<B, A>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of_v<B, A>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of<C, B>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_cppreference::is_base_of_v<C, B>));

	BOOST_CHECK((test_type_traits::type_relationships::is_base_of_by_boost::is_base_of<A, B>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_base_of_by_boost::is_base_of_v<A, B>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_boost::is_base_of<B, A>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_boost::is_base_of_v<B, A>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_boost::is_base_of<C, B>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_base_of_by_boost::is_base_of_v<C, B>));
}

BOOST_AUTO_TEST_CASE(c_std_is_convertible)
{
	//#include <type_traits>
	//C++11
	//template<class From, class To>
	//struct is_convertible;

	//C++20
	//template<class From, class To>
	//struct is_nothrow_convertible;

	using namespace test_type_traits::c_std_is_convertible;

	BOOST_CHECK((std::is_convertible<B*, A*>::value));
	BOOST_CHECK((std::is_convertible_v<B*, A*>));
	BOOST_CHECK(!(std::is_convertible<A*, B*>::value));
	BOOST_CHECK(!(std::is_convertible_v<A*, B*>));
	BOOST_CHECK(!(std::is_convertible<B*, C*>::value));
	BOOST_CHECK(!(std::is_convertible_v<B*, C*>));
	BOOST_CHECK((std::is_convertible<D, C>::value));
	BOOST_CHECK((std::is_convertible_v<D, C>));

	//完美转发构造函数使类能从任何类型转换
	BOOST_CHECK((std::is_convertible<A, E>::value));   //B C D等
	BOOST_CHECK((std::is_convertible_v<A, E>));   //B C D等
}

BOOST_AUTO_TEST_CASE(c_boost_is_convertible)
{
	//#include <boost/type_traits/is_convertible.hpp>
	//template<class T>
	//struct is_convertible;

	using namespace test_type_traits::c_boost_is_convertible;

	BOOST_CHECK((boost::is_convertible<B*, A*>::value));
	BOOST_CHECK(!(boost::is_convertible<A*, B*>::value));
	BOOST_CHECK(!(boost::is_convertible<B*, C*>::value));
	BOOST_CHECK((boost::is_convertible<D, C>::value));

	//完美转发构造函数使类能从任何类型转换
	BOOST_CHECK((boost::is_convertible<A, E>::value));   //B C D等
}

BOOST_AUTO_TEST_CASE(c_test_is_convertible)
{
	using namespace test_type_traits::c_test_is_convertible;

	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible<B*, A*>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible_v<B*, A*>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible<A*, B*>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible_v<A*, B*>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible<B*, C*>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible_v<B*, C*>));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible<D, C>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible_v<D, C>));

	//完美转发构造函数使类能从任何类型转换
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible<A, E>::value));   //B C D等
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_cppreference::is_convertible_v<A, E>));   //B C D等


	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible<B*, A*>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible_v<B*, A*>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_boost::is_convertible<A*, B*>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_boost::is_convertible_v<A*, B*>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_boost::is_convertible<B*, C*>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_convertible_by_boost::is_convertible_v<B*, C*>));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible<D, C>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible_v<D, C>));

	//完美转发构造函数使类能从任何类型转换
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible<A, E>::value));   //B C D等
	BOOST_CHECK((test_type_traits::type_relationships::is_convertible_by_boost::is_convertible_v<A, E>));   //B C D等
}

BOOST_AUTO_TEST_CASE(c_std_is_same)
{
	//#include <type_traits>
	//C++11
	//template<class T, class U>
	//struct is_same;

	//若'int'为32位，则通常为true
	BOOST_CHECK((std::is_same<int, std::int32_t>::value));
	BOOST_CHECK((std::is_same_v<int, std::int32_t>));
	//若使用ILP64数据模型则可能为true
	BOOST_CHECK(!(std::is_same<int, std::int64_t>::value));
	BOOST_CHECK(!(std::is_same_v<int, std::int64_t>));

	BOOST_CHECK(!(std::is_same<float, std::int32_t>::value));
	BOOST_CHECK(!(std::is_same_v<float, std::int32_t>));

	//'int'为隐式的'signed'
	BOOST_CHECK((std::is_same<int, int>::value));
	BOOST_CHECK((std::is_same_v<int, int>));
	BOOST_CHECK(!(std::is_same<int, unsigned int>::value));
	BOOST_CHECK(!(std::is_same_v<int, unsigned int>));
	BOOST_CHECK((std::is_same<int, signed int>::value));
	BOOST_CHECK((std::is_same_v<int, signed int>));

	//不同于其他类型，'char'既非'unsigned'亦非'signed'
	BOOST_CHECK((std::is_same<char, char>::value));
	BOOST_CHECK((std::is_same_v<char, char>));
	BOOST_CHECK(!(std::is_same<char, unsigned char>::value));
	BOOST_CHECK(!(std::is_same_v<char, unsigned char>));
	BOOST_CHECK(!(std::is_same<char, signed char>::value));
	BOOST_CHECK(!(std::is_same_v<char, signed char>));
}

BOOST_AUTO_TEST_CASE(c_boost_is_same)
{
	//#include <boost/type_traits/is_same.hpp>
	//template<class T, class U>
	//struct is_same;

		//若'int'为32位，则通常为true
	BOOST_CHECK((boost::is_same<int, std::int32_t>::value));
	//若使用ILP64数据模型则可能为true
	BOOST_CHECK(!(boost::is_same<int, std::int64_t>::value));

	BOOST_CHECK(!(boost::is_same<float, std::int32_t>::value));

	//'int'为隐式的'signed'
	BOOST_CHECK((boost::is_same<int, int>::value));
	BOOST_CHECK(!(boost::is_same<int, unsigned int>::value));
	BOOST_CHECK((boost::is_same<int, signed int>::value));

	//不同于其他类型，'char'既非'unsigned'亦非'signed'
	BOOST_CHECK((boost::is_same<char, char>::value));
	BOOST_CHECK(!(boost::is_same<char, unsigned char>::value));
	BOOST_CHECK(!(boost::is_same<char, signed char>::value));
}

BOOST_AUTO_TEST_CASE(c_test_is_same)
{
	//若'int'为32位，则通常为true
	BOOST_CHECK((test_type_traits::type_relationships::is_same<int, std::int32_t>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_same_v<int, std::int32_t>));
	//若使用ILP64数据模型则可能为true
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same<int, std::int64_t>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same_v<int, std::int64_t>));

	BOOST_CHECK(!(test_type_traits::type_relationships::is_same<float, std::int32_t>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same_v<float, std::int32_t>));

	//'int'为隐式的'signed'
	BOOST_CHECK((test_type_traits::type_relationships::is_same<int, int>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_same_v<int, int>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same<int, unsigned int>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same_v<int, unsigned int>));
	BOOST_CHECK((test_type_traits::type_relationships::is_same<int, signed int>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_same_v<int, signed int>));

	//不同于其他类型，'char'既非'unsigned'亦非'signed'
	BOOST_CHECK((test_type_traits::type_relationships::is_same<char, char>::value));
	BOOST_CHECK((test_type_traits::type_relationships::is_same_v<char, char>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same<char, unsigned char>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same_v<char, unsigned char>));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same<char, signed char>::value));
	BOOST_CHECK(!(test_type_traits::type_relationships::is_same_v<char, signed char>));
}

BOOST_AUTO_TEST_CASE(c_std_alignment_of)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct alignment_of;

	//C++17
	//template<class T>
	//inline constexpr std::size_t alignment_of_v;

	using namespace test_type_traits::c_std_alignment_of;

	BOOST_CHECK(std::alignment_of<A>::value == 1);
	BOOST_CHECK(std::alignment_of_v<A> == 1);
	BOOST_CHECK(std::alignment_of<int>() == 4);    //另一种语法
	BOOST_CHECK(std::alignment_of<double>::value == 8);
	BOOST_CHECK(std::alignment_of_v<double> == 8);
}

BOOST_AUTO_TEST_CASE(c_boost_alignment_of)
{
	//#include <boost/type_traits/alignment_of.hpp>
	//template<class T>
	//struct alignment_of;

	using namespace test_type_traits::c_boost_alignment_of;

	BOOST_CHECK(boost::alignment_of<A>::value == 1);
	BOOST_CHECK(boost::alignment_of<int>() == 4);    //另一种语法
	BOOST_CHECK(boost::alignment_of<double>::value == 8);
}

BOOST_AUTO_TEST_CASE(c_test_alignment_of)
{
	using namespace test_type_traits::c_test_alignment_of;

	BOOST_CHECK(test_type_traits::property_queries::alignment_of<A>::value == 1);
	BOOST_CHECK(test_type_traits::property_queries::alignment_of_v<A> == 1);
	BOOST_CHECK(test_type_traits::property_queries::alignment_of<int>() == 4);    //另一种语法
	BOOST_CHECK(test_type_traits::property_queries::alignment_of<double>::value == 8);
	BOOST_CHECK(test_type_traits::property_queries::alignment_of_v<double> == 8);
}

BOOST_AUTO_TEST_CASE(c_std_extent)
{
	//#include <type_traits>
	//C++11
	//template<class T, unsigned N = 0>
	//struct extent;

	BOOST_CHECK((std::extent<int[3]>::value == 3));
	BOOST_CHECK((std::extent<int[3][4], 0>::value == 3));
	BOOST_CHECK((std::extent<int[3][4], 1>::value == 4));
	BOOST_CHECK((std::extent<int[3][4], 2>::value == 0));  //默认维度为0
	BOOST_CHECK((std::extent<int[]>::value == 0));

	const auto ext = std::extent<int[9]>{};   //隐式转换到std::size_t
	BOOST_CHECK(ext == 9);

	const int ints[] = { 1,2,3,4 };
	BOOST_CHECK(std::extent<decltype(ints)>::value);  //数组大小
}

BOOST_AUTO_TEST_CASE(c_boost_extent)
{
	//#include <boost/type_traits/extent.hpp>
	//template<class T>
	//struct extent;

	BOOST_CHECK((boost::extent<int[3]>::value == 3));
	BOOST_CHECK((boost::extent<int[3][4], 0>::value == 3));
	BOOST_CHECK((boost::extent<int[3][4], 1>::value == 4));
	BOOST_CHECK((boost::extent<int[3][4], 2>::value == 0));  //默认维度为0
	BOOST_CHECK((boost::extent<int[]>::value == 0));

	const auto ext = boost::extent<int[9]>{};   //隐式转换到std::size_t
	BOOST_CHECK(ext == 9);

	const int ints[] = { 1,2,3,4 };
	BOOST_CHECK(boost::extent<decltype(ints)>::value);  //数组大小
}

BOOST_AUTO_TEST_CASE(c_test_extent)
{
	BOOST_CHECK((test_type_traits::property_queries::extent<int[3]>::value == 3));
	BOOST_CHECK((test_type_traits::property_queries::extent<int[3][4], 0>::value == 3));
	BOOST_CHECK((test_type_traits::property_queries::extent<int[3][4], 1>::value == 4));
	BOOST_CHECK((test_type_traits::property_queries::extent<int[3][4], 2>::value == 0));  //默认维度为0
	BOOST_CHECK((test_type_traits::property_queries::extent<int[]>::value == 0));

	const auto ext = test_type_traits::property_queries::extent<int[9]>{};   //隐式转换到std::size_t
	BOOST_CHECK(ext == 9);

	const int ints[] = { 1,2,3,4 };
	BOOST_CHECK(test_type_traits::property_queries::extent<decltype(ints)>::value);  //数组大小
}

BOOST_AUTO_TEST_CASE(c_std_rank)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct rank;

	BOOST_CHECK(std::rank<int[1][2][3]>::value == 3);
	BOOST_CHECK(std::rank_v<int[1][2][3]> == 3);
	BOOST_CHECK(std::rank<int[][2][3][4]>::value == 4);
	BOOST_CHECK(std::rank_v<int[][2][3][4]> == 4);
	BOOST_CHECK(std::rank<int>::value == 0);
	BOOST_CHECK(std::rank_v<int> == 0);
}

BOOST_AUTO_TEST_CASE(c_boost_rank)
{
	//#include <boost/type_traits/rank.hpp>
	//template<class T>
	//struct rank;

	BOOST_CHECK(boost::rank<int[1][2][3]>::value == 3);
	BOOST_CHECK(boost::rank<int[][2][3][4]>::value == 4);
	BOOST_CHECK(boost::rank<int>::value == 0);
}

BOOST_AUTO_TEST_CASE(c_test_rank)
{
	BOOST_CHECK(test_type_traits::property_queries::rank<int[1][2][3]>::value == 3);
	BOOST_CHECK(test_type_traits::property_queries::rank_v<int[1][2][3]> == 3);
	BOOST_CHECK(test_type_traits::property_queries::rank<int[][2][3][4]>::value == 4);
	BOOST_CHECK(test_type_traits::property_queries::rank_v<int[][2][3][4]> == 4);
	BOOST_CHECK(test_type_traits::property_queries::rank<int>::value == 0);
	BOOST_CHECK(test_type_traits::property_queries::rank_v<int> == 0);
}

BOOST_AUTO_TEST_CASE(c_std_add_cv)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct add_cv;

	using namespace test_type_traits::c_std_add_cv;

	foo{}.m();   //non-cv
	std::add_const<foo>::type{}.m();
	std::add_volatile<foo>::type{}.m();
	std::add_cv<foo>::type{}.m();
}

BOOST_AUTO_TEST_CASE(c_boost_add_cv)
{
	//#include <boost/type_traits/add_cv.hpp>
	//template<class T>
	//struct add_cv;

	using namespace test_type_traits::c_boost_add_cv;

	foo{}.m();   //non-cv
	boost::add_const<foo>::type{}.m();
	boost::add_volatile<foo>::type{}.m();
	boost::add_cv<foo>::type{}.m();
}

BOOST_AUTO_TEST_CASE(c_test_add_cv)
{
	using namespace test_type_traits::c_test_add_cv;

	foo{}.m();   //non-cv
	test_type_traits::type_transformations::const_volatile_qualifications::add_const<foo>::type{}.m();
	test_type_traits::type_transformations::const_volatile_qualifications::add_volatile<foo>::type{}.m();
	test_type_traits::type_transformations::const_volatile_qualifications::add_cv<foo>::type{}.m();
}

BOOST_AUTO_TEST_CASE(c_std_remove_cv)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct remove_cv;

	BOOST_CHECK((std::is_same<int, std::remove_cv<const int>::type>::value));
	BOOST_CHECK((std::is_same<int, std::remove_cv_t<const int>>::value));
	BOOST_CHECK((std::is_same<int, std::remove_cv<volatile int>::type>::value));
	BOOST_CHECK((std::is_same<int, std::remove_cv_t<volatile int>>::value));
	BOOST_CHECK((std::is_same<int, std::remove_cv<const volatile int>::type>::value));
	BOOST_CHECK((std::is_same<int, std::remove_cv_t<const volatile int>>::value));
	BOOST_CHECK((std::is_same<const volatile int*, std::remove_cv<const volatile int*>::type>::value));
	BOOST_CHECK((std::is_same<const volatile int*, std::remove_cv_t<const volatile int*>>::value));
	BOOST_CHECK((std::is_same<int*, std::remove_cv<int * const volatile>::type>::value));
	BOOST_CHECK((std::is_same<int*, std::remove_cv_t<int * const volatile>>::value));
}

BOOST_AUTO_TEST_CASE(c_boost_remove_cv)
{
	//#include <boost/type_traits/remove_cv.hpp>
	//template<class T>
	//struct remove_cv;

	BOOST_CHECK((std::is_same<int, boost::remove_cv<const int>::type>::value));
	BOOST_CHECK((std::is_same<int, boost::remove_cv<volatile int>::type>::value));
	BOOST_CHECK((std::is_same<int, boost::remove_cv<const volatile int>::type>::value));
	BOOST_CHECK((std::is_same<const volatile int*, boost::remove_cv<const volatile int*>::type>::value));
	BOOST_CHECK((std::is_same<int*, boost::remove_cv<int* const volatile>::type>::value));
}

BOOST_AUTO_TEST_CASE(c_test_remove_cv)
{
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv<const int>::type>::value));
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv_t<const int>>::value));
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv<volatile int>::type>::value));
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv_t<volatile int>>::value));
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv<const volatile int>::type>::value));
	BOOST_CHECK((std::is_same<int, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv_t<const volatile int>>::value));
	BOOST_CHECK((std::is_same<const volatile int*, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv<const volatile int*>::type>::value));
	BOOST_CHECK((std::is_same<const volatile int*, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv_t<const volatile int*>>::value));
	BOOST_CHECK((std::is_same<int*, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv<int* const volatile>::type>::value));
	BOOST_CHECK((std::is_same<int*, test_type_traits::type_transformations::const_volatile_qualifications::remove_cv_t<int* const volatile>>::value));
}

BOOST_AUTO_TEST_CASE(c_std_add_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct add_pointer;

	int i = 123;
	int& ri = i;

	using IntPtr = std::add_pointer<decltype(i)>::type;
	using IntPtr2 = std::add_pointer<decltype(ri)>::type;

	IntPtr pi = &i;

	BOOST_CHECK(*pi == 123);

	static_assert(std::is_pointer<IntPtr>::value, "IntPtr should be a pointer");
	static_assert(std::is_same<IntPtr, int*>::value, "IntPtr should be a pointer to int");
	static_assert(std::is_same<IntPtr2, IntPtr>::value, "IntPtr2 should be equal to IntPtr");

	using IntAgain = std::remove_pointer<IntPtr>::type;
	IntAgain j = i;
	BOOST_CHECK(j == 123);

	static_assert(!std::is_pointer<IntAgain>::value, "IntAgain should not be a pointer");
	static_assert(std::is_same<IntAgain, int>::value, "IntAgain should be equal to int");
}

BOOST_AUTO_TEST_CASE(c_boost_add_pointer)
{
	//#include <boost/type_traits/add_pointer.hpp>
	//template<class T>
	//struct add_pointer;

	int i = 123;
	int& ri = i;

	using IntPtr = boost::add_pointer<decltype(i)>::type;
	using IntPtr2 = boost::add_pointer<decltype(ri)>::type;

	IntPtr pi = &i;

	BOOST_CHECK(*pi == 123);

	static_assert(std::is_pointer<IntPtr>::value, "IntPtr should be a pointer");
	static_assert(std::is_same<IntPtr, int*>::value, "IntPtr should be a pointer to int");
	static_assert(std::is_same<IntPtr2, IntPtr>::value, "IntPtr2 should be equal to IntPtr");

	using IntAgain = std::remove_pointer<IntPtr>::type;
	IntAgain j = i;
	BOOST_CHECK(j == 123);

	static_assert(!std::is_pointer<IntAgain>::value, "IntAgain should not be a pointer");
	static_assert(std::is_same<IntAgain, int>::value, "IntAgain should be equal to int");
}

BOOST_AUTO_TEST_CASE(c_test_add_pointer)
{
	{
		int i = 123;
		int& ri = i;

		using IntPtr = test_type_traits::type_transformations::compound_type_alterations::add_pointer_by_cppreference::add_pointer<decltype(i)>::type;
		using IntPtr2 = test_type_traits::type_transformations::compound_type_alterations::add_pointer_by_cppreference::add_pointer<decltype(ri)>::type;

		IntPtr pi = &i;

		BOOST_CHECK(*pi == 123);

		static_assert(std::is_pointer<IntPtr>::value, "IntPtr should be a pointer");
		static_assert(std::is_same<IntPtr, int*>::value, "IntPtr should be a pointer to int");
		static_assert(std::is_same<IntPtr2, IntPtr>::value, "IntPtr2 should be equal to IntPtr");

		using IntAgain = boost::remove_pointer<IntPtr>::type;
		IntAgain j = i;
		BOOST_CHECK(j == 123);

		static_assert(!std::is_pointer<IntAgain>::value, "IntAgain should not be a pointer");
		static_assert(std::is_same<IntAgain, int>::value, "IntAgain should be equal to int");
	}

	{
		int i = 123;
		int& ri = i;

		using IntPtr = test_type_traits::type_transformations::compound_type_alterations::add_pointer_by_boost::add_pointer<decltype(i)>::type;
		using IntPtr2 = test_type_traits::type_transformations::compound_type_alterations::add_pointer_by_boost::add_pointer<decltype(ri)>::type;

		IntPtr pi = &i;

		BOOST_CHECK(*pi == 123);

		static_assert(std::is_pointer<IntPtr>::value, "IntPtr should be a pointer");
		static_assert(std::is_same<IntPtr, int*>::value, "IntPtr should be a pointer to int");
		static_assert(std::is_same<IntPtr2, IntPtr>::value, "IntPtr2 should be equal to IntPtr");

		using IntAgain = boost::remove_pointer<IntPtr>::type;
		IntAgain j = i;
		BOOST_CHECK(j == 123);

		static_assert(!std::is_pointer<IntAgain>::value, "IntAgain should not be a pointer");
		static_assert(std::is_same<IntAgain, int>::value, "IntAgain should be equal to int");
	}
	
}

BOOST_AUTO_TEST_CASE(c_std_remove_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct remove_pointer;

	BOOST_CHECK((std::is_same_v<int, std::remove_pointer<int>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer_t<int>>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer<int*>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer_t<int*>>));
	BOOST_CHECK(!(std::is_same_v<int, std::remove_pointer<int**>::type>));
	BOOST_CHECK(!(std::is_same_v<int, std::remove_pointer_t<int**>>));

	BOOST_CHECK((std::is_same_v<int, std::remove_pointer<int* const>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer_t<int* const>>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer<int* volatile>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer_t<int* volatile>>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer<int* const volatile>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_pointer_t<int* const volatile>>));
}

BOOST_AUTO_TEST_CASE(c_boost_remove_pointer)
{
	//#include <boost/type_traits/remove_pointer.hpp>
	//template<class T>
	//struct remove_pointer;

	BOOST_CHECK((std::is_same_v<int, boost::remove_pointer<int>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::remove_pointer<int*>::type>));
	BOOST_CHECK(!(std::is_same_v<int, boost::remove_pointer<int**>::type>));

	BOOST_CHECK((std::is_same_v<int, boost::remove_pointer<int* const>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::remove_pointer<int* volatile>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::remove_pointer<int* const volatile>::type>));
}

BOOST_AUTO_TEST_CASE(c_test_remove_pointer)
{
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int>>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int*>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int*>>));
	BOOST_CHECK(!(std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int**>::type>));
	BOOST_CHECK(!(std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int**>>));

	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int* const>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int* const>>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int* volatile>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int* volatile>>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer<int* const volatile>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_pointer_t<int* const volatile>>));
}

BOOST_AUTO_TEST_CASE(c_std_remove_reference)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct remove_reference;

	BOOST_CHECK((std::is_same_v<int, std::remove_reference<int>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_reference_t<int>>));
	BOOST_CHECK((std::is_same_v<int, std::remove_reference<int&>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_reference_t<int&>>));
	BOOST_CHECK((std::is_same_v<int, std::remove_reference<int&&>::type>));
	BOOST_CHECK((std::is_same_v<int, std::remove_reference_t<int&&>>));
}

BOOST_AUTO_TEST_CASE(c_boost_remove_reference)
{
	//#include <boost/type_traits/remove_reference.hpp>
	//template<class T>
	//struct remove_reference;

	BOOST_CHECK((std::is_same_v<int, boost::remove_reference<int>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::remove_reference<int&>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::remove_reference<int&&>::type>));
}

BOOST_AUTO_TEST_CASE(c_test_remove_reference)
{
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference<int>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference_t<int>>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference<int&>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference_t<int&>>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference<int&&>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::compound_type_alterations::remove_reference_t<int&&>>));
}

BOOST_AUTO_TEST_CASE(c_std_add_lvalue_reference_add_rvalue_reference)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct add_lvalue_reference;
	//template<class T>
	//struct add_rvalue_reference;

	using nonref = int;
	using lref = typename std::add_lvalue_reference<nonref>::type;
	//using lref = typename std::add_lvalue_reference_t<nonref>;
	using rref = typename std::add_rvalue_reference<nonref>::type;
	//using rref = typename std::add_rvalue_reference_t<nonref>;
	
	BOOST_CHECK(!std::is_lvalue_reference<nonref>::value);
	BOOST_CHECK(std::is_lvalue_reference<lref>::value);
	BOOST_CHECK(std::is_rvalue_reference<rref>::value);
}

BOOST_AUTO_TEST_CASE(c_boost_add_lvalue_reference_add_rvalue_reference)
{
	//#include <boost/type_traits/add_lvalue_reference.hpp>
	//template<class T>
	//struct add_lvalue_reference;

	//#include <boost/type_traits/add_rvalue_reference.hpp>
	//template<class T>
	//struct add_rvalue_reference;

	using nonref = int;
	using lref = typename boost::add_lvalue_reference<nonref>::type;
	using rref = typename boost::add_rvalue_reference<nonref>::type;
	
	BOOST_CHECK(!std::is_lvalue_reference<nonref>::value);
	BOOST_CHECK(std::is_lvalue_reference<lref>::value);
	BOOST_CHECK(std::is_rvalue_reference<rref>::value);
}

BOOST_AUTO_TEST_CASE(c_test_add_lvalue_reference_add_rvalue_reference)
{
	{
		using nonref = int;
		using lref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_cppreference::add_lvalue_reference<nonref>::type;
		//using lref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_cppreference::add_lvalue_reference_t<nonref>;
		using rref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_cppreference::add_rvalue_reference<nonref>::type;
		//using rref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_cppreference::add_rvalue_reference_t<nonref>;

		BOOST_CHECK(!std::is_lvalue_reference<nonref>::value);
		BOOST_CHECK(std::is_lvalue_reference<lref>::value);
		BOOST_CHECK(std::is_rvalue_reference<rref>::value);
	}

	{
		using nonref = int;
		using lref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_boost::add_lvalue_reference<nonref>::type;
		//using lref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_boost::add_lvalue_reference_t<nonref>;
		using rref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_boost::add_rvalue_reference<nonref>::type;
		//using rref = typename test_type_traits::type_transformations::compound_type_alterations::add_lvalue_reference_add_rvalue_reference_by_boost::add_rvalue_reference_t<nonref>;

		BOOST_CHECK(!std::is_lvalue_reference<nonref>::value);
		BOOST_CHECK(std::is_lvalue_reference<lref>::value);
		BOOST_CHECK(std::is_rvalue_reference<rref>::value);
	}
}


BOOST_AUTO_TEST_CASE(c_std_remove_extent)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct remove_extent;

	using namespace test_type_traits::c_std_remove_extent;

	int a[][3] = { {1,2,3}, {4,5,6} };
	//print(a);  //编译时错误
	print(a[1]); //输出4 5 6
}

BOOST_AUTO_TEST_CASE(c_boost_remove_extent)
{
	//#include <boost/type_traits/remove_extent.hpp>
	//template<class T>
	//struct remove_extent;

	using namespace test_type_traits::c_boost_remove_extent;

	int a[][3] = { {1,2,3}, {4,5,6} };
	//print(a);  //编译时错误
	print(a[1]); //输出4 5 6
}

BOOST_AUTO_TEST_CASE(c_test_remove_extent)
{
	using namespace test_type_traits::c_test_remove_extent;

	int a[][3] = { {1,2,3}, {4,5,6} };
	//print(a);  //编译时错误
	print(a[1]); //输出4 5 6
}

BOOST_AUTO_TEST_CASE(c_std_remove_all_extent)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct remove_all_extent;

	using namespace test_type_traits::c_std_remove_all_extents;

	float a1[1][2][3];
	int a2[3][2];
	float a3[1][1][1][1][2];
	double a4[2][3];

	foo(a1);
	foo(a2);
	foo(a3);
	foo(a4);

	//输出：
	//underlying type : f
	//underlying type : i
	//underlying type : f
	//underlying type : d
}

BOOST_AUTO_TEST_CASE(c_boost_remove_all_extents)
{
	//#include <boost/type_traits/remove_all_extents.hpp>
	//template<class T>
	//struct remove_all_extents;

	using namespace test_type_traits::c_boost_remove_all_extents;

	float a1[1][2][3];
	int a2[3][2];
	float a3[1][1][1][1][2];
	double a4[2][3];

	foo(a1);
	foo(a2);
	foo(a3);
	foo(a4);

	//输出：
	//underlying type : f
	//underlying type : i
	//underlying type : f
	//underlying type : d
}

BOOST_AUTO_TEST_CASE(c_test_remove_all_extents)
{
	using namespace test_type_traits::c_test_remove_all_extents;

	float a1[1][2][3];
	int a2[3][2];
	float a3[1][1][1][1][2];
	double a4[2][3];

	foo(a1);
	foo(a2);
	foo(a3);
	foo(a4);

	//输出：
	//underlying type : f
	//underlying type : i
	//underlying type : f
	//underlying type : d
}

BOOST_AUTO_TEST_CASE(c_std_decay)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct decay;

	using namespace test_type_traits::c_std_decay;

	BOOST_CHECK((decay_equiv<int, int>::value));
	BOOST_CHECK((decay_equiv<int&, int>::value));
	BOOST_CHECK((decay_equiv<int&&, int>::value));
	BOOST_CHECK((decay_equiv<const int&, int>::value));
	BOOST_CHECK((decay_equiv<int[2], int*>::value));
	BOOST_CHECK((decay_equiv<int(int), int(*)(int)>::value));
}

BOOST_AUTO_TEST_CASE(c_boost_decay)
{
	//#include <boost/type_traits/decay.hpp>
	//template<class T>
	//struct decay;

	using namespace test_type_traits::c_boost_decay;

	BOOST_CHECK((decay_equiv<int, int>::value));
	BOOST_CHECK((decay_equiv<int&, int>::value));
	BOOST_CHECK((decay_equiv<int&&, int>::value));
	BOOST_CHECK((decay_equiv<const int&, int>::value));
	BOOST_CHECK((decay_equiv<int[2], int*>::value));
	BOOST_CHECK((decay_equiv<int(int), int(*)(int)>::value));
}

BOOST_AUTO_TEST_CASE(c_test_decay)
{
	using namespace test_type_traits::c_test_decay;

	BOOST_CHECK((decay_equiv<int, int>::value));
	BOOST_CHECK((decay_equiv<int&, int>::value));
	BOOST_CHECK((decay_equiv<int&&, int>::value));
	BOOST_CHECK((decay_equiv<const int&, int>::value));
	BOOST_CHECK((decay_equiv<int[2], int*>::value));
	BOOST_CHECK((decay_equiv<int(int), int(*)(int)>::value));
}

BOOST_AUTO_TEST_CASE(c_std_make_signed)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct make_signed;

	using char_type = std::make_signed<char>::type;
	//using char_type = std::make_signed_t<char>;
	using int_type = std::make_signed<int>::type;
	//using int_type = std::make_signed_t<int>;
	using long_type = std::make_signed<volatile long>::type;
	//using long_type = std::make_signed_t<volatile long>;

	BOOST_CHECK((std::is_same_v<char_type, signed char>));
	BOOST_CHECK((std::is_same_v<int_type, signed int>));
	BOOST_CHECK((std::is_same_v<long_type, volatile signed long>));
}

BOOST_AUTO_TEST_CASE(c_boost_make_signed)
{
	//#include <boost/type_traits/make_signed.hpp>
	//template<class T>
	//struct make_signed;

	using char_type = boost::make_signed<char>::type;
	using int_type = boost::make_signed<int>::type;
	using long_type = boost::make_signed<volatile long>::type;

	BOOST_CHECK((std::is_same_v<char_type, signed char>));
	BOOST_CHECK((std::is_same_v<int_type, signed int>));
	BOOST_CHECK((std::is_same_v<long_type, volatile signed long>));
}

BOOST_AUTO_TEST_CASE(c_test_make_signed)
{
	//略
}

BOOST_AUTO_TEST_CASE(c_std_make_unsigned)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct make_unsigned;

	using char_type = std::make_unsigned<char>::type;
	//using char_type = std::make_unsigned_t<char>;
	using int_type = std::make_unsigned<int>::type;
	//using int_type = std::make_unsigned_t<int>;
	using long_type = std::make_unsigned<volatile long>::type;
	//using long_type = std::make_unsigned_t<volatile long>;

	BOOST_CHECK((std::is_same_v<char_type, unsigned char>));
	BOOST_CHECK((std::is_same_v<int_type, unsigned int>));
	BOOST_CHECK((std::is_same_v<long_type, volatile unsigned long>));
}

BOOST_AUTO_TEST_CASE(c_boost_make_unsigned)
{
	//#include <boost/type_traits/make_unsigned.hpp>
	//template<class T>
	//struct make_unsigned;

	using char_type = boost::make_unsigned<char>::type;
	using int_type = boost::make_unsigned<int>::type;
	using long_type = boost::make_unsigned <volatile long>::type;

	BOOST_CHECK((std::is_same_v<char_type, unsigned char>));
	BOOST_CHECK((std::is_same_v<int_type, unsigned int>));
	BOOST_CHECK((std::is_same_v<long_type, volatile unsigned long>));
}

BOOST_AUTO_TEST_CASE(c_test_make_unsigned)
{
	//略
}

BOOST_AUTO_TEST_CASE(c_std_underlying_type)
{
	//#include <type_traits>
	//C++11
	//template< class T >
	//struct underlying_type;

	using namespace test_type_traits::c_std_underlying_type;

	BOOST_CHECK((std::is_same<int, typename std::underlying_type<e1>::type>::value));
	BOOST_CHECK((std::is_same<int, std::underlying_type_t<e1>>::value));
	BOOST_CHECK((std::is_same<int, typename std::underlying_type<e2>::type>::value));
	BOOST_CHECK((std::is_same<int, std::underlying_type_t<e2>>::value));
}

BOOST_AUTO_TEST_CASE(c_boost_underlying_type)
{
	//boost没有underlying_type
}

BOOST_AUTO_TEST_CASE(c_test_underlying_type)
{
	//没有
}

BOOST_AUTO_TEST_CASE(c_std_conditional)
{
	//#include <type_traits>
	//C++11
	//template<bool B, class T, class F>
	//struct conditional;

	BOOST_CHECK((std::is_same_v<int, std::conditional<true, int, double>::type>));
	BOOST_CHECK((std::is_same_v<int, std::conditional_t<true, int, double>>));
	BOOST_CHECK((std::is_same_v<double, std::conditional<false, int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, std::conditional_t<false, int, double>>));
	BOOST_CHECK((std::is_same_v<double, std::conditional<sizeof(int) >= sizeof(double), int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, std::conditional_t<sizeof(int) >= sizeof(double), int, double>>));
}

BOOST_AUTO_TEST_CASE(c_boost_conditional)
{
	//#include <boost/type_traits/conditional.hpp>
	//template<bool B, class T, class F>
	//struct conditional;

	BOOST_CHECK((std::is_same_v<int, boost::conditional<true, int, double>::type>));
	BOOST_CHECK((std::is_same_v<int, boost::conditional_t<true, int, double>>));
	BOOST_CHECK((std::is_same_v<double, boost::conditional<false, int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, boost::conditional_t<false, int, double>>));
	BOOST_CHECK((std::is_same_v<double, boost::conditional<sizeof(int) >= sizeof(double), int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, boost::conditional_t<sizeof(int) >= sizeof(double), int, double>>));
}

BOOST_AUTO_TEST_CASE(c_test_conditional)
{
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::other_type_generators::conditional<true, int, double>::type>));
	BOOST_CHECK((std::is_same_v<int, test_type_traits::type_transformations::other_type_generators::conditional_t<true, int, double>>));
	BOOST_CHECK((std::is_same_v<double, test_type_traits::type_transformations::other_type_generators::conditional<false, int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, test_type_traits::type_transformations::other_type_generators::conditional_t<false, int, double>>));
	BOOST_CHECK((std::is_same_v<double, test_type_traits::type_transformations::other_type_generators::conditional<sizeof(int) >= sizeof(double), int, double>::type>));
	BOOST_CHECK((std::is_same_v<double, test_type_traits::type_transformations::other_type_generators::conditional_t<sizeof(int) >= sizeof(double), int, double>>));
}

BOOST_AUTO_TEST_CASE(c_std_common_type)
{
	//#include <type_traits>
	//C++11
	//template<class... T>
	//struct common_type;

	using namespace test_type_traits::c_std_common_type;

	Number<int> i1 = { 1 }, i2 = {2};
	Number<double> d1 = { 2.3 }, d2 = { 3.5 };
	std::cout << (i1 + i2).n << '\n';
	std::cout << (i1 + d2).n << '\n';
	std::cout << (d1 + i2).n << '\n';
	std::cout << (d1 + d2).n << '\n';
}

BOOST_AUTO_TEST_CASE(c_boost_common_type)
{
	//#include <boost/type_traits/common_type.hpp>
	//template<class... T>
	//struct common_type;

	using namespace test_type_traits::c_boost_common_type;

	Number<int> i1 = { 1 }, i2 = {2};
	Number<double> d1 = { 2.3 }, d2 = { 3.5 };
	std::cout << (i1 + i2).n << '\n';
	std::cout << (i1 + d2).n << '\n';
	std::cout << (d1 + i2).n << '\n';
	std::cout << (d1 + d2).n << '\n';
}

BOOST_AUTO_TEST_CASE(c_test_common_type)
{
	using namespace test_type_traits::c_test_common_type;

	Number<int> i1 = { 1 }, i2 = {2};
	Number<double> d1 = { 2.3 }, d2 = { 3.5 };
	std::cout << (i1 + i2).n << '\n';
	std::cout << (i1 + d2).n << '\n';
	std::cout << (d1 + i2).n << '\n';
	std::cout << (d1 + d2).n << '\n';
}

BOOST_AUTO_TEST_CASE(c_std_aligned_storage)
{
	//#include <type_traits>
	//C++11
	//template<std::size_t Len, std::size_t Align = /*default-alignment*/>
	//struct aligned_storage;

	using namespace test_type_traits::c_std_aligned_storage;

	static_vector<std::string, 10> v1;
	v1.emplace_back(5, '*');
	v1.emplace_back(10, '*');
	std::cout << v1[0] << '\n' << v1[1] << '\n';
}

BOOST_AUTO_TEST_CASE(c_boost_aligned_storage)
{
	//#include <boost/type_traits/aligned_storage.hpp>
	//template<std::size_t Len, std::size_t Align = /*default-alignment*/>
	//struct aligned_storage;

	using namespace test_type_traits::c_boost_aligned_storage;

	static_vector<std::string, 10> v1;
	v1.emplace_back(5, '*');
	v1.emplace_back(10, '*');
	std::cout << v1[0] << '\n' << v1[1] << '\n';
}

BOOST_AUTO_TEST_CASE(c_test_aligned_storage)
{
	using namespace test_type_traits::c_test_aligned_storage;

	static_vector<std::string, 10> v1;
	v1.emplace_back(5, '*');
	v1.emplace_back(10, '*');
	std::cout << v1[0] << '\n' << v1[1] << '\n';
}

BOOST_AUTO_TEST_CASE(c_std_invoke_result)
{
	using namespace test_type_traits::c_std_invoke_result;

	//调用自由函数
	std::invoke(print_num, -9);

	//调用lambda
	std::invoke([]() {print_num(42); });

	//调用成员函数
	const Foo foo(314159);
	std::invoke(&Foo::print_add, foo, 1);

	//调用(访问)数据成员
	std::cout << std::invoke(&Foo::num_, foo) << '\n';

	//调用函数对象
	std::invoke(PrintNum(), 18);
}

BOOST_AUTO_TEST_CASE(c_boost_invoke_result)
{
	//boost中invoke函数返回值用boost::result_of类模板表示，而std::result_of已被弃用

	using namespace test_type_traits::c_boost_invoke_result;

}

BOOST_AUTO_TEST_CASE(c_test_invoke_result)
{
	using namespace test_type_traits::c_test_invoke_result;

	//有问题，待解决

	////调用自由函数
	//test_type_traits::type_transformations::other_type_generators::invoke_result_invoke_by_cppreference::invoke(print_num, -9);

	////调用lambda
	//test_type_traits::type_transformations::other_type_generators::invoke_result_invoke_by_cppreference::invoke([]() {print_num(42); });

	////调用成员函数
	//const Foo foo(314159);
	//test_type_traits::type_transformations::other_type_generators::invoke_result_invoke_by_cppreference::invoke(&Foo::print_add, foo, 1);

	////调用(访问)数据成员
	//std::cout << test_type_traits::type_transformations::other_type_generators::invoke_result_invoke_by_cppreference::invoke(&Foo::num_, foo) << '\n';

	////调用函数对象
	//test_type_traits::type_transformations::other_type_generators::invoke_result_invoke_by_cppreference::invoke(PrintNum(), 18);
}
BOOST_AUTO_TEST_CASE(c_std_enable_if)
{
	using namespace test_type_traits::c_std_enable_if;

	std::aligned_union_t<0, int, std::string> u;

	construct(reinterpret_cast<int*>(&u));
	destroy(reinterpret_cast<int*>(&u));

	construct(reinterpret_cast<std::string*>(&u), "Hello");
	destroy(reinterpret_cast<std::string*>(&u));

	A<int>{};   //OK:匹配初等模板
	A<double>{};//Ok:匹配偏特化

}

BOOST_AUTO_TEST_CASE(c_boost_enable_if)
{
	//略
}

BOOST_AUTO_TEST_CASE(c_test_enable_if)
{
	//略
}

BOOST_AUTO_TEST_CASE(c_std_is_member_object_pointer)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_member_object_pointer;

	//C++17
	//template<class T>
	//inline constexpr bool is_member_object_pointer_v;

	using namespace test_type_traits::c_std_is_member_object_pointer;

	BOOST_CHECK(std::is_member_object_pointer<int(cls::*)>::value);
	BOOST_CHECK(std::is_member_object_pointer_v<int(cls::*)>);
	BOOST_CHECK(!std::is_member_object_pointer<int(cls::*)()>::value);
	BOOST_CHECK(!std::is_member_object_pointer_v<int(cls::*)()>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_member_object_pointer)
{
	//#include <boost/type_traits/is_member_object_pointer.hpp>
	//template<class T>
	//struct is_member_object_pointer;

	using namespace test_type_traits::c_boost_is_member_object_pointer;

	BOOST_CHECK(boost::is_member_object_pointer<int(cls::*)>::value);
	BOOST_CHECK(!boost::is_member_object_pointer<int(cls::*)()>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_member_object_pointer)
{
	using namespace test_type_traits::c_test_is_member_object_pointer;

	BOOST_CHECK(test_type_traits::composite_type_categories::is_member_object_pointer<int(cls::*)>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_member_object_pointer_v<int(cls::*)>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_member_object_pointer<int(cls::*)()>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_member_object_pointer_v<int(cls::*)()>);
}

BOOST_AUTO_TEST_CASE(c_std_is_arithmetic)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_arithmetic;

	//C++17
	//template<class T>
	//inline constexpr bool is_arithmetic_v;

	using namespace test_type_traits::c_std_is_arithmetic;

	BOOST_CHECK(!std::is_arithmetic<A>::value);
	BOOST_CHECK(!std::is_arithmetic_v<A>);
	BOOST_CHECK(std::is_arithmetic<bool>::value);
	BOOST_CHECK(std::is_arithmetic_v<bool>);
	BOOST_CHECK(std::is_arithmetic<int>::value);
	BOOST_CHECK(std::is_arithmetic_v<int>);
	BOOST_CHECK(std::is_arithmetic<int const>::value);
	BOOST_CHECK(std::is_arithmetic_v<int const>);
	BOOST_CHECK(!std::is_arithmetic<int&>::value);
	BOOST_CHECK(!std::is_arithmetic_v<int&>);
	BOOST_CHECK(!std::is_arithmetic<int*>::value);
	BOOST_CHECK(!std::is_arithmetic_v<int*>);
	BOOST_CHECK(std::is_arithmetic<float>::value);
	BOOST_CHECK(std::is_arithmetic_v<float>);
	BOOST_CHECK(std::is_arithmetic<float const>::value);
	BOOST_CHECK(std::is_arithmetic_v<float const>);
	BOOST_CHECK(!std::is_arithmetic<float&>::value);
	BOOST_CHECK(!std::is_arithmetic_v<float&>);
	BOOST_CHECK(!std::is_arithmetic<float*>::value);
	BOOST_CHECK(!std::is_arithmetic_v<float*>);
	BOOST_CHECK(std::is_arithmetic<char>::value);
	BOOST_CHECK(std::is_arithmetic_v<char>);
	BOOST_CHECK(std::is_arithmetic<char const>::value);
	BOOST_CHECK(std::is_arithmetic_v<char const>);
	BOOST_CHECK(!std::is_arithmetic<char&>::value);
	BOOST_CHECK(!std::is_arithmetic_v<char&>);
	BOOST_CHECK(!std::is_arithmetic<char*>::value);
	BOOST_CHECK(!std::is_arithmetic_v<char*>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_arithmetic)
{
	//#include <boost/type_traits/is_arithmetic.hpp>
	//template<class T>
	//struct is_arithmetic;

	using namespace test_type_traits::c_boost_is_arithmetic;

	BOOST_CHECK(!boost::is_arithmetic<A>::value);
	BOOST_CHECK(boost::is_arithmetic<bool>::value);
	BOOST_CHECK(boost::is_arithmetic<int>::value);
	BOOST_CHECK(boost::is_arithmetic<int const>::value);
	BOOST_CHECK(!boost::is_arithmetic<int&>::value);
	BOOST_CHECK(!boost::is_arithmetic<int*>::value);
	BOOST_CHECK(boost::is_arithmetic<float>::value);
	BOOST_CHECK(boost::is_arithmetic<float const>::value);
	BOOST_CHECK(!boost::is_arithmetic<float&>::value);
	BOOST_CHECK(!boost::is_arithmetic<float*>::value);
	BOOST_CHECK(boost::is_arithmetic<char>::value);
	BOOST_CHECK(boost::is_arithmetic<char const>::value);
	BOOST_CHECK(!boost::is_arithmetic<char&>::value);
	BOOST_CHECK(!boost::is_arithmetic<char*>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_arithmetic)
{
	using namespace test_type_traits::c_test_is_arithmetic;

	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<A>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<A>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<bool>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<bool>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<int>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<int>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<int const>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<int const>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<int&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<int&>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<int*>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<int*>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<float>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<float>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<float const>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<float const>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<float&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<float&>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<float*>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<float*>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<char>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<char>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic<char const>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_arithmetic_v<char const>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<char&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<char&>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic<char*>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_arithmetic_v<char*>);
}

BOOST_AUTO_TEST_CASE(c_std_is_fundamental)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_fundamental;

	//C++17
	//template<class T>
	//inline constexpr bool is_fundamental_v;

	using namespace test_type_traits::c_std_is_fundamental;

	BOOST_CHECK(!std::is_fundamental<A>::value);
	BOOST_CHECK(!std::is_fundamental_v<A>);
	BOOST_CHECK(std::is_fundamental<int>::value);
	BOOST_CHECK(std::is_fundamental_v<int>);
	BOOST_CHECK(!std::is_fundamental<int&>::value);
	BOOST_CHECK(!std::is_fundamental_v<int&>);
	BOOST_CHECK(!std::is_fundamental<int*>::value);
	BOOST_CHECK(!std::is_fundamental_v<int*>);
	BOOST_CHECK(std::is_fundamental<float>::value);
	BOOST_CHECK(std::is_fundamental_v<float>);
	BOOST_CHECK(!std::is_fundamental<float&>::value);
	BOOST_CHECK(!std::is_fundamental_v<float&>);
	BOOST_CHECK(!std::is_fundamental<float*>::value);
	BOOST_CHECK(!std::is_fundamental_v<float*>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_fundamental)
{
	//#include <boost/type_traits/is_fundamental.hpp>
	//template<class T>
	//struct is_fundamental;

	using namespace test_type_traits::c_boost_is_fundamental;

	BOOST_CHECK(!boost::is_fundamental<A>::value);
	BOOST_CHECK(boost::is_fundamental<int>::value);
	BOOST_CHECK(!boost::is_fundamental<int&>::value);
	BOOST_CHECK(!boost::is_fundamental<int*>::value);
	BOOST_CHECK(boost::is_fundamental<float>::value);
	BOOST_CHECK(!boost::is_fundamental<float&>::value);
	BOOST_CHECK(!boost::is_fundamental<float*>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_fundamental)
{
	using namespace test_type_traits::c_test_is_fundamental;

	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental<A>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental_v<A>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_fundamental<int>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_fundamental_v<int>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental<int&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental_v<int&>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental<int*>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental_v<int*>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_fundamental<float>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_fundamental_v<float>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental<float&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental_v<float&>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental<float*>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_fundamental_v<float*>);
}

BOOST_AUTO_TEST_CASE(c_std_is_compound)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_compound;

	//C++17
	//template<class T>
	//inline constexpr bool is_compound_v;

	using namespace test_type_traits::c_std_is_compound;

	BOOST_CHECK(std::is_compound<cls>::value);
	BOOST_CHECK(std::is_compound_v<cls>);
	BOOST_CHECK(!std::is_compound<int>::value);
	BOOST_CHECK(!std::is_compound_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_compound)
{
	//#include <boost/type_traits/is_compound.hpp>
	//template<class T>
	//struct is_compound;

	using namespace test_type_traits::c_boost_is_compound;

	BOOST_CHECK(boost::is_compound<cls>::value);
	BOOST_CHECK(!boost::is_compound<int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_compound)
{
	using namespace test_type_traits::c_test_is_compound;

	BOOST_CHECK(test_type_traits::composite_type_categories::is_compound<cls>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_compound_v<cls>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_compound<int>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_compound_v<int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_scalar)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_scalar;

	//C++17
	//template<class T>
	//inline constexpr bool is_scalar_v;

	using namespace test_type_traits::c_std_is_scalar;

	BOOST_CHECK(std::is_scalar<int>::value);
	BOOST_CHECK(std::is_scalar_v<int>);
	BOOST_CHECK(!std::is_scalar<cls>::value);
	BOOST_CHECK(!std::is_scalar_v<cls>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_scalar)
{
	//#include <boost/type_traits/is_scalar.hpp>
	//template<class T>
	//struct is_scalar;

	using namespace test_type_traits::c_boost_is_scalar;

	BOOST_CHECK(boost::is_scalar<int>::value);
	BOOST_CHECK(!boost::is_scalar<cls>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_scalar)
{
	using namespace test_type_traits::c_test_is_scalar;

	BOOST_CHECK(test_type_traits::composite_type_categories::is_scalar<int>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_scalar_v<int>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_scalar<cls>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_scalar_v<cls>);
}

BOOST_AUTO_TEST_CASE(c_std_is_object)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_object;

	//C++17
	//template<class T>
	//inline constexpr bool is_object_v;

	using namespace test_type_traits::c_std_is_object;

	BOOST_CHECK(std::is_object<int>::value);
	BOOST_CHECK(std::is_object_v<int>);
	BOOST_CHECK(!std::is_object<int&>::value);
	BOOST_CHECK(!std::is_object_v<int&>);
	BOOST_CHECK(std::is_object<cls>::value);
	BOOST_CHECK(std::is_object_v<cls>);
	BOOST_CHECK(!std::is_object<cls&>::value);
	BOOST_CHECK(!std::is_object_v<cls&>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_object)
{
	//#include <boost/type_traits/is_object.hpp>
	//template<class T>
	//struct is_object;

	using namespace test_type_traits::c_boost_is_object;

	BOOST_CHECK(boost::is_object<int>::value);
	BOOST_CHECK(!boost::is_object<int&>::value);
	BOOST_CHECK(boost::is_object<cls>::value);
	BOOST_CHECK(!boost::is_object<cls&>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_object)
{
	using namespace test_type_traits::c_test_is_object;

	BOOST_CHECK(test_type_traits::composite_type_categories::is_object<int>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_object_v<int>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_object<int&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_object_v<int&>);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_object<cls>::value);
	BOOST_CHECK(test_type_traits::composite_type_categories::is_object_v<cls>);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_object<cls&>::value);
	BOOST_CHECK(!test_type_traits::composite_type_categories::is_object_v<cls&>);
}

BOOST_AUTO_TEST_CASE(c_std_is_abstract)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_abstract;

	//C++17
	//template<class T>
	//inline constexpr bool is_abstract_v;

	using namespace test_type_traits::c_std_is_abstract;

	BOOST_CHECK(!std::is_abstract<A>::value);
	BOOST_CHECK(!std::is_abstract_v<A>);
	BOOST_CHECK(!std::is_abstract<B>::value);
	BOOST_CHECK(!std::is_abstract_v<B>);
	BOOST_CHECK(std::is_abstract<C>::value);
	BOOST_CHECK(std::is_abstract_v<C>);
	BOOST_CHECK(std::is_abstract<D>::value);
	BOOST_CHECK(std::is_abstract_v<D>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_abstract)
{
	//#include <boost/type_traits/is_abstract.hpp>
	//template<class T>
	//struct is_abstract;

	using namespace test_type_traits::c_boost_is_abstract;

	BOOST_CHECK(!boost::is_abstract<A>::value);
	BOOST_CHECK(!boost::is_abstract<B>::value);
	BOOST_CHECK(boost::is_abstract<C>::value);
	BOOST_CHECK(boost::is_abstract<D>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_abstract)
{
	using namespace test_type_traits::c_test_is_abstract;

	BOOST_CHECK(!test_type_traits::type_properties::is_abstract_by_is_abstract::is_abstract<A>::value);
	BOOST_CHECK(!std::is_abstract_v<A>);
	BOOST_CHECK(!std::is_abstract<B>::value);
	BOOST_CHECK(!std::is_abstract_v<B>);
	BOOST_CHECK(std::is_abstract<C>::value);
	BOOST_CHECK(std::is_abstract_v<C>);
	BOOST_CHECK(std::is_abstract<D>::value);
	BOOST_CHECK(std::is_abstract_v<D>);
}

BOOST_AUTO_TEST_CASE(c_std_is_const)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_const;

	//C++17
	//template<class T>
	//inline constexpr bool is_const_v;

	BOOST_CHECK(!std::is_const<int>::value);
	BOOST_CHECK(!std::is_const_v<int>);
	BOOST_CHECK(std::is_const<const int>::value);
	BOOST_CHECK(std::is_const_v<const int>);
	BOOST_CHECK(!std::is_const<const int*>::value);
	BOOST_CHECK(!std::is_const_v<const int*>);
	BOOST_CHECK(!std::is_const<const int&>::value);
	BOOST_CHECK(!std::is_const_v<const int&>);
	BOOST_CHECK(std::is_const<int* const>::value);
	BOOST_CHECK(std::is_const_v<int* const>);
	BOOST_CHECK(std::is_const<std::remove_reference_t<const int&>>::value);
	BOOST_CHECK(std::is_const_v<std::remove_reference_t<const int&>>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_const)
{
	//#include <boost/type_traits/is_const.hpp>
	//template<class T>
	//struct is_const;

	BOOST_CHECK(!boost::is_const<int>::value);
	BOOST_CHECK(boost::is_const<const int>::value);
	BOOST_CHECK(!boost::is_const<const int*>::value);
	BOOST_CHECK(!boost::is_const<const int&>::value);
	BOOST_CHECK(boost::is_const<int* const>::value);
	BOOST_CHECK(boost::is_const<std::remove_reference_t<const int&>>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_const)
{
	BOOST_CHECK(!test_type_traits::type_properties::is_const<int>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_const_v<int>);
	BOOST_CHECK(test_type_traits::type_properties::is_const<const int>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_const_v<const int>);
	BOOST_CHECK(!test_type_traits::type_properties::is_const<const int*>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_const_v<const int*>);
	BOOST_CHECK(!test_type_traits::type_properties::is_const<const int&>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_const_v<const int&>);
	BOOST_CHECK(test_type_traits::type_properties::is_const<int* const>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_const_v<int* const>);
	BOOST_CHECK(test_type_traits::type_properties::is_const<std::remove_reference_t<const int&>>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_const_v<std::remove_reference_t<const int&>>);
}

BOOST_AUTO_TEST_CASE(c_std_is_volatile)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_volatile;

	//C++17
	//template<class T>
	//inline constexpr bool is_volatile_v;

	BOOST_CHECK(!std::is_volatile<int>::value);
	BOOST_CHECK(!std::is_volatile_v<int>);
	BOOST_CHECK(std::is_volatile<volatile int>::value);
	BOOST_CHECK(std::is_volatile_v<volatile int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_volatile)
{
	//#include <boost/type_traits/is_volatile.hpp>
	//template<class T>
	//struct is_volatile;

	BOOST_CHECK(!boost::is_volatile<int>::value);
	BOOST_CHECK(boost::is_volatile<volatile int>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_volatile)
{
	BOOST_CHECK(!test_type_traits::type_properties::is_volatile<int>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_volatile_v<int>);
	BOOST_CHECK(test_type_traits::type_properties::is_volatile<volatile int>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_volatile_v<volatile int>);
}

BOOST_AUTO_TEST_CASE(c_std_is_empty)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_empty;

	//C++17
	//template<class T>
	//inline constexpr bool is_empty_v;

	using namespace test_type_traits::c_std_is_empty;

	BOOST_CHECK(std::is_empty<A>::value);
	BOOST_CHECK(std::is_empty_v<A>);
	BOOST_CHECK(!std::is_empty<B>::value);
	BOOST_CHECK(!std::is_empty_v<B>);
	BOOST_CHECK(!std::is_empty<C>::value);
	BOOST_CHECK(!std::is_empty_v<C>);
	BOOST_CHECK(!std::is_empty<D>::value);
	BOOST_CHECK(!std::is_empty_v<D>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_empty)
{
	//boost中没有is_empty
}

BOOST_AUTO_TEST_CASE(c_test_is_empty)
{
	//未知
}

BOOST_AUTO_TEST_CASE(c_std_is_polymorphic)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_polymorphic;

	//C++17
	//template<class T>
	//inline constexpr bool is_polymorphic_v;

	using namespace test_type_traits::c_std_is_polymorphic;

	BOOST_CHECK(!std::is_polymorphic<A>::value);
	BOOST_CHECK(!std::is_polymorphic_v<A>);
	BOOST_CHECK(std::is_polymorphic<B>::value);
	BOOST_CHECK(std::is_polymorphic_v<B>);
	BOOST_CHECK(std::is_polymorphic<C>::value);
	BOOST_CHECK(std::is_polymorphic_v<C>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_polymorphic)
{
	//#include <boost/type_traits/is_polymorphic.hpp>
	//template<class T>
	//struct is_polymorphic;

	using namespace test_type_traits::c_boost_is_polymorphic;

	BOOST_CHECK(!boost::is_polymorphic<A>::value);
	BOOST_CHECK(boost::is_polymorphic<B>::value);
	BOOST_CHECK(boost::is_polymorphic<C>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_polymorphic)
{
	using namespace test_type_traits::c_test_is_polymorphic;

	BOOST_CHECK(!test_type_traits::type_properties::is_polymorphic<A>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_polymorphic_v<A>);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic<B>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_v<B>);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic<C>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_v<C>);

	BOOST_CHECK(!test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic<A>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic_v<A>);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic<B>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic_v<B>);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic<C>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_polymorphic_by_is_polymorphic::is_polymorphic_v<C>);
}

BOOST_AUTO_TEST_CASE(c_std_is_signed)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_signed;

	//C++17
	//template<class T>
	//inline constexpr bool is_signed_v;

	using namespace test_type_traits::c_std_is_signed;

	BOOST_CHECK(!std::is_signed<A>::value);
	BOOST_CHECK(!std::is_signed_v<A>);
	BOOST_CHECK(std::is_signed<float>::value);//与boost库不同
	BOOST_CHECK(std::is_signed_v<float>);//与boost库不同
	BOOST_CHECK(std::is_signed<signed int>::value);
	BOOST_CHECK(std::is_signed_v<signed int>);
	BOOST_CHECK(!std::is_signed<unsigned int>::value);
	BOOST_CHECK(!std::is_signed_v<unsigned int>);
	BOOST_CHECK(!std::is_signed<B>::value);//与boost库不同
	BOOST_CHECK(!std::is_signed_v<B>);//与boost库不同
	BOOST_CHECK(!std::is_signed<C>::value);//与boost库不同
	BOOST_CHECK(!std::is_signed_v<C>);//与boost库不同

	//简短形式
	std::cout << std::is_signed<signed int>() << '\n';
	std::cout << std::is_signed<unsigned int>() << '\n';
}
BOOST_AUTO_TEST_CASE(c_boost_is_signed)
{
	//#include <boost/type_traits/is_signed.hpp>
	//template<class T>
	//struct is_signed;

	using namespace test_type_traits::c_boost_is_signed;

	BOOST_CHECK(!boost::is_signed<A>::value);
	BOOST_CHECK(!boost::is_signed<float>::value);//与标准库不同
	BOOST_CHECK(boost::is_signed<signed int>::value);
	BOOST_CHECK(!boost::is_signed<unsigned int>::value);
	BOOST_CHECK(boost::is_signed<B>::value);//与标准库不同
	BOOST_CHECK(boost::is_signed<C>::value);//与标准库不同
}

BOOST_AUTO_TEST_CASE(c_test_is_signed)
{
	using namespace test_type_traits::c_test_is_signed;

	BOOST_CHECK(!test_type_traits::type_properties::is_signed<A>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed_v<A>);
	BOOST_CHECK(test_type_traits::type_properties::is_signed<float>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_signed_v<float>);
	BOOST_CHECK(test_type_traits::type_properties::is_signed<signed int>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_signed_v<signed int>);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed<unsigned int>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed_v<unsigned int>);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed<B>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed_v<B>);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed<C>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_signed_v<C>);
}

BOOST_AUTO_TEST_CASE(c_std_is_unsigned)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_unsigned;

	//C++17
	//template<class T>
	//inline constexpr bool is_unsigned_v;

	using namespace test_type_traits::c_std_is_unsigned;

	BOOST_CHECK(!std::is_unsigned<A>::value);
	BOOST_CHECK(!std::is_unsigned_v<A>);
	BOOST_CHECK(!std::is_unsigned<float>::value);
	BOOST_CHECK(!std::is_unsigned_v<float>);
	BOOST_CHECK(!std::is_unsigned<signed int>::value);
	BOOST_CHECK(!std::is_unsigned_v<signed int>);
	BOOST_CHECK(std::is_unsigned<unsigned int>::value);
	BOOST_CHECK(std::is_unsigned_v<unsigned int>);
	BOOST_CHECK(!std::is_unsigned<B>::value);//与boost库不同
	BOOST_CHECK(!std::is_unsigned_v<B>);//与boost库不同
	BOOST_CHECK(!std::is_unsigned<C>::value);//与boost库不同
	BOOST_CHECK(!std::is_unsigned_v<C>);//与boost库不同
}
BOOST_AUTO_TEST_CASE(c_boost_is_unsigned)
{
	//#include <boost/type_traits/is_unsigned.hpp>
	//template<class T>
	//struct is_unsigned;

	using namespace test_type_traits::c_boost_is_unsigned;

	BOOST_CHECK(!boost::is_unsigned<A>::value);
	BOOST_CHECK(!boost::is_unsigned<float>::value);
	BOOST_CHECK(!boost::is_unsigned<signed int>::value);
	BOOST_CHECK(boost::is_unsigned<unsigned int>::value);
	BOOST_CHECK(boost::is_unsigned<B>::value);//与标准库不同
	BOOST_CHECK(boost::is_unsigned<C>::value);//与标准库不同
}

BOOST_AUTO_TEST_CASE(c_test_is_unsigned)
{
	using namespace test_type_traits::c_test_is_unsigned;

	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned<A>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned_v<A>);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned<float>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned_v<float>);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned<signed int>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned_v<signed int>);
	BOOST_CHECK(test_type_traits::type_properties::is_unsigned<unsigned int>::value);
	BOOST_CHECK(test_type_traits::type_properties::is_unsigned_v<unsigned int>);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned<B>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned_v<B>);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned<C>::value);
	BOOST_CHECK(!test_type_traits::type_properties::is_unsigned_v<C>);
}

BOOST_AUTO_TEST_CASE(c_std_is_standard_layout)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_standard_layout;

	//C++17
	//template<class T>
	//inline constexpr bool is_standard_layout_v;

	using namespace test_type_traits::c_std_is_standard_layout;

	BOOST_CHECK(std::is_standard_layout<A>::value);
	BOOST_CHECK(std::is_standard_layout_v<A>);
	BOOST_CHECK(!std::is_standard_layout<B>::value);
	BOOST_CHECK(!std::is_standard_layout_v<B>);
	BOOST_CHECK(!std::is_standard_layout<C>::value);
	BOOST_CHECK(!std::is_standard_layout_v<C>);
}
BOOST_AUTO_TEST_CASE(c_boost_is_standard_layout)
{
	//boost没有is_standard_layout
}

BOOST_AUTO_TEST_CASE(c_test_is_standard_layout)
{
	using namespace test_type_traits::c_test_is_standard_layout;

	//未知
}

BOOST_AUTO_TEST_CASE(c_std_is_trivially_copyable)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_trivially_copyable;

	//C++17
	//template<class T>
	//inline constexpr bool is_trivially_copyable_v;

	using namespace test_type_traits::c_std_is_trivially_copyable;

	BOOST_CHECK(std::is_trivially_copyable<A>::value);
	BOOST_CHECK(std::is_trivially_copyable_v<A>);
	BOOST_CHECK(!std::is_trivially_copyable<B>::value);
	BOOST_CHECK(!std::is_trivially_copyable_v<B>);
	BOOST_CHECK(!std::is_trivially_copyable<C>::value);
	BOOST_CHECK(!std::is_trivially_copyable_v<C>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_trivially_copyable)
{
	//boost没有is_trivially_copyable
}

BOOST_AUTO_TEST_CASE(c_test_is_trivially_copyable)
{
	using namespace test_type_traits::c_test_is_trivially_copyable;

	//未知
}

BOOST_AUTO_TEST_CASE(c_std_is_trivial)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct is_trivial;

	//C++17
	//template<class T>
	//inline constexpr bool is_trivial_v;

	using namespace test_type_traits::c_std_is_trivial;

	BOOST_CHECK(std::is_trivial<A>::value);
	BOOST_CHECK(std::is_trivial_v<A>);
	BOOST_CHECK(!std::is_trivial<B>::value);
	BOOST_CHECK(!std::is_trivial_v<B>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_trivial)
{
	//boost没有is_trivial
}

BOOST_AUTO_TEST_CASE(c_test_is_trivial)
{
	using namespace test_type_traits::c_test_is_trivial;

	//未知
}

BOOST_AUTO_TEST_CASE(c_std_has_virtual_destructor)
{
	//#include <type_traits>
	//C++11
	//template<class T>
	//struct has_virtual_destructor;

	//C++17
	//template<class T>
	//inline constexpr bool has_virtual_destructor_v;

	using namespace test_type_traits::c_std_has_virtual_destructor;

	BOOST_CHECK(std::has_virtual_destructor<A>::value);
	BOOST_CHECK(std::has_virtual_destructor_v<A>);
	BOOST_CHECK(std::has_virtual_destructor<B>::value);
	BOOST_CHECK(std::has_virtual_destructor_v<B>);
	BOOST_CHECK(std::has_virtual_destructor<C>::value);
	BOOST_CHECK(std::has_virtual_destructor_v<C>);
	BOOST_CHECK(!std::has_virtual_destructor<std::string>::value);
	BOOST_CHECK(!std::has_virtual_destructor_v<std::string>);
	BOOST_CHECK(std::has_virtual_destructor<std::runtime_error>::value);
	BOOST_CHECK(std::has_virtual_destructor_v<std::runtime_error>);
}

BOOST_AUTO_TEST_CASE(c_boost_has_virtual_destructor)
{
	//#include <boost/type_traits/has_virtual_destructor.hpp>
	//template<class T>
	//struct has_virtual_destructor;

	using namespace test_type_traits::c_boost_has_virtual_destructor;

	BOOST_CHECK(boost::has_virtual_destructor<A>::value);
	BOOST_CHECK(boost::has_virtual_destructor<B>::value);
	BOOST_CHECK(boost::has_virtual_destructor<C>::value);
	BOOST_CHECK(!boost::has_virtual_destructor<std::string>::value);
	BOOST_CHECK(boost::has_virtual_destructor<std::runtime_error>::value);
}

BOOST_AUTO_TEST_CASE(c_test_has_virtual_destructor)
{
	using namespace test_type_traits::c_test_has_virtual_destructor;

	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor<A>::value);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor_v<A>);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor<B>::value);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor_v<B>);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor<C>::value);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor_v<C>);
	BOOST_CHECK(!test_type_traits::type_features::has_virtual_destructor<std::string>::value);
	BOOST_CHECK(!test_type_traits::type_features::has_virtual_destructor_v<std::string>);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor<std::runtime_error>::value);
	BOOST_CHECK(test_type_traits::type_features::has_virtual_destructor_v<std::runtime_error>);
}

BOOST_AUTO_TEST_CASE(c_std_is_assignable)
{
	//#include <type_traits>
	//C++11

	//template<class T, class U>
	//struct is_assignable;
	//template<class T, class U>
	//struct is_trivially_assignable;
	//template<class T, class U>
	//struct is_nothrow_assignable;


	//C++17
	//template<class T, class U>
	//inline constexpr bool is_assignable_v = is_assignable<T, U>::value;
	//template<class T, class U>
	//inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;
	//template<class T, class U>
	//inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

	using namespace test_type_traits::c_std_is_assignable;

	BOOST_CHECK(!(std::is_assignable<int, int>::value));      //1=1
	BOOST_CHECK(!(std::is_assignable_v<int, int>));      //1=1
	BOOST_CHECK((std::is_assignable<int&, int>::value));      //int a; a = 1;
	BOOST_CHECK((std::is_assignable_v<int&, int>));      //int a; a = 1;
	BOOST_CHECK(!(std::is_assignable<int, double>::value));
	BOOST_CHECK(!(std::is_assignable_v<int, double>));
	BOOST_CHECK((std::is_nothrow_assignable<int&, double>::value));
	BOOST_CHECK((std::is_nothrow_assignable_v<int&, double>));
	BOOST_CHECK((std::is_assignable<std::string, double>::value));
	BOOST_CHECK((std::is_assignable_v<std::string, double>));
	BOOST_CHECK((std::is_trivially_assignable<Exl&, const Exl&>::value));
	BOOST_CHECK((std::is_trivially_assignable_v<Exl&, const Exl&>));
}

BOOST_AUTO_TEST_CASE(c_boost_is_assignable)
{
	//#include <boost/type_traits/is_assignable.hpp>
	//template<class T, class U>
	//struct is_assignable;

	using namespace test_type_traits::c_boost_is_assignable;

	BOOST_CHECK(!(boost::is_assignable<int, int>::value));      //1=1
	BOOST_CHECK((boost::is_assignable<int&, int>::value));      //int a; a = 1;
	BOOST_CHECK(!(boost::is_assignable<int, double>::value));
	BOOST_CHECK((boost::is_assignable<std::string, double>::value));
}

BOOST_AUTO_TEST_CASE(c_test_is_assignable)
{
	using namespace test_type_traits::c_test_is_assignable;

	BOOST_CHECK(!(test_type_traits::type_features::is_assignable<int, int>::value));      //1=1
	BOOST_CHECK(!(test_type_traits::type_features::is_assignable_v<int, int>));      //1=1
	BOOST_CHECK((test_type_traits::type_features::is_assignable<int&, int>::value));      //int a; a = 1;
	BOOST_CHECK((test_type_traits::type_features::is_assignable_v<int&, int>));      //int a; a = 1;
	BOOST_CHECK(!(test_type_traits::type_features::is_assignable<int, double>::value));
	BOOST_CHECK(!(test_type_traits::type_features::is_assignable_v<int, double>));
	BOOST_CHECK((test_type_traits::type_features::is_assignable<std::string, double>::value));
	BOOST_CHECK((test_type_traits::type_features::is_assignable_v<std::string, double>));
}

BOOST_AUTO_TEST_CASE(c_std_is_copy_assignable)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_copy_assignable;
	//template<class T>
	//struct is_trivially_copy_assignable;
	//template<class T>
	//struct is_nothrow_copy_assignable;


	//C++17
	//template<class T>
	//inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

	using namespace test_type_traits::c_std_is_copy_assignable;

	BOOST_CHECK(std::is_trivially_copy_assignable<Foo>::value);
	BOOST_CHECK(std::is_trivially_copy_assignable_v<Foo>);
	BOOST_CHECK(!std::is_copy_assignable<int[2]>::value);
	BOOST_CHECK(!std::is_copy_assignable_v<int[2]>);
	BOOST_CHECK(std::is_nothrow_copy_assignable<int>::value);
	BOOST_CHECK(std::is_nothrow_copy_assignable_v<int>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_copy_assignable)
{
	//#include <boost/type_traits/is_copy_assignable.hpp>
	//template<class T>
	//struct is_copy_assignable;

	using namespace test_type_traits::c_boost_is_copy_assignable;

	BOOST_CHECK(!boost::is_copy_assignable<int[2]>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_copy_assignable)
{
	using namespace test_type_traits::c_test_is_copy_assignable;

	BOOST_CHECK(!test_type_traits::type_features::is_copy_assignable<int[2]>::value);
	BOOST_CHECK(!test_type_traits::type_features::is_copy_assignable_v<int[2]>);
}

BOOST_AUTO_TEST_CASE(c_std_is_move_assignable)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_move_assignable;
	//template<class T>
	//struct is_trivially_move_assignable;
	//template<class T>
	//struct is_nothrow_move_assignable;


	//C++17
	//template<class T>
	//inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

	using namespace test_type_traits::c_std_is_move_assignable;

	BOOST_CHECK(std::is_nothrow_move_assignable<std::string>::value);
	BOOST_CHECK(std::is_nothrow_move_assignable_v<std::string>);
	BOOST_CHECK(!std::is_move_assignable<int[2]>::value);
	BOOST_CHECK(!std::is_move_assignable_v<int[2]>);
	BOOST_CHECK(std::is_trivially_move_assignable<Foo>::value);
	BOOST_CHECK(std::is_trivially_move_assignable_v<Foo>);
	BOOST_CHECK(std::is_move_assignable<NoMove>::value);
	BOOST_CHECK(std::is_move_assignable_v<NoMove>);
	BOOST_CHECK(!std::is_nothrow_move_assignable<NoMove>::value);
	BOOST_CHECK(!std::is_nothrow_move_assignable_v<NoMove>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_move_assignable)
{
	//#include <boost/type_traits/is_copy_assignable.hpp>
	//template<class T>
	//struct is_copy_assignable;

	using namespace test_type_traits::c_boost_is_move_assignable;

	BOOST_CHECK(boost::is_nothrow_move_assignable<std::string>::value);
	BOOST_CHECK(!boost::is_nothrow_move_assignable<NoMove>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_move_assignable)
{
	using namespace test_type_traits::c_test_is_move_assignable;

	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_assignable<std::string>::value);
	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_assignable_v<std::string>);
	BOOST_CHECK(!test_type_traits::type_features::is_nothrow_move_assignable<NoMove>::value);
	BOOST_CHECK(!test_type_traits::type_features::is_nothrow_move_assignable_v<NoMove>);
}

BOOST_AUTO_TEST_CASE(c_std_is_destructible)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_destructible;
	//template<class T>
	//struct is_trivially_destructible;
	//template<class T>
	//struct is_nothrow_destructible;


	//C++17
	//template<class T>
	//inline constexpr bool is_destructible_v = is_destructible<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

	using namespace test_type_traits::c_std_is_destructible;

	BOOST_CHECK(std::is_destructible<std::string>::value);
	BOOST_CHECK(std::is_destructible_v<std::string>);
	BOOST_CHECK(std::is_nothrow_destructible<Foo>::value);
	BOOST_CHECK(std::is_nothrow_destructible_v<Foo>);
	BOOST_CHECK(std::is_trivially_destructible<Bar>::value);
	BOOST_CHECK(std::is_trivially_destructible_v<Bar>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_destructible)
{
	//#include <boost/type_traits/is_destructible.hpp>
	//template<class T>
	//struct is_destructible;

	using namespace test_type_traits::c_boost_is_move_assignable;

	BOOST_CHECK(boost::is_destructible<std::string>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_destructible)
{
	using namespace test_type_traits::c_test_is_move_assignable;

	BOOST_CHECK(test_type_traits::type_features::is_destructible<std::string>::value);
	BOOST_CHECK(test_type_traits::type_features::is_destructible_v<std::string>);
}

BOOST_AUTO_TEST_CASE(c_std_is_default_constructible)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_default_constructible;
	//template<class T>
	//struct is_trivially_default_constructible;
	//template<class T>
	//struct is_nothrow_default_constructible;


	//C++17
	//template<class T>
	//inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

	using namespace test_type_traits::c_std_is_default_constructible;

	BOOST_CHECK(std::is_default_constructible<Ex1>::value);
	BOOST_CHECK(std::is_default_constructible_v<Ex1>);
	BOOST_CHECK(!std::is_trivially_default_constructible<Ex1>::value);
	BOOST_CHECK(!std::is_trivially_default_constructible_v<Ex1>);
	BOOST_CHECK(std::is_trivially_default_constructible<Ex2>::value);
	BOOST_CHECK(std::is_trivially_default_constructible_v<Ex2>);
	BOOST_CHECK(std::is_nothrow_default_constructible<Ex2>::value);
	BOOST_CHECK(std::is_nothrow_default_constructible_v<Ex2>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_default_constructible)
{
	//#include <boost/type_traits/is_default_constructible.hpp>
	//template<class T>
	//struct is_default_constructible;

	using namespace test_type_traits::c_boost_is_default_constructible;

	BOOST_CHECK(boost::is_default_constructible<Ex1>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_default_constructible)
{
	using namespace test_type_traits::c_test_is_default_constructible;

	BOOST_CHECK(test_type_traits::type_features::is_default_constructible<Ex1>::value);
	BOOST_CHECK(test_type_traits::type_features::is_default_constructible_v<Ex1>);
}

BOOST_AUTO_TEST_CASE(c_std_is_constructible)
{
	//#include <type_traits>
	//C++11

	//template<class T, class... Args>
	//struct is_constructible;
	//template<class T, class... Args>
	//struct is_trivially_constructible;
	//template<class T, class... Args>
	//struct is_nothrow_constructible;


	//C++17
	//template<class T, class... Args>
	//inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;
	//template<class T, class... Args>
	//inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;
	//template<class T, class... Args>
	//inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

	using namespace test_type_traits::c_std_is_constructible;

	BOOST_CHECK((std::is_trivially_constructible<Foo, const Foo&>::value));
	BOOST_CHECK((std::is_trivially_constructible_v<Foo, const Foo&>));
	BOOST_CHECK(!(std::is_trivially_constructible<Foo, int>::value));
	BOOST_CHECK(!(std::is_trivially_constructible_v<Foo, int>));
	BOOST_CHECK((std::is_constructible<Foo, int>::value));
	BOOST_CHECK((std::is_constructible_v<Foo, int>));
	BOOST_CHECK(!(std::is_nothrow_constructible<Foo, int>::value));
	BOOST_CHECK(!(std::is_nothrow_constructible_v<Foo, int>));
	BOOST_CHECK((std::is_nothrow_constructible<Foo, int, double>::value));
	BOOST_CHECK((std::is_nothrow_constructible_v<Foo, int, double>));
}

BOOST_AUTO_TEST_CASE(c_boost_is_constructible)
{
	//#include <boost/type_traits/is_constructible.hpp>
	//template<class T>
	//struct is_constructible;

	using namespace test_type_traits::c_boost_is_constructible;

	BOOST_CHECK((boost::is_constructible<Foo, int>::value));
}

BOOST_AUTO_TEST_CASE(c_test_is_constructible)
{
	using namespace test_type_traits::c_test_is_constructible;

	BOOST_CHECK((test_type_traits::type_features::is_constructible<Foo, int>::value));
	BOOST_CHECK((test_type_traits::type_features::is_constructible_v<Foo, int>));
}

BOOST_AUTO_TEST_CASE(c_std_is_copy_constructible)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_copy_constructible;
	//template<class T>
	//struct is_trivially_copy_constructible;
	//template<class T>
	//struct is_nothrow_copy_constructible;

	//C++17
	//template<class T>
	//inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

	using namespace test_type_traits::c_std_is_copy_constructible;

	BOOST_CHECK(std::is_copy_constructible<Ex1>::value);
	BOOST_CHECK(std::is_copy_constructible_v<Ex1>);
	BOOST_CHECK(!std::is_trivially_copy_constructible<Ex1>::value);
	BOOST_CHECK(!std::is_trivially_copy_constructible_v<Ex1>);
	BOOST_CHECK(std::is_trivially_copy_constructible<Ex2>::value);
	BOOST_CHECK(std::is_trivially_copy_constructible_v<Ex2>);
	BOOST_CHECK(std::is_nothrow_copy_constructible<Ex2>::value);
	BOOST_CHECK(std::is_nothrow_copy_constructible_v<Ex2>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_copy_constructible)
{
	//#include <boost/type_traits/is_copy_constructible.hpp>
	//template<class T>
	//struct is_copy_constructible;

	using namespace test_type_traits::c_boost_is_copy_constructible;

	BOOST_CHECK(boost::is_copy_constructible<Ex1>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_copy_constructible)
{
	using namespace test_type_traits::c_test_is_copy_constructible;

	BOOST_CHECK(test_type_traits::type_features::is_copy_constructible<Ex1>::value);
	BOOST_CHECK(test_type_traits::type_features::is_copy_constructible_v<Ex1>);
}

BOOST_AUTO_TEST_CASE(c_std_is_move_constructible)
{
	//#include <type_traits>
	//C++11

	//template<class T>
	//struct is_move_constructible;
	//template<class T>
	//struct is_trivially_move_constructible;
	//template<class T>
	//struct is_nothrow_move_constructible;

	//C++17
	//template<class T>
	//inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;
	//template<class T>
	//inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

	using namespace test_type_traits::c_std_is_move_constructible;

	BOOST_CHECK(std::is_move_constructible<Ex1>::value);
	BOOST_CHECK(std::is_move_constructible_v<Ex1>);
	BOOST_CHECK(!std::is_trivially_move_constructible<Ex1>::value);
	BOOST_CHECK(!std::is_trivially_move_constructible_v<Ex1>);
	BOOST_CHECK(std::is_nothrow_move_constructible<Ex1>::value);
	BOOST_CHECK(std::is_nothrow_move_constructible_v<Ex1>);
	BOOST_CHECK(std::is_trivially_move_constructible<Ex2>::value);
	BOOST_CHECK(std::is_trivially_move_constructible_v<Ex2>);
	BOOST_CHECK(std::is_nothrow_move_constructible<Ex2>::value);
	BOOST_CHECK(std::is_nothrow_move_constructible_v<Ex2>);
	BOOST_CHECK(std::is_move_constructible<NoMove>::value);
	BOOST_CHECK(std::is_move_constructible_v<NoMove>);
	BOOST_CHECK(!std::is_nothrow_move_constructible<NoMove>::value);
	BOOST_CHECK(!std::is_nothrow_move_constructible_v<NoMove>);
}

BOOST_AUTO_TEST_CASE(c_boost_is_move_constructible)
{
	//#include <boost/type_traits/is_nothrow_move_constructible.hpp>
	//template<class T>
	//struct is_nothrow_move_constructible;

	using namespace test_type_traits::c_boost_is_move_constructible;

	BOOST_CHECK(boost::is_nothrow_move_constructible<Ex1>::value);
	BOOST_CHECK(boost::is_nothrow_move_constructible<Ex2>::value);
	BOOST_CHECK(!boost::is_nothrow_move_constructible<NoMove>::value);
}

BOOST_AUTO_TEST_CASE(c_test_is_move_constructible)
{
	using namespace test_type_traits::c_test_is_move_constructible;

	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_constructible<Ex1>::value);
	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_constructible_v<Ex1>);
	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_constructible<Ex2>::value);
	BOOST_CHECK(test_type_traits::type_features::is_nothrow_move_constructible_v<Ex2>);
	BOOST_CHECK(!test_type_traits::type_features::is_nothrow_move_constructible<NoMove>::value);
	BOOST_CHECK(!test_type_traits::type_features::is_nothrow_move_constructible_v<NoMove>);
}

BOOST_AUTO_TEST_SUITE_END()