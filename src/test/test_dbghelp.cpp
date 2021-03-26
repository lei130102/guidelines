#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_dbghelp.h>

BOOST_AUTO_TEST_SUITE(s_dbghelp)

BOOST_AUTO_TEST_CASE(c_dbghelp)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_dbghelp dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
