#pragma once

//container
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/avl_set.hpp>
#include <boost/intrusive/splay_set.hpp>
#include <boost/intrusive/sg_set.hpp>
#include <boost/intrusive/treap_set.hpp>

#include <boost/intrusive/parent_from_member.hpp>

#include <boost/intrusive/any_hook.hpp>


//algrithm
#include <boost/intrusive/circular_slist_algorithms.hpp>
#include <boost/intrusive/circular_list_algorithms.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <boost/intrusive/splaytree_algorithms.hpp>
#include <boost/intrusive/avltree_algorithms.hpp>

#include <boost/intrusive/pointer_traits.hpp>
#include <boost/intrusive/link_mode.hpp>

#include <boost/intrusive/derivation_value_traits.hpp>
#include <boost/intrusive/member_value_traits.hpp>

#include <boost/functional/hash.hpp>

#include <boost/static_assert.hpp> //BOOS_STATIC_ASSERT

#include <boost/type_traits/is_same.hpp>

//#include <boost/interprocess/offset_ptr.hpp>
//#include <boost/interprocess/managed_shared_memory.hpp>
//#include <boost/interprocess/containers/vector.hpp>
//#include <boost/interprocess/allocators/allocator.hpp>

#include <vector>
#include <functional>
#include <iostream>

namespace test_intrusive_test
{
	//boost::intrusive::list_base_hook
	//带有 boost::intrusive::list_base_hook<缺省tag> 的元素类型
	class my_element : public boost::intrusive::list_base_hook<>
	{};

	//boost::intrusive::list
	//针对父类为 boost::intrusive::list_base_hook<缺省tag> 的my_element类型元素的容器
	using my_list = boost::intrusive::list<
											my_element
											>;
}

namespace test_intrusive_test2
{
	struct tag_for_my_list_a;//使用不完整类型就可以
	struct tag_for_my_list_b;//同上

	class my_element : public boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>>, public boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>>
	{};

	//针对父类为 boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>> 的my_element类型元素的容器
	using my_list_a = boost::intrusive::list<
											my_element,
											boost::intrusive::base_hook<//注意下面写法
																		boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>>
																		>
											>;
	//针对父类为 boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>> 的my_element类型元素的容器
	using my_list_b = boost::intrusive::list<
											my_element,
											boost::intrusive::base_hook<//注意下面写法
																		boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>>
																		>
											>;
}

namespace test_intrusive_test3
{
	class my_element
	{
	public:
		boost::intrusive::list_member_hook<> m_list_hook;//public成员变量
	};

	using my_list = boost::intrusive::list<
										   my_element,
										   boost::intrusive::member_hook<//注意下面写法
																		 my_element, boost::intrusive::list_member_hook<>, &my_element::m_list_hook
																		>
										  >;

	//  boost::intrusive::list_base_hook缺点:会破坏继承结果，违反"is a"含义
	//boost::intrusive::list_member_hook缺点:有些情况下不能使用
}

namespace test_intrusive_test4
{
	//同时存在

	class my_element : public boost::intrusive::list_base_hook<>
	{
	public:
		my_element(int i) :m_int(i) {}

	public:
		boost::intrusive::list_member_hook<> m_list_hook;
	private:
		int m_int;
	};

	using my_base_list = boost::intrusive::list<my_element>;
	using my_member_list = boost::intrusive::list<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::list_member_hook<>, &my_element::m_list_hook>>;
}

namespace test_intrusive_test5
{
	//抽象基类
	class window : public boost::intrusive::list_base_hook<>
	{
	public:
		using window_list = boost::intrusive::list<window>;//window是抽象类，std::list无法使用，除非是window*

		window()
		{
			m_window_list.push_back(*this);
		}

		virtual ~window()
		{
			m_window_list.erase(window_list::s_iterator_to(*this));
		}

		virtual void paint() = 0;

		static window_list m_window_list;
	};

	window::window_list window::m_window_list;//这个定义应该放到源文件中，避免重复定义，这里暂且忽略这个问题

	class framewindow : public window
	{
	private:
		void paint() override
		{
			std::cout << "framewindow" << "\n";
		}
	};

	class editwindow : public window
	{
		void paint() override
		{
			std::cout << "editwindow" << "\n";
		}
	};

	class canvaswindow : public window
	{
		void paint()
		{
			std::cout << "canvaswindow" << "\n";
		}
	};

	//...

	class mainwindow : public window
	{
	private:
		void paint() override
		{
			std::cout << "mainwindow" << "\n";
		}

	private:
		framewindow m_frame;
		editwindow m_edit;
		canvaswindow m_canvas;
	};

	void paint_all_windows()
	{
		for (window::window_list::iterator iter = window::m_window_list.begin(); iter != window::m_window_list.end(); ++iter)
		{
			iter->paint();
		}
	}

}

namespace test_intrusive_test6
{
	//link mode为auto unlink的hook

	class my_element : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>
	{
	public:
		using base = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;

		my_element(int i) :m_int(i) {}

		int get_int() { return m_int; }

		void unlink()
		{
			base::unlink();
		}

		bool is_linked()
		{
			return base::is_linked();
		}

	private:
		int m_int;
	};

	//Boost.Intrusive auto-unlink hooks are incompatible with containers that have constant-time size()
	using my_list = boost::intrusive::list<my_element, boost::intrusive::constant_time_size<false>>;

}

namespace test_intrusive_slist
{
	//intrusive singly linked list:slist

	//slist is the simplest intrusive container of Boost.Intrusive:a singly linked list. The memory overhead it 
	//imposes is 1 pointer per node. The size of an empty, non-constant-time size slist is the size of 1 pointer.
	//This lightweight memory overhead comes with drawbacks, though:many operations have linear time complexity,
	//even some that usually are constant time,like swap.slist only provides forward iterators.

	//For most cases, a doubly linked list is preferable because it offers more constant-time functions with a
	//slightly bigger size overhead. However, for some applications like constructing more elaborate containers,
	//singly linked lists are essential because of their low size overhead.

	////slist hooks
	//a.
	//template<class... Options>
	//class slist_base_hook;
	//b.
	//template<class... Options>
	//class slist_member_hook;

	//tag<class Tag>(for base hooks only)
	//--This argument servers as a tag, so you can derive from more than one slist hook.Default:tag<default_tag>.

	//link_mode<link_mode_type LinkMode>
	//--The linking policy.Default:link_mode<safe_link>.
	
	//void_pointer<class VoidPointer>
	//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>

	////slist container
	//template<class T, class... Options>
	//class slist;

	//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
	//--To specify the hook type or value traits used to configure the container

	//constant_time_size<bool Enabled>
	//--To activate the constant-time size() operation.Default:constant_time_size<true>

	//size_type<typename SizeType>
	//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

	////slist can receive addtional options
	//linear<bool Enable>
	//--the single linked list is implemented as a null-terminated list instead of a circular list.This allows 0(1) swap,
	//--but losses some operations like container_from_end_iterator.
	//cache_last<bool Enable>
	//--slist also stores a pointer to the last element of the singly linked list.This allows 0(1) swap,
	//--splice_after(iterator, slist&) and makes the list offer new functions like push_back(reference) and back().
	//--Logically,the size an empty list is increased in sizeof(void_pointer) and the cached last node pointer
	//--must be updated in every operation, and that might incur in a slight performance impace

	//auto_unlink hooks are not usable if linear<true> and/or cached_last<true> operator are used. if auto_unlink hooks
	//are used and those options are specified, a static assertion will be raised.


	//#include <boost/intrusive/slist.hpp>

	class my_slist_element : public boost::intrusive::slist_base_hook<>
	{
	public:
		my_slist_element(int i):m_int(i)
		{}

	public:
		boost::intrusive::slist_member_hook<> m_member_hook;

	private:
		int m_int;
	};

	using my_base_slist = boost::intrusive::slist<my_slist_element>;
	using my_member_slist = boost::intrusive::slist<my_slist_element, boost::intrusive::member_hook<my_slist_element, boost::intrusive::slist_member_hook<>, &my_slist_element::m_member_hook>>;
}

namespace test_intrusive_list
{
	//Intrusive doubly linked list:list

	//list is a doubly linked list.The memory overhead it imposes is 2 pointers per node. An empty, non constant-time
	//size list also has the size of 2 pointers. list has many more constant-time operations than slist and provides
	//a bidirectional iterator. It is recommended to use list instead of slist if the size overhead is acceptable.

	////list hooks
	//a.
	//template<class... Options>
	//class list_base_hook;
	//b.
	//template<class... Options>
	//class list_member_hook;

	//tag<class Tag>(for base hooks only)
	//--This argument serves as a tag, so you can derive from more than one list hook.Default:tag<default_tag>

	//link_mode<link_mode_type LinkMode>
	//--The linking policy.Default:link_mode<safe_link>
	
	//void_pointer<class VoidPointer>
	//--The pointer type to be used internally in the hook and propagated to the containers.Default:void_pointer<void*>

	////list container
	//template<class T, class... Options>
	//class list;

	//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
	//--To specify the hook type or value traits used to configure the container.

	//constant_time_size<bool Enabled>
	//--To activate the constant-time size() operation.Default constant_time_size<true>

	//size_type<typename SizeType>
	//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

	//#include <boost/intrusive/list.hpp>

	class my_list_element : public boost::intrusive::list_base_hook<>
	{
	public:
		my_list_element(int i):m_int(i)
		{}

	public:
		boost::intrusive::list_member_hook<> m_member_hook;

	private:
		int m_int;
	};

	using my_base_list = boost::intrusive::list<my_list_element>;
	using my_member_list = boost::intrusive::list<my_list_element, boost::intrusive::member_hook<my_list_element, boost::intrusive::list_member_hook<>, &my_list_element::m_member_hook>>;
}

////Intrusive associative containers:set, multiset, rbtree
//Boost.Intrusive also offers associative containers that can be very useful when creating more complex associative containers, like
//containers maintaining one or more indices with different sorting semantics.Boost.Intrusive associative containers,like most STL
//associative container implementations are based on red-black trees

//The memory overhead of these containers is usually 3 pointers and a bit(with alignment issues, this means 3 pointers and an integer).
//This size can be reduced to 3 pointers if pointers have even alignment (which is usually true in most systems).

//An empty, non constant-time size set, multiset or rbtree has also the size of 3 pointers and an integer (3 points when optimized for size.),
//These containers have complexity in many operations like searches, insertions, erasures, etc.

//set and multiset are the intrusive equivalents of standard std::set and std::multiset containers.

//rbtree is a superset of set and multiset containers that offers functions to insert unique and multiple keys

////set, multiset and rbtree hooks
//--set,multiset and rbtree share the same hooks.This is an advantage, because the same user type can be inserted first in a multiset and after that in
//--set without changing the definition of the user class
//a.
//template<class... Options>
//class set_base_hook;
//b.
//template<class... Options>
//class set_member_hook;

//tag<class Tag>(for base hooks only)
//--This argument serves as a tag, so you can derive from more than one base hook.Default:tag<default_tag>.

//link_mode<link_mode_type LinkMode>
//--The linking policy.Default:link_mode<safe_link>.

//void_pointer<class VoidPointer>
//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>

//optimize_size<bool Enable>
//--The hook will be optimized for size instead of speed.The hook will embed the color bit of the red-black tree node in the parent
//--pointer if pointer alignment is even. In some platforms, optimizing the size might reduce speed performance a bit since masking
//--operations will be needed to access parent pointer and color attributes, in other platforms this option improves performance due
//--to improved memory locality.Default:optimize_size<false>

////set, multiset and rbtree containers
//template<class T, class... Options>
//class set;
//template<class T, class... Options>
//class multiset;
//template<class T, class... Options>
//class rbtree;

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container.

//constant_time_size<bool Enabled>
//--To activate the constant-time size() operation.Default:constant_time_size<true>

//size_type<typename SizeType>
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//and they also can receive an additional option:

//compare<class Compare>
//--Comparison function for the objects to be inserted in containers. The comparison functor must induce a strict weak ordering.Default:compare<std::less<key_type>>
//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.Default:key_type is equal to value_type(set-like interface)

namespace test_intrusive_set
{
	//my_set_element、my_multiset_element和my_rbtree_element都是一样的，所以这里命名用my_element
	class my_element : public boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>  //这里选择使用optimize_size选项
	{
	public:
		my_element(int i) :m_int(i)
		{}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	//Define a set using the base hook that will store values in reverse order
	using my_base_set = boost::intrusive::set<my_element, boost::intrusive::compare<std::greater<my_element>>>;
}

namespace test_intrusive_multiset
{
	//my_set_element、my_multiset_element和my_rbtree_element都是一样的，所以这里命名用my_element
	class my_element
	{
	public:
		my_element(int i) :m_int(i)
		{}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	public:
		boost::intrusive::set_member_hook<> m_member_hook;

	private:
		int m_int;
	};

