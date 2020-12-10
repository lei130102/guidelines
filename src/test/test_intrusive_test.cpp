#include <test_intrusive_test.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE(s_intrusive_test)

BOOST_AUTO_TEST_CASE(c_intrusive_test)
{
	using namespace test_intrusive_test;
	my_element element;

	my_list list1;
	//my_list list2;

	list1.push_back(element);//注意侵入式容器的push_back成员函数的函数参数类型——左值引用
	//list2.push_back(element);

	BOOST_CHECK(&list1.front() == &element);
	//BOOST_CHECK(&list2.front() == &element);

	//list1被销毁
	//element被销毁

	//注意！取消注释会抛出异常
}

BOOST_AUTO_TEST_CASE(c_intrusive_test2)
{
	using namespace test_intrusive_test2;

	my_element element;

	my_list_a list_a_1;
	//my_list_a list_a_2;

	my_list_b list_b_1;
	//my_list_b list_b_2;

	list_a_1.push_back(element);
	//list_a_2.push_back(element);
	list_b_1.push_back(element);
	//list_b_2.push_back(element);

	BOOST_CHECK(&list_a_1.front() == &element);
	//BOOST_CHECK(&list_a_2.front() == &element);
	BOOST_CHECK(&list_b_1.front() == &element);
	//BOOST_CHECK(&list_b_2.front() == &element);

	//注意！取消注释会抛出异常
}

BOOST_AUTO_TEST_CASE(c_intrusive_test3)
{
	using namespace test_intrusive_test3;

	my_element element;

	my_list list;

	list.push_back(element);

	BOOST_CHECK(&list.front() == &element);
}

BOOST_AUTO_TEST_CASE(c_intrusive_test4)
{
	using namespace test_intrusive_test4;

	std::vector<my_element> elements;
	std::iota(elements.begin(), elements.end(), 0);

	my_base_list base_list;
	//无法编译，因为侵入式容器的push_front函数参数与front_insert_iterator类成员函数中调用的push_front函数参数不同     std::copy(elements.begin(), elements.end(), std::front_inserter(base_list));//base_list.push_front
	std::for_each(elements.rbegin(), elements.rend(), [&base_list](my_element& element) {
			base_list.push_front(element);
		});

	my_base_list::reverse_iterator base_list_riter(base_list.rbegin());
	for (auto iter = elements.begin(); iter != elements.end(); ++iter)
	{
		BOOST_CHECK(&*base_list_riter == &*iter);
		++base_list_riter;
	}

	my_member_list member_list;
	//无法编译，因为侵入式容器的push_back函数参数与back_insert_iterator类成员函数中调用的push_back函数参数不同   std::copy(elements.begin(), elements.end(), std::back_inserter(member_list));
	std::for_each(elements.begin(), elements.end(), [&member_list](my_element& element) {
			member_list.push_back(element);
		});

	my_member_list::iterator member_list_iter(member_list.begin());
	for (auto iter = elements.begin(); iter != elements.end(); ++iter)
	{
		BOOST_CHECK(&*member_list_iter == &*iter);
		++member_list_iter;
	}
}

//综上：
//侵入式容器不对其元素进行内存管理，就好像存的是元素指针
//1.当容器先于元素被销毁时，元素不会被销毁，所以注意内存泄漏问题
//2.当元素先于容器被销毁时，程序很可能会崩溃，因为容器中包含了指向不存在对象的指针

//何时使用侵入式容器：
//侵入式容器可用于高度优化的算法，其中速度是至关重要的问题，并且：
//1.应避免额外的内存管理
//2.程序员需要有效地跟踪对象的创建和销毁
//3.需要异常安全，特别是无抛出异常保证
//4.从指向某个元素的指针或引用对该元素的迭代器的计算应该是一个常量时间操作
//5.实现众所周知的最坏时间系统响应是很重要的
//6.数据本地化(例如，对于缓存命中优化)导致可测量的效果                    (重点，比如同一组元素放在多个容器中，std::vector<Object> 和 std::list<Object*>、std::list<Object*>中的节点放在堆内存中，获取性能较差，如果是侵入式容器，那么节点就嵌入Object内部，获取性能更高)
//7.侵入式容器允许将不同大小的或大小未知的对象放入容器中，比如不同派生类   (重点)

BOOST_AUTO_TEST_CASE(c_intrusive_test5)
{
	using namespace test_intrusive_test5;

	mainwindow window;

	paint_all_windows();
}

BOOST_AUTO_TEST_CASE(c_intrusive_test6)
{
	using namespace test_intrusive_test6;

	my_list list;

	{
		my_element element;
		BOOST_CHECK(element.is_linked() == false);

		list.push_back(element);
		BOOST_CHECK(list.empty() == false);
		BOOST_CHECK(&list.front() == &element);
		BOOST_CHECK(element.is_linked() == true);

		//element的析构函数会调用基类的析构函数，从而auto unlink与容器的链接  
		//(之前提到不要在容器销毁前销毁元素，有了auto unlink就没有这个限制了，但性能会降低)
	}

	BOOST_CHECK(list.empty() == true);

	{
		my_element element;
		BOOST_CHECK(element.is_linked() == false);

		list.push_back(element);
		BOOST_CHECK(list.empty() == false);
		BOOST_CHECK(&list.front() == &element);
		BOOST_CHECK(element.is_linked() == true);

		//也可以在element调用析构函数之前主动unlink
		element.unlink();

		BOOST_CHECK(list.empty() == true);
	}
}

BOOST_AUTO_TEST_SUITE_END()