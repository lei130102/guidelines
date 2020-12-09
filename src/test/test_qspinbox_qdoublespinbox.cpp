#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <QApplication>

#include <dlg_qspinbox_qdoublespinbox.h>

BOOST_AUTO_TEST_SUITE(s_qspinbox_qdoublespinbox_class)

BOOST_AUTO_TEST_CASE(c_qspinbox_qdoublespinbox_class)
{
    QApplication app(::boost::unit_test::framework::master_test_suite().argc, ::boost::unit_test::framework::master_test_suite().argv);

    dlg_qspinbox_qdoublespinbox dlg;
    dlg.exec();
}

BOOST_AUTO_TEST_SUITE_END()
