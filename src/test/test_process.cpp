#define WIN32_LEAN_AND_MEAN

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <test_process.h>

#include <boost/process.hpp>

BOOST_AUTO_TEST_SUITE(s_process)

BOOST_AUTO_TEST_CASE(c_process_child)
{
	boost::process::ipstream pipe_stream;

	boost::process::child child_("gcc --version", boost::process::std_out > pipe_stream);

	std::string line;

	while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
	{
		std::cerr << line << std::endl;
	}

	child_.wait();
}

BOOST_AUTO_TEST_CASE(c_process_exe)
{
}

BOOST_AUTO_TEST_SUITE_END()