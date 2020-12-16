#pragma once

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/unordered_set.hpp>

#include <boost/functional/hash.hpp>

#include <boost/static_assert.hpp> //BOOS_STATIC_ASSERT

#include <boost/type_traits/is_same.hpp>

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
		my_element(int i = 0) :m_int(i) {}

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

		my_element(int i = 0) :m_int(i) {}

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
		my_slist_element(int i = 0):m_int(i)
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
		my_list_element(int i = 0):m_int(i)
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
//template<clas... Options>
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
//operations will be needed to access parent pointer and color attributes, in other platforms this option improves performance due
//to improved memory locality.Default:optimize_size<false>

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
//--A function object that will define the key_type of the value type to be stored.This type will allow a map-like interface.Default:
//key_type is equal to value_type(set-like interface)


namespace test_intrusive_set
{
	//my_set_element、my_multiset_element和my_rbtree_element都是一样的，所以这里命名用my_element
	class my_element : public boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>  //这里选择使用optimize_size选项
	{
	public:
		my_element(int i = 0) :m_int(i)
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
		my_element(int i = 0) :m_int(i)
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
//When this option is used,the unordered container will store the calculated hash value in the hook and rehashing operations won't
//need to recalculate the hash of the value.This option will improve the performance of unordered containers when rehashing is
//frequent or hashing the value is a slow operation.Default:store_hash<false>.

//optimize_multikey<bool Enabled>
//--This option reserves additional space in the hook that will be used to group equal elements in unordered multisets,improving
//significantly the performance when many equal values are inserted in these containers.Default:optimize_multikey<false>.

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

//if a user specifies that option when defining a set/multiset intrusive container,it specifies a function objec that will tell the container
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
		friend struct first_int_is_key;

	public:
		explicit my_element(int i = 0) : m_first(i) {}

	private:
		int m_first;
	};

	//key_of_value function object, must:
	//a1.be default constructible if the container constructor requires it
	//a2.define the key type using "type"
	//a3.define an operator() taking "const value_type&" and returning "type" or "const type&"
	struct first_int_is_key
	{
		using type = int;

		type const& operator()(my_element const& value) const   //注意该函数是const成员函数
		{
			return value.m_first;
		}
	};

	using my_base_map_like = boost::intrusive::set<my_element, boost::intrusive::key_of_value<first_int_is_key>>;

}

namespace test_intrusive_unordered_map_like
{
	class my_element : public boost::intrusive::unordered_set_base_hook<>
	{
		friend struct first_int_is_key;

	public:
		explicit my_element(int i = 0) : m_first(i) {}

	private:
		int m_first;
	};

	//key_of_value function object, must:
	//a1.be default constructible if the container constructor requires it
	//a2.define the key type using "type"
	//a3.define an operator() taking "const value_type&" and returning "type" or "const type&"
	struct first_int_is_key
	{
		using type = int;

		type const& operator()(my_element const& value) const   //注意该函数是const成员函数
		{
			return value.m_first;
		}
	};

	using my_base_unordered_map_like = boost::intrusive::unordered_set<my_element, boost::intrusive::key_of_value<first_int_is_key>>;
}
