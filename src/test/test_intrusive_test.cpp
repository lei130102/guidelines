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

	std::vector<my_element> elements(100);
	int i = 0;
	std::generate(elements.begin(), elements.end(), [&i]() {return my_element(i++); });

	my_base_list base_list;
	//无法编译，因为侵入式容器的push_front函数参数与front_insert_iterator类成员函数中调用的push_front函数参数不同     std::copy(elements.begin(), elements.end(), std::front_inserter(base_list));//base_list.push_front
	std::for_each(elements.rbegin(), elements.rend(), [&base_list](my_element& element) {
			base_list.push_front(element);
		});

	my_base_list::iterator base_list_riter(base_list.begin());
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
		my_element element(0);
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
		my_element element(0);
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

BOOST_AUTO_TEST_CASE(c_intrusive_slist)
{
	using namespace test_intrusive_slist;

	std::vector<my_slist_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() { return my_slist_element(i++); });

	//对于slist，push_back只对cache_last<>为true时才可用
	my_base_slist base_slist;
	std::for_each(values.begin(), values.end(), [&base_slist](my_slist_element& value) {
		base_slist.push_front(value);//注意push_back的参数类型是左值引用，因为会修改value
		});

	my_member_slist member_slist;
	std::for_each(base_slist.begin(), base_slist.end(), [&member_slist](my_slist_element& value) {
		member_slist.push_front(value);//注意push_front的参数类型是左值引用，因为会修改value
		});

	{
		my_base_slist::iterator base_slist_iter(base_slist.begin());
		for (auto riter = values.rbegin(); riter != values.rend(); ++riter)
		{
			BOOST_CHECK(&*base_slist_iter == &*riter);
			++base_slist_iter;
		}

		my_member_slist::iterator member_slist_iter(member_slist.begin());
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			BOOST_CHECK(&*member_slist_iter == &*iter);
			++member_slist_iter;
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_list)
{
	using namespace test_intrusive_list;

	std::vector<my_list_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() { return my_list_element(i++); });

	my_base_list base_list;
	std::for_each(values.begin(), values.end(), [&base_list](my_list_element& value) {
		base_list.push_front(value);
		});

	my_member_list member_list;
	std::for_each(values.begin(), values.end(), [&member_list](my_list_element& value) {
		member_list.push_back(value);
		});

	{
		my_base_list::reverse_iterator base_list_riter(base_list.rbegin());
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			BOOST_CHECK(&*base_list_riter == &*iter);
			++base_list_riter;
		}

		my_member_list::iterator member_list_iter(member_list.begin());
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			BOOST_CHECK(&*member_list_iter == &*iter);
			++member_list_iter;
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_set)
{
	using namespace test_intrusive_set;

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() {return my_element(i++); });

	//Check that size optimization is activated in the base hook
	BOOST_CHECK(sizeof(boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>) == 3 * sizeof(void*));

	my_base_set base_set;
	//Now insert them in the reverse order in the base hook set
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_set.insert(*iter);
	}

	//{
	//	my_base_set::reverse_iterator base_set_riter(base_set.rbegin());

	//	for (auto iter = values.begin(); iter != values.end(); ++iter)
	//	{
	//		BOOST_CHECK(&*base_set_riter == &*iter);
	//	}
	//}
}

BOOST_AUTO_TEST_CASE(c_intrusive_multiset)
{
	using namespace test_intrusive_multiset;

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() {return my_element(i++); });

	//Check that size optimization is deactivated in the member hook
	BOOST_CHECK(sizeof(boost::intrusive::set_member_hook<>) > 3 * sizeof(void*));

	my_member_multiset member_multiset;
	//Now insert them in the reverse order in the base hook set
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_multiset.insert(*iter);
	}

	//{
	//	my_member_multiset::iterator member_multiset_iter(member_multiset.begin());

	//	for (auto iter = values.begin(); iter != values.end(); ++iter)
	//	{
	//		BOOST_CHECK(&*member_multiset_iter == &*iter);
	//	}
	//}
}

