#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qstring.h>

BOOST_AUTO_TEST_SUITE(s_qstring)

BOOST_AUTO_TEST_CASE(c_qstring)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qstring dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