	using my_member_multiset = boost::intrusive::multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::set_member_hook<>, &my_element::m_member_hook>>;
}

namespace test_intrusive_rbtree
{
	//略
}

////Semi-intrusive unordered associative containers:unordered_set, unordered_multiset

//Boost.Intrusive also offers hashed containers that can be very useful to implement fast-lookup containers.
//These containers (unordered_set and unordered_multiset) are semi-intrusive containers:they need additional memory
//apart from the hook stored in the value_type. This additional memory must be passed in the constructor of the 
//container.

//Unlike C++ TR1 unordered associative containers (which are also hashed containers), the contents of these semi-intrusive
//containers are not rehashed to maintain a load factor:that would require memory management and intrusive containers don't
//implement any memory management at all.However,the user can request an explicit rehashing passing a new bucket array.
//This also offers an additional guarantee over TR1 unordered associative containers:iterators are not invalidated when
//inserting an element in the container.

//As with TR1 unordered associative containers,rehasing invalidates iterators,changes ordering between elements and changes
//which buckets elements appear in, but does not invalidate pointers or references to elements.

//apart from expected hash and equality function objects,Boost.Intrusive unordered associative containers' constructors take
//an argument specifying an auxiliary bucket vector to be used by the container.

////unordered_set and unordered_multiset performance notes
//The size overhead for a hashed container is moderate: 1 pointer per value plus a bucket array per container.The size of an
//element of the bucket array is usually one pointer.To obtain a good performance hashed container,the bucket length is usually
//the same as the number of elements that the container contains,so a well-balanced hashed container(bucket_count() is equal to size())
//will have an equivalent overhead of two pointers per element.

//An empty,non constant-time size unordered_set or unordered_multiset has also the size of bucket_count() pointers.

//Insertions, erasures, and searches, have amortized constant-time complexity in hashed containers.However, some worst-case guarantees
//are linear.See unordered_set or unordered_multiset for complexity guarantees of each operation.

//Be careful with non constant-time size hashed containers:some operations, like empty(), have linear complexity, unlike other Boost.Intrusive containers

////unordered_set and unordered_multiset hooks
//unordered_set and unordered_multiset share the same hooks.This is an advantage,because the same user type can be inserted first in a
//unordered_multiset and after that in unordered_set without changing the definition of the user class.

//template<class... Options>
//class unordered_set_base_hook;
//--unordered_set_base_hook:the user class derives publicly from unordered_set_base_hook to make it unordered_set/unordered_multiset-compatible.

//template<class... Options>
//class unordered_set_member_hook;
//--unordered_set_member_hook:the user class contains a public unordered_set_member_hook to make it unordered_set/unordered_multiset-compatible

//tag<class tag>(for base hooks only)
//--This argument serves as a tag,so you can derive from more than one base hook.Default:tag<default_tag>.

//link_mode<link_mode_type LinkMode>
//--The linking policy.Default:link_mode<safe_link>.

//void_pointer<class VoidPointer>
//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>.

//Apart from them, these hooks offer additional options:

//store_hash<bool Enabled>:
//--This option reserves additional space in the hook to store the hash value of the object once it's introduced in the containers.
//--When this option is used,the unordered container will store the calculated hash value in the hook and rehashing operations won't
//--need to recalculate the hash of the value.This option will improve the performance of unordered containers when rehashing is
//--frequent or hashing the value is a slow operation.Default:store_hash<false>.

//optimize_multikey<bool Enabled>
//--This option reserves additional space in the hook that will be used to group equal elements in unordered multisets,improving
//--significantly the performance when many equal values are inserted in these containers.Default:optimize_multikey<false>.

////unordered_set and unordered_multiset containers

//template<class T, class... Options>
//class unordered_set;

//template<class T, class... Options>
//class unordered_multiset;

//As mentioned,unordered containers need an auxiliary array to work.Boost.Intrusive unordered containers receive this auxiliary array
//packed in a type called bucket_traits (which can be also customized by a container option).All unordered containers receive a 
//bucket_traits object in their constructors.The default bucket_traits class is initialized with a point to an array of buckets and its size

//Each hashed container needs its own bucket traits,that is,its own bucket vector.Two hashed containers can't share the same bucket_type
//elements.The bucket array must be destroyed after the container using it is destroyed, otherwise, the result is undefined.

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container.

//constant_time_size<bool Enabled>
//--To activate the constant-time size() operation.Default:constant_time_size<true>

//size_type<typename SizeType>
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//And they also can receive additional options:

//equal<class Equal>
//--Equality function for the objects to be inserted in containers.Default:equal<std::equal_to<T>>

//hash<class Hash>
//--Hash function to be used in the container.Default:hash<boost::hash<T>>

//bucket_traits<class BucketTraits>
//--A type that wraps the bucket vector to be used by the unordered container.Default:a type initialized by the address and size of a bucket
//--array and stores both variables internally.

//power_2_buckets<bool Enabled>
//--The user guarantees that only bucket arrays with power of two length will be used.The container will then use masks instead of modulo
//--operations to obtain the bucket number from the hash value.Masks are faster than modulo operations and for some applications modulo
//--operations can impose a considerable overhead.In debug mode an assertion will be raised if the user provides a bucket length that is not
//--power of two.Default:power_2_buckets<false>

//cache_begin<bool Enabled>
//--Note:this option is not compatible with auto_unlink hooks.
//--Due to its internal structure,finding the first element of an unordered container(begin() operation) is amortized constant-time.It's
//--possible to speed up begin() and other operations related to it(like clear()) if the container caches internally the position of the
//--first element.This imposes the overhead of one pointer to the size of the container.Default:cache_begin<false>.

//compare_hash<bool Enabled>
//--Note:this option requires store_hash<true> option in the hook
//--When the comparison function is expensive,(e.g.strings with a long common predicate)sometimes (specially when the load factor is high or
//--we have many equivalent elements in an unordered_multiset and no optimize_multikey<> is activated in the hook)the equality function is
//--a performance problem.Two equal values must have equal hashes, so comparing the hash values of two elements before using the comparison
//--functor can speed up some implementations.

//incremental<bool Enabled>
//--Activates incremental hashing (also known as Linear Hashing).This option implies power_2_buckets<true> and the container will require
//--power of two buckets.Default:incremental<false>

//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.Default:key_type is
//--equal to value_type(set-like interface)

namespace test_intrusive_unordered_set
{
	//#include <boost/intrusive/unordered_set.hpp>

	class my_element : public boost::intrusive::unordered_set_base_hook<>   //This is a derivation hook
	{
	public:
		my_element(int i = 0) :m_int(i) {}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

		friend std::size_t hash_value(my_element const& value)
		{
			return std::size_t(value.m_int);
		}

	private:
		int m_int;
	};

	//Define an unordered_set that will store my_element objects using the base hook
	using my_base_unordered_set = boost::intrusive::unordered_set<my_element>;
}

namespace test_intrusive_unordered_multiset
{
	class my_element
	{
	public:
		my_element(int i = 0) :m_int(i) {}

	public:
		boost::intrusive::unordered_set_member_hook<> m_member_hook;//This is a member hook

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

		friend std::size_t hash_value(my_element const& value)
		{
			return std::size_t(value.m_int);
		}

	private:
		int m_int;
	};

	using my_member_unordered_multiset = boost::intrusive::unordered_multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::unordered_set_member_hook<>, &my_element::m_member_hook>>;
}

////Map and multimap-like interface for associative containers
//Implementing map-like intrusive containers is not a trivial task as STL's std::map and std::multimap containers store copies of a value_type
//which is defined as std::pair<const key_type, mapped_type>.In order to reproduce this interface in Boost.Intrusive it shall require that objects
//stored in the intrusive containers contain that std::pair member with pair.first hardcoded as the key part and pair.second hardcoded as the
//mapped_type,which is limiting and also not very useful in practice.Any intrusive associative container can be used like a map using advanced
//lookup and insertions and the user can change the key type in each lookup/insertion check call.

//On the other hand,in many cases containers are indexed by a well-known key type,and the user is forced to write some repetitive code using
//advanced lookup and insertions.Boost.Intrusive associative containers offer an alternative to build a useful map-like lookup interfaces
//without forcing users to define value_types containing std::pair-like classes.The option is called boost::intrusive::key_of_value.

//if a user specifies that option when defining a set/multiset intrusive container,it specifies a function object that will tell the container
//which is the type of the key that value_type holds and how to obtain it.This function object must be:
//a.Lightweight to copy.
//b.Default constructible (when the container constructor overload requires it).
//c.It shall define:
//	c1.A type member that defines the type of the key
//  c2.A member function that returns the key derived a value_type,either by value or by const-reference.

namespace test_intrusive_map_like
{
	class my_element : public boost::intrusive::set_base_hook<>
	{
		friend struct m_first_is_key;

	public:
		explicit my_element(int i) : m_first(i) {}

	private:
		int m_first;
	};

	//key_of_value function object, must:
	//a1.be default constructible if the container constructor requires it
	//a2.define the key type using "type"
	//a3.define an operator() taking "const value_type&" and returning "type" or "const type&"
	struct m_first_is_key
	{
		using type = int;

		type const& operator()(my_element const& value) const   //注意该函数是const成员函数
		{
			return value.m_first;
		}
	};

	using my_base_map_like = boost::intrusive::set<my_element, boost::intrusive::key_of_value<m_first_is_key>>;

}

namespace test_intrusive_unordered_map_like
{
	class my_element : public boost::intrusive::unordered_set_base_hook<>
	{
		friend struct m_first_is_key;

	public:
		explicit my_element(int i = 0) : m_first(i) {}

	private:
		int m_first;
	};

	//key_of_value function object, must:
	//a1.be default constructible if the container constructor requires it
	//a2.define the key type using "type"
	//a3.define an operator() taking "const value_type&" and returning "type" or "const type&"
	struct m_first_is_key
	{
		using type = int;

		type const& operator()(my_element const& value) const   //注意该函数是const成员函数
		{
			return value.m_first;
		}
	};

	using my_base_unordered_map_like = boost::intrusive::unordered_set<my_element, boost::intrusive::key_of_value<m_first_is_key>>;
}

////Intrusive avl tree based associative containers:avl_set,avl_multiset and avltree
//Similar to red-black trees,AVL trees are balanced binary trees.AVL trees are often compared with red-black trees because
//they support the same set of operations and because both take O(log n) time for basic operations. AVL trees are more rigidly
//balanced than Red-Black trees,leading to slower insertion and removal but faster retrieval.so AVL trees perform better than
//red-black trees for lookup-intensive applications.

//Boost.Intrusive offers 3 containers based on avl trees:avl_set,avl_multiset and avltree.The first two are similar to set or
//multiset and the latter is a generalization that offers functions both to insert unique and multiple keys

//The memory overhead of these containers with Boost.Intrusive hooks is usually 3 pointers and 2 bits (due to alignment, this
//usually means 3 pointers plus an integer).This size can be reduced to 3 pointers if pointers have 4 byte alignment (which is
//usually true in 32 bit systems)

//An empty,non constant-time size avl_set,avl_multiset or avltree also has a size of 3 pointers and an integer(3 pointers when
//optimized for size).

////avl_set,avl_multiset and avltree hooks

//avl_set,avl_multiset and avltree share the same hooks

//template<class... Options>
//class avl_set_base_hook;
//--the user class derives publicly from this class to make it compatible with avl tree based containers.

//template<class... Options>
//class avl_set_member_hook;
//--the user class contains a public member of this class to make it compatible with avl tree based containers.

//tag<class Tag>(for base hooks only)
//--This argument serves as a tag, so you can derive from more than one base hook.Default:tag<default_tag>.

//link_mode<link_mode_type LinkMode>
//--The linking policy.Default:link_mode<safe_link>.

//void_pointer<class VoidPointer>
//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>.

//optimize_size<bool Enable>
//--The hook will be optimized for size instead of speed.The hook will embed the balance bits of the AVL tree node in the parent
//pointer if pointer alignment is multiple of 4.In some platforms,optimizing the size might reduce speed performance a bit since
//masking operations will be needed to access parent pointer and balance factor attributes,in other platforms this option improves
//performance due to improved memory locality.Default:optimize_size<false>.

////avl_set,avl_multiset and avltree containers

//template<class T, class... Options>
//class avl_set;

//template<class T, class... Options>
//class avl_multiset;

//template<class T, class... Options>
//class avltree;

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container.

//constant_time_size<bool Enabled>
//--To activate the constant-time size() operation.Default:constant_time_size<true>

//size_type<typename SizeType>
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//And they also can receive an additional option:

//compare<class Compare>
//--Comparison function for the objects to be inserted in containers.The comparison functor must induce a strict weak ordering.
//--Default:compare<std::less<key_type>>

//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.
//--Default:key_type is equal to value_type(set-like interface)

namespace test_intrusive_avl_set
{
	class my_element : public boost::intrusive::avl_set_base_hook<>
	{
	public:
		explicit my_element(int i) :m_int(i) {}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	//Define an avl_set using the base hook that will store values in reverse order
	using my_base_avl_set = boost::intrusive::avl_set<my_element, boost::intrusive::compare<std::greater<my_element>>>;
}

namespace test_intrusive_avl_multiset
{
	class my_element
	{
	public:
		explicit my_element(int i) :m_int(i) {}

