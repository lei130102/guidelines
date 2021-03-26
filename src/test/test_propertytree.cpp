#include "test_propertytree.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <boost/filesystem.hpp>

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

//BOOST_AUTO_TEST_CASE(c_propertytree)
//{
//	using namespace test_propertytree;
//
//	try
//	{
//		debug_settings ds;
//		ds.load("debug_settings.xml");      //打开exe所在目录的debug_settings.xml文件
//		ds.save("debug_settings_out.xml");  //输出到exe所在目录，UTF-8编码，自动添加<?xml version="1.0" encoding="utf-8"?>，除了第一行换行外，不自动换行
//		std::cout << "Success\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cout << "Error: " << e.what() << "\n";
//	}
//}

BOOST_AUTO_TEST_CASE(c_child)
{
	std::vector<std::wstring> vec{
		std::to_wstring(1)
		, std::to_wstring(2)
		, std::to_wstring(3)
		, std::to_wstring(4)
		, std::to_wstring(5)
	};

	boost::property_tree::wptree tree;
	std::for_each(vec.begin(), vec.end(), [&](std::wstring const& str) {
		boost::property_tree::wptree child;
		child.add(std::wstring(L"number"), str);
		tree.add_child(std::wstring(L"item"), child);
		});
	boost::filesystem::path p = boost::filesystem::current_path();
	p /= std::wstring(L"test");
	boost::property_tree::xml_parser::write_xml(p.string(), tree);

	boost::filesystem::path p2 = boost::filesystem::current_path();
	p2 /= std::wstring(L"test2");
	boost::property_tree::json_parser::write_json(p2.string(), tree);
}

BOOST_AUTO_TEST_CASE(c_property_raw)
{
	//写
	{
		boost::property_tree::ptree root_tree;

		boost::property_tree::ptree kiwi_fast_tree;
		{//kiwi_fast_tree 初始化
			boost::property_tree::ptree background_rgb_kiwi_fast_item_tree;
			{//background_rgb_kiwi_fast_item_tree 初始化
				background_rgb_kiwi_fast_item_tree.put("name", "background_rgb");
				background_rgb_kiwi_fast_item_tree.put("type", "std::deque<value_item>");

				boost::property_tree::ptree value_item_tree;
				{//value_item_tree 初始化
					boost::property_tree::ptree r_value_item_tree;
					r_value_item_tree.put("name", "r");
					r_value_item_tree.put("type", "unsigned short");
					r_value_item_tree.put("value", 20);
					value_item_tree.add_child("item", r_value_item_tree);

					boost::property_tree::ptree g_value_item_tree;
					g_value_item_tree.put("name", "g");
					g_value_item_tree.put("type", "unsigned short");
					g_value_item_tree.put("value", 15);
					value_item_tree.add_child("item", g_value_item_tree);

					boost::property_tree::ptree b_value_item_tree;
					b_value_item_tree.put("name", "b");
					b_value_item_tree.put("type", "unsigned short");
					b_value_item_tree.put("value", 55);
					value_item_tree.add_child("item", b_value_item_tree);
				}
				background_rgb_kiwi_fast_item_tree.put_child("value", value_item_tree);
			}
			kiwi_fast_tree.add_child("item", background_rgb_kiwi_fast_item_tree);

			boost::property_tree::ptree foreground_rgb_kiwi_fast_item_tree;
			{//foreground_rgb_kiwi_fast_item_tree 初始化
				foreground_rgb_kiwi_fast_item_tree.put("name", "foreground_rgb");
				foreground_rgb_kiwi_fast_item_tree.put("type", "std::deque<value_item>");

				boost::property_tree::ptree value_item_tree;
				{//value_item_tree 初始化
					boost::property_tree::ptree r_value_item_tree;
					r_value_item_tree.put("name", "r");
					r_value_item_tree.put("type", "unsigned short");
					r_value_item_tree.put("value", 20);
					value_item_tree.add_child("item", r_value_item_tree);

					boost::property_tree::ptree g_value_item_tree;
					g_value_item_tree.put("name", "g");
					g_value_item_tree.put("type", "unsigned short");
					g_value_item_tree.put("value", 15);
					value_item_tree.add_child("item", g_value_item_tree);

					boost::property_tree::ptree b_value_item_tree;
					b_value_item_tree.put("name", "b");
					b_value_item_tree.put("type", "unsigned short");
					b_value_item_tree.put("value", 50);   //不支持std::vector<int> 因为basic_stream的operator<<不支持
					value_item_tree.add_child("item", b_value_item_tree);
				}
				foreground_rgb_kiwi_fast_item_tree.put_child("value", value_item_tree);
			}
			kiwi_fast_tree.add_child("item", foreground_rgb_kiwi_fast_item_tree);
		}
		root_tree.put_child("kiwi_fast", kiwi_fast_tree);

		//json中可以没有"item"对应位置，但xml不行，所以这里添加"item"，表示数组元素    (简单说xml不支持数组)

		////保存
		boost::filesystem::path p = boost::filesystem::current_path();
		p /= std::wstring(L"raw.xml");
		boost::property_tree::xml_parser::write_xml(p.string(), root_tree);

		boost::filesystem::path p2 = boost::filesystem::current_path();
		p2 /= std::wstring(L"raw.json");
		boost::property_tree::json_parser::write_json(p2.string(), root_tree);
	}

	//读
	{
		boost::property_tree::ptree root_tree;

		boost::filesystem::path p = boost::filesystem::current_path();
		p /= std::wstring(L"raw.xml");
		boost::property_tree::xml_parser::read_xml(p.string(), root_tree);


		boost::property_tree::ptree kiwi_fast_tree = root_tree.get_child("kiwi_fast");

		//for (boost::property_tree::ptree::iterator iter = kiwi_fast_tree.begin();
		//	iter != kiwi_fast_tree.end();
		//	++iter)
		//{
		//	std::cout << iter->first << "\n";
		//	std::cout << iter->second.get<std::string>("name") << "\n";
		//}
		boost::property_tree::ptree item_tree = kiwi_fast_tree.get_child("item");
		boost::property_tree::ptree::assoc_iterator iter;
		iter = item_tree.find("name");
		if (iter == item_tree.not_found())
		{
			return;
		}
		std::string name = iter->second.data();

		iter = item_tree.find("type");
		if (iter == item_tree.not_found())
		{
			return;
		}
		std::string type = iter->second.data();

		iter = item_tree.find("value");
		if (iter == item_tree.not_found())
		{
			return;
		}
		if (type == "std::deque<value_item>")
		{
			boost::property_tree::ptree value_tree = iter->second;

			boost::property_tree::ptree item_tree2 = value_tree.get_child("item");

			boost::property_tree::ptree::assoc_iterator iter2;
			iter2 = item_tree2.find("name");
			if (iter2 == item_tree2.not_found())
			{
				return;
			}
			std::string name = iter2->second.data();

			iter2 = item_tree2.find("type");
			if (iter2 == item_tree2.not_found())
			{
				return;
			}
			std::string type = iter2->second.data();

			iter2 = item_tree2.find("value");
			if (iter2 == item_tree2.not_found())
			{
				return;
			}

			//...
		}
		else
		{
			std::string value = iter->second.data();
		}






		for (auto const& v : item_tree)
		{
			std::cout << v.first << "\n"; //name

			std::string name = v.second.data();



			//std::string name = v.second.data();
			//boost::property_tree::ptree kiwi_fast_item_tree = v.second;

			//for (auto const& vv : kiwi_fast_item_tree)
			//{
			//	std::cout << vv.first << "\n";
			//	std::string name = vv.second.data();

			//	int i = 0;
			//	++i;
			//}

			////std::string name = v.second.get<std::string>("name");
			//std::string type = v.second.get<std::string>("type");
			//
			//boost::property_tree::ptree value_tree = v.second.get_child("value");
			//判断

			//for (auto const& vv : value_tree.get_child("item"))
			//{
			//	std::string name = vv.second.get<std::string>("name");
			//	std::string type = vv.second.get<std::string>("type");

			//	boost::property_tree::ptree value_item_tree = vv.second.get_child("value");
			//	//判断
			//}
		}
	}
}

