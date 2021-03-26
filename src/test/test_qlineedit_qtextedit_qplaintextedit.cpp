#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qlineedit_qtextedit_qplaintextedit.h>

BOOST_AUTO_TEST_SUITE(s_qlineedit_qtextedit_qplaintextedit)

BOOST_AUTO_TEST_CASE(c_qlineedit_qtextedit_qplaintextedit)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qlineedit_qtextedit_qplaintextedit dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
