#include "test_propertytree.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <iostream>


//boost.propertytree可以解析xml json ini
//解析xml用的rapidxml库，这个库只需要4个头文件：
//rapidxml.hpp
//rapidxml_iterators.hpp
//rapidxml_print.hpp
//rapidxml_utils.hpp
//boost.propertytree自动包含这些，不需要再额外依赖

BOOST_AUTO_TEST_SUITE(s_propertytree)

BOOST_AUTO_TEST_CASE(c_propertytree)
{
	using namespace test_propertytree;

	try
	{
		debug_settings ds;
		ds.load("debug_settings.xml");      //打开exe所在目录的debug_settings.xml文件
		ds.save("debug_settings_out.xml");  //输出到exe所在目录，UTF-8编码，自动添加<?xml version="1.0" encoding="utf-8"?>，除了第一行换行外，不自动换行
		std::cout << "Success\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
}

BOOST_AUTO_TEST_SUITE_END()