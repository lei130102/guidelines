#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qgroupbox.h>

BOOST_AUTO_TEST_SUITE(s_qgroupbox)

BOOST_AUTO_TEST_CASE(c_qgroupbox)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qgroupbox dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