	public:
		boost::intrusive::avl_set_member_hook<> m_member_hook;

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	//Define an multiset using the member hook
	using my_member_avl_multiset = boost::intrusive::avl_multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::avl_set_member_hook<>, &my_element::m_member_hook>>;
}

////Intrusive splay tree based associative containers:splay_set, splay_multiset and splay_tree
//C++ associative containers are usually based on red-black tree implementations (e.g.:STL, Boost.Intrusive associative containers).However,
//there are other interesting data structures that offer some advantages (and also disadvantages).

//Splay trees are self-adjusting binary search trees used typically in caches, memory allocators and other applications,because splay trees
//have a "caching effect":recently accessed elements have better access times than elements accessed less frequently.

//Boost.Intrusive offers 3 containers based on splay trees:splay_set,splay_multiset and splaytree.The first two are similar to set or multiset
//and the latter is a generalization that offers functions both to insert unique and multiple keys.

//The memory overhead of these containers with Boost.Intrusive hooks is usually 3 pointers.An empty,non constant-time size splay container
//has also a size of 3 pointers

////Advantages and disadvantages of splay tree based containers

//Splay tree based intrusive containers have logarithmic complexity in many operations like searches,insertions,erasures,etc.but if some
//elements are more frequently accessed than others , splay trees perform faster searches than equivalent balanced binary trees (such as red-black trees).

//The caching effect offered by splay trees comes with a cost:the tree must be rebalanced when an element is searched.To maintain
//const-correctness and thread-safety guarantees, this caching effect is not updated when const versions of search functions like find(),
//lower_bound(),upper_bound(),equal_range(),count()... are called.This means that using splay-tree based associative containers as drop-in
//replacements of set/multiset, specially for const search functions,might not result in desired performance improvements.

//if element searches are randomized, the tree will be continuously srebalanced without taking advantage of the cache effect,so splay trees
//can offer worse performance than other balanced trees for several search patterns.

//Boost.Intrusive splay associative containers don't use their own hook types but plain Binary search tree hooks

////splay_set,splay_multiset and splaytree containers

//template<class T, class... Options>
//class splay_set;
//
//template<class T, class... Options>
//class splay_multiset;
//
//template<class T, class... Options>
//class splaytree;

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container

//constant_time_size<bool Enabled>
//--To activate the constant-time size() operation.Default:constant_time_size<true>

//size_type<typename SizeType>
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//And they also can receive an additional option:

//compare<class Compare>
//--Comparison function for the objects to be inserted in containers.The comparison functor must induce a strict weak ordering.Default:
//--compare<std::less<key_type>>

//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.Default:key_type
//--is equal to value_type(set-like interface)

namespace test_intrusive_splay_set
{
	class my_element : public boost::intrusive::bs_set_base_hook<>
	{
	public:
		explicit my_element(int i) :m_int(i) {}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	//
	using my_base_splay_set = boost::intrusive::splay_set<my_element, boost::intrusive::compare<std::greater<my_element>>>;
}

namespace test_intrusive_splay_multiset
{
	class my_element
	{
	public:
		explicit my_element(int i) :m_int(i) {}

	public:
		boost::intrusive::bs_set_member_hook<> m_member_hook;

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	using my_member_splay_multiset = boost::intrusive::splay_multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::bs_set_member_hook<>, &my_element::m_member_hook>>;
}

////Intrusive scapegoat tree based associative containers:sg_set,sg_multiset and sgtree
//A scapegoat tree is a self-balancing binary search tree, that provides worst-case O(log n) lookup time,and O(log n)amortized insertion
//and deletion time.Unlike other self-balancing binary search trees that provide worst case O(log n) lookup time,scapegoat trees have
//no additional per-node overhead compared to a regular binary search tree.


//A binary search tree is said to be weight balanced if half nodes are on the left of the root,and half on the right.An a-height-balanced
//tree is defined with defined with the following equation:

//height(tree) <= log1/a(tree.size())
//--a==1:A tree forming a linked list is considered balanced.
//--a==0.5:Only a perfectly balanced binary is considered balanced.

//scapegoat trees are loosely a-height-balanced so:
//height(tree) <= log1/a(tree.size()) + 1

//scapegoat trees support any a between 0.5 and 1.If a is higher,the tree is rebalanced less often,obtaining quicker insertions
//but slower searches.Lower a values improve search times.Scapegoat-trees implemented in Boost.Intrusive offer the possibility of
//changing a at run-time taking advantage of the flexibility of scapegoat trees

//scapegoat trees also have downsides:
//1.They need additional storage of data on the root(the size of the tree, for example) to achieve logarithmic complexity operations
//so it's not possible to offer auto_unlink hooks.The size of an empty scapegoat tree is also considerably increased.
//2.The operations needed to determine if the tree is unbalanced require floating-point operations like log1/a.If the system has
//no floating point operations(like some embedded systems).scapegoat tree operations might become slow.

//Boost.Intrusive offers 3 containers based on scapegoat trees:sg_set,sg_multiset and sgtree.The memory overhead of these containers
//with Boost.Intrusive hooks is 3 pointers.

//An empty,sg_set,sg_multiset or sgtree has also the size of 3 pointers, two integers and two floating point values(equivalent to
//the size of 7 pointers on most systems).

//Boost.Intrusive scapegoat associative containers don't use their own hook types but plain Binary search tree hooks.



////sg_set,sg_multiset and sgtree containers

//template<class T, class... Options>
//class sg_set;

//template<class T, class... Options>
//class sg_multiset;

//template<class T, class... Options>
//class sgtree;

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container.

//size_type<typename SizeType>:
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//And they also can receive additional options:

//compare<class Compare>
//--Comparison function for the objects to be inserted in containers.The comparison functor must induce a strict weak ordering.
//--Default:compare<std::less<key_type>>

//floating_point<bool Enable>
//--When this option is deactivated,the scapegoat tree loses the ability to change the balance factor a at run-time,but the size of
//--an empty container is reduced and no floating operations are performed, normally increasing container performance.The fixed a
//--factor that is used when this option is activated is 1/sqrt(2)~0.70711.Default:floating_point<true>

//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.
//--Default:key_type is equal to value_type(set-like interface)

namespace test_intrusive_sg_set
{
	class my_element : public boost::intrusive::bs_set_base_hook<>
	{
	public:
		explicit my_element(int i) :m_int(i) {}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	using my_base_sg_set = boost::intrusive::sg_set<my_element, boost::intrusive::compare<std::greater<my_element>>>;
}

namespace test_intrusive_sg_multiset
{
	class my_element
	{
	public:
		explicit my_element(int i) :m_int(i) {}

	public:
		boost::intrusive::bs_set_member_hook<> m_member_hook;

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}

		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}

	private:
		int m_int;
	};

	using my_member_sg_multiset = boost::intrusive::sg_multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::bs_set_member_hook<>, &my_element::m_member_hook>>;
}

////Intrusive treap based associative containers:treap_set,treap_multiset and treap
//The name treap is a mixture of tree and heap indicating that Treaps exhibit the properties of both binary search trees
//and heaps.A treap is a binary search tree that orders the nodes by a key but also by a priority attribute.The nodes are
//ordered so that the keys form a binary search tree and the priorities obey the max heap order property.
//1.if v is a left descendant of u, then key[v] < key[u];
//2.if v is a right descendant of u, then key[v] > key[u];
//3.if v is a child of u,then priority[v] <= priority[u];

//If priorities are non-random, the tree will usually be unbalanced:this worse theoretical average-case behavior may be
//outweighed by better expected-case behavior, as the most important items will be near the root.This means most important
//objects will be retrieved faster than less important items and for items keys with equal keys most important objects
//will be found first.These properties are important for some applications.

//The priority comparison will be provided just like the key comparison, via a function object that will be stored in the 
//intrusive container.This means that the priority can be stored in the value to be introduced in the treap or computed on
//flight(via hashing or similar)

//Boost.Intrusive offers 3 containers based on treaps:treap_set,treap_multiset and treap

//The memory overhead of these containers with Boost.Intrusive hook is 3 pointers.

//An empty, treap_set,treap_multiset or treap has also the size of 3 pointers and an integer(supposing empty function objects
//for key and priority comparison and constant-time size).

//Boost.Intrusive treap associative containers don't use their own hook types but plain Binary search tree hooks.


////treap_set,treap_multiset and treap containers

//template<class T, class... Options>
//class treap_set;

//template<class T, class... Options>
//class treap_multiset;

//template<class T, class... Options>
//class treap;

//base_hook<class Hook>/member_hook<class T, class Hook, Hook T::* PtrToMember>/value_traits<class ValueTraits>
//--To specify the hook type or value traits used to configure the container

//constant-time_size<bool Enabled>
//--To activate the constant-time size() operation.Default:constant_time_size<true>

//size_type<typename SizeType>
//--To specify the type that will be used to store the size of the container.Default:size_type<std::size_t>

//And they also can receive additional options:

//compare<class Compare>
//--Comparison function for the objects to be inserted in containers.The comparison functor must induce a strict weak ordering.Default:
//--compare<std::less<key_type>>

//priority_of_value<class PriorityOfValue>
//--A function object that specifies the type of the priority(priority_type) of a treap container and an operator to obtain it from a
//--value type.Default:priority_type is equal to value_type(set-like interface).

//priority<class PriorityCompare>
//--Priority Comparison function for the objects to be inserted in containers.The comparison functor must induce a strict weak ordering.
//--Default:priority<priority_compare<priority_type>>

//key_of_value<class KeyOfValueFunctionObject>
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.Default
//key_type is equal to value_type(set-like interface)

//The default priority_compare<T> object function will call an unqualified function priority_order passing two constant T references
//as arguments and should return true if the first argument has higher priority (it will be searched faster),inducing strict weak ordering.
//The function will be found using ADL lookup so that the user just needs to define a priority_order function in the same namespace
//as the class:

//struct MyType
//{
//	friend bool priority_order(MyType const& lhs, MyType const& rhs)
//	{
//		...
//	}
//};

//or

//namespace mytype
//{
//	struct MyType{...};
//
//	bool priority_order(MyType const& lhs, MyType const& rhs){...}
//}

////Exception safety of treap-based intrusive containers
//In general,intrusive containers offer strong safety guarantees,but treap containers must deal with two possibly throwing functors
//(one for value ordering,another for priority ordering).Moreover,treap erasure operations require rotations based on the priority
//order function and this issue degrades usual erase(const_iterator) no-throw guarantee.However intrusive offers the strongest possible
//behaviour in these situations.In summary:
//1.If the priority order functor does not throw,treap-based containers,offer exactly the same guarantees as other tree-based containers
//2.If the priority order functor throws,treap-based containers offer strong guarantee.

namespace test_intrusive_treap_set
{
	class my_element : public boost::intrusive::bs_set_base_hook<>
	{
	public:
		my_element(int i, unsigned int prio) :m_int(i), m_prio(prio) {}

		unsigned int get_priority() const
		{
			return this->m_prio;
		}

		//Less and greater operators
		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}
		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}
		//Default priority compare
		friend bool priority_order(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_prio < rhs.m_prio;//Lower value means higher priority
		}

	private:
		int m_int;
		unsigned int m_prio;
	};

	//Define an treap_set using the base hook that will store values in reverse order
	using my_base_treap_set = boost::intrusive::treap_set<my_element, boost::intrusive::compare<std::greater<my_element>>>;

}

namespace test_intrusive_treap_multiset
{
	class my_element
	{
	public:
		my_element(int i, unsigned int prio) :m_int(i), m_prio(prio) {}

	public:
		boost::intrusive::bs_set_member_hook<> m_member_hook;

		unsigned int get_priority() const
		{
			return this->m_prio;
		}

		//Less and greater operators
		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}
		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}
		//Default priority compare
		friend bool priority_order(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_prio < rhs.m_prio;//Lower value means higher priority
		}
		//Inverse priority compare
		friend bool priority_inverse_order(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_prio > rhs.m_prio;//Higher value means higher priority
		}

	private:
		int m_int;
		unsigned int m_prio;
	};

	struct inverse_priority
	{
		bool operator()(my_element const& lhs, my_element const& rhs) const  //注意是const成员函数
		{
			return priority_inverse_order(lhs, rhs);
		}
	};

	//Define an multiset using the member hook that will store
	using my_member_treap_multiset = boost::intrusive::treap_multiset<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::bs_set_member_hook<>, &my_element::m_member_hook>, boost::intrusive::priority<inverse_priority>>;
}

////Binary search tree hooks:bs_set_base_hook and bs_set_member_hook
//Binary search tree hooks can be used with several tree-like containers that don't need any additional metadata for
//rebalancing operations.This has many advantages since binary search tree hooks can also be used to insert values in
//plain binary search tree,splay tree, scapegoat tree,and treap containers.

//template<class... Options>
//class bs_set_base_hook;
//--bs_set_base_hook:the user class derives publicly from this class to make it compatible with the mentioned tree based containers.

//template<class... Options>
//class bs_set_member_hook;
//--bs_set_member_hook:the user class contains a public member of this class to make it compatible with the mentioned tree based containers.

//tag<class Tag>(for base hooks only)
//--This argument serves as a tag, so you can derive from more than one base hook.Default:tag<default_tag>.

//link_mode<link_mode_type LinkMode>
//--The linking policy.Default:link_mode<safe_link>.

//void_pointer<class VoidPointer>
//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>.










////Advanced lookups/Advanced insertions/Positional insertions for associative containers

////Advanced lookups
//Boost.Intrusive associative containers offer an interface similar to STL associative containers.However,STL's ordered
//and unordered simple associative containers(std::set, std::multiset, std::unordered_set and std::unordered_multiset)
//have some inefficiencies caused by the interface in several search, insertion or erasure functions(equal_range, lower_bound,
//upper_bound, find,insert, erase...):the user can only operate with value_type objects or (starting from C++11),
//heterogeneous comparison lookups which are not flexible enough as key_compare shall support the comparison between the
//provided key and value_type, which precludes the use of user-defined comparison objects that can partition the search
//in a compatible but advanced way.

//To solve these problems, Boost.Intrusive containers offers functions where a key type different from key_type and a
//comparison object are provided by the user.This applies to:* equal_range * lower_bound * upper_bound * count * find * erase

//Requirements for such functions are:
//1.For unordered container the provided comparison and hashing function with the given key shall induce the same hash
//and equivalence as key_compare and hasher.
//2.For ordered associative containers,lookup and erasure functions, the container to be searched shall be partitioned
//in regards to the supplied comparison object and key.

namespace test_intrusive_advanced_lookups_advanced_insertions
{
	//Hash function for strings
	struct StrHasher
	{
		std::size_t operator()(const char* str) const
		{
			std::size_t seed = 0;
			for (; *str; ++str)
			{
				boost::hash_combine(seed, *str);
			}
			return seed;
		}
	};

	//假设Expensive的构造是昂贵的
	class Expensive : public boost::intrusive::set_base_hook<>, public boost::intrusive::unordered_set_base_hook<>
	{
	public:
		Expensive(const char* key):m_key(key)
		{
			//其他昂贵的初始化操作，这里省略
		}

		const std::string& get_key() const
		{
			return m_key;
		}

		friend bool operator<(Expensive const& lhs, Expensive const& rhs)
		{
			return lhs.m_key < rhs.m_key;
		}

