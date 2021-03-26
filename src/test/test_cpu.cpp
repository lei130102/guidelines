#define WIN32_LEAN_AND_MEAN

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <test_cpu.h>

#include <boost/process.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(s_cpu)

BOOST_AUTO_TEST_CASE(c_number_of_processors)
{
	//获取逻辑核心数量
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	std::cout << "dwNumberOfProcessors:" << info.dwNumberOfProcessors << '\n';
}

//正确代码，为了不影响之后的代码执行，这里暂时注释掉
//BOOST_AUTO_TEST_CASE(c_windows_process_cpu)
//{
//	std::cout << "sizeof(DWORD) (unsigned long):" << sizeof(DWORD) << '\n';
//
//	//SetProcessAffinityMask(GetCurrentProcess(), 0x00000001);//CPU 0
//	//SetProcessAffinityMask(GetCurrentProcess(), 0x00000002);//CPU 1
//	SetProcessAffinityMask(GetCurrentProcess(), 0x00000004);//CPU 2
//	//...
//
//	int i = 3495;
//	for (;;)
//	{
//		i ^= 100;
//	}
//}

//正确代码，为了不影响之后的代码执行，这里暂时注释掉
//BOOST_AUTO_TEST_CASE(c_windows_thread_cpu)
//{
//	SYSTEM_INFO info;
//	GetSystemInfo(&info);
//
//	int thread_number = info.dwNumberOfProcessors - 1;//少创建一个线程，防止机器负载过重
//
//	std::vector<std::shared_ptr<std::thread>> threads;
//
//	for (int i = 0; i < thread_number; ++i)
//	{
//		threads.push_back(std::make_shared<std::thread>([](DWORD mask) {
//				
//				SetThreadAffinityMask(GetCurrentThread(), mask);
//
//				int i = 3495;
//				for (;;)
//				{
//					i ^= 100;
//				}
//
//			}, 1 << (i+1)));//不用CPU 0
//	}
//
//	for (auto& thread : threads)
//	{
//		thread->join();
//	}
//}

void without_thread_local_variable()
{
	thread_local int tl = 6;

	std::cout << tl << '\n';
}

void with_thread_local_variable()
{
	thread_local int tl = 5;

	without_thread_local_variable();

	std::cout << tl << '\n';
}

thread_local int g_tl = 0;

void without_global_thread_local_variable()
{
	g_tl = 3;

	std::cout << g_tl << '\n';
}

void without_global_thread_local_variable2()
{
	std::cout << g_tl << '\n';
}

BOOST_AUTO_TEST_CASE(c_windows_thread_local)
{
	std::vector<std::shared_ptr<std::thread>> threads;

	for (int i = 0; i < 1; ++i)
	{
		threads.push_back(std::make_shared<std::thread>([&]() {

			without_global_thread_local_variable();
			without_global_thread_local_variable2();

			}));
	}

	for (auto& thread : threads)
	{
		thread->join();
	}
}

BOOST_AUTO_TEST_SUITE_END()