BOOST_AUTO_TEST_CASE(c_intrusive_unordered_set)
{
	using namespace test_intrusive_unordered_set;

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() {return my_element(i++); });

	//Create a bucket array
	my_base_unordered_set::bucket_type buckets[100];

	//Create unordered containers taking buckets as arguments
	my_base_unordered_set base_unordered_set(my_base_unordered_set::bucket_traits(buckets, 100));

	//Now insert values's elements in the unordered_set
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_unordered_set.insert(*iter);
	}

	{
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			//base_unordered_set should contain one element for each key
			BOOST_CHECK(base_unordered_set.count(*iter) == 1);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_unordered_multiset)
{
	using namespace test_intrusive_unordered_multiset;

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() {return my_element(i++); });

	//Create a copy of the vector
	std::vector<my_element> values2(values);

	//Create a bucket array
	my_member_unordered_multiset::bucket_type buckets[200];

	//Create unordered containers taking buckets as arguments
	my_member_unordered_multiset member_unordered_multiset(my_member_unordered_multiset::bucket_traits(buckets, 200));

	//Now insert values's and values2's elements in the unordered_multiset
	for (auto iter = values.begin(), iter2 = values2.begin(); iter != values.end(); ++iter, ++iter2)
	{
		member_unordered_multiset.insert(*iter);
		member_unordered_multiset.insert(*iter2);
	}

	{
		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			//member_unordered_multiset should contain two element for each key
			BOOST_CHECK(member_unordered_multiset.count(*iter) == 2);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_map_like)
{
	using namespace test_intrusive_map_like;

	BOOST_STATIC_ASSERT((boost::is_same<my_base_map_like::key_type, int>::value));

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() { return my_element(i++); });

	my_base_map_like base_map_like(values.begin(), values.end());

	//Test each element using the key_type (int)
	for (int i = 0; i != 100; ++i)
	{
		BOOST_CHECK(base_map_like.find(i) != base_map_like.end());

		BOOST_CHECK(base_map_like.lower_bound(i) != base_map_like.end());
		BOOST_CHECK(++base_map_like.lower_bound(i) == base_map_like.upper_bound(i));

		BOOST_CHECK(base_map_like.equal_range(i).first != base_map_like.equal_range(i).second);
	}

	//Count and erase by key
	for (int i = 0; i != 100; ++i)
	{
		assert(1 == base_map_like.count(i));
		assert(1 == base_map_like.erase(i));
	}
	//BOOST_CHECK(base_map_like.empty());   无法通过，待解决
}

BOOST_AUTO_TEST_CASE(c_intrusive_unordered_map_like)
{
	using namespace test_intrusive_unordered_map_like;

	BOOST_STATIC_ASSERT((boost::is_same<my_base_unordered_map_like::key_type, int>::value));

	std::vector<my_element> values(100);
	int i = 0;
	std::generate(values.begin(), values.end(), [&i]() { return my_element(i++); });

	my_base_unordered_map_like::bucket_type buckets[100];
	my_base_unordered_map_like base_unordered_map_like(values.begin(), values.end(), my_base_unordered_map_like::bucket_traits(buckets, 100));

	//Test each element using the key_type (int)
	for (int i = 0; i != 100; ++i)
	{
		BOOST_CHECK(base_unordered_map_like.find(i) != base_unordered_map_like.end());

		BOOST_CHECK(base_unordered_map_like.equal_range(i).first != base_unordered_map_like.equal_range(i).second);
	}

	//Count and erase by key
	for (int i = 0; i != 100; ++i)
	{
		assert(1 == base_unordered_map_like.count(i));
		assert(1 == base_unordered_map_like.erase(i));
	}
	//BOOST_CHECK(base_unordered_map_like.empty());     无法通过，待解决
}

BOOST_AUTO_TEST_SUITE_END()