		friend bool operator==(Expensive const& lhs, Expensive const& rhs)
		{
			return lhs.m_key == rhs.m_key;
		}

		friend std::size_t hash_value(Expensive const& object)
		{
			return StrHasher()(object.get_key().c_str());
		}

	private:
		std::string m_key;
	};

	//A set and unordered_set that store Expensive objects
	using my_set = boost::intrusive::set<Expensive>;
	using my_unordered_set = boost::intrusive::unordered_set<Expensive>;

	//Search functions
	Expensive* get_from_set(const char* key, my_set& set_object)
	{
		my_set::iterator iter = set_object.find(Expensive(key));
		if (iter == set_object.end())
		{
			return 0;
		}
		return &*iter;
	}

	Expensive* get_from_uset(const char* key, my_unordered_set& uset_object)
	{
		my_unordered_set::iterator iter = uset_object.find(Expensive(key));
		if (iter == uset_object.end())
		{
			return 0;
		}
		return &*iter;
	}
	//If "key" c-string is quite long Expensive has to construct a std::string using heap memory.Like Expensive,many times
	//the only member taking part in ordering issues is just a small part of the class.E.g.:with Expensive,only the internal
	//std::string is needed to compare the object.

	//In both containers, if we call get_from_set/get_from_unordered_set in a loop,we might get a performance penalty,because
	//we are forced to create a whole Expensive object to be able to find an equivalent one.

	//Sometimes the problem is not only performance-related,as we might not have enough information to construct the object
	//but we might have enough information to find the object.In this case.a name is enough to search Expensive objects in
	//the container but constructing an Expensive object might require more information that the searcher might not have.

	//To solve this, we can use the functions that take any type comparable with the value and a the 'compatible' comparison
	//object(and hash,when the associative container is unordered)

	//These compare Expensive and a c-string
	struct StrExpComp
	{
		bool operator()(const char* str, Expensive const& expensive) const
		{
			return std::strcmp(str, expensive.get_key().c_str()) < 0;
		}

		bool operator()(Expensive const& expensive, const char* str) const
		{
			return std::strcmp(expensive.get_key().c_str(), str) < 0;
		}
	};

	struct StrExpEqual
	{
		bool operator()(const char* str, Expensive const& expensive) const
		{
			return std::strcmp(str, expensive.get_key().c_str()) == 0;
		}

		bool operator()(Expensive const& expensive, const char* str) const
		{
			return std::strcmp(expensive.get_key().c_str(), str) == 0;
		}
	};

	//Optimized search functions
	Expensive* get_from_set_optimized(const char* key, my_set& set_object)
	{
		my_set::iterator iter = set_object.find(key, StrExpComp());
		if (iter == set_object.end())
		{
			return 0;
		}
		return &*iter;
	}

	Expensive* get_from_uset_optimized(const char* key, my_unordered_set& uset_object)
	{
		my_unordered_set::iterator iter = uset_object.find(key, StrHasher(), StrExpEqual());//这里使用了StrHasher()，这也是为什么他要单独拿出来做成函数对象的原因
		if (iter == uset_object.end())
		{
			return 0;
		}
		return &*iter;
	}


	////Advanced insertions   (针对非multi-，multi-的一定成功固不需要讨论)  (跟查询同理，因为插入前先构建然后查询是否已存在)
	//A similar issue happens with insertions in simple ordered and unordered associative containers with unique keys
	//(std::set and std::unordered_set).In these containers,if a value is already present,the value to be inserted is
	//discarded.With expensive values,if the value is already present,we can suffer efficiency problems.

	//set and unordered_set-like containers have insertion functions (insert_check, insert_unique_check,...) to check
	//efficiently, without constructing the value,if a value is present or not and if it's not present, a function to
	//insert it immediately(insert_commit) without any further lookup.Requirements for functions that check the existence
	//of such value are:

	//1.For unordered container the provided comparison and hashing function with the given key shall induce the same hash
	//and equivalence as key_compare and hasher.
	//2.For ordered associative containers,the provided comparison function with the given key,shall induce the same strict
	//weak order as key_compare.

	//To sum up, insert_check is similar to a normal insert but:
	//1.insert_check can be used with arbitary keys
	//2.if the insertion is possible (there is no equivalent value) insert_check collects all the needed information in an
	//insert_commit_data structure,so that insert_commit:
	//	a.does not execute further comparisons
	//	b.can be executed with constant-time complexity
	//  c.has no-throw guarantee.

	//These functions must be used with care,no other insertion or erasure must be executed between an insert_check and an
	//insert_commit pair.Otherwise,the behaviour is undefined.

	//With multiple ordered and unordered associative containers(multiset and unordered_multiset)there is no need for these
	//advanced insertion functions,since insertions are always successful.

	//Insertion functions
	bool insert_to_set(const char* key, my_set& set_object)
	{
		Expensive* pExpensive = new Expensive(key);
		bool success = set_object.insert(*pExpensive).second;
		if (!success)
		{
			delete pExpensive;
		}
		return success;
	}

	bool insert_to_uset(const char* key, my_unordered_set& uset_object)
	{
		Expensive* pExpensive = new Expensive(key);
		bool success = uset_object.insert(*pExpensive).second;
		if (!success)
		{
			delete pExpensive;
		}
		return success;
	}

	//If the object is already present,we are constructing an Expensive that will be discarded, and this is a waste of
	//resources.Instead of that,let's use insert_check and insert_commit functions:

	//Optimized insertion functions
	bool insert_to_set_optimized(const char* key, my_set& set_object)
	{
		my_set::insert_commit_data insert_data;
		bool success = set_object.insert_check(key, StrExpComp(), insert_data).second;
		if (success)
		{
			set_object.insert_commit(*new Expensive(key), insert_data);
		}
		return success;
	}
	bool insert_to_uset_optimized(const char* key, my_unordered_set& uset_object)
	{
		my_unordered_set::insert_commit_data insert_data;
		bool success = uset_object.insert_check(key, StrHasher(), StrExpEqual(), insert_data).second;
		if (success)
		{
			uset_object.insert_commit(*new Expensive(key), insert_data);
		}
		return success;
	}
}

////Positional insertions
//Some ordered associative containers offer low-level functions to bypass ordering checks and insert nodes directly in
//desired tree positions.These functions are provided for performance reasons when values to be inserted in the container
//are known to fulfill order (sets and multisets) and uniqueness(sets) invariants. A typical usage of these functions is
//when intrusive associative containers are used to build non-intrusive containers and the programmer wants to speed up
//assignments from other associative containers:if the ordering and uniqueness properties are the same,there is no need
//to waste time checking the position of each source value, because values are already ordered:back insertions will be
//much more efficient.

//Note:These functions don't check preconditions so they must used with care.They are low-level operations that will break
//container invariants if ordering and uniqueness preconditions are not assured by the caller.

namespace test_intrusive_positional_insertions
{
	class my_element : public boost::intrusive::set_base_hook<>
	{
	public:
		my_element(int i) :m_int(i) {}

		friend bool operator<(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int < rhs.m_int;
		}
		friend bool operator>(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int > rhs.m_int;
		}

	private:
		int m_int;
	};
}

////Erasing and disposing values from Boost.Intrusive containers
//One of the most tedious tasks when using intrusive containers is the management of the erased elements.When using STL containers, the
//container itself unlinks and destroys the contained elements, but with intrusive containers,the user must explicity destroy the object
//after erasing an element from the container.This makes STL-like functions erasing multiple objects unhelpful:the user can't destroy
//every erased element.For example,let's take the function remove_if from list:

//template<class Pred>
//void remove_if(Pred pred);

//How can the user destroy the elements(say,using operator delete)that will be erased according to the predicate?Boost.Intrusive containers
//offer additional functions that take a function object that will be called after the element has been erased from the container.For
//example,list offers:

//template<class Pred, class Disposer>
//void remove_and_dispose_if(Pred pred, Disposer disposer)

//With this function the user can efficiently remove and destroy elements if the disposer function destroys an object:remove_and_dispose_if
//will call the "disposer" function object for every removed element.list offers more functions taking a disposer function object as argument,
//like erase_and_dispose,clear_and_dispose,remove_and_dispose.etc.

//Note that the disposing function does not need to just destroy the object.It can implement any other operation like inserting the remove
//object in another container.

namespace test_intrusive_erase_clear_remove_and_dispose
{
	//A class that can be inserted in an intrusive list
	class my_class : public boost::intrusive::list_base_hook<>
	{
	public:
		my_class(int i):m_int(i)
		{}

		int m_int;
	};

	//Definition of the intrusive list
	typedef boost::intrusive::list<my_class> my_class_list;

	//The predicate function
	struct is_even
	{
		bool operator()(my_class const& c) const
		{
			return 0 == (c.m_int % 2);
		}
	};

	//The disposer object function
	struct delete_disposer
	{
		void operator()(my_class* delete_this)
		{
			delete delete_this;
		}
	};
}
//All Boost.Intrusive containers offer these "erase + dispose" additional members for all functions
//that erase an element from the container.


//Cloning Boost.Intrusive containers
//As previously mentioned,Boost.Intrusive containers are non-copyable and non-assignable, because intrusive containers don't
//allocate memory at all.To implement a copy-constructor or assignment operator,the user must clone one by one all the elements
//of the container and insert them in another intrusive container.However,cloning by hand is usually more inefficient than a
//member cloning function and a speciallized cloning function can offer more guarantees than the manual cloning (better exception
//safety guarantees,for example).

//To ease the implementation of copy constructors and assignment operations of classes containing Boost.Intrusive containers,
//all Boost.Intrusive containers offer a special cloning function called clone_from.

//Apart from the container to be cloned, clone_from takes two function objects as arguments.For example,consider the clone_from
//member function of list:

//template<class Cloner, class Disposer>
//void clone_from(const list& src, Cloner cloner, Disposer disposer);

//This function will make *this a clone of src.Let's explain the arguments:
//1.The first parameter is the list to be cloned.
//2.The second parameter is a function object that will clone value_type objects and return a pointer to be clone.It must 
//implement the following function:pointer operator()(const value_type&).
//3.The third parameter is a function object that will dispose value_type objects.It's used first to empty the container
//before cloning and to dispose the elements if an exception is thrown.

//The cloning function works as follows:
//1.First it clears and disposes all the elements from *this using the disposer function object.
//2.After that it starts cloning all the elements of the source container using the cloner function object.
//3.If any operation in the cloning function(for example,the cloner function object) throws,all the constructed elements are
//disposed using the disposer function object

namespace test_intrusive_clone_from
{
	class my_element : public boost::intrusive::list_base_hook<>
	{
	public:
		explicit my_element(int i = 0) :m_int(i) {}

		friend bool operator==(my_element const& lhs, my_element const& rhs)
		{
			return lhs.m_int == rhs.m_int;
		}
	private:
		int m_int;
	};

	using my_base_list = boost::intrusive::list<my_element>;

	//Cloner object function
	struct new_cloner
	{
		my_element* operator()(my_element const& clone_this)
		{
			return new my_element(clone_this);
		}
	};

	//The disposer object function
	struct delete_disposer
	{
		void operator()(my_element* delete_this)
		{
			delete delete_this;
		}
	};
}

////Using function hooks
//A programmer might find that base or member hooks are not flexible enough in some situations.In some applications it would be optimal to
//put a hook deep inside a member of a class or just outside the class.Boost.Intrusive has an easy option to allow such cases:function_hook.

//This option is similar to member_hook or base_hook, but the programmer can specify a function object that tells the container how to obtain
//a hook from a value and vice versa.The programmer just needs to define the following function object:

//This functor converts between value_type and a hook_type
//struct Functor
//{
//	//Required types
//	typedef /*impl-defined*/ hook_type;
//	typedef /*impl-defined*/ hook_ptr;
//	typedef /*impl-defined*/ const_hook_ptr;
//	typedef /*impl-defined*/ value_type;
//	typedef /*impl-defined*/ pointer;
//	typedef /*impl-defined*/ const_pointer;
//	//Required static functions
//	static hook_ptr to_hook_ptr(value_type& value);
//	static const_hook_ptr to_hook_ptr(const value_type& value);
//	static pointer to_value_ptr(hook_ptr n);
//	static const_pointer to_value_ptr(const_hook_ptr n);
//};

//Converting from values to hooks is generally easy, since most hooks are in practice members or base classes of class data members.The
//inverse operation is a bit more complicated,but Boost.Intrusive offers a bit of help with the function get_parent_from_member,which allows
//easy conversions from the address of a data member to the address of the parent holding that member 

namespace test_intrusive_function_hook
{
	struct my_class
	{
		int dummy;
		//This internal type has a member hook
		struct inner_node
		{
			int dummy;
			boost::intrusive::list_member_hook<> hook;
		}inner;
	};

	//This functor converts between my_class and inner_node's member hook
	struct functor
	{
		//Required types
		using hook_type = boost::intrusive::list_member_hook<>;//注意hook_type是什么
		using hook_ptr = hook_type*;
		using const_hook_ptr = const hook_type*;
		using value_type = my_class;//注意value_type是什么
		using pointer = value_type*;
		using const_pointer = const value_type*;

		//Required static functions
		static hook_ptr to_hook_ptr(value_type& value)
		{
			return &value.inner.hook;
		}
		static const_hook_ptr to_hook_ptr(const value_type& value)
		{
			return &value.inner.hook;
		}
		static pointer to_value_ptr(hook_ptr n)
		{
			return boost::intrusive::get_parent_from_member<my_class>
				(boost::intrusive::get_parent_from_member<my_class::inner_node>(n, &my_class::inner_node::hook), &my_class::inner);
		}
		static const_pointer to_value_ptr(const_hook_ptr n)
		{
			return boost::intrusive::get_parent_from_member<my_class>
				(boost::intrusive::get_parent_from_member<my_class::inner_node>(n, &my_class::inner_node::hook), &my_class::inner);
		}
	};

