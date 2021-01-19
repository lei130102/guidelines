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
	for (int i = 0; i < 100; ++i)
	{
		elements.emplace_back(i);
	}

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
//侵入式容器不对其元素进行内存管理(关键是node，即节点)，就好像存的是元素指针
//1.当容器先于元素被销毁时，元素不会被销毁，所以注意内存泄漏问题                     (可以)
//2.当元素先于容器被销毁时，程序很可能会崩溃，因为容器中包含了指向不存在对象的指针    (不可以！！)

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

	std::vector<my_slist_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	//对于slist，push_back只对cache_last<>为true时才可用
	my_base_slist base_slist;
	std::for_each(values.begin(), values.end(), [&base_slist](my_slist_element& value) {               //根据values填充
		base_slist.push_front(value);//注意push_front的参数类型是左值引用，因为会修改value
		});

	my_member_slist member_slist;
	std::for_each(base_slist.begin(), base_slist.end(), [&member_slist](my_slist_element& value) {     //根据base_slist填充
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

	std::vector<my_list_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

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

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	//Check that size optimization is activated in the base hook
	BOOST_CHECK(sizeof(boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>) == 3 * sizeof(void*));

	my_base_set base_set;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_set.insert(*iter);
	}

	{
		my_base_set::reverse_iterator base_set_riter(base_set.rbegin());

		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			BOOST_CHECK(&*base_set_riter == &*iter);
			++base_set_riter;
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_multiset)
{
	using namespace test_intrusive_multiset;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	//Check that size optimization is deactivated in the member hook
	BOOST_CHECK(sizeof(boost::intrusive::set_member_hook<>) > 3 * sizeof(void*));

	my_member_multiset member_multiset;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_multiset.insert(*iter);
	}

	{
		my_member_multiset::iterator member_multiset_iter(member_multiset.begin());

		for (auto iter = values.begin(); iter != values.end(); ++iter)
		{
			BOOST_CHECK(&*member_multiset_iter == &*iter);
			++member_multiset_iter;
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_unordered_set)
{
	using namespace test_intrusive_unordered_set;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

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

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

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

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

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

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

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

BOOST_AUTO_TEST_CASE(c_intrusive_avl_set)
{
	using namespace test_intrusive_avl_set;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_base_avl_set base_avl_set;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_avl_set.insert(*iter);
	}

	{
		my_base_avl_set::reverse_iterator avl_set_riter(base_avl_set.rbegin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++avl_set_riter)
		{
			BOOST_CHECK(&*avl_set_riter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_avl_multiset)
{
	using namespace test_intrusive_avl_multiset;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_member_avl_multiset member_avl_multiset;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_avl_multiset.insert(*iter);
	}

	{
		my_member_avl_multiset::iterator avl_set_iter(member_avl_multiset.begin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++avl_set_iter)
		{
			BOOST_CHECK(&*avl_set_iter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_splay_set)
{
	using namespace test_intrusive_splay_set;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_base_splay_set base_splay_set;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_splay_set.insert(*iter);
	}

	{
		my_base_splay_set::reverse_iterator base_splay_set_riter(base_splay_set.rbegin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++base_splay_set_riter)
		{
			BOOST_CHECK(&*base_splay_set_riter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_splay_multiset)
{
	using namespace test_intrusive_splay_multiset;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_member_splay_multiset member_splay_multiset;

	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_splay_multiset.insert(*iter);
	}

	{
		my_member_splay_multiset::iterator member_splay_multiset_iter(member_splay_multiset.begin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++member_splay_multiset_iter)
		{
			BOOST_CHECK(&*member_splay_multiset_iter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_sg_set)
{
	using namespace test_intrusive_sg_set;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_base_sg_set base_sg_set;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_sg_set.insert(*iter);
	}

	//可以运行时改变balance factor，但不是必须的
	base_sg_set.balance_factor(0.9f);

	{
		my_base_sg_set::reverse_iterator base_sg_set_riter(base_sg_set.rbegin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++base_sg_set_riter)
		{
			BOOST_CHECK(&*base_sg_set_riter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_sg_multiset)
{
	using namespace test_intrusive_sg_multiset;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i);
	}

	my_member_sg_multiset member_sg_multiset;
	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_sg_multiset.insert(*iter);
	}

	//可以运行时改变balance factor，但不是必须的
	member_sg_multiset.balance_factor(0.9f);

	{
		my_member_sg_multiset::iterator member_sg_multiset_iter(member_sg_multiset.begin());

		for (auto iter = values.begin(); iter != values.end(); ++iter, ++member_sg_multiset_iter)
		{
			BOOST_CHECK(&*member_sg_multiset_iter == &*iter);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_treap_set)
{
	using namespace test_intrusive_treap_set;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i, (i % 10));
	}

	my_base_treap_set base_treap_set;

	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		base_treap_set.insert(*iter);
	}

	{
		my_base_treap_set::reverse_iterator base_treap_set_riter(base_treap_set.rbegin());

		//Test the objects inserted in the base hook treap_set
		for (auto iter = values.begin(); iter != values.end(); ++iter, ++base_treap_set_riter)
		{
			BOOST_CHECK(&*base_treap_set_riter == &*iter);
		}

		//Test priority order
		for (int i = 0; i < 100; ++i)
		{
			BOOST_CHECK(base_treap_set.top()->get_priority() == static_cast<unsigned int>(i / 10));
			base_treap_set.erase(base_treap_set.top());
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_treap_multiset)
{
	using namespace test_intrusive_treap_multiset;

	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.emplace_back(i, (i % 10));
	}

	my_member_treap_multiset member_treap_multiset;

	for (auto iter = values.begin(); iter != values.end(); ++iter)
	{
		member_treap_multiset.insert(*iter);
	}

	{
		my_member_treap_multiset::iterator member_treap_multiset_iter(member_treap_multiset.begin());

		//Test the objects inserted in the member hook treap_set
		for (auto iter = values.begin(); iter != values.end(); ++iter, ++member_treap_multiset_iter)
		{
			BOOST_CHECK(&*member_treap_multiset_iter == &*iter);
		}

		//Test priority order
		for (int i = 0; i < 100; ++i)
		{
			BOOST_CHECK(member_treap_multiset.top()->get_priority() == (9u - static_cast<unsigned int>(i / 10)));
			member_treap_multiset.erase(member_treap_multiset.top());
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_positional_insertions)
{
	using namespace test_intrusive_positional_insertions;

	//Create some ORDERED elements
	std::vector<my_element> values;
	for (int i = 0; i < 100; ++i)
	{
		values.push_back(my_element(i));
	}

	{
		//Data is naturally ordered in the vector with the same criteria as multiset's comparison predicate, so we can just
		//push back all elements, which is more efficient than normal insertion
		boost::intrusive::multiset<my_element> mset;
		for (int i = 0; i < 100; ++i)
		{
			mset.push_back(values[i]);
		}

		//Now check ordered invariant
		boost::intrusive::multiset<my_element>::const_iterator next(mset.cbegin()), it(next++);
		for (int i = 0; i < 99; ++i, ++it, ++next)
		{
			BOOST_CHECK(*it < *next);
		}
	}

	{
		//Now push the first and the last and insert the rest
		//before the last position using "insert_before"
		boost::intrusive::multiset<my_element> mset;
		mset.insert_before(mset.begin(), values[0]);
		boost::intrusive::multiset<my_element>::const_iterator pos =
			mset.insert_before(mset.end(), values[99]);
		for (int i = 1; i < 99; ++i)
		{
			mset.insert_before(pos, values[i]);
		}

		//Now check ordered invariant
		boost::intrusive::multiset<my_element>::const_iterator next(mset.cbegin()),
			it(next++);
		for (int i = 1; i < 99; ++i, ++it, ++next)
		{
			BOOST_CHECK(*it < *next);
		}
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_erase_clear_remove_and_dispose)
{
	using namespace test_intrusive_erase_clear_remove_and_dispose;

	const int MaxElem = 100;

	my_class_list list;

	try
	{
		//Insert new objects in the container
		for (int i = 0; i < MaxElem; ++i)
		{
			list.push_back(*new my_class(i));
		}

		//Now use remove_and_dispose_if to erase and delete the objects
		list.remove_and_dispose_if(is_even(), delete_disposer());
	}
	catch (...)
	{
		//If something throws,make sure that all the memory is freed
		list.clear_and_dispose(delete_disposer());
		throw;
	}

	//Dispose remaining elements
	list.erase_and_dispose(list.begin(), list.end(), delete_disposer());
}

BOOST_AUTO_TEST_CASE(c_intrusive_clone_from)
{
	using namespace test_intrusive_clone_from;

	const int MaxElem = 100;
	std::vector<my_element> nodes(MaxElem);
	std::generate(nodes.begin(), nodes.end(), [n = 0]() mutable {return my_element(n++); });

	//Fill all the nodes and insert them in the list
	my_base_list list;
	list.insert(list.end(), nodes.begin(), nodes.end());

	//Now clone "list" using "new" and "delete" object function
	my_base_list cloned_list;
	cloned_list.clone_from(list, new_cloner(), delete_disposer());

	//Test that both are equal
	BOOST_CHECK(cloned_list == list);

	//Don't forget to free the memory from the second list
	cloned_list.clear_and_dispose(delete_disposer());
}

BOOST_AUTO_TEST_CASE(c_intrusive_function_hook)
{
	using namespace test_intrusive_function_hook;

	my_class n;

	my_list l;
	l.insert(l.begin(), n);//insert the node in both lists

	BOOST_CHECK(l.size() == 1);
}

BOOST_AUTO_TEST_CASE(c_intrusive_recursive_container)
{
	using namespace test_intrusive_recursive_container;

	recursive f, f2;

	//A recursive list of recursive
	boost::intrusive::list<recursive,
		boost::intrusive::base_hook<boost::intrusive::list_base_hook<>>> l;

	//Insert a node in parent list;
	l.insert(l.begin(), f);

	//Insert a node in child list
	l.begin()->children.insert(l.begin()->children.begin(), f2);

	//Objects properly inserted
	BOOST_CHECK(l.size() == l.begin()->children.size());
	BOOST_CHECK(l.size() == 1);

	//Clear both lists
	l.begin()->children.clear();
	l.clear();
}

BOOST_AUTO_TEST_CASE(c_intrusive_recursive_container_member_hook)
{
	using namespace test_intrusive_recursive_container_member_hook;

	recursive f, f2;

	//A recursive list of recursive
	boost::intrusive::list<recursive, boost::intrusive::function_hook<functor>> l;

	//Insert a node in parent list
	l.insert(l.begin(), f);

	//Insert a node in parent list
	l.begin()->children.insert(l.begin()->children.begin(), f2);

	//Objects properly inserted
	BOOST_CHECK(l.size() == l.begin()->children.size());
	BOOST_CHECK(l.size() == 1);

	//Clear both lists
	l.begin()->children.clear();
	l.clear();
}

BOOST_AUTO_TEST_CASE(c_intrusive_smart_pointer)
{
	//using namespace test_intrusive_smart_pointer;

	////Now create an intrusive list in shared memory;
	////nodes and the container itself must be created in shared memory
	//const int MaxElem = 100;
	//const int ShmSize = 50000;
	//const char* ShmName = "test";
	//{
	//	//Erase all old shared memory
	//	boost::interprocess::shared_memory_object::remove(ShmName);
	//	boost::interprocess::managed_shared_memory shm(boost::interprocess::create_only, ShmName, ShmSize);

	//	//Create all nodes in shared memory using a shared memory vector
	//	//See Boost.Interprocess documentation for more information on this
	//	using shm_allocator_t = boost::interprocess::allocator<shared_memory_data,
	//		boost::interprocess::managed_shared_memory::segment_manager>;
	//	using shm_vector_t = boost::interprocess::vector<shared_memory_data, shm_allocator_t>;

	//	shm_allocator_t shm_alloc(shm.get_segment_manager());
	//	shm_vector_t* pshm_vect = shm.construct<shm_vector_t>(boost::interprocess::anonymous_instance)(shm_alloc);
	//	pshm_vect->resize(MaxElem);

	//	//Initialize all the nodes
	//	for (int i = 0; i < MaxElem; ++i)
	//	{
	//		(*pshm_vect)[i].set(i);
	//	}

	//	//Now create the shared memory intrusive list
	//	intrusive_list_t* plist = shm.construct<intrusive_list_t>(boost::interprocess::anonymous_instance)();

	//	//Insert objects stored in shared memory vector in the intrusive list
	//	plist->insert(plist->end(), pshm_vect->begin(), pshm_vect->end());

	//	//Check all the inserted nodes
	//	int checker = 0;
	//	for (intrusive_list_t::const_iterator it = plist->begin(), itend = plist->end();
	//		it != itend;
	//		++it, ++checker)
	//	{
	//		BOOST_CHECK(it->get() == checker);
	//	}

	//	//Now delete the list and after that, the ndoes
	//	shm.destroy_ptr(plist);
	//	shm.destroy_ptr(pshm_vect);
	//}

	//boost::interprocess::shared_memory_object::remove(ShmName);
}

BOOST_AUTO_TEST_CASE(c_intrusive_iterator_to_local_iterator_to)
{
	using namespace test_intrusive_iterator_to_local_iterator_to;

	//Create MaxElem objects
	const int MaxElem = 100;
	std::vector<intrusive_data> nodes(MaxElem);

	//Declare the intrusive containers
	list_t list;
	unordered_set_t::bucket_type buckets[MaxElem];
	unordered_set_t unordered_set(unordered_set_t::bucket_traits(buckets, MaxElem));

	//Initialize all the nodes
	for (int i = 0; i < MaxElem; ++i)
	{
		nodes[i].set(i);
	}

	//Now insert them in both intrusive containers
	list.insert(list.end(), nodes.begin(), nodes.end());
	unordered_set.insert(nodes.begin(), nodes.end());

	//Now check the iterator_to function
	list_t::iterator list_it(list.begin());
	for (int i = 0; i < MaxElem; ++i, ++list_it)
	{
		BOOST_CHECK(list.iterator_to(nodes[i]) == list_it);
		BOOST_CHECK(list_t::s_iterator_to(nodes[i]) == list_it);
	}

	//Now check unordered_set::local_iterator_to (which is a member function)
	//and unordered_set::s_local_iterator_to(which is an static member function)
	unordered_set_t::iterator unordered_set_it(unordered_set.begin());
	for (int i = 0; i < MaxElem; ++i)
	{
		unordered_set_it = unordered_set.find(nodes[i]);
		BOOST_CHECK(unordered_set.iterator_to(nodes[i]) == unordered_set_it);

		BOOST_CHECK(*unordered_set.local_iterator_to(nodes[i]) == *unordered_set_it &&
			*unordered_set_t::s_local_iterator_to(nodes[i]) == *unordered_set_it);
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_any_hook)
{
	using namespace test_intrusive_any_hook;

	//Define a base hook option that converts any_base_hook to a slist hook
	using base_slist = boost::intrusive::slist<my_class, boost::intrusive::any_to_slist_hook<boost::intrusive::base_hook<boost::intrusive::any_base_hook<>>>>;
	//Define a member hook option that converts any_member_hook to a list hook
	using member_list = boost::intrusive::list<my_class, boost::intrusive::any_to_list_hook<boost::intrusive::member_hook<my_class, boost::intrusive::any_member_hook<>, &my_class::member_hook_>>>;

	//Create several my_class objects, each one with a different value
	std::vector<my_class> values;
	for (int i = 0; i < 100; ++i)
	{
		values.push_back(my_class(i));
	}

	base_slist base_slist_;
	member_list member_list_;

	//Now insert them in reverse order in the slist and in order in the list
	for (std::vector<my_class>::iterator it(values.begin()), itend(values.end()); it != itend; ++it)
	{
		base_slist_.push_front(*it), member_list_.push_back(*it);
	}

	//Now test lists
	base_slist::iterator bit(base_slist_.begin());
	member_list::reverse_iterator mrit(member_list_.rbegin());
	std::vector<my_class>::reverse_iterator rit(values.rbegin()), ritend(values.rend());

	//Test the objects inserted in the base hook list
	for (; rit != ritend; ++rit, ++bit, ++mrit)
	{
		BOOST_CHECK(&*bit == &*rit && &*mrit == &*rit);
	}
}

BOOST_AUTO_TEST_CASE(c_intrusive_singly_linked_list_algorithms)
{
	using namespace test_intrusive_singly_linked_list_algorithms;

	using algo = boost::intrusive::circular_slist_algorithms<my_slist_node_traits>;
	my_node one, two, three;

	//Create an empty singly linked list container:
	//"one" will be the first node of the container
	algo::init_header(&one);
	BOOST_CHECK(algo::count(&one) == 1);

	//Now add a new node
	algo::link_after(&one, &two);
	BOOST_CHECK(algo::count(&one) == 2);

	//Node add a new node after "one"
	algo::link_after(&one, &three);
	BOOST_CHECK(algo::count(&one) == 3);

	//Now unlink the node after one
	algo::unlink_after(&one);
	BOOST_CHECK(algo::count(&one) == 2);

	//Now unlink the node two
	algo::unlink(&two);
	BOOST_CHECK(algo::count(&one) == 1);
}

BOOST_AUTO_TEST_CASE(c_intrusive_doubly_linked_list_algorithms)
{
	using namespace test_intrusive_doubly_linked_list_algorithms;

	using algo = boost::intrusive::circular_list_algorithms<my_list_node_traits>;
	my_node one, two, three;

	//Create an empty doubly linked list container:
	//"one" will be the first node of the container
	algo::init_header(&one);
	BOOST_CHECK(algo::count(&one) == 1);

	//Now add a new node before "one"
	algo::link_before(&one, &two);
	BOOST_CHECK(algo::count(&one) == 2);

	//Now add a new node after "two"
	algo::link_after(&two, &three);
	BOOST_CHECK(algo::count(&one) == 3);

	//Now unlink the node after one
	algo::unlink(&three);
	BOOST_CHECK(algo::count(&one) == 2);

	//Now unlink two
	algo::unlink(&two);
	BOOST_CHECK(algo::count(&one) == 1);

	//Now unlink one
	algo::unlink(&one);
	BOOST_CHECK(algo::count(&one) == 1);
}

BOOST_AUTO_TEST_CASE(c_intrusive_red_black_tree_algorithms)
{
	using namespace test_intrusive_red_black_tree_algorithms;

	using algo = boost::intrusive::rbtree_algorithms<my_rbtree_node_traits>;
	my_node header, two(2), three(3);

	//Create an empty rbtree container:
	//"header" will be the header node of the tree
	algo::init_header(&header);

	//Now insert node "two" in the tree using the sorting functor
	algo::insert_equal_upper_bound(&header, &two, node_ptr_compare());

	//Now insert node "three" in the tree using the sorting functor
	algo::insert_equal_lower_bound(&header, &three, node_ptr_compare());

	//Now take the first node (the left node of the header)
	my_node* n = header.left_;
	BOOST_CHECK(n == &two);

	//Now go to the next node
	n = algo::next_node(n);
	BOOST_CHECK(n == &three);

	//Erase a node just using a pointer to it
	algo::unlink(&two);

	//Erase a node using also the header(faster)
	algo::erase(&header, &three);
}

BOOST_AUTO_TEST_CASE(c_intrusive_splay_tree_algorithms)
{
	using namespace test_intrusive_splay_tree_algorithms;

	using algo = boost::intrusive::splaytree_algorithms<my_splaytree_node_traits>;
	my_node header, two(2), three(3);

	//Create an empty splaytree container
	//"header" will be the header node of the tree
	algo::init_header(&header);

	//Now insert node "two" in the tree using the sorting functor
	algo::insert_equal_upper_bound(&header, &two, node_ptr_compare());

	//Now insert node "three" in the tree using the sorting functor
	algo::insert_equal_lower_bound(&header, &three, node_ptr_compare());

	//Now take the first node (the left node of the header)
	my_node* n = header.left_;
	BOOST_CHECK(n == &two);

	//Now go to the next node
	n = algo::next_node(n);
	BOOST_CHECK(n == &three);

	//Erase a node just using a pointer to it
	algo::unlink(&two);

	//Erase a node using also the header(faster)
	algo::erase(&header, &three);
}

BOOST_AUTO_TEST_CASE(c_intrusive_avl_tree_algorithm)
{
	using namespace test_intrusive_avl_tree_algorithm;

	using algo = boost::intrusive::avltree_algorithms<my_avltree_node_traits>;
	my_node header, two(2), three(3);

	//Create an empty avltree container
	//"header" will be the header node of the tree
	algo::init_header(&header);

	//Now insert node "two" in the tree using the sorting functor
	algo::insert_equal_upper_bound(&header, &two, node_ptr_compare());

	//Now insert node "three" in the tree using the sorting functor
	algo::insert_equal_lower_bound(&header, &three, node_ptr_compare());

	//Now take the first node (the left node of the header)
	my_node* n = header.left_;
	BOOST_CHECK(n == &two);

	//Now go to the next node
	n = algo::next_node(n);
	BOOST_CHECK(n == &three);

	//Erase a node just using a pointer to it
	algo::unlink(&two);

	//Erase a node using also the header (faster)
	algo::erase(&header, &three);
}

BOOST_AUTO_TEST_CASE(c_intrusive_treap_algorithms)
{
	using namespace test_intrusive_treap_algorithms;

	using algo = boost::intrusive::treap_algorithms<my_treap_node_traits>;
	my_node header, two(2, 5), three(3, 1);

	//Create an empty treap container:
	//"header" will be the header node of the tree
	algo::init_header(&header);

	//Now insert node "two" in the tree using the sorting functor
	algo::insert_equal_upper_bound(&header, &two, node_ptr_compare(), node_ptr_priority());

	//Now insert node "three" int the tree using the sorting functor
	algo::insert_equal_lower_bound(&header, &three, node_ptr_compare(), node_ptr_priority());

	//Now take the first node (the left node of the header)
	my_node* n = header.left_;
	BOOST_CHECK(n == &two);

	//Now go to the next node
	n = algo::next_node(n);
	BOOST_CHECK(n == &three);

	//Erase a node just using a pointer to it
	algo::unlink(&two, node_ptr_priority());

	//Erase a node using also the header (faster)
	algo::erase(&header, &three, node_ptr_priority());
}

BOOST_AUTO_TEST_CASE(c_intrusive_custom_valuetraits)
{
	using namespace test_intrusive_custom_valuetraits;

	BOOST_CHECK(test_list<legacy_abi_list>());
	BOOST_CHECK(test_list<legacy_abi_slist>());

	BOOST_CHECK(test_list<trivial_legacy_abi_list>());
	BOOST_CHECK(test_list<trivial_legacy_abi_slist>());
}

BOOST_AUTO_TEST_CASE(c_intrusive_stateful_value_traits)
{
	using namespace test_intrusive_stateful_value_traits;

	const int NumElements = 100;

	//This is an array of ids that we want to "store"
	identifier_t ids[NumElements];

	//This is an array of nodes that is necessary to form the linked list
	boost::intrusive::list_node_traits<void*>::node nodes[NumElements];

	//Initialize id objects, each one with a different number
	for (int i = 0; i != NumElements; ++i)
	{
		ids[i] = i;
	}

	//Define a list that will "link" identifiers using external nodes
	using List = boost::intrusive::list<identifier_t, boost::intrusive::value_traits<stateful_value_traits>>;

	//This list will store ids without modifying identifier_t instances
	//Stateful value traits must be explicitly passed in the constructor
	List my_list(stateful_value_traits(ids, nodes));

	//Insert ids in reverse order in the list
	for (identifier_t* it(&ids[0]), *itend(&ids[NumElements]); it != itend; ++it)
	{
		my_list.push_front(*it);
	}

	//Now test lists
	List::const_iterator list_it(my_list.cbegin());
	identifier_t* it_val(&ids[NumElements]), * it_rbeg_val(&ids[0]);

	//Test the objects inserted in the base hook list
	for (; it_val != it_rbeg_val; --it_val, ++list_it)
	{
		BOOST_CHECK(&*list_it == &it_val[-1]);
	}
}

BOOST_AUTO_TEST_SUITE_END()