#include <test_string_wstring_u8string.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(s_string_wstring_u8string)

BOOST_AUTO_TEST_CASE(c_string_wstring_u8string)
{
	using namespace test_string_wstring_u8string;

	auto char_str = "编码";
	auto wchar_t_str = L"编码";
	auto char8_t_str = u8"编码";

	std::string char_utf = {'\xe7', '\xbc', '\x96', '\xe7', '\xa0', '\x81'}; //utf8编码
	auto char_utf_str = char_utf.c_str();

	BOOST_CHECK(to_local(wchar_t_str) == char_str);
	BOOST_CHECK(to_local(char8_t_str) == char_str);
	BOOST_CHECK(to_utf8(char_str) == char8_t_str);
	BOOST_CHECK(to_wide(char_str) == wchar_t_str);
	BOOST_CHECK(to_utf8(wchar_t_str) == char8_t_str);
	BOOST_CHECK(to_wide(char8_t_str) == wchar_t_str);

	BOOST_CHECK(to_wide(char8_t_str) == wchar_t_str);
	BOOST_CHECK(to_wide(char8_t_str) == wchar_t_str);

	BOOST_CHECK(to_utf8_string(wchar_t_str) == char_utf_str);
	BOOST_CHECK(to_utf8_string(char8_t_str) == char_utf_str);

}

BOOST_AUTO_TEST_CASE(c_from_utf)
{
	using namespace test_string_wstring_u8string;

	auto char_str = "编码";

	std::wstring wchar = L"编码";
	std::string char_result = to_local(wchar);
	BOOST_CHECK(char_result == char_str);
}

BOOST_AUTO_TEST_SUITE_END()