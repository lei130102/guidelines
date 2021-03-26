#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qstring.h>

BOOST_AUTO_TEST_SUITE(s_qstring)

BOOST_AUTO_TEST_CASE(c_utf8)
{
    std::u8string utf8 = u8"编码";

    std::cout << "sizeof(char):" << sizeof(char) << '\n';
    std::cout << "sizeof(char8_t):" << sizeof(char8_t) << '\n';
    std::cout << "sizeof(char16_t):" << sizeof(char16_t) << '\n';
    std::cout << "sizeof(char32_t):" << sizeof(char32_t) << '\n';

    //不同平台不一样
    std::cout << "sizeof(wchar_t):" << sizeof(wchar_t) << '\n';
}

BOOST_AUTO_TEST_CASE(c_qstring)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qstring dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