	using my_list = boost::intrusive::list<my_class, boost::intrusive::function_hook<functor>>;
}


//Boost.Intrusive containers can be used to define recursive structures very easily, allowing complex data
//structures with very low overhead
namespace test_intrusive_recursive_container
{
	//A recursive class
	class recursive : public boost::intrusive::list_base_hook<>
	{
	public:
		recursive() :boost::intrusive::list_base_hook<>(), children(){}

	private:
		recursive(recursive const&);
		recursive& operator=(recursive const&);

	public:
		boost::intrusive::list<recursive,
			boost::intrusive::base_hook<boost::intrusive::list_base_hook<>>> children;
	};
}

//注意：
//1.
//recursive data struct using Boost.Intrusive containers must avoid using hook deduction to
//avoid early type instantiation:

//This leads to compilation error (recursive is instantiated by 'list' to deduce hook properties
//pointer type, tag, safe-mode...)
//class recursive
//{
//	//...
//	boost::intrusive::list<recursive> l;
//	//...
//};

//Ok, programmer must specify the hook type to avoid early recursive instantiation
//class recursive
//{
//	//...
//	boost::intrusive::list<recursive,
//		boost::intrusive::base_hook<boost::intrusive::list_base_hook<>>> l;
//	//...
//};

//2.
//member hooks are not suitable for recursive structures:
//class recursive
//{
//private:
//	recursive(recursive const&);
//	recursive& operator=(recursive const&);
//
//public:
//	boost::intrusive::list_member_hook<> memhook;
//
//	boost::intrusive::list<recursive, boost::intrusive::member_hook<recursive, boost::intrusive::list_member_hook<>, &recursive::memhook>> children;
//};
//Specifying &Recursive::memhook(that is,the offset between memhook and recursive) provokes an early instantiation of recursive.To
//define recursive structures using member hooks,a programmer should use function_hook:

namespace test_intrusive_recursive_container_member_hook
{
	class recursive;

	//Declaration of the functor that converts between the recursive class and the hook
	struct functor
	{
		//Required types
		using hook_type = boost::intrusive::list_member_hook<>;
		using hook_ptr = hook_type*;
		using const_hook_ptr = const hook_type*;
		using value_type = recursive;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		//Required static functions
		static hook_ptr to_hook_ptr(value_type& value);
		static const_hook_ptr to_hook_ptr(const value_type& value);
		static pointer to_value_ptr(hook_ptr n);
		static const_pointer to_value_ptr(const_hook_ptr n);
	};

	//Define the recursive class
	class recursive
	{
	public:
		recursive() :hook(), children() {}

	private:
		recursive(const recursive&);
		recursive& operator=(const recursive&);

	public:
		boost::intrusive::list_member_hook<> hook;
		boost::intrusive::list<recursive, boost::intrusive::function_hook<functor>> children;
	};

	//definition of functor functions
	inline functor::hook_ptr functor::to_hook_ptr(functor::value_type& value)
	{
		return &value.hook;
	}
	inline functor::const_hook_ptr functor::to_hook_ptr(functor::value_type const& value)
	{
		return &value.hook;
	}
	inline functor::pointer functor::to_value_ptr(functor::hook_ptr n)
	{
		return boost::intrusive::get_parent_from_member<recursive>(n, &recursive::hook);
	}
	inline functor::const_pointer functor::to_value_ptr(functor::const_hook_ptr n)
	{
		return boost::intrusive::get_parent_from_member<recursive>(n, &recursive::hook);
	}
}



////Using smart pointers with Boost.Intrusive containers
namespace test_intrusive_smart_pointer
{
	////Boost.Intrusive hooks can be configured to use other pointers than raw pointers.When a Boost.Intrusive hook is
	////configured with a smart pointer as an argument, this pointer configuration is passed to the containers.For example,
	////if the following hook is configured with a smart pointer (for example,an offset pointer from Boost.Interprocess):
	//class shared_memory_data
	//	//Declare the hook with an offset_ptr from Boost.Interprocess to make this class compatible with shared memory
	//	: public boost::intrusive::list_base_hook<boost::intrusive::void_pointer<boost::interprocess::offset_ptr<void>>>
	//{
	//public:
	//	int get() const
	//	{
	//		return data_id_;
	//	}
	//	void set(int id)
	//	{
	//		data_id_ = id;
	//	}

	//private:
	//	int data_id_;
	//};

	////Any intrusive list constructed using this hook will be ready for shared memory, because the intrusive list will also use
	////offset pointers internally,For example, we can create an intrsive list in shared memory combining Boost.Interprocess and
	////Boost.Intrusive
	//using intrusive_list_t = boost::intrusive::list<shared_memory_data>;
}
////Requirements for smart pointers compatible with Boost.Intrusive
//Not every smart pointer is compatible with Boost.Intrusive:

//1.It must be compatible with C++11 std::pointer_traits requirements.Boost.Intrusive uses its own pointer_traits class to
//implement those features in both C++11 and C++03 compilers.
//2.It must have the same ownership semantics as a raw pointer.This means that resource management smart pointers(like boost::shared_ptr) can't be used.

//The conversion from the smart pointer to a raw pointer will be implemented as a recursive call to operator->() until
//the function returns a raw pointer.






////Obtaining iterators from values
//Boost.Intrusive offers another useful feature that's not present in STL containers:it's possible to obtain an iterator
//to a value from the value itself.This feature is implemented in Boost.Intrusive containers by a function called iterator_to:

//iterator iterator_to(reference value);
//const_iterator iterator_to(const_reference value);

//For Boost.Intrusive containers that have local iterators,like unordered associative containers,we can also obtain local
//iterators:

//local_iterator local_iterator_to(reference value);
//const_local_iterator local_iterator_to(const_reference value) const;

//For most Boost.Intrusive containers(list, slist, set, multiset) we have an alternative static s_iterator_to function.
//For unordered associative containers(unordered_set, multiset), iterator_to has no static alternative function.On the
//other hand,local_iterator_to functions have their s_local_iterator_to static alternatives.

//Alternative static functions are available under certain circumstances explained in the Stateful value traits section;
//if the programmer uses hooks provided by Boost.Intrusive,those functions will be available.

namespace test_intrusive_iterator_to_local_iterator_to
{
	class intrusive_data
	{
	public:
		void set(int id)
		{
			data_id_ = id;
		}

		//This class can be inserted in an intrusive list
		boost::intrusive::list_member_hook<> list_hook_;

		//This class can be inserted in an intrusive unordered_set
		boost::intrusive::unordered_set_member_hook<> unordered_set_hook_;

		//Comparison operators
		friend bool operator==(intrusive_data const& lhs, intrusive_data const& rhs)
		{
			return lhs.data_id_ == rhs.data_id_;
		}

		friend bool operator!=(intrusive_data const& lhs, intrusive_data const& rhs)
		{
			return lhs.data_id_ != rhs.data_id_;
		}

		//The hash function
		friend std::size_t hash_value(intrusive_data const& i)
		{
			return boost::hash<int>()(i.data_id_);
		}

	private:
		int data_id_;
	};

	//Definition of the intrusive list that will hold intrusive_data
	using list_t = boost::intrusive::list<intrusive_data, boost::intrusive::member_hook<intrusive_data, boost::intrusive::list_member_hook<>, &intrusive_data::list_hook_>>;

	//Definition of the intrusive unordered_set that will hold intrusive_data
	using unordered_set_t = boost::intrusive::unordered_set<intrusive_data, boost::intrusive::member_hook<intrusive_data, boost::intrusive::unordered_set_member_hook<>, &intrusive_data::unordered_set_hook_>>;
}




////Any Hooks:A single hook for any Intrusive container
//Sometimes, a class programmer wants to place a class in several intrusive containers
//but no at the same time.In this case,the programmer might decide to insert two hooks
//in the same class.
//However, there is a more size-efficient alternative in Boost.Intrusive:"any" hooks
//(any_base_hook and any_member_hook).These hooks can be used to store a type in several
//containers offered by Boost.Intrusive minimizing the size of the class.

//These hooks support these options:

//tag<class Tag>(for base hooks only):
//--This argument serves as a tag, so you can derive from more than one slist hook.Default:tag<default_tag>.

//link_mode<link_mode_type LinkMode>:
//--The linking policy.link_mode<auto_unlink> is not supported and link_mode<safe_mode> might offer
//--weaker error detection in any hooks than in other hooks.Default:link_mode<safe_link>.

//void_pointer<class VoidPointer>:
//--The pointer type to be used internally in the hook and propagated to the container.Default:void_pointer<void*>.

//auto_unlink can't be supported because the hook does not know in which type of container might
//be currently inserted.Additionally,these hooks don't support unlink() and swap_nodes()
//operations for the same reason.

namespace test_intrusive_any_hook
{
	//Here is an example that creates a class with two any hooks, and uses one to insert
	//the class in a slist and the other one in a list.

	//#include <boost/intrusive/any_hook.hpp>
	class my_class : public boost::intrusive::any_base_hook<> //Base hook
	{
	public:
		my_class(int i = 0):int_(i)
		{}

	public:
		boost::intrusive::any_member_hook<> member_hook_;//Member hook

	private:
		int int_;
	};
}




////Concepts explained
//This section will expand the explanation of previously presented basic concepts before explaining the 
//customization options of Boost.Intrusive.

//1.Node Algorithms:
//A set of static functions that implement basic operations on a group of nodes:initialize a node,link a node
//to a group of nodes,unlink a node from another group of nodes,etc.For example,a circular single linked list
//is a group of nodes,where each node has a pointer to the next node.Node Algorithms just require a NodeTraits
//template parameter and they can work with any NodeTraits class that fulfills the needed interface.As an example,
//here is a class that implements operations to manage a group of nodes forming a circular singly linked list:
namespace test_intrusive_node_algorithms
{
	template<class NodeTraits>
	struct my_slist_algorithms
	{
		using node_ptr = typename NodeTraits::node_ptr;
		using const_node_ptr = typename NodeTraits::const_node_ptr;

		//Get the previous node of "this_node"
		static node_ptr get_prev_node(node_ptr this_node)
		{
			node_ptr p = this_node;
			while (this_node != NodeTraits::get_next(p))
			{
				p = NodeTraits::get_next(p);
			}
			return p;
		}

		//number of elements in the group of nodes containing "this_node"
		static std::size_t count(const_node_ptr this_node)
		{
			std::size_t result = 0;
			const_node_ptr p = this_node;
			do
			{
				p = NodeTraits::get_next(p);
				++result;
			} while (p != this_node);

			return result;
		}

		//More operations
		//...
	};
}

//2.Node Traits:
//A class that encapsulates the basic information and operations on a node within a group of nodes:the type of the node,
//a function to obtain the pointer to the next node.etc.Node Traits specify the configuration information Node Algorithms need.
//Each type of Node Algorithm expects an interface that compatible Node Traits classes must implement.As an example,this is the
//definition of a Node Traits class that is compatible with the previously presented my_slist_algorithms:
namespace test_intrusive_node_traits
{
	struct my_slist_node_traits
	{
		//The type of the node
		struct node
		{
			node* next_;
		};

		//NodeTraits的node_ptr类型
		using node_ptr = node*;
		//NodeTraits的const_node_ptr类型
		using const_node_ptr = const node*;

		//NodeTraits的static node_ptr get_next(const_node_ptr n)函数
		//A function to obtain a pointer to the next node
		static node_ptr get_next(const_node_ptr n)
		{
			return n->next_;
		}

		//NodeTraits的static void set_next(node_ptr n, node_ptr next)函数
		//A function to set the pointer to the next node
		static void set_next(node_ptr n, node_ptr next)
		{
			n->next_ = next;
		}
	};
}

//3.Hook
//A class that the user must add as a base class or as a member to his own class to make that class insertable in a intrusive container.
//Usually the hook contains a ndoe object that will be used to form the group of nodes:For example,the following class is a Hook that
//the user can add as a base class,to make the user class compatible with a singly linked list container:
namespace test_intrusive_hook
{
	using namespace test_intrusive_node_traits;

	class my_slist_base_hook
		//This hook contains a node,that will be used
		//to link the user object in the group of nodes
		: private my_slist_node_traits::node
	{
	public:
		using node_ptr = my_slist_node_traits::node_ptr;
		using const_node_ptr = my_slist_node_traits::const_node_ptr;

		//Converts the generic node to the hook
		static my_slist_base_hook* to_hook_ptr(node_ptr p)
		{
			return static_cast<my_slist_base_hook*>(p);
		}

		//Returns the generic node stored by this hook
		node_ptr to_node_ptr()
		{
			return static_cast<node* const>(this);
		}

		//More operations
		//...
	};

	//To make my_class compatible with an intrusive singly linked list derive our class from the hook.
	class my_class : public my_slist_base_hook
	{
	//	void set(int value);
	//	int get() const;

	//private:
	//	int value_;
	};
}

//4.Intrusive Container:
//A container that offers a STL-like interface to store user objects.An intrusive container can be templatized to store different
//value types that use different hooks.An intrusive container is also more elaborate than a group of nodes:it can store the number
//of elements to achieve constant-time size information,it can offer debugging facilities,etc.For example,an slist container
//(intrusive singly linked list) should be able to hold my_class objects that might have decided to store the hook as a base class
//or as a member.Internally,the container will use Node Algorithms to implement its operations,and an intrusive container is
//configured using a template parameter called ValueTraits.ValueTraits will contain the information to convert user classes in nodes
//compatible with Node Algorithms.For example,this a possible slist implementation:
namespace test_intrusive_intrusive_container
{
	//template<class ValueTraits,...>
	//class slist
	//{
	//public:
	//	using value_type = typename ValueTraits::value_type;

	//	//More typedefs and functions
	//	//...

