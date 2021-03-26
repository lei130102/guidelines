#pragma once

#include <boost/locale.hpp>

#include <string>

namespace test_string_wstring_u8string
{
	//规定：
	//1.std::string中的编码默认为本地编码
	//2.字符串字面量用u8""或者u8R"()"
	//3.只有在第三方库api需要utf-8编码std::string(或者char const*)时，才使用to_utf_string函数，并且返回值不要保留
	//4.目前编译工具对char16_t、char32_t和char8_t的支持都不够好，将来或许会改变
	//5.调用系统API时尽量使用宽字符版本(wchar_t)
	//6.一般不用to_utf8_string函数，std::string默认表示本地编码，主要用在需要utf-8编码std::string(或者char const*)的第三方库api上

	inline char const* charset = "GBK";
	//使用GB18030时，boost::locale::conv::from_utf<wchar_t>会崩溃，原因不明

	//UTF-16编码(wchar_t)到本地编码(char)
	inline std::string to_local(std::wstring const& str)
	{
		return boost::locale::conv::from_utf<wchar_t>(str, charset);
	}
	inline std::string to_local(wchar_t const* begin, wchar_t const* end)
	{
		return boost::locale::conv::from_utf<wchar_t>(begin, end, charset);
	}
	inline std::string to_local(wchar_t const* text)
	{
		return boost::locale::conv::from_utf<wchar_t>(text, charset);
	}

	//本地编码(char)到UTF-16编码(wchar_t)
	inline std::wstring to_wide(std::string const& str)
	{
		return boost::locale::conv::to_utf<wchar_t>(str, charset);
	}
	inline std::wstring to_wide(char const* begin, char const* end)
	{
		return boost::locale::conv::to_utf<wchar_t>(begin, end, charset);
	}
	inline std::wstring to_wide(char const* text)
	{
		return boost::locale::conv::to_utf<wchar_t>(text, charset);
	}

	//UTF-8编码(char8_t)到本地编码(char)
	inline std::string to_local(std::u8string const& str)
	{
		//编译环境不支持 boost::locale::conv::from_utf<char8_t>(str, charset);
		return to_local(boost::locale::conv::utf_to_utf<wchar_t, char8_t>(str));
	}
	inline std::string to_local(char8_t const* begin, char8_t const* end)
	{
		//编译环境不支持 boost::locale::conv::from_utf<char8_t>(begin, end, charset);
		return to_local(boost::locale::conv::utf_to_utf<wchar_t, char8_t>(std::u8string(begin, end)));
	}
	inline std::string to_local(char8_t const* text)
	{
		//编译环境不支持 boost::locale::conv::from_utf<char8_t>(text, charset);
		return to_local(boost::locale::conv::utf_to_utf<wchar_t, char8_t>(std::u8string(text)));
	}

	//本地编码(char)到UTF-8编码(char8_t)
	inline std::u8string to_utf8(std::string const& str)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(str, charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(to_wide(str));
	}
	inline std::u8string to_utf8(char const* begin, char const* end)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(begin, end, charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(to_wide(std::string(begin, end)));
	}
	inline std::u8string to_utf8(char const* text)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(text, charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(to_wide(std::string(text)));
	}

	//UTF-16编码(wchar_t)到UTF-8编码(char8_t)
	inline std::u8string to_utf8(std::wstring const& str)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(boost::locale::conv::from_utf<wchar_t>(str, charset), charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(str);
	}
	inline std::u8string to_utf8(wchar_t const* begin, wchar_t const* end)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(boost::locale::conv::from_utf<wchar_t>(begin, end, charset), charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(std::wstring(begin, end));
	}
	inline std::u8string to_utf8(wchar_t const* text)
	{
		//编译环境不支持 boost::locale::conv::to_utf<char8_t>(boost::locale::conv::from_utf<wchar_t>(text, charset), charset);
		return boost::locale::conv::utf_to_utf<char8_t, wchar_t>(std::wstring(text));
	}

	//UTF-16编码(char8_t)到UTF-16编码(wchar_t)
	inline std::wstring to_wide(std::u8string const& str)
	{
		//编译环境不支持 boost::locale::conv::to_utf<wchar_t>(boost::locale::conv::from_utf<char8_t>(str, charset), charset);
		return boost::locale::conv::utf_to_utf<wchar_t, char8_t>(str);
	}
	inline std::wstring to_wide(char8_t const* begin, char8_t const* end)
	{
		//编译环境不支持 return boost::locale::conv::to_utf<wchar_t>(boost::locale::conv::from_utf<char8_t>(begin, end, charset), charset);
		return boost::locale::conv::utf_to_utf<wchar_t, char8_t>(std::u8string(begin, end));
	}
	inline std::wstring to_wide(char8_t const* text)
	{
		//编译环境不支持 boost::locale::conv::to_utf<wchar_t>(boost::locale::conv::from_utf<char8_t>(text, charset), charset);
		return boost::locale::conv::utf_to_utf<wchar_t, char8_t>(std::u8string(text));
	}

	///UTF-16编码(wchar_t)到UTF-8编码(char)
	inline std::string to_utf8_string(std::wstring const& str)
	{
		return boost::locale::conv::utf_to_utf<char, wchar_t>(str);
	}
	inline std::string to_utf8_string(wchar_t const* begin, wchar_t const* end)
	{
		return boost::locale::conv::utf_to_utf<char, wchar_t>(begin, end);
	}
	inline std::string to_utf8_string(wchar_t const* text)
	{
		return boost::locale::conv::utf_to_utf<char, wchar_t>(text);
	}
	///UTF-16编码(char16_t)到UTF-8编码(char)
	inline std::string to_utf8_string(std::u8string const& str)
	{
		return boost::locale::conv::utf_to_utf<char, char8_t>(str);
	}
	inline std::string to_utf8_string(char8_t const* begin, char8_t const* end)
	{
		return boost::locale::conv::utf_to_utf<char, char8_t>(begin, end);
	}
	inline std::string to_utf8_string(char8_t const* text)
	{
		return boost::locale::conv::utf_to_utf<char, char8_t>(text);
	}
}