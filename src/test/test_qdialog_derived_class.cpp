#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qdialog_derived_class.h>

BOOST_AUTO_TEST_SUITE(s_qdialog_derived_class)

BOOST_AUTO_TEST_CASE(c_qdialog_derived_class)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qdialog_derived_class dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