	//	//Insert the value as the first element of the list
	//	void push_front(reference value)
	//	{
	//		node_ptr to_insert(ValueTraits::to_node_ptr(value));
	//		circular_list_algorithms::link_after(to_insert, get_root_node());
	//	}

	//	//More operations
	//	//...
	//};
}

//5.Semi-Intrusive Container:
//A semi-intrusive container is similar to an Intrusive container,but apart from the values to be inserted in the container. it
//needs additional memory (for example, auxiliary arrays or indexes).
//6.Value Traits:
//As we can see,to make our classes intrusive-friendly we add a simple hook as a member or base class.The hook contains a generic
//node that will be inserted in a group of nodes.Node Algorithms just work with nodes and don't know anything about user classes.
//On the other hand,an intrusive container needs to know how to obtain a node from a user class,and also the inverse operation.
//So we can define ValueTraits as the glue between user classes and nodes required by Node Algorithms.Let's see a possible implementation
//of a value traits class that glues my_class and the node stored in the hook:
namespace test_intrusive_value_traits
{
	using namespace test_intrusive_node_traits;
	using namespace test_intrusive_hook;

	struct my_slist_derivation_value_traits
	{
	public:
		//ValueTraits的node_traits类型
		using node_traits = my_slist_node_traits;
		//ValueTraits的value_type类型
		using value_type = my_class;
		//ValueTraits的node_ptr类型
		using node_ptr = node_traits::node_ptr;
		//ValueTraits的pointer类型
		using pointer = value_type;

		//...
		using reference = my_class;


		//ValueTraits的static node_ptr to_node_ptr(reference value)
		//Converts user's value to a generic node
		static node_ptr to_node_ptr(reference value)
		{
			return static_cast<my_slist_base_hook&>(value).to_node_ptr();
		}

		//ValueTraits的static value_type* to_value_ptr(node_traits::node* n)
		//Converts a generic node into user's value
		static value_type* to_value_ptr(node_traits::node* n)
		{
			static_cast<value_type*>(my_slist_base_hook::to_hook_ptr(n));
		}

		//More operations
		//...

	};
}





////Node algorithms with custom NodeTraits

//As explained in the Concepts section,Boost.Intrusive containers are implemented using node algorithms that work on
//generic nodes.

//Sometimes,the use of intrusive containers is expensive for some environments and the programmer might want to avoid
//all the template instantiations related to Boost.Intrusive containers.However,the user can still benefit from 
//Boost.Intrusive using the node algorithms,because some of those algorithms,like red-black tree algorithms,are not
//trivial to write.

//All node algorithm classes are templatized by a NodeTraits class.This class encapsulates the needed internal type
//declarations and operations to make a node compatible with node algorithms.Each type of node algorithms has its own
//requirements:




////Intrusive singly linked list algorithms

//These algorithms are static members of the circular_slist_algorithms class:

//template<class NodeTraits>
//struct circular_slist_algorithms;

//An empty list is formed by a node whose pointer to the next node points to itself.circular_slist_algorithms is configured with
//a NodeTraits class,which encapsulates the information about the node to be manipulated.NodeTraits must support the following 
//interface:
//Typedefs:
//node:The type of the node that forms the circular list
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node (usually const node*)
//Static functions:
//static node_ptr get_next(const_node_ptr n):Returns a pointer to the next node stored in "n".
//static void set_next(node_ptr n, node_ptr next):Sets the pointer to the next node stored in "n" to "next".

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_singly_linked_list_algorithms
{
	struct my_node
	{
		my_node* next_;
		//other members...
	};

	//注意NodeTraits是单独创建一个类，而不是和my_node混在一起
	//Define our own slist_node_traits
	struct my_slist_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;
		static node_ptr get_next(const_node_ptr n)
		{
			return n->next_;
		}
		static void set_next(node_ptr n, node_ptr next)
		{
			n->next_ = next;
		}
	};
}
//For a complete list of functions see circular_slist_algorithms reference





////Intrusive doubly linked list algorithms

//These algorithms are static members of the circular_list_algorithms class:

//template<class NodeTraits>
//struct circular_list_algorithms;

//An empty list is formed by a node whose pointer to the next node points to itself.circular_list_algorithms is configured with a
//NodeTraits class,which encapsulates the information about the node to be manipulated.NodeTraits must support the following inteface:
//Typedefs:
//node:The type of the node that forms the circular list
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node(usually const node*)
//Static functions:
//static node_ptr get_next(const_node_ptr n):Returns a pointer to the next node stored in "n".
//static void set_next(node_ptr n, node_ptr next):Sets the pointer to the next node stored in "n" to "next".
//static node_ptr get_previous(const_node_ptr n):Returns a pointer to the previous node stored in "n".
//static void set_previous(node_ptr n, node_ptr prev):Sets the pointer to the previous node stored in "n" to "prev".

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_doubly_linked_list_algorithms
{
	struct my_node
	{
		my_node* next_, * prev_;
		//other members...
	};

	//注意NodeTraits是单独创建一个类，而不是和my_node混在一起
	//Define our own list_node_traits
	struct my_list_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;
		static node_ptr get_next(const_node_ptr n)
		{
			return n->next_;
		}
		static void set_next(node_ptr n, node_ptr next)
		{
			n->next_ = next;
		}
		static node* get_previous(const_node_ptr n)
		{
			return n->prev_;
		}
		static void set_previous(node_ptr n, node_ptr prev)
		{
			n->prev_ = prev;
		}
	};
}
//For a complete list of functions see circular_list_algorithms reference.



////Intrusive red-black tree algorithms

//These algorithms are static members of the rbtree-algorithms class:

//template<class NodeTraits>
//struct rbtree_algorithms;

//An empty tree is formed by a node whose pointer to the parent node is null,the left and right node pointers point to itself,and
//whose color is red.rbtree_algorithms is configured with a NodeTraits class,which encapsulates the information about the node to
//be manipulated.NodeTraits must support the following interface:

//Typedefs:
//node:The type of the node that forms the circular rbtree
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node (usually const node*)
//color:The type that can store the color of a node

//Static functions:
//static node_ptr get_parent(const_node_ptr n):Returns a pointer to the parent node stored in "n".
//static void set_parent(node_ptr n, node_ptr p):Sets the pointer to the parent node stored in "n" to "p".
//static node_ptr get_left(const_node_ptr n):Returns a pointer to the left node stored in "n".
//static void set_left(node_ptr n, node_ptr l):Sets the pointer to the left node stored in "n" to "l".
//static node_ptr get_right(const_node_ptr n):Returns a pointer to the right node stored in "n".
//static void set_right(node_ptr n, node_ptr r):Sets the pointer to the right node stored in "n" to "r".
//static color get_color(const_node_ptr n):Returns the color stored in "n".
//static void set_color(node_ptr n, color c):Sets the color stored in "n" to "c".
//static color black():Returns a value representing the black color.
//static color red():Returns a value representing the red color.

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_red_black_tree_algorithms
{
	struct my_node
	{
		my_node(int i = 0):int_(i)
		{}

		my_node* parent_, * left_, * right_;
		int color_;
		//other members
		int int_;
	};

	//Define our own rbtree_node_traits
	struct my_rbtree_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;
		using color = int;

		static node_ptr get_parent(const_node_ptr n)
		{
			return n->parent_;
		}
		static void set_parent(node_ptr n, node_ptr parent)
		{
			n->parent_ = parent;
		}
		static node_ptr get_left(const_node_ptr n)
		{
			return n->left_;
		}
		static void set_left(node_ptr n, node_ptr left)
		{
			n->left_ = left;
		}
		static node_ptr get_right(const_node_ptr n)
		{
			return n->right_;
		}
		static void set_right(node_ptr n, node_ptr right)
		{
			n->right_ = right;
		}
		static color get_color(const_node_ptr n)
		{
			return n->color_;
		}
		static void set_color(node_ptr n, color c)
		{
			n->color_ = c;
		}
		static color black()
		{
			return color(0);
		}
		static color red()
		{
			return color(1);
		}
	};

	struct node_ptr_compare
	{
		bool operator()(const my_node* a, const my_node* b)
		{
			return a->int_ < b->int_;
		}
	};
}
//For a complete list of functions see rbtree_algorithms reference.



////Intrusive splay tree algorithms
//These algorithms are static members of the splaytree_algorithms class:

//template<class NodeTraits>
//struct splaytree_algorithms;

//An empty tree is formed by a node whose pointer to the parent node is null,and whose left and right nodes pointers point to itself.
//splaytree_algorithms is configured with a NodeTraits class,which encapsulates the information about the node to be manipulated.
//NodeTraits must support the following interface:
//Typedefs:
//node:The type of the node that forms the circular splaytree
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node (usually const node*)
//Static functions:
//static node_ptr get_parent(const_node_ptr n):Returns a pointer to the parent node stored in "n".
//static void set_parent(node_ptr n, node_ptr p):Sets the pointer to the parent node stored in "n" to "p".
//static node_ptr get_left(const_node_ptr n):Returns a pointer to the left node stored in "n".
//static void set_left(node_ptr n, node_ptr l):Sets the pointer to the left node stored in "n" to "l".
//static node_ptr get_right(const_node_ptr n):Returns a pointer to the right node stored in "n".
//static void set_right(node_ptr n, node_ptr r):Sets the pointer to the right node stored in "n" to "r".

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_splay_tree_algorithms
{
	struct my_node
	{
		my_node(int i = 0)
			:int_(i)
		{}

		my_node* parent_, * left_, * right_;
		//other members
		int int_;
	};

	//Define our own splaytree_node_traits
	struct my_splaytree_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;

		static node_ptr get_parent(const_node_ptr n)
		{
			return n->parent_;
		}
		static void set_parent(node_ptr n, node_ptr parent)
		{
			n->parent_ = parent;
		}
		static node_ptr get_left(const_node_ptr n)
		{
			return n->left_;
		}
		static void set_left(node_ptr n, node_ptr left)
		{
			n->left_ = left;
		}
		static node_ptr get_right(const_node_ptr n)
		{
			return n->right_;
		}
		static void set_right(node_ptr n, node_ptr right)
		{
			n->right_ = right;
		}
	};

	struct node_ptr_compare
	{
		bool operator()(const my_node* a, const my_node* b)
		{
			return a->int_ < b->int_;
		}
	};
}
//For a complete list of functions see splaytree_algorithms reference.



////Intrusive avl tree algorithms
//avltree_algorithms have the same interface as rbtree_algorithms.

//template<class NodeTraits>
//struct avltree_algorithms;

//avltree_algorithms is configured with a NodeTraits class,which encapsulates the information about the node to be manipulated.
//NodeTraits must support the following interface:
//Typedefs:
//node:The type of the node that forms the circular avltree
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node (usually const node*)
//balance:A type that can represent 3 balance types (usually an integer)
//Static functions:
//static node_ptr get_parent(const_node_ptr n):Returns a pointer to the parent node stored in "n".
//static void set_parent(node_ptr n, node_ptr p):Sets the pointer to the parent node stored in "n" to "p".
//static node_ptr get_left(const_node_ptr n):Returns a pointer to the left node stored in "n".
//static void set_left(node_ptr n, node_ptr l):Sets the pointer to the left node stored in "n" to "l".
//static node_ptr get_right(const_node_ptr n):Returns a pointer to the right node stored in "n".
//static void set_right(node_ptr n, node_ptr r):Sets the pointer to the right node stored in "n" to "r".
//static balance get_balance(const_node_ptr n):Returns the balance factor stored in "n".
//static void set_balance(node_ptr n, balance b):Sets the balance factor stored in "n" to "b".
//static balance negative():Returns a value representing a negative balance factor
//static balance zero():Returns a value representing a zero balance factor.
//static balance positive():Returns a value representing a positive balance factor.

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_avl_tree_algorithm
{
	struct my_node
	{
		my_node(int i = 0) :int_(i)
		{}

		my_node* parent_, * left_, * right_;
		int balance_;

		//other members
		int int_;
	};

	//Define our own avltree_node_traits
	struct my_avltree_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;
		using balance = int;

		static node_ptr get_parent(const_node_ptr n)
		{
			return n->parent_;
		}
		static void set_parent(node_ptr n, node_ptr parent)
		{
			n->parent_ = parent;
		}
		static node_ptr get_left(const_node_ptr n)
		{
			return n->left_;
		}
		static void set_left(node_ptr n, node_ptr left)
		{
			n->left_ = left;
		}
		static node_ptr get_right(const_node_ptr n)
		{
			return n->right_;
		}
		static void set_right(node_ptr n, node_ptr right)
		{
			n->right_ = right;
		}
		static balance get_balance(const_node_ptr n)
		{
			return n->balance_;
		}
		static void set_balance(node_ptr n, balance b)
		{
			n->balance_ = b;
		}
		static balance negative()
		{
			return -1;
		}
		static balance zero()
		{
			return 0;
		}
		static balance positive()
		{
			return 1;
		}
	};

	struct node_ptr_compare
	{
		bool operator()(const my_node* a, const my_node* b)
		{
			return a->int_ < b->int_;
		}
	};
}
//For a complete list of functions see avltree_algorithms reference





////Intrusive treap algorithms
//treap_algorithms have the same interface as rbtree_algorithms.

//template<class NodeTraits>
//struct treap_algorithms;

//treap_algorithms is configured with a NodeTraits class,which encapsulates the information about the node to be manipulated.
//NodeTraits must support the following interface:
//Typedefs:
//node:The type of the node that forms the circular treap
//node_ptr:The type of a pointer to a node (usually node*)
//const_node_ptr:The type of a pointer to a const node (usually const node*)
//Static functions:
//static node_ptr get_parent(const_node_ptr n):Returns a pointer to the parent node stored in "n".
//static void set_parent(node_ptr n, node_ptr p):Sets the pointer to the parent node stored in "n" to "p".
//static node_ptr get_left(const_node_ptr n):Returns a pointer to the left node stored in "n".
//static void set_left(node_ptr n, node_ptr l):Sets the pointer to the left node stored in "n" to "l".
//static node_ptr get_right(const_node_ptr n):Returns a pointer to the right node stored in "n".
//static void set_right(node_ptr n, node_ptr r):Sets the pointer to the right node stored in "n" to "r".

