#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

//or

//#include <boost/test/included/unit_test.hpp>
//::boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[])
//{
//    //

//    return 0;
//}

////////////////////////////

#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>

#include <test.h>

BOOST_AUTO_TEST_SUITE(s_boosttest)

BOOST_AUTO_TEST_CASE(c_boosttest)
{
    //WARN CHECK REQUIRE
    int a = 5;
    BOOST_WARN((a != 5));//false

    int b = 5;
    BOOST_CHECK((a == b));//true

    int* p = nullptr;
    BOOST_REQUIRE((p == nullptr));//true


    //TEST_MESSAGE EQUAL/NE/GE/GT/LE/LT THROW/NO_THROW ...
    BOOST_TEST_MESSAGE("output message");

    BOOST_CHECK_EQUAL(a, b);


    //::boost::unit_test::framework::master_test_suite().argc
    //::boost::unit_test::framework::master_test_suite().argv


    //int* c = new int;
    //Detected memory leaks!
    //Dumping objects ->
    //{6392} normal block at 0x000002412F2162A0, 4 bytes long.
    // Data: <    > CD CD CD CD
    //Object dump complete.
}

BOOST_AUTO_TEST_CASE(c_test)
{

}

BOOST_AUTO_TEST_SUITE_END()


//使用vs cmake时，有时会C++ IntelliSense工作异常，代码下都是红线，这时应该检查CMakeSettings.json中的intelliSenseMode，然后删除.vs文件夹并重新生成缓存