//BOOST_AUTO_TEST_CASE(c_property_type)
//{
//	using namespace test_property_type;
//
//	//写
//	{
//		//初始化
//
//		value_item value1{ "r", "unsigned short", 20 };
//		value_item value2{ "g", "unsigned short", 15 };
//		value_item value3{ "b", "unsigned short", 55 };
//
//		value_item::group_type dv;
//		dv.push_back(value1);
//		dv.push_back(value2);
//		dv.push_back(value3);
//
//		value_item item1;
//		item1.name = "background_rgb";
//		item1.type = "std::deque<value_item>";
//		item1.value = dv;
//
//		value_item item2;
//		item2.name = "foreground_rgb";
//		item2.type = "std::deque<value_item>";
//		item2.value = dv;
//
//		root root_;
//		root_.kiwi_fast.push_back(item1);
//		root_.kiwi_fast.push_back(item2);
//
//		//
//		boost::property_tree::ptree root_tree = root_.to_ptree();
//
//		////保存
//		boost::filesystem::path p = boost::filesystem::current_path();
//		p /= std::wstring(L"kiwi_fast.xml");
//		boost::property_tree::xml_parser::write_xml(p.string(), root_tree);
//
//		boost::filesystem::path p2 = boost::filesystem::current_path();
//		p2 /= std::wstring(L"kiwi_fast.json");
//		boost::property_tree::json_parser::write_json(p2.string(), root_tree);
//	}
//
//	//读
//	{
//		boost::filesystem::path p = boost::filesystem::current_path();
//		p /= std::wstring(L"kiwi_fast.xml");
//		//boost::property_tree::xml_parser::read_xml(p.string(), root_tree);
//
//		//
//	}
//}

BOOST_AUTO_TEST_SUITE_END()