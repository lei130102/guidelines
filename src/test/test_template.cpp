#include <test_template.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <test_template.h>

BOOST_AUTO_TEST_SUITE(s_template)

BOOST_AUTO_TEST_CASE(c_template)
{

}

BOOST_AUTO_TEST_CASE(c_template_template_value)
{
	using namespace test_template_value;

	//从右算起到第一个1为止，共有多少个0
	BOOST_CHECK((ls_zeros<0>::value == 0));
	BOOST_CHECK((ls_zeros<1>::value == 0));
	BOOST_CHECK((ls_zeros<2>::value == 1));  //2:010
	BOOST_CHECK((ls_zeros<3>::value == 0));  //3:011
	BOOST_CHECK((ls_zeros<4>::value == 2));  //4:100
	BOOST_CHECK((ls_zeros<5>::value == 0));  //5:101
}

BOOST_AUTO_TEST_CASE(c_template_pack_options)
{
	using namespace xll;

	////测试

	BOOST_CHECK((std::is_same_v<build_number_seq<0>::type, index_tuple<>> == true));
	BOOST_CHECK((std::is_same_v<build_number_seq<1>::type, index_tuple<0>> == true));
	BOOST_CHECK((std::is_same_v<build_number_seq<2>::type, index_tuple<0,1>> == true));
	BOOST_CHECK((std::is_same_v<build_number_seq<3>::type, index_tuple<0,1,2>> == true));

	BOOST_CHECK((std::is_same_v<typelist_element<0, typelist<int ,double, long>>::type, int> == true));
	BOOST_CHECK((std::is_same_v<typelist_element<1, typelist<int ,double, long>>::type, double> == true));
	BOOST_CHECK((std::is_same_v<typelist_element<2, typelist<int ,double, long>>::type, long> == true));

	BOOST_CHECK((sizeof_typelist<typelist<>>::value == 0));
	BOOST_CHECK((sizeof_typelist<typelist<int>>::value == 1));
	BOOST_CHECK((sizeof_typelist<typelist<int, double>>::value == 2));
	BOOST_CHECK((sizeof_typelist<typelist<int, double, long>>::value == 3));

	////inverted_typelist函数模板
	//获取反向索引为0的类型形成的typelist
	BOOST_CHECK((std::is_same_v<decltype(inverted_typelist(index_tuple<0>{}, typelist<int, double, long>{})), decltype(typelist<long>{})> == true));
	BOOST_CHECK((std::is_same_v<decltype(inverted_typelist(index_tuple<1>{}, typelist<int, double, long>{})), decltype(typelist<double>{})> == true));
	BOOST_CHECK((std::is_same_v<decltype(inverted_typelist(index_tuple<2>{}, typelist<int, double, long>{})), decltype(typelist<int>{})> == true));
	//获取反向索引为0的类型、反向索引为1的类型形成的typelist
	BOOST_CHECK((std::is_same_v<decltype(inverted_typelist(index_tuple<0, 1>{}, typelist<int, double, long>{})), decltype(typelist<long, double>{})> == true));
	BOOST_CHECK((std::is_same_v<decltype(inverted_typelist(index_tuple<1, 2>{}, typelist<int, double, long>{})), decltype(typelist<double, int>{})> == true));

	////invert_typelist_impl类模板
	//index_tuple中类型个数为0或者1时形成的typelist是原来的typelist
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<>>::type, typelist<int, double, long>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<283>>::type, typelist<int, double, long>> == true));
	//获取反向索引为任意值的类型形成的typelist是原来的typelist
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<0>>::type, typelist<int, double, long>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<1>>::type, typelist<int, double, long>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<2>>::type, typelist<int, double, long>> == true));
	//获取反向索引为0的类型、反向索引为1的类型形成的typelist
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<0, 1>>::type, typelist<long, double>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist_impl<typelist<int, double, long>, index_tuple<1, 2>>::type, typelist<double, int>> == true));

	////利用build_number_seq和invert_typelist_impl创建invert_typelist
	BOOST_CHECK((std::is_same_v<invert_typelist<typelist<>>::type, typelist<>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist<typelist<int>>::type, typelist<int>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist<typelist<int, double>>::type, typelist<double, int>> == true));
	BOOST_CHECK((std::is_same_v<invert_typelist<typelist<int, double, long>>::type, typelist<long, double, int>> == true));

	//do_pack的要求typelist中的类型必须有pack类模板，可以使用辅助宏XLL_OPTION_TYPE和XLL_OPTION_CONSTANT提供
	//当do_pack的typelist中只有一个类型时
	BOOST_CHECK((std::is_same_v<do_pack<typelist<custom_value_option_name<true>>>::type, custom_value_option_name<true>> == true));
	BOOST_CHECK((std::is_same_v<do_pack<typelist<custom_type_option_name<int>>>::type, custom_type_option_name<int>> == true));
	//当do_pack的typelist中有两个类型时
	BOOST_CHECK((std::is_same_v<do_pack<typelist<custom_value_option_name<true>, custom_type_option_name<int>>>::type, custom_value_option_name<true>::pack<custom_type_option_name<int>>> == true));

	//检查选项
	BOOST_CHECK((std::is_same_v<custom_type_option_name<int>::pack<defaults>::custom_type_option_name, int> == true));
	BOOST_CHECK((custom_value_option_name<true>::pack<defaults>::custom_value_option_name == true));
	//检验pack类模板实例化后的成员
	//1.
	BOOST_CHECK((custom_value_option_name<true>::pack<custom_type_option_name<int>>::custom_value_option_name == true));
	//2.名称custom_type_option_name总是被误认为是struct的名称，不知道怎么解决，使用pack_options没有这个问题，所以不影响pack_options的使用
	//BOOST_CHECK((std::is_same_v<do_pack<typelist<custom_value_option_name<true>, custom_type_option_name<int>>>::type::custom_type_option_name, int> == true));
	//3.
	BOOST_CHECK((std::is_same_v<custom_type_option_name<int>::pack<custom_value_option_name<true>>::custom_type_option_name, int> == true));
	//4.名称custom_value_option_name总是被误认为是struct的名称，不知道怎么解决，使用pack_options没有这个问题，所以不影响pack_options的使用
	//BOOST_CHECK((do_pack<typelist<custom_type_option_name<int>, custom_value_option_name<true>>>::type == true));
	

	//当do_pack的typelist中有三个类型或三个以上类型时
	BOOST_CHECK((std::is_same_v<do_pack<typelist<custom_value_option_name<true>, custom_type_option_name<int>, custom_value_option_name<false>>>::type, custom_value_option_name<true>::pack<custom_type_option_name<int>::pack<custom_value_option_name<false>>>> == true));
	//当do_pack的typelist中有void时过滤掉，同时void不能作为pack的基类
	BOOST_CHECK((std::is_same_v<do_pack<typelist<void, custom_type_option_name<int>, custom_value_option_name<false>>>::type, custom_type_option_name<int>::pack<custom_value_option_name<false>>> == true));

	//再次检验pack类模板实例化后的成员，发现靠前的优先级高，defaults应该放最后，但是这样很不方便，应该颠倒一下
	BOOST_CHECK((custom_value_option_name<true>::pack<custom_type_option_name<int>::pack<custom_value_option_name<false>>>::custom_value_option_name == true));

	//颠倒并添加缺省defaults
	BOOST_CHECK((std::is_same_v<do_pack<invert_typelist<typelist<defaults, custom_value_option_name<false>, custom_type_option_name<int>, custom_value_option_name<true>>>::type>::type, custom_value_option_name<true>::pack<custom_type_option_name<int>::pack<custom_value_option_name<false>::pack<defaults>>>> == true));
	//利用pack_options简化
	BOOST_CHECK((std::is_same_v<pack_options<defaults, custom_value_option_name<false>, custom_type_option_name<int>, custom_value_option_name<true>>::type, custom_value_option_name<true>::pack<custom_type_option_name<int>::pack<custom_value_option_name<false>::pack<defaults>>>> == true));

	////实际应用
	using packed_options = pack_options<defaults, custom_value_option_name<false>, custom_type_option_name<int>, custom_value_option_name<true>>::type;
	//名称custom_type_option_name和custom_value_option_name解析正确
	BOOST_CHECK((std::is_same_v<packed_options::custom_type_option_name, int> == true));
	BOOST_CHECK((packed_options::custom_value_option_name == true));
}

BOOST_AUTO_TEST_CASE(c_template_pointer_rebind)
{
	using namespace xll;
	using namespace test_template_pointer_rebind;

	//U定义为int

	BOOST_CHECK((pointer_has_rebind<my_dont_have_rebindU, int>::value == false));
	BOOST_CHECK((pointer_has_rebind<my_have_rebindU_dont_have_rebindU_other, int>::value == true));
	BOOST_CHECK((pointer_has_rebind<my_have_rebindU_have_rebindU_other, int>::value == true));

	BOOST_CHECK((pointer_has_rebind_other<my_dont_have_rebindU, int>::value == false));
	BOOST_CHECK((pointer_has_rebind_other<my_have_rebindU_dont_have_rebindU_other, int>::value == false));
	BOOST_CHECK((pointer_has_rebind_other<my_have_rebindU_have_rebindU_other, int>::value == true));

	BOOST_CHECK((pointer_rebind_mode<my_dont_have_rebindU, int>::mode == 0));
	BOOST_CHECK((pointer_rebind_mode<my_have_rebindU_dont_have_rebindU_other, int>::mode == 1));
	BOOST_CHECK((pointer_rebind_mode<my_have_rebindU_have_rebindU_other, int>::mode == 2));

	//my_dont_have_rebindU不适用于pointer_rebinder
	BOOST_CHECK((std::is_same_v<pointer_rebinder<my_have_rebindU_dont_have_rebindU_other, int, pointer_rebind_mode<my_have_rebindU_dont_have_rebindU_other, int>::mode>::type, my_have_rebindU_dont_have_rebindU_other::rebind<int>> == true));
	BOOST_CHECK((std::is_same_v<pointer_rebinder<my_have_rebindU_have_rebindU_other, int, pointer_rebind_mode<my_have_rebindU_have_rebindU_other, int>::mode>::type, my_have_rebindU_have_rebindU_other::rebind<int>::other> == true));
	//my_dont_have_rebindU_but_have_A_An中只有一个模板参数时，结果int替换double
	BOOST_CHECK((std::is_same_v<pointer_rebinder<my_dont_have_rebindU_but_have_A_An<double>, int, pointer_rebind_mode<my_dont_have_rebindU_but_have_A_An<double>, int>::mode>::type, my_dont_have_rebindU_but_have_A_An<int>> == true));
	//my_dont_have_rebindU_but_have_A_An中至少有两个模板参数时，结果double，long仍未double，long
	BOOST_CHECK((std::is_same_v<pointer_rebinder<my_dont_have_rebindU_but_have_A_An<double, long>, int, pointer_rebind_mode<my_dont_have_rebindU_but_have_A_An<double, long>, int>::mode>::type, my_dont_have_rebindU_but_have_A_An<double, long>> == true));

	//pointer_rebind
	//除了上面pointer_rebinder的逻辑外，在补充一个T*
	BOOST_CHECK((std::is_same_v<pointer_rebind<my_have_rebindU_dont_have_rebindU_other, int>::type, my_have_rebindU_dont_have_rebindU_other::rebind<int>> == true));
	BOOST_CHECK((std::is_same_v<pointer_rebind<my_have_rebindU_have_rebindU_other, int>::type, my_have_rebindU_have_rebindU_other::rebind<int>::other> == true));
	BOOST_CHECK((std::is_same_v<pointer_rebind<my_dont_have_rebindU_but_have_A_An<double>, int>::type, my_dont_have_rebindU_but_have_A_An<int>> == true));
	BOOST_CHECK((std::is_same_v<pointer_rebind<my_dont_have_rebindU_but_have_A_An<double, long>, int>::type, my_dont_have_rebindU_but_have_A_An<double, long>> == true));
	BOOST_CHECK((std::is_same_v<pointer_rebind<double*, int>::type, int*> == true));
}

BOOST_AUTO_TEST_CASE(c_template_pointer_traits)
{

}

BOOST_AUTO_TEST_CASE(c_template_hook)
{

}


BOOST_AUTO_TEST_SUITE_END()
