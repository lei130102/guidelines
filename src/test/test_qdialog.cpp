#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qdialog.h>

BOOST_AUTO_TEST_SUITE(s_qdialog)

BOOST_AUTO_TEST_CASE(c_qdialog)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qdialog dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