//Once we have a node traits configuration we can use Boost.Intrusive algorithms with our nodes:
namespace test_intrusive_treap_algorithms
{
	struct my_node
	{
		my_node(int i = 0, unsigned int priority = 0) : prio_(priority), int_(i)
		{}

		my_node* parent_, * left_, * right_;
		int prio_;
		//other members
		int int_;
	};

	//Define our own treap_node_traits
	struct my_treap_node_traits
	{
		using node = my_node;
		using node_ptr = my_node*;
		using const_node_ptr = const my_node*;

		static node_ptr get_parent(const_node_ptr n)
		{
			return n->parent_;
		}
		static void set_parent(node_ptr n, node_ptr parent)
		{
			n->parent_ = parent;
		}
		static node_ptr get_left(const_node_ptr n)
		{
			return n->left_;
		}
		static void set_left(node_ptr n, node_ptr left)
		{
			n->left_ = left;
		}
		static node_ptr get_right(const_node_ptr n)
		{
			return n->right_;
		}
		static void set_right(node_ptr n, node_ptr right)
		{
			n->right_ = right;
		}
	};

	struct node_ptr_compare
	{
		bool operator()(const my_node* a, const my_node* b)
		{
			return a->int_ < b->int_;
		}
	};

	struct node_ptr_priority
	{
	bool operator()(const my_node* a, const my_node* b)
	{
		return a->prio_ < b->prio_;
	}
	};
}
//For a complete list of functions see treap_algorithms reference




//以上可以看出NodeTraits是节点与算法的桥梁，算法可以很好的复用






////Containers with custom ValueTraits
//As explained in the Concepts section,Boost.Intrusive containers need a ValueTraits class to perform transformations
//between nodes and user values.ValueTraits can be explicity configured (using the value_traits<> option) or implicitly
//configured (using hooks and their base_hook<>/member_hook<> options).ValueTraits contains all the information to glue
//the value_type of the containers and the node to be used in node algorithms,since these types can be different.Apart
//from this,ValueTraits also stores information about the link policy of the values to be inserted.

//Instead of using Boost.Intrusive predefined hooks a user might want to develop customized containers,for example,using
//nodes that are optimized for a specific application or that are compatible with a legacy ABI.A user might want to have
//only two additional pointers in his class and insert the class in a doubly linked list sometimes and in a singly linked
//list in other situations.You can't achieve this using Boost.Intrusive predefined hooks.Now, instead of using 
//base_hook<...> or member_hook<...> options the user will specify the value_traits<...> options.


////ValueTraits interface
//struct my_value_traits
//{
//	using node_traits = implementation_defined;
//	using value_type = implementation_defined;
//
//	using node_ptr = node_traits::node_ptr;
//	using const_node_ptr = node_traits::const_node_ptr;
//
//	using pointer = boost::intrusive::pointer_traits<node_ptr>::rebind_traits<value_type>::type::pointer;
//	using const_pointer = boost::intrusive::pointer_traits<node_ptr>::rebind_traits<const value_type>::type::pointer;
//
//	static const link_mode_type link_mode = some_linking_policy;
//
//	static node_ptr       to_node_ptr(value_type& value);
//	static const_node_ptr to_node_ptr(const value_type& value);
//	static pointer        to_value_ptr(node_ptr n);
//	static const_pointer  to_value_ptr(const_node_ptr n);
//};

//node_traits
//--The node configuration that is needed by node algorithms.These node traits and algorithms are described in the previous chapter:Node Algorithms.
//--If my_value_traits is meant to be used with slist,node_traits should follow the interface needed by circular_slist_algorithms.
//--If my_value_traits is meant to be used with list,node_traits shoud follow the interface needed by circular_list_algorithms.
//--If my_value_traits is meant to be used with set/multiset,node_traits should follow the interface needed by rbtree_algorithms.
//--If my_value_traits is meant to be used with unordered_set/unordered_multiset,node_traits should follow the interface needed by circular_slist_algorithms.

//node_ptr
//--A typedef for node_traits::node_ptr.

//const_node_ptr
//--A typedef for node_traits::const_node_ptr.

//value_type
//--The type that the user wants to insert in the container.This type can be the same as node_traits::node but it can be different
//--(for example,node_traits::node can be a member type of value_type).If value_type and node_traits::node are the same type, the
//--to_node_ptr and to_value_ptr functions are trivial.

//pointer
//--The type of a pointer to a value_type.It must be the same pointer type as node_ptr:If node_ptr is node*, pointer must be value_type*.
//--If node_ptr is smart_ptr<node_traits::node>, pointer must be smart_ptr<value_type>.This can be generically achieved using
//--boost::intrusive::pointer_traits(portable implementation of C++11 std::pointer_traits)

//const_pointer
//--The type of a pointer to a const value_type.It must be the same pointer type as node_ptr:If node_ptr is node*, const_pointer must be
//--const value_type*.If node_ptr is smart_ptr<node_traits::node>, const_pointer must be smart_ptr<const value_type>.

//link_mode
//--Indicates that value_traits needs some additional work or checks from the container.The types are enumerations defined in the
//--link_mode.hpp header.These are the possible type:
//--1.normal_link:If this linking policy is specified in a ValueTraits class as the link mode, containers configured with such 
//--ValueTraits won't set the hooks of the erased values to a default state.Containers also will check that the hooks of the new values
//--are default initialized.
//--2.safe_link:If this linking policy is specified as the link mode in a ValueTraits class,containers configurated with this ValueTraits
//--will set the hooks of the erased values to a default state.Containers also will check that the hooks of the new values are default initialized.
//--3.auto_unlink:Same as "safe_link" but containers with constant-time size features won't be compatible with ValueTraits configured
//--with this policy.Containers also know that a value can be silently erased from the container without using any function provided by
//--the containers.

//static node_ptr to_node_ptr(value_type& value) and static const_node_ptr to_node_ptr(const value_type& value)
//--These functions take a reference to a value_type and return a pointer to the node to be used with node algorithms.

//static pointer to_value_ptr(node_ptr n) and static const_pointer to_value_ptr(const_node_ptr n)
//--These functions take a pointer to a node and return a pointer to the value that contains the node.


////Custom ValueTraits example
//let's define our own value_traits class to be able to use Boost.Intrusive containers with an old C structure whose definition can't
//be changed.That legacy type has two pointers that can be used to build singly and doubly linked lists:in singly linked lists we only
//need a pointer, whereas in doubly linked lists,we need two pointers.Since we only have two pointers,we can't insert the object in both
//a singly and a doubly linked list at the same time.
namespace test_intrusive_custom_valuetraits
{
	//This node is the legacy type we can't modify and we want to insert in
	//intrusive list and slist containers using only two pointers, since we
	//know the object will never be at the same time in both lists.
	struct legacy_value
	{
		legacy_value* prev_;
		legacy_value* next_;
		int id_;
	};

	//Now we have to define a NodeTraits class that will implement the functions/typedefs that will make the legacy node compatible
	//with Boost.Intrusive algorithms.After that,we'll define a ValueTraits class that will configure Boost.Intrusive containers:

	//Define our own NodeTraits that will configure singly and doubly linked list algorithms.Note that this node traits is compatible
	//with circular_slist_algorithms and circular_list_algorithms.
	struct legacy_node_traits
	{
		using node = legacy_value;
		using node_ptr = legacy_value*;
		using const_node_ptr = const legacy_value*;

		static node* get_next(const node* n)
		{
			return n->next_;
		}
		static void set_next(node* n, node* next)
		{
			n->next_ = next;
		}
		static node* get_previous(const node* n)
		{
			return n->prev_;
		}
		static void set_previous(node* n, node* prev)
		{
			n->prev_ = prev;
		}
	};

	//This ValueTraits will configure list and slist. In this case,
	//legacy_node_traits::node is the same as the
	//legacy_value_traits::value_type so to_node_ptr/to_value_ptr functions are trivial
	struct legacy_value_traits
	{
		using node_traits = legacy_node_traits;
		using node_ptr = node_traits::node_ptr;
		using const_node_ptr = node_traits::const_node_ptr;
		using value_type = legacy_value;
		using pointer = legacy_value*;
		using const_pointer = const legacy_value*;

		static const boost::intrusive::link_mode_type link_mode = boost::intrusive::normal_link;

		static node_ptr to_node_ptr(value_type& value)
		{
			return node_ptr(&value);
		}

		static const_node_ptr to_node_ptr(const value_type& value)
		{
			return const_node_ptr(&value);
		}

		static pointer to_value_ptr(node_ptr n)
		{
			return pointer(n);
		}

		static const_pointer to_value_ptr(const_node_ptr n)
		{
			return const_pointer(n);
		}
	};

	//Defining a value traits class that simply defines value_type as legacy_node_traits::node is a common approach when defining
	//customized intrusive containers,so Boost.Intrusive offers a templatized trivial_value_traits class that does exactly what we want:
	using trivial_legacy_value_traits = boost::intrusive::trivial_value_traits<legacy_node_traits, boost::intrusive::normal_link>;

	//Now we can just define the containers that will store the legacy abi objects and write a little test:
	//Now define an intrusive list and slist that will store legacy_value objects
	using legacy_abi_list = boost::intrusive::list<legacy_value, boost::intrusive::value_traits<legacy_value_traits>>;
	using legacy_abi_slist = boost::intrusive::slist<legacy_value, boost::intrusive::value_traits<legacy_value_traits>>;
	using trivial_legacy_abi_list = boost::intrusive::list<legacy_value, boost::intrusive::value_traits<trivial_legacy_value_traits>>;
	using trivial_legacy_abi_slist = boost::intrusive::slist<legacy_value, boost::intrusive::value_traits<trivial_legacy_value_traits>>;

	//测试
	template<class List>
	bool test_list()
	{
		using Vect = std::vector<legacy_value>;

		//Create legacy_value objects, with a different internal number
		Vect legacy_vector;
		for (int i = 0; i < 100; ++i)
		{
			legacy_value value;
			value.id_ = i;

			legacy_vector.push_back(value);
		}

		//Create the list with the objects
		List mylist(legacy_vector.begin(), legacy_vector.end());

		//Now test both lists
		typename List::const_iterator bit(mylist.begin()), bitend(mylist.end());
		typename Vect::const_iterator it(legacy_vector.begin()), itend(legacy_vector.end());

		//Test the objects inserted in our list
		for (; it != itend; ++it, ++bit)
		{
			if (&*bit != &*it)
			{
				return false;
			}
		}
		return true;
	}
}
//As seen, several key elements of Boost.Intrusive can be reused with custom user types, if the user does not want to use
//the provided Boost.Intrusive facilities.


//以上可以看出ValueTraits是value_type(The type that the user wants to insert in the container.This type can be the same as node_traits::node
//but it can be different(for example,node_traits::node can be a member type of value_type))与算法节点的桥梁，算法节点可以很好的复用


////Reusing node algorithms for different values
//In the previous example, legacy_node_traits::node type and legacy_value_traits::value_type are the same type, but this
//is not necessary.It's possible to have several ValueTraits defining the same node_traits type (and thus, the same node_traits::node).
//This reduces the number of node algorithm instantiations,but now ValueTraits::to_node_ptr and ValueTraits::to_value_ptr functions need
//to offer conversions between both types.Let's see a small example:
namespace test_intrusive_reusing_node_algorithms_for_different_values
{
	//First,we'll define the node to be used in the algorithms.For a linked list,we just need a node that stores two pointers:
	//This is the node that will be used with algorithms
	struct simple_node
	{
		simple_node* prev_;
		simple_node* next_;
	};

	//Now we'll define two different types that will be inserted in intrusive lists and a templatized ValueTraits that will
	//work for both types:
	class base_1 {};
	class base_2 {};

	struct value_1 : public base_1, public simple_node
	{
		int id_;
	};

	struct value_2 : public base_1, public base_2, public simple_node
	{
		float id_;
	};

	//Define the node traits. A single node_traits will be enough
	struct simple_node_traits
	{
		using node = simple_node;
		using node_ptr = node*;
		using const_node_ptr = const node*;

		static node* get_next(const node* n)
		{
			return n->next_;
		}
		static void set_next(node* n, node* next)
		{
			n->next_ = next;
		}
		static node* get_previous(const node* n)
		{
			return n->prev_;
		}
		static void set_previous(node* n, node* prev)
		{
			n->prev_ = prev;
		}
	};

	//A templatized value traits for value_1 and value_2
	template<class ValueType>
	struct simple_value_traits
	{
		using node_traits = simple_node_traits;
		using node_ptr = node_traits::node_ptr;
		using const_node_ptr = node_traits::const_node_ptr;
		using value_type = ValueType;
		using pointer = ValueType*;
		using const_pointer = const ValueType*;

		static const boost::intrusive::link_mode_type link_mode = boost::intrusive::normal_link;
		
		static node_ptr to_node_ptr(value_type& value)
		{
			return node_ptr(&value);
		}
		static const_node_ptr to_node_ptr(const value_type& value)
		{
			return const_node_ptr(&value);
		}
		static pointer to_value_ptr(node_ptr n)
		{
			return static_cast<value_type*>(n);
		}
		static const_pointer to_value_ptr(const_node_ptr n)
		{
			return static_cast<const value_type*>(n);
		}
	};

