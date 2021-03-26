#pragma once

#include <boost/mp11.hpp>

namespace ns1
{//boost.mpl的问题，和与boost.mp11的不同

	//boost.mpl被广泛使用，虽然不完美但够用了

	//随着C++11的发布，语言特性有了很大变化，导致没有理由再使用boost.mpl中的实现方式，boost.mp11出现了

	//C++11支持带有参数包(parameter packs)的变长模板(variadic template)，可以按以下方式定义类型列表(type list)
	template<class... T>
	struct type_list
	{};

	//C++11支持别名模板，原来元函数(提供给他输入类型就返回结果类型)
	//定义：
	//template<class T>
	//struct add_pointer
	//{
	//	typedef T* type;
	//};
	//使用：
	//typedef typename add_pointer<int>::type Intp;
	//可以分别简写为
	//定义：
	template<class T>
	using add_pointer = T*;
	//使用：
	using Intp = add_pointer<int>;

	//C++11支持模板的>符号可以连续写而不需要空格隔开

	//C++11支持别名模板传递给模板模板参数
	template<template<class... T> class F> struct X
	{
	};
	X<add_pointer>; //正确

	//总结：boost.mpl已不再是最好选择
}

namespace ns2
{//类型列表(type list)的表示 和 mp_rename元函数的实现

	//定义类型列表  (注意类型列表是模板)
	template<class... T>
	struct mp_list
	{};

	//boost.mpl中使用命名空间防止名称冲突，而boost.mp11中除了使用命名空间外还是用了mp_前缀，下面是原因(我没看明白)
	/*
	* Why the mp_ prefix? mp obviously stands for metaprogramming, but could we not have used a namespace?
	* Indeed we could have. Past experience with Boost.MPL however indicates that name conflicts between our metaprogramming primitives and standard identifiers (such as list) and keywords (such as if, int or true) will be common and will be a source of problems. With a prefix, we avoid all that trouble.
	*/

	//往类型列表中盛放类型   (注意盛放后是类型)
	using list = mp_list<int, char, float, double, void>;  //注意可以有void

	//类型列表可以有不同的表示方法，当使用多个第三方库时，会遇到不同的类型列表，比如
	template<class... T>
	struct tuple
	{};
	//或者
	template<class... T>
	struct packer
	{};
	//当需要对类型列表进行操作时，比如
	//packer<int, float, void> 转为 packer<int*, float*, void*>
	//那么需要
	//packer<int, float, void> 转为 mp_list<int, float, void> 转为 mp_list<int*, float*, void*> 转为 packer<int*, float*, void*>

	//元函数 mp_rename 的定义
	template<class A, template<class...> class B>      //定义了输入参数的格式
	struct mp_rename_impl;

	template<template<class...> class A, class... T, template<class...> class B>
	struct mp_rename_impl<A<T...>, B>                  //类模板偏特化
	{
		using type = B<T...>;
	};

	template<class A, template<class...> class B>      //与上面定义的输入参数格式保持一致
	using mp_rename = typename mp_rename_impl<A, B>::type;    //这种后跟一个别名模板的形式是普遍写法，别名模板不支持偏特化和特化

	//使用：
	//mp_rename<packer<int, float, void*>, mp_list> 返回 mp_list<int, float, void*>

	//注意上面mp_rename的实现并没有依赖mp_list，所以他可以适用很多情况：
	//mp_rename<std::pair<int, float>, std::tuple> 返回 std::tuple<int, float>
	//mp_rename<mp_list<int, float>, std::pair> 返回 std::pair<int, float>
	//mp_rename<std::shared_ptr<int>, std::unique_ptr> 返回 std::unique_ptr<int>

	//注意 mp_rename<std::unique_ptr<int>, std::shared_ptr> 是错误的，因为 std::unique_ptr<int> 实际是 std::unique_ptr<int, std::default_delete<int>>
	//返回的 std::shared_ptr<int, std::default_delete<int>> 无法编译通过






	//定义一个依赖于 mp_list 的元函数 mp_size (不好)
	namespace mp_size_by_mp_list
	{
		template<class L>
		struct mp_size_impl;

		template<class... T>
		struct mp_size_impl<mp_list<T...>>
		{
			using type = std::integral_constant<std::size_t, sizeof...(T)>;
		};

		template<class L>
		using mp_size = typename mp_size_impl<L>::type;
	}
	//integral_constant的作用是将编译期整型常量封装为类型，因为类型列表中盛放的都是类型，方便操作，要想再转为编译期常量只需要::value就可以了

	//定义一个不依赖于 mp_list 的元函数 mp_size，类似mp_rename   (好，不需要先转为 mp_list 了)
	namespace mp_size_
	{
		template<class L>
		struct mp_size_impl;

		template<template<class...> class L, class... T>
		struct mp_size_impl<L<T...>>
		{
			using type = std::integral_constant<std::size_t, sizeof...(T)>;
		};

		template<class T>
		using mp_size = typename mp_size_impl<L>::type;
	}
	//这样 mp_size 也跟 mp_rename 一样，可以应用到多种类型列表上

	//避免转换是好的，但并不能说 mp_rename 就没有用了。比如有些情况下当给定 mp_list<int, void, float> 返回 X<int, void, float> 时，
	//可以用 mp_rename 简化:
	//template<class L>
	//using B = mp_rename<L, X>;
	
	//同理，mp_apply可以这样实现
	template<template<class...> class F, class L>
	using mp_apply = mp_rename<L, F>;
	//calls the metafunction F with the contents of the list L
}