	//Now define two containers.Both containers will instantiate the same list algorithms(circular_list_algorithms<simple_node_traits>),
	//due to the fact that the value traits used to define the containers provide the same node_traits type:
	using Value1List = boost::intrusive::list<value_1, boost::intrusive::value_traits<simple_value_traits<value_1>>>;
	using Value2List = boost::intrusive::list<value_2, boost::intrusive::value_traits<simple_value_traits<value_2>>>;
}
//All Boost.Intrusive containers using predefined hooks use this technique to minimize code size:all possible list containers
//created with predefined hooks that define the same VoidPointer type share the same list algorithms.



////Simplifying value traits definition
namespace test_intrusive_simplifying_value_traits_definition
{
	struct simple_node
	{
		simple_node* prev_;
		simple_node* next_;
	};

	class base_1 {};
	class base_2 {};

	struct value_1 : public base_1, public simple_node
	{
		int id_;
		simple_node node_;
	};

	struct value_2 : public base_1, public base_2, public simple_node
	{
		float id_;
		simple_node node_;
	};

	//Define the node traits.A single node_traits will be enough
	struct simple_node_traits
	{
		using node = simple_node;
		using node_ptr = node*;
		using const_node_ptr = const node*;

		static node* get_next(const node* n)
		{
			return n->next_;
		}
		static void set_next(node* n, node* next)
		{
			n->next_ = next;
		}
		static node* get_previous(const node* n)
		{
			return n->prev_;
		}
		static void set_previous(node* n, node* prev)
		{
			n->prev_ = prev;
		}
	};

	///////////////////////////
	//#include <boost/intrusive/derivation_value_traits.hpp>


	//The previous example can be further simplified using the derivation_value_traits class to define a value traits class
	//with a value that stores the simple_node as a base class:

	//Now define the needed value traits using derivation_value_traits
	using ValueTraits1 = boost::intrusive::derivation_value_traits<value_1, simple_node_traits, boost::intrusive::normal_link>;
	using ValueTraits2 = boost::intrusive::derivation_value_traits<value_2, simple_node_traits, boost::intrusive::normal_link>;

	//Now define two intrusive lists.Both lists will use the same algorithms:
	//circular_list_algorithms<simple_node_traits>
	using Value1List = boost::intrusive::list<value_1, boost::intrusive::value_traits<ValueTraits1>>;
	using Value2List = boost::intrusive::list<value_2, boost::intrusive::value_traits<ValueTraits2>>;


	///////////////////////////
	//#include <boost/intrusive/member_value_traits.hpp>


	//We can even choose to store simple_node as a member of value_1 and value_2 classes and use member_value_traits to
	//define the needed value traits classes:

	using ValueTraits3 = boost::intrusive::member_value_traits<value_1, simple_node_traits, &value_1::node_, boost::intrusive::normal_link>;
	using ValueTraits4 = boost::intrusive::member_value_traits<value_2, simple_node_traits, &value_2::node_, boost::intrusive::normal_link>;

	using Value3List = boost::intrusive::list<value_1, boost::intrusive::value_traits<ValueTraits3>>;
	using Value4List = boost::intrusive::list<value_2, boost::intrusive::value_traits<ValueTraits4>>;
}


////Stateful value traits
//Until now all shown custom value traits are stateless,that is,the transformation between nodes and values is
//implemented in terms of static functions.It's possible to use stateful value traits so that we can separate
//nodes and values and avoid modifying types to insert nodes.Boost.Intrusive differentiates between stateful
//and stateless value traits by checking if all Node <--> Value transformation functions are static or not:

//1.If all Node <-> Value transformation functions are static, a stateless value traits is assumed.transformations
//must be static functions.
//2.Otherwise a stateful value traits is assumed.

//Using stateful value traits it's possible to create containers of non-copyable/movable objects without modifying
//the definition of the class to be inserted.This interesting property is achieved without using global variables
//(stateless value traits could use global variables to achieve the same goal),so:
//1.Thread-safety guarantees
//--Better thread-safety guarantees can be achieved with stateful value traits,since accessing global resources
//--might require synchronization primitives that can be avoided when using internal state.
//2.Flexibility
//--A stateful value traits type can be configured at run-time.
//3.Run-time polymorphism
//--A value traits might implement node <-> value transformations as virtual functions.A single container type 
//--could be configured at run-time to use different node <-> value relationships.

//Statful value traits have many advantages but also some downsides:
//1.Performance
//--Value traits operations should be very efficient since they are basic operations used by containers.
//--A heavy node <-> value transformation will hurt intrusive containers' performance.
//2.Exception guarantees
//--The stateful ValueTraits must maintain no-throw guarantees,otherwise,the container invariants won't be preserved.
//3.Static functions
//--Some static functions offered by intrusive containers are not available because node <-> value transformations
//--are not static.
//4.Bigger iterators
//--The size of some iterators is increased because the iterator needs to store a pointer to the stateful value traits
//--to implement node to value transformations(e.g. operator*() and operator->())
namespace test_intrusive_stateful_value_traits
{
	//This type is not modifiable so we can't store hooks or custom nodes
	using identifier_t = int;

	//This value traits will associate elements from an array of identifiers with elements of an array of nodes.
	//The element i of the value array will use the node i of the node array
	struct stateful_value_traits
	{
		using node_traits = boost::intrusive::list_node_traits<void*>;
		using node = node_traits::node;
		using node_ptr = node*;
		using const_node_ptr = const node*;
		using value_type = identifier_t;
		using pointer = identifier_t*;
		using const_pointer = const identifier_t*;
		
		static const boost::intrusive::link_mode_type link_mode = boost::intrusive::normal_link;

		stateful_value_traits(pointer ids, node_ptr node_array)
			:ids_(ids), nodes_(node_array)
		{}

		//Note: non static functions!
		node_ptr to_node_ptr(value_type& value) const
		{
			return this->nodes_ + (&value - this->ids_);
		}
		const_node_ptr to_node_ptr(const value_type& value) const
		{
			return this->nodes_ + (&value - this->ids_);
		}
		pointer to_value_ptr(node_ptr n) const
		{
			return this->ids_ + (n - this->nodes_);
		}
		const_pointer to_value_ptr(const_node_ptr n) const
		{
			return this->ids_ + (n - this->nodes_);
		}

	private:
		pointer ids_;
		node_ptr nodes_;
	};
}



////Thread safety guarantees
//Intrusive containers have thread safety guarantees similar to STL containers.

//1.Several threads having read or write access to different instances is safe as long as inserted objects are different.
//2.Concurrent read - only access to the same container is safe.

//Some Intrusive hooks(auto - unlink hooks, for example) modify containers without having a reference to them : this is considered a write access to the container.
//Other functions, like checking if an object is already inserted in a container using the is_linked() member of safe hooks, constitute read access on the container without having a reference to it, so no other thread should have write access(direct or indirect) to that container.
//Since the same object can be inserted in several containers at the same time using different hooks, the thread safety of Boost.Intrusive is related to the containersand also to the object whose lifetime is manually managed by the user.
//As we can see, the analysis of the thread - safety of a program using Boost.Intrusive is harder than with non - intrusive containers.

//To analyze the thread safety, consider the following points :

//1.The auto - unlink hook's destructor and unlink() functions modify the container indirectly.
//2.The safe mode and auto - unlink hooks' is_linked() functions are a read access to the container.
//3.Inserting an object in containers that will be modified by different threads has no thread safety guarantee, although in most platforms it will be thread - safe without locking.






////Null forward iterators
//Boost.Intrusive implements C++14 Null Forward Iterators, a feature that was introduced with C++14. This means that equality and inequality comparison 
//are defined over all iterators for the same underlying sequence and the value initialized-iterators.Value initialized iterators behave as if they refer
//past the end of the same empty sequence :

//list<MyType> l = { ... };
//auto ni = list<MyType>::iterator();
//auto nd = list<MyType2>::iterator();
//ni == ni; // True.
//nd != nd; // False.
//ni == nd; // Won't compile.


////Scary Iterators

//The paper N2913, titled SCARY Iterator Assignment and Initialization, proposed a requirement that a standard container's iterator types have no 
//dependency on any type argument apart from the container's value_type, difference_type, pointer type, and const_pointer type. In particular, according
//to the proposal, the types of a standard container's iterators should not depend on the container's key_compare, hasher, key_equal, or allocator types.

//That paper demonstrated that SCARY operations were crucial to the performant implementation of common design patterns using STL components.It showed
//that implementations that support SCARY operations reduce object code bloat by eliminating redundant specializations of iteratorand algorithm 
//templates.

//Boost.Intrusive containers are a bit different from standard containers.In particular, they have no allocator parameterand they can be configured
//with additional options not present in STL - like containers.Thus Boost.Intrusive offers its own SCARY iterator implementation, where iterator
//types don't change when the container is configured with an option that does not alter the value <-> node transformation. More concretely, the
//following options and conditions guarantee that iterator types are unchanged:

//1.All containers : size_type<>, constant_time_size<>,
//2.slist : cache_last<>, linear<>,
//3.unordered_[multi]set : hash<>, equal<>, power_2_buckets<>, cache_begin<>.
//4.All tree - like containers([multi]set, avl_[multi]set, sg_[multi]set, bs_[multi]set, splay_[multi]set, treap_[multi]set) : compare<>.
//5.treap_[multi]set : priority<>
//6.bs_[multi]set, sg_[multi]set, treap_[multi]set, splay_[multi]set : They share the same iterator type when configured with the same options.






////Stabilityand insertion with hint in ordered associative containers with equivalent keys
//Boost.Intrusive ordered associative containers with equivalent keys offer stability guarantees, following C++ standard library's defect
//#233 resolution, explained in document Comments on LWG issue 233: Insertion hints in associative containers. This means that:

//1.A Insert without hint member function always insert at the upper bound of an equal range.
//2.A Insert with hint member function inserts the new value before the hint if hint's and new node's keys are equivalent.
//3.Implements Andrew Koenig as close as possible to hint proposal.A new element is always be inserted as close to the hint as possible.So, 
//for example, if there is a subsequence of equivalent values, a.begin() as the hint means that the new element should be inserted before the
//subsequence even if a.begin() is far away.This allows code to always append(or prepend) an equal range with something as simple as :
//m.insert(m.end(), new_node); or m.insert(m.begin(), new_node);







////Obtaining the same typesand reducing symbol length
//The flexible option specification mechanism used by Boost.Intrusive for hooksand containers has a couple of downsides :

//1.If a user specifies the same options in different order or specifies some options and leaves the rest as defaults, the
//type of the created container / hook will be different.Sometimes this is annoying, because two programmers specifying the
//same options might end up with incompatible types.For example, the following two lists, although using the same options, 
//do not have the same type :

//#include <boost/intrusive/list.hpp>
//using namespace boost::intrusive;
////Explicitly specify constant-time size and size type
//typedef list < T, constant_time_size<true>, size_type<std::size_t> List1;
////Implicitly specify constant-time size and size type
//typedef list<T> List2;

//2.Option specifiers lead to long template symbols for classesand functions.Option specifiers themselves are verboseand without
//variadic templates, several default template parameters are assigned for non - specified options.Object and debugging information
//files can grow and compilation times may suffer if long names are produced.

//To solve these issues Boost.Intrusive offers some helper metafunctions that reduce symbol lengthsand create the same type if
//the same options(either explicitly or implicitly) are used.These also improve compilation times.All containersand hooks have
//their respective make_xxx versions.The previously shown example can be rewritten like this to obtain the same list type :

//#include <boost/intrusive/list.hpp>
//using namespace boost::intrusive;
//#include <boost/intrusive/list.hpp>
//using namespace boost::intrusive;
////Explicitly specify constant-time size and size type
//typedef make_list < T, constant_time_size<true>, size_type<std::size_t>::type List1;
////Implicitly specify constant-time size and size type
//typedef make_list<T>::type List2;

//Produced symbol lengthsand compilation times will usually be shorterand object / debug files smaller.If you are concerned with
//file sizes and compilation times, this option is your best choice.






////Design Notes

//When designing Boost.Intrusive the following guidelines have been taken into account :

////Boost.Intrusive in performance sensitive environments
//Boost.Intrusive should be a valuable tool in performance sensitive environments, and following this guideline, Boost.Intrusive has been designed
//to offer well known complexity guarantees.Apart from that, some options, like optional constant - time, have been designed to offer faster complexity
//guarantees in some functions, like slist::splice.

//The advanced lookupand insertion functions for associative containers, taking an arbitrary key typeand predicates, were designed to avoid unnecessary
//object constructions.

////Boost.Intrusive in space constrained environments
//Boost.Intrusive should be useful in space constrained environments, and following this guideline Boost.Intrusive separates node algorithmsand intrusive
//containers to avoid instantiating node algorithms for each user type.For example, a single class of red - black algorithms will be instantiated to
//implement all set and multiset containers using raw pointers.This way, Boost.Intrusive seeks to avoid any code size overhead associated with templates.

//Apart from that, Boost.Intrusive implements some size improvements : for example, red - black trees embed the color bit in the parent pointer lower
//bit, if nodes are two - byte aligned.The option to forgo constant - time size operations can reduce container size, and this extra size optimization
//is noticeable when the container is empty or contains few values.

////Boost.Intrusive as a basic building block
//Boost.Intrusive can be a basic building block to build more complex containers and this potential has motivated many design decisions.For example,
//the ability to have more than one hook per user type opens the opportunity to implement multi - index containers on top of Boost.Intrusive.

//Boost.Intrusive containers implement advanced functions taking function objects as arguments(clone_from, erase_and_dispose, insert_check, etc.).
//These functions come in handy when implementing non - intrusive containers(for example, STL - like containers) on top of intrusive containers.

////Extending Boost.Intrusive
//Boost.Intrusive offers a wide range of containers but also allows the construction of custom containers reusing Boost.Intrusive elements.The programmer
//might want to use node algorithms directly or build special hooks that take advantage of an application environment.

//For example, the programmer can customize parts of Boost.Intrusive to manage old data structures whose definition can't be changed.

