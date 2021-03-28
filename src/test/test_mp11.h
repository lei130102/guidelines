#pragma once

#include <boost/mp11.hpp>

#include <tuple>
#include <type_traits>

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
		using mp_size = typename mp_size_impl<T>::type;
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

namespace ns3
{//实现 mp_transform

	//传入一个函数和一个列表，然后对列表中的每个元素调用该函数，这样的算法叫做 transform   
	//(boost.mpl、stl等都是这个叫法。不过这个算法一般列表是第一个参数，第二个参数是函数，这里的mp_transform颠倒了，因为列表作为变长模板参数时需要放到最后)

	//实现 mp_transform的方式有多种，这里使用基于 mp_push_front 实现

	// mp_push_front元函数功能是在类型列表前面插入类型
	// mp_push_front的实现：   (不好，一次只能插入一种类型)
	//template<typename L, typename T>
	//struct mp_push_front_impl;

	//template<template<class...> class L, class... U, class T>
	//struct mp_push_front_impl<L<U...>, T>
	//{
	//	using type = L<T, U...>;
	//};

	//template<typename L, typename T>
	//using mp_push_front = typename mp_push_front_impl<L, T>::type;

	//没有必要限制必须插入一个类型(也就上面的T)，注意在C++11中，变长模板(variadic template)应该是默认选择！！！
	// mp_push_front的实现：    (好，一次可以插入任意多个类型)
	template<typename L, typename... T>    
	struct mp_push_front_impl;

	template<template<class...> class L, class... U, class... T>
	struct mp_push_front_impl<L<U...>, T...>
	{
		using type = L<T..., U...>;
	};

	template<typename L, typename... T>
	using mp_push_front = typename mp_push_front_impl<L, T...>::type;






	//利用递归，基于 mp_push_front 实现 mp_transform   (不好，旧写法，使用递归实现)
	//template<template<class...> class F, class L>      //注意第二个参数是class L，L可以是mp_list也可以是其他形式的类型列表，这样元函数可以作用于不同形式的类型列表
	//struct mp_transform_impl;

	////因为下面使用 mp_transform ，所以这里定义它
	//template<template<class...> class F, class L>
	//using mp_transform = typename mp_transform_impl<F, L>::type;   //注意，可以在mp_transform_impl的特化/偏特化前使用typename mp_transform_impl<F, L>::type

	////递归终止
	//template<template<class...> class F, template<class...> class L>
	//struct mp_transform_impl<F, L<>>
	//{
	//	using type = L<>;
	//};

	//template<template<class...> class F, template<class...> L, class T1, class... T>     
	//struct mp_transform_impl<F, L<T1, T...>>     //因为使用递归，所以这里要把 T... 写成 T1, T...
	//{
	//	//注意下面的三个语句是有顺序的
	//	using _first = F<T1>;                     //_开头表示提示调用方这个不对外开放，相当于private，这里因为语法原因不能真写private
	//	using _rest = mp_transform<F, L<T...>>;   //递归开始 注意用的 mp_transform 而不是 mp_transform_impl，因为mp_transform是类型列表的别名，而mp_transform_impl::type才是类型列表的别名
	//	using type = mp_push_front<_rest, _first>;
	//};


	//利用参数包扩展(pack expansion)，不需要 mp_push_front 就可以实现 mp_transform   (好，C++11写法，使用参数包扩展(pack expansion))
	template<template<class...> class F, class L>
	struct mp_transform_impl;

	template<template<class...> class F, template<class...> class L, class... T>
	struct mp_transform_impl<F, L<T...>>
	{
		using type = L<F<T>...>;
	};

	template<template<class...> class F, class L>
	using mp_transform = typename mp_transform_impl<F, L>::type;




	//使用 mp_transform 举例：

	//F:
	template<class T>
	using add_pointer = T*;         //旧写法是struct中定义type，不过别名模板不支持特化/偏特化

	//L:
	using input = std::tuple<int, void, float>;
	
	//预期结果：
	using expected = std::tuple<int*, void*, float*>;

	//计算：
	using result = mp_transform<add_pointer, input>;
	static_assert(std::is_same<result, expected>::value, "");


	//注意！！！上面的两种 mp_transform 实现有一个区别：
	//对于 mp_transform<add_pointer, std::pair<int, float>> ，利用参数包扩展(pack expansion)的 mp_transform 可以编译通过，但利用递归的 mp_transform 不能编译通过
}

namespace ns4
{//实现多元 mp_transform
	
	template<class... T>
	struct mp_list
	{};



	//上面介绍的 mp_transform 只作用于一个类型列表：
	//输入：
	//L<T...>
	//F
	//输出：
	//L<F<T>...>

	//如果作用于两个类型列表
	//输入：
	//L<T11...>   L<T21...>
	//F
	//输出：
	//L<F<T11, T21>...>
	//具体实现：
	//template<template<class...> class F, class L1, class L2>
	//struct mp_transform2_impl;

	//template<template<class...> class F,
	//	template<class...> class L1, class... T1,
	//	template<class...> class L2, class... T2>
	//struct mp_transform2_impl<F, L1<T1...>, L2<T2...>>
	//{
	//	static_assert(sizeof...(T1) == sizeof...(T2),
	//		"The arguments of mp_transform2 should be of the same size");

	//	using type = L1<F<T1, T2>...>;
	//};

	//template<template<class...> class F, class L1, class L2>
	//using mp_transform2 = typename mp_transform2_impl<F, L1, L2>::type;

	//但如果作用于三个类型列表呢？显然这种方式麻烦，而且如果是任意多个类型列表呢？
	//正如之前说的，C++11中变长模板参数应该是默认形式，即 class... L

	template<template<class...> class F, class E, class... L>
	struct mp_transform_impl;

	template<template<class...> class F, class... L>
	using mp_transform = typename mp_transform_impl<F, boost::mp11::mp_empty<mp_list<L...>>, L...>::type;

	template<template<class...> class F, class L1, class... L>
	struct mp_transform_impl<F, boost::mp11::mp_true, L1, L...>
	{
		using type = boost::mp11::mp_clear<L1>;
	};

	template<template<class...> class F, class... L>
	struct mp_transform_impl<F, boost::mp11::mp_false, L...>
	{
		using _first = F<boost::mp11::mp_front<L>...>;
		using _rest = mp_transform<F, boost::mp11::mp_pop_front<L>...>;
		using type = boost::mp11::mp_push_front<_rest, _first>;
	};

	//mp_true - an alias for std::integral_constant<bool, true>
	//mp_false - an alias for std::integral_constant<bool, false>
	//mp_empty<L...> - returns mp_true if all lists are empty, mp_false otherwise.
	//mp_clear<L> - returns an empty list of the same type as L.
	//mp_front<L> - returns the first element of L.
	//mp_pop_front<L> - returns L without its first element.
}

namespace ns5
{//实现 tuple_cat


	//之前定义的先列在这里
	template<class... T>
	struct mp_list
	{};
	template<template<class...> class F, class E, class... L>
	struct mp_transform_impl;

	template<template<class...> class F, class... L>
	using mp_transform = typename mp_transform_impl<F, boost::mp11::mp_empty<mp_list<L...>>, L...>::type;

	template<template<class...> class F, class L1, class... L>
	struct mp_transform_impl<F, boost::mp11::mp_true, L1, L...>
	{
		using type = boost::mp11::mp_clear<L1>;
	};

	template<template<class...> class F, class... L>
	struct mp_transform_impl<F, boost::mp11::mp_false, L...>
	{
		using _first = F<boost::mp11::mp_front<L>...>;
		using _rest = mp_transform<F, boost::mp11::mp_pop_front<L>...>;
		using type = boost::mp11::mp_push_front<_rest, _first>;
	};












	//tuple_cat的功能：
	//输入：(若干个类型列表实例对应运行时实例)
	//std::tuple<int, short, long> t1;
	//std::tuple<> t2;
	//std::tuple<float, double, long double> t3;
	//std::tuple<void*, char*> t4;
	//计算：
	//auto res = tuple_cat(t1, t2, t3, t4);
	//输出：
	//std::tuple<std::tuple<int, short, long>, std::tuple<>, std::tuple<float, double, long double>, std::tuple<void*, char*>> t{ t1, t2, t3, t4 };
	//获取t中的元素：
	//std::get<0>(std::get<0>(t)) //t1[0]
	//std::get<1>(std::get<0>(t)) //t1[1]
	//std::get<2>(std::get<0>(t)) //t1[2]
	//std::get<0>(std::get<2>(t)) //t3[0]
	//std::get<1>(std::get<2>(t)) //t3[1]
	//std::get<2>(std::get<2>(t)) //t3[2]
	//std::get<0>(std::get<3>(t)) //t4[0]
	//std::get<1>(std::get<3>(t)) //t4[1]
	//(t2是空的，所以我们无法从其中获取任何东西)

	//这里将其中涉及的元函数命名为 mp_append ，下面介绍其如何实现，进而实现tuple_cat函数






	// mp_append 实现：
	template<class... L>
	struct mp_append_impl;

	template<class... L>
	using mp_append = typename mp_append_impl<L...>::type;

	template<>
	struct mp_append_impl<>                               //对应输入参数是0个类型列表的情况  没有输入类型列表，于是返回 mp_list<>
	{
		using type = mp_list<>;
	};

	template<template<class...> class L, class... T>
	struct mp_append_impl<L<T...>>                        //对应输入参数是1个类型列表的情况
	{
		using type = L<T...>;
	};

	template<template<class...> class L1, class... T1, template<class...> class L2, class... T2, class... Lr>  //注意，可以这样写，多个变长模板参数
	struct mp_append_impl<L1<T1...>, L2<T2...>, Lr...>    //对应输入参数是2个或2个以上类型列表的情况    L2被舍弃，使用L1
	{
		using type = mp_append<L1<T1..., T2...>, Lr...>;
	};

	//以上实现这是多种实现中的一种，也是利用C++11最普遍的实现方式，利用偏特化实现编译期if-else，注意没有递归

	//进而实现tuple_cat
	//template<class... Tp, class R = mp_append<typename std::remove_reference<Tp>::type...>>
	//R tuple_cat(Tp&&... tp)
	//{
	//	//待实现
	//}

	//因为输入是利用完美转发的万能引用(Tp&&)，所以Tp有可能是std::tuple<int, short, long>(当实参是右值时(tuple后面的int等只是举例，下面同理))或者std::tuple<int, short, long>&(当实参是左值时)这里
	//用std::remove_reference处理他，返回std::tuple<int, short, long>，因为 mp_append 无法将std::tuple<int, short, long>&(引用类型)看作类型列表




	//上面的 tuple_cat 实现有缺陷
	//缺陷1：当输入参数为空时，返回 mp_list<> ，但正确结果是 std::tuple<>
	//缺陷2：当第一个输入参数为 std::pair 时，mp_append 会向其加入元素导致其超出2个，进而导致编译失败 (参考对应输入参数是2个或2个以上类型列表的情况，L1是std::pair)

	//这两个缺陷都可以通过如下方式解决，将 std::tuple<> 作为 mp_append 的第一个参数 (这时 mp_append 就没有对应输入参数是0个类型列表的情况)
	namespace tuple_cat_ns1
	{
		template<class... Tp, class R = mp_append<std::tuple<>, typename std::remove_reference<Tp>::type...>>
		R tuple_cat(Tp&&... tp)
		{
			//待实现
		}
	}





	//下面开始实现tuple_cat的内部

	//返回类型已经确定了，回顾一下前面的：
	//t的类型：std::tuple<std::tuple<int, short, long>, std::tuple<>, std::tuple<float, double, long double>, std::tuple<void*, char*>>
	//获取t中的元素：
	//std::get<0>(std::get<0>(t)) //t1[0]
	//std::get<1>(std::get<0>(t)) //t1[1]
	//std::get<2>(std::get<0>(t)) //t1[2]
	//std::get<0>(std::get<2>(t)) //t3[0]
	//std::get<1>(std::get<2>(t)) //t3[1]
	//std::get<2>(std::get<2>(t)) //t3[2]
	//std::get<0>(std::get<3>(t)) //t4[0]
	//std::get<1>(std::get<3>(t)) //t4[1]

	//内部索引：(inner)
	//[0, 0, 0, 2, 2, 2, 3, 3]
	//外部索引：(outer)
	//[0, 1, 2, 0, 1, 2, 0, 1]

	//所以，只要在 tuple_cat 实现中获取这两套索引即可，编译期整型常量可以用std::integral_constant封装成类型，从而可以看作类型列表

	//1.首先分析内部索引：(inner)

	//内部索引可以看作为一个类型列表，进而看作若干个类型列表组成的类型列表
	//[[0, 0, 0], [], [2, 2, 2], [3, 3]]

	//输入参数为若干个类型列表
	//[x1, x2, x3] [] [y1, y2, y3] [z1, z2]
	//可以组合成若干个类型列表组成的类型列表（很简单，使用 mp_list 就可以）
	//[[x1, x2, x3], [], [y1, y2, y3], [z1, z2]]

	//(重点)
	//为了获得内部索引，需要一个索引组成的类型列表
	//[0, 1, 2, 3]
	//然后需要一个元函数 mp_fill ，他使用 mp_transform 并接受两个类型列表 [[x1, x2, x3], [], [y1, y2, y3], [z1, z2]] 和 [0, 1, 2, 3]，还接受一个元函数，这个元函数通过 mp_transform 获取[x1, x2, x3]和0作为作为输入参数，返回0，对应[x1, x2, x3]在 [[x1, x2, x3], [], [y1, y2, y3], [z1, z2]] 中的位置，依此类推

	//mp_file的简短实现  (代码少，但不容易看懂)
	namespace mp_file_short
	{
		template<class V>
		struct mp_constant
		{
			template<class...>
			using apply = V;
		};
		template<class L, class V>
		using mp_fill = mp_transform<mp_constant<V>::template apply, L>;
	}

	//mp_file的常规实现  (大众写法，容易看懂)
	namespace mp_file_normal
	{
		template<class L, class V>
		struct mp_fill_impl;

		template<template<class...> class L, class... T, class V>
		struct mp_fill_impl<L<T...>, V>
		{
			template<class...> using _fv = V;
			using type = L<_fv<T>...>;
		};

		template<class L, class V>
		using mp_fill = typename mp_fill_impl<L, V>::type;
	}

	//使用哪种实现都行

	//然后就是获取[0, 1, 2, 3]，可以实现一个元函数 mp_iota，但是C++14已经提供了生成索引序列的方式：std::make_index_sequence，所以这里我们用C++11实现它

	template<class T, T... Ints>
	struct integer_sequence
	{};

	template<class S>
	struct next_integer_sequence;

	template<class T, T... Ints>
	struct next_integer_sequence<integer_sequence<T, Ints...>>
	{
		using type = integer_sequence<T, Ints..., sizeof...(Ints)>;
	};

	template<class T, T I, T N>
	struct make_int_seq_impl;

	template<class T, T N>
	using make_integer_sequence = typename make_int_seq_impl<T, 0, N>::type;

	template<class T, T I, T N>
	struct make_int_seq_impl
	{
		using type = typename next_integer_sequence<typename make_int_seq_impl<T, I + 1, N>::type>::type;
	};
	template<class T, T N>
	struct make_int_seq_impl<T, N, N>
	{
		using type = integer_sequence<T>;
	};
	template<std::size_t... Ints>
	using index_sequence = integer_sequence<std::size_t, Ints...>;
	template<std::size_t N>
	using make_index_sequence = make_integer_sequence<std::size_t, N>;


	//使用
	//std::size_t const N = sizeof...(Tp);
	//using seq = make_index_sequence<N>;

	//注意seq是 integer_sequence<std::size_t, 0, 1, 2, 3> ，他不是一个类型列表，所以需要用 mp_from_sequence 转为类型列表

	// mp_from_sequence 的实现
	template<class S>
	struct mp_from_sequence_impl;
	template<template<class T, T... I> class S, class U, U... J>
	struct mp_from_sequence_impl<S<U, J...>>
	{
		using type = mp_list<std::integral_constant<U, J>...>;
	};
	template<class S>
	using mp_from_sequence = typename mp_from_sequence_impl<S>::type;








	////总结计算内部索引的实现:
	//std::size_t const N = sizeof...(Tp);

	//using list1 = mp_list<typename std::remove_reference<Tp>::type...>;
	//using list2 = mp_from_sequence<make_index_sequence<N>>;

	////list1: [[x1, x2, x3], [], [y1, y2, y3], [z1, z2]]
	////list2：[0, 1, 2, 3]

	//using list3 = mp_transform<mp_fill, list1, list2>;

	////list3：[[0, 0, 0], [], [2, 2, 2], [3, 3]]

	//using inner = mp_rename<list3, mp_append>;      //or mp_apply<mp_append, list3>

	////inner：[0, 0, 0, 2, 2, 2, 3, 3]


	//2.然后分析外部索引：(outer)

	//外部索引可以看作为一个类型列表，进而看作若干个类型列表组成的类型列表
	//[[0, 1, 2], [], [0, 1, 2], [0, 1]]

	//输入参数为若干个类型列表
	//[x1, x2, x3] [] [y1, y2, y3] [z1, z2]
	//可以组合成若干个类型列表组成的类型列表（很简单，使用 mp_list 就可以）
	//[[x1, x2, x3], [], [y1, y2, y3], [z1, z2]]

	//这个跟之前不同之处是使用递增值填充而不是常量值填充              (不管什么值，都是编译期常量值)
	//仅需要定义一个元函数F来实现即可
	template<class N>
	using mp_iota = mp_from_sequence<make_index_sequence<N::value>>;
	template<class L>
	using F = mp_iota<boost::mp11::mp_size<L>>;

	////总结计算外部索引的实现:
	//std::size_t const N = sizeof...(Tp);

	//using list1 = mp_list<typename std::remove_reference<Tp>::type...>;
	//using list2 = mp_transform<F, list1>;  //注意这里

	////list1: [[x1, x2, x3], [], [y1, y2, y3], [z1, z2]]
	////list2：[[0, 1, 2], [], [0, 1, 2], [0, 1]]

	//using outer = mp_rename<list2, mp_append>;

	////outer：[0, 1, 2, 0, 1, 2, 0, 1]

	//总结 tuple_cat 的完整实现
	namespace tuple_cat_complete
	{
		template<class N>
		using mp_iota = mp_from_sequence<make_index_sequence<N::value>>;

		template<class L>
		using F = mp_iota<boost::mp11::mp_size<L>>;

		template<class R, class... Is, class... Ks, class Tp>
		R tuple_cat_(mp_list<Is...>, mp_list<Ks...>, Tp tp)
		{
			return R{ std::get<Ks::value>(std::get<Is::value>(tp))... };
		}

		template<class... Tp, class R = mp_append<std::tuple<>, typename std::remove_reference<Tp>::type...>>
		R tuple_cat(Tp&&... tp)
		{
			std::size_t const N = sizeof...(Tp);

			//inner

			using list1 = mp_list<typename std::remove_reference<Tp>::type...>;
			using list2 = mp_from_sequence<make_index_sequence<N>>;

			//list1:[[x1, x2, x3], [], [y1, y2, y3], [z1, z2]]
			//list2:[0, 1, 2, 3]

			using list3 = mp_transform<boost::mp11::mp_fill, list1, list2>;

			//list3:[[0, 0, 0], [], [2, 2, 2], [3, 3]]

			//using inner = mp_rename<list3, mp_append>;     // or mp_apply<mp_append, list3>
			//如果下面的t1改为:
			//std::pair<int, long> t1{1, 2};
			//那么会报错，因为list3的第一个元素是std::pair，导致使用 mp_append时失败
			//正确为在list3前面插入一个空的mp_list：
			using inner = boost::mp11::mp_rename<boost::mp11::mp_push_front<list3, boost::mp11::mp_list<>>, mp_append>;
			//或者仍是using inner = mp_rename<list3, mp_append>;
			//然后修改list1的定义为：
			//using list1 = mp_list<mp_rename<typename std::remove_reference<Tp>::type, mp_list>...>;
			//即将所有输入转为mp_list

			//inner:[0, 0, 0, 2, 2, 2, 3, 3]

			//outer

			using list4 = mp_transform<F, list1>;

			//list4:[[0, 1, 2], [], [0, 1, 2], [0, 1]]

			using outer = boost::mp11::mp_rename<list4, boost::mp11::mp_append>;

			//outer: [0, 1, 2, 0, 1, 2, 0, 1]

			return tuple_cat_<R>(mp_rename<inner, mp_list>(), outer(), std::forward_as_tuple(std::forward<Tp>(tp)...));
			//inner 是 std::tuple，所以需要转换为 mp_list
		}




		//定义print_tuple函数检查正确性
		template<int I, int N, class... T>
		struct print_tuple_
		{
			void operator()(std::tuple<T...> const& tp) const
			{
				using Tp = typename std::tuple_element<I, std::tuple<T...>>::type;

				print_type<Tp>(" ", ": ");

				std::cout << std::get<I>(tp) << ";";

				print_tuple_<I + 1, N, T...>()(tp);
			}
		};
		template<int N, class... T>
		struct print_tuple_<N, N, T...>
		{
			void operator()(std::tuple<T...> const&) const
			{}
		};
		template<class... T>
		void print_tuple(std::tuple<T...> const& tp)
		{
			std::cout << "{";
			print_tuple_ < 0, sizeof...(T), T... >()(tp);
			std::cout << " }\n";
		}

		//int func()
		//{
		//	std::tuple<int, long> t1{ 1,2 };
		//	std::tuple<> t2;
		//	std::tuple<float, double, long double> t3{ 3, 4, 5 };
		//	std::pair<void const*, char const*> t4{ "pv", "test" };

		//	using expected = std::tuple<int, long, float, double, long double, void const*, char const*>;

		//	auto result = tuple_cat(t1, t2, t3, t4);

		//	static_assert(std::is_same<decltype(result), expected>::value, "");

		//	print_tuple(result);
		//}
	}
}

namespace ns6
{//之前讨论的都相对简单，因为没有进行任何高阶元编程，即没有引入任何返回元函数的元函数

	//在大多数情况下，这种高阶元编程在C++11中是不必要的

	//比如:
	//using outer =
	//	typelist_cat_t<
	//	typelist_transform_t<
	//	typelist<as_typelist_t<Tuples>...>,
	//	meta_compose<
	//	meta_quote<as_typelist_t>,
	//	meta_quote_i<std::size_t, make_index_sequence>,
	//	meta_quote<typelist_size_t> > > >;

	//The meta_compose expression takes three other ("quoted") metafunctions and creates 
	//a new metafunction that applies them in order. Eric uses this example as motivation 
	//to introduce the concept of a "metafunction class" and then to supply various primitives
	//that operate on metafunction classes.

	//如果有元函数F、G和H，那么在C++11中的替代meta_compose的方案是
	//template<class... T>
	//using Fgh = F<G<H<T...>>>;
	//语言特性使复合函数的定义更加容易

	//类似的，如果需要一个元函数，他返回sizeof(T) < sizeof(U)
	//不需要使用元编程lambda库
	//lambda<_a, _b, less<sizeof_<_a>, sizeof_<_b>>>
	//可以直接定义为
	//template<class T, class U>
	//using sizeof_less = mp_bool<(sizeof(T) < sizeof(U))>;
}

namespace ns7
{//实现 mp_count<L, V> 和 mp_count_if<L, P>

	//首先实现 mp_plus

	template<class... T>
	struct mp_plus_impl;

	template<class... T>
	using mp_plus = typename mp_plus_impl<T...>::type;

	template<>
	struct mp_plus_impl<>
	{
		using type = std::integral_constant<int, 0>;
	};

	template<class T1, class... T>
	struct mp_plus_impl<T1, T...>
	{
		static constexpr auto _v = T1::value + mp_plus<T...>::value;    //C++17支持 T1::value + ...

		using type = std::integral_constant<typename std::remove_const<decltype(_v)>::type, _v>;
	};


	//实现 mp_count

	template<class L, class V>
	struct mp_count_impl;
	template<template<class...> class L, class... T, class V>
	struct mp_count_impl<L<T...>, V>
	{
		using type = mp_plus<std::is_same<T, V>...>;
	};
	template<class L, class V>
	using mp_count = typename mp_count_impl<L, V>::type;

	//实现 mp_count_if

	//template<class L, template<class...> class P>
	//struct mp_count_if_impl;
	//template<template<class...> class L, class... T, template<class...> class P>
	//struct mp_count_if_impl<L<T...>, P>
	//{
	//	using type = mp_plus<P<T>...>;
	//};
	//template<class L, template<class...> class P>
	//using mp_count_if = typename mp_count_if_impl<L, P>::type;

	//至少要求P可以转为bool，要不就强制P<T>::value转为0或者1，否则返回的计数不正确

	template<bool v>
	using mp_bool = std::integral_constant<bool, v>;
	template<class L, template<class...> class P>
	struct mp_count_if_impl;
	template<template<class...> class L, class... T, template<class...> class P>
	struct mp_count_if_impl<L<T...>, P>
	{
		using type = mp_plus<mp_bool<P<T>::value != 0>...>;
	};
	template<class L, template<class...> class P>
	using mp_count_if = typename mp_count_if_impl<L, P>::type;


	//实现 mp_contains

	//template<class L, class V>
	//using mp_contains = mp_bool<mp_count<L, V>::value != 0>;

	//这个实现简单，但是低效，只需要关注布尔返回值，却计算每一个元素的个数
	//mp_contains的实现不是问题，问题在于mp_contains和mp_count所依赖的mp_plus，只需要稍微优化一下他

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class... T>
	struct mp_plus_impl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T...>
	{
		static constexpr auto _v = T1::value + T2::value + T3::value + T4::value + T5::value + T6::value + T7::value + T8::value
			+ T9::value + T10::value + mp_plus<T...>::value;

		using type = std::integral_constant<typename std::remove_const<decltype(_v)>::type, _v>;
	};
	//这减少了模板实例化数量至少10倍
}

namespace ns8
{//vector、set和map

	//在STL中，除了list，还有vector、set和map
	//vector提供了按索引随机访问的能力
	//set提供了高效的成员测试
	//map提供了键值结构和基于键的高效搜索

	//不提供其他单独的数据成员，如mp_vector、mp_set和mp_map。而是为list提供按索引随机访问的能力、高效的成员测试和提供了键值结构和基于键的高效搜索的算法
}

namespace ns9
{
	//set是只有唯一值的list，为了获得set，需要一个移除重复元素的元函数 mp_unique<L>

	//mp_if
	template<bool C, class T, class E>
	struct mp_if_c_impl;
	template<class T, class E>
	struct mp_if_c_impl<true, T, E>
	{
		using type = T;
	};
	template<class T, class E>
	struct mp_if_c_impl<false, T, E>
	{
		using type = E;
	};
	template<bool C, class T, class E>
	using mp_if_c = typename mp_if_c_impl<C, T, E>::type;
	template<class C, class T, class E>
	using mp_if = typename mp_if_c_impl<C::value != 0, T, E>::type;

	//mp_unique
	template<class L>
	struct mp_unique_impl;
	template<class L>
	using mp_unique = typename mp_unique_impl<L>::type;
	template<template<class...> class L>
	struct mp_unique_impl<L<>>
	{
		using type = L<>;
	};
	template<template<class...> class L, class T1, class... T>
	struct mp_unique_impl<L<T1, T...>>
	{
		using _rest = mp_unique<L<T...>>;
		using type = mp_if<boost::mp11::mp_contains<_rest, T1>, _rest, boost::mp11::mp_push_front<_rest, T1>>;
	};

	//可见这个实现是基于 mp_contains 实现的


	///mp_contains 的直白递归实现：(性能不高)
	template<class L, class V>
	struct mp_contains_impl;
	template<class L, class V>
	using mp_contains = typename mp_contains_impl<L, V>::type;
	template<template<class...> class L, class V>
	struct mp_contains_impl<L<>, V>
	{
		using type = std::false_type;
	};
	template<template<class...> class L, class... T, class V>
	struct mp_contains_impl<L<V, T...>, V>
	{
		using type = std::true_type;
	};
	template<template<class...> class L, class T1, class... T, class V>
	struct mp_contains_impl<L<T1, T...>, V> : mp_contains_impl<L<T...>, V>
	{};

	// mp_unique<L> 会调用 mp_contains N次，N是列表L的元素个数，所以 mp_contains 效率要高，但是上面实现的效率并不高

	//回想C++11标准中规定包扩展能够在下面上下文中发生：
	//(重点)In a function parameter pack(8.3.5); the pattern is the parameter - declaration without the ellipsis.
	//In a template parameter pack that is a pack expansion(14.1) :
	//(重点)In an initializer - list(8.5); the pattern is an initializer - clause.
	//(重点)In a base - specifier - list(Clause 10); the pattern is a base - specifier.
	//In a mem - initializer - list(12.6.2); the pattern is a mem - initializer.
	//In a template - argument - list(14.3); the pattern is a template - argument.
	//In a dynamic - exception - specification(15.4); the pattern is a type - id.
	//In an attribute - list(7.6.1); the pattern is an attribute.
	//In an alignment - specifier(7.6.2); the pattern is the alignment - specifier without the ellipsis.
	//In a capture - list(5.1.2); the pattern is a capture.
	//In a sizeof... expression(5.3.3); the pattern is an identifier.

	//第一个选择是将参数包扩展为函数调用的参数，因为C++11函数可以是constexpr，所以这样的函数可以在编译期调用

	//回顾一下，之前 mp_count 的定义

	//template<class L, class V>
	//struct mp_count_impl;
	//template<template<class...> class L, class... T, class V>
	//struct mp_count_impl<L<T...>, V>
	//{
	//	using type = mp_plus<std::is_same<T, V>...>;
	//};
	//template<class L, class V>
	//using mp_count = typename mp_count_impl<L, V>::type;



	//除了使用模板别名 mp_plus 来求和 is_same 表达式，我们还可以使用一个constexpr函数：
	namespace pack_expansions_by_function_parameter
	{
		constexpr std::size_t cx_plus()
		{
			return 0;
		}

		template<class T1, class... T>
		constexpr std::size_t cx_plus(T1 t1, T... t)
		{
			return t1 + cx_plus(t...);   //递归
		}
		//mp_size_t
		template<std::size_t N>
		using mp_size_t = std::integral_constant<std::size_t, N>;
		//mp_count
		template<class L, class V>
		struct mp_count_impl;
		template<template<class...> class L, class... T, class V>
		struct mp_count_impl<L<T...>, V>
		{
			using type = mp_size_t<cx_plus(std::is_same<T, V>::value...)>;    //包扩展在函数实参
		};
		template<class L, class V>
		using mp_count = typename mp_count_impl<L, V>::type;
	}
	//比之前性能提升很多

	//这次尝试将包扩展用到初始值设定项列表(initializer-list)中，不在传递 is_same 表达式到函数，而是用他们构建一个常量数组，然后用一个constexpr函数对这个数组求和
	namespace pack_expansions_by_initializer_list
	{
		constexpr std::size_t cx_plus2(bool const* first, bool const* last)
		{
			return first == last ? 0 : *first + cx_plus2(first + 1, last);   //这里利用了bool类型隐式转为int类型，true对应1，false对应0
		}
		//mp_count
		template<class L, class V>
		struct mp_count_impl;
		template<template<class...> class L, class... T, class V>
		struct mp_count_impl<L<T...>, V>
		{
			static constexpr bool _v[] = { std::is_same<T, V>::value... };
			using type = boost::mp11::mp_size_t<cx_plus2(_v, _v + sizeof...(T))>;
		};
		template<class L, class V>
		using mp_count = typename mp_count_impl<L, V>::type;
	}
	//比之前技巧性强一些，但性能几乎和上一个一样

	//这次包扩展在基类说明列表中
	//struct U:T...{};
	//然后可以使用std::is_base_of<V, U>来测试一个类型V是否是类型U的基类，也就是说是否是包扩展中的一个元素
	//注意，void、int和void(int)等不能作为基类类型，但可以使用 mp_identity 将他们封装到空类模板中
	namespace pack_expansions_by_base_specifier_list
	{
		template<class T>
		struct mp_identity
		{
			using type = T;
		};
		template<class L, class V>
		struct mp_contains_impl;
		template<class L, class V>
		using mp_contains = typename mp_contains_impl<L, V>::type;
		template<template<class...> class L, class... T, class V>
		struct mp_contains_impl<L<T...>, V>
		{
			struct U : mp_identity<T>...{};   //这里要求T...是无重复的
			using type = std::is_base_of<mp_identity<V>, U>;
		};
	}
	//这个的性能最佳

	//公平地说，mp_contains 的前四个实现并不依赖类型列表元素的唯一性，即他支持任意的类型列表，不仅仅是 set
	//然而，is_base_of 的实现不支持包含重复元素的类型列表，即一个类型不可能直接继承相同类型多次，所以上面实现
	//的 mp_contains 不是通用的 mp_contains，应该起名叫 mp_set_contains

	//可以通过修改实现，通过中间基类，间接地从 mp_identity 继承，从而避免“无重复”的要求
	namespace pack_expansions_by_base_specifier_list_and_indirect_inherit
	{
		template<std::size_t I, class T>
		struct inherit_second : T
		{};
		template<class L, class S>
		struct indirect_inherit_impl;
		template<template<class...> class L, class... T, std::size_t... J>
		struct indirect_inherit_impl<L<T...>, std::integer_sequence<std::size_t, J...>> : inherit_second<J, boost::mp11::mp_identity<T>>...
		{};
		template<class L>
		using indirect_inherit = indirect_inherit_impl<L, std::make_index_sequence<boost::mp11::mp_size<L>::value>>;
		//mp_contains
		template<class L, class V>
		struct mp_contains_impl
		{
			using U = indirect_inherit<L>;
			using type = std::is_base_of<boost::mp11::mp_identity<V>, U>;
		};
		template<class L, class V>
		using mp_contains = typename mp_contains_impl<L, V>::type;
	}
	//这样 mp_contains 就是通用的了，但是性能又打回去了
}

namespace ns10
{//mp_map_find

	//这里定义 map 是元素为类型列表(每个元素中至少有一个元素作为key，其余的认为是关联的value)的类型列表，
	//比如：
	//[[A, B], [C, D, E], [F], [G, H]]
	//键为A,C,F,G   (键应该是唯一的)
	//值为[B],[D,E],[],[H]

	//using Map = mp_list<mp_list<int, int*>, mp_list<void, void*>, mp_list<char, char*>>;
	//using Map2 = std::tuple<std::pair<int, int[2]>, std::pair<char, char[2]>>;

	//mp_map_find<M,K>执行基于键的查找，查找M中第一个键为K的元素
	//比如mp_map_find<Map2, int>将返回std::pair<int, int[2]>

	//mp_map_find 的递归版本，他的性能很差
	namespace mp_map_find_recursive
	{
		template<class M, class K>
		struct mp_map_find_impl;
		template<class M, class K>
		using mp_map_find = typename mp_map_find_impl<M, K>::type;
		template<template<class...> class M, class K>
		struct mp_map_find_impl<M<>, K>
		{
			using type = void;
		};
		template<template<class...> class M, class T1, class... T, class K>
		struct mp_map_find_impl<M<T1, T...>, K>
		{
			using type = boost::mp11::mp_if<std::is_same<boost::mp11::mp_front<T1>, K>, T1, mp_map_find<M<T...>, K>>;
		};
	}
	//性能很差

	//另外这个实现还可以优化，因为if条件即使为true，也会评估(evaluate)else分支，这是非常低效的。优化后性能平均提高大约两倍，而且只有在元素存在的情况下
	namespace mp_map_find_recursive_and_eval_if
	{
		//mp_eval_if
		template<bool C, class T, template<class...> class E, class... A>
		struct mp_eval_if_c_impl;
		template<class T, template<class...> class E, class... A>
		struct mp_eval_if_c_impl<true, T, E, A...>
		{
			using type = T;
		};
		template<class T, template<class...> class E, class... A>
		struct mp_eval_if_c_impl<false, T, E, A...>
		{
			using type = E<A...>;
		};
		template<bool C, class T, template<class...> class E, class... A>
		using mp_eval_if_c = typename mp_eval_if_c_impl<C, T, E, A...>::type;
		template<class C, class T, template<class...> class E, class... A>
		using mp_eval_if = typename mp_eval_if_c_impl<C::value != 0, T, E, A...>::type;

		//mp_map_find
		template<class M, class K>
		struct mp_map_find_impl;
		template<class M, class K>
		using mp_map_find = typename mp_map_find_impl<M, K>::type;
		template<template<class...> class M, class K>
		struct mp_map_find_impl<M<>, K>
		{
			using type = void;
		};
		template<template<class...> class M, class T1, class... T, class K>
		struct mp_map_find_impl<M<T1, T...>, K>
		{
			using type = mp_eval_if<std::is_same<boost::mp11::mp_front<T1>, K>, T1, mp_map_find, M<T...>, K>;
		};
	}

	//不管有无意义，证明递归实现是低效的并不等同于提出一个高效的实现，有两件事使得之前 mp_contains 讨论的技术不适用于我们目前的情况：
	//1. mp_contains 仅返回true或者false，而 mp_map_find 返回一个类型
	//2.在 mp_contains 中，我们知道要查找的元素的确切类型，而这里，我们只知道他的 mp_front

	//幸运的是，存在一种语言特性，可以解决这两个问题：C++在传递派生类时，可以推断基类的模板参数，比如：
	//struct K1 {};
	//struct V1 {};

	//struct X : std::pair<K1, V1> {};

	//template<class A, class B>
	//void f(std::pair<A, B> const& p);

	//int main()
	//{
	//	f(X());
	//}
	//f(X())的调用推断A为K1，推断B为V1

	//如果有多个std::pair基类，那么可以将A固定为K1
	//struct K1 {};
	//struct V1 {};

	//struct K2 {};
	//struct V2 {};

	//struct X : std::pair<K1, V1>, std::pair<K2, V2> {};

	//template<class B>
	//void f(std::pair<K1, B> const& p);

	//int main()
	//{
	//	f(X());
	//}
	//推断B为V1



	//通过将函数的返回值类型定义为推导类型，可以获得推导结果，比如：
	//template<class B> std::pair<K1, B> f(std::pair<K1, B> const& p);
	//然后使用 decltype(f(X())) 获得这个返回值类型

	//注意，如果X没有类型为std::pair<K1, B>的基类，那么推导将失败并且提示f(X())无法调用的错误，为了避免这个错误，添加一个f函数的重载函数，他可以接受任意参数
	//并返回void，但是这样修改后，如果X有两个基类std::pair<K1, Y>和std::pair<K1, Z>会怎么样？推导会失败，第二个重载f函数被调用并返回void，这不是我们想要的
	//这就是为什么要求 map 必须是无重复键

	//下面以这种技术实现 mp_map_find
	namespace mp_map_find_deduction
	{
		template<class M, class K>
		struct mp_map_find_impl;

		template<class M, class K>
		using mp_map_find = typename mp_map_find_impl<M, K>::type;

		template<template<class...> class M, class... T, class K>
		struct mp_map_find_impl<M<T...>, K>
		{
			struct U : boost::mp11::mp_identity<T>...{};

			template<template<class...> class L, class... U>
			static boost::mp11::mp_identity<L<K, U...>> f(boost::mp11::mp_identity<L<K, U...>>*);

			static boost::mp11::mp_identity<void> f(...);

			using V = decltype(f((U*)0));

			using type = typename V::type;
		};
	}
	//性能很好

	//这个实现还有优化的地方，当使用mp_map_find<M, K1>后再使用mp_map_find<M, K2>时，两个嵌套的U类型会被各自独立实例化，但这并不应该，因为他只依赖于M
	//所以应该将这个类型移到mp_map_find_impl之外，以便可以重用他
	namespace mp_map_find_deduction_reuse_U
	{
		template<class... T>
		struct mp_inherit : T...{};  //
		template<class M, class K>
		struct mp_map_find_impl;
		template<class M, class K>
		using mp_map_find = typename mp_map_find_impl<M, K>::type;
		template<template<class...> class M, class... T, class K>
		struct mp_map_find_impl<M<T...>, K>
		{
			using U = mp_inherit<boost::mp11::mp_identity<T>...>; //

			template<template<class...> class L, class... U>
			static boost::mp11::mp_identity<L<K, U...>> f(boost::mp11::mp_identity<L<K, U...>>*);

			static boost::mp11::mp_identity<void> f(...);

			using V = decltype(f((U*)0));

			using type = typename V::type;
		};
	}
	//这种实现也适用于 is_base_of 版本的 mp_contains 实现
}

namespace ns11
{//mp_at

	//有了 set 和 map，现在该 vector 了，vector实际就是可以按索引高效访问元素的类型列表，相关的元函数是 mp_at<L, I>  (I是std::integral_constant) mp_at_c<L, I> (I是size_t)

	//mp_at 的递归实现
	namespace mp_at_recursive
	{
		template<class L, std::size_t I>
		struct mp_at_c_impl;
		template<class L, std::size_t I>
		using mp_at_c = typename mp_at_c_impl<L, I>::type;
		template<class L, class I>
		using mp_at = typename mp_at_c_impl<L, I::value>::type;
		template<template<class...> class L, class T1, class... T>
		struct mp_at_c_impl<L<T1, T...>, 0>
		{
			using type = T1;
		};
		template<template<class...> class L, class T1, class... T, std::size_t I>
		struct mp_at_c_impl<L<T1, T...>, I>
		{
			using type = mp_at_c<L<T...>, I - 1>;
		};
	}
	//性能很差

	//为了改进，再次利用包扩展进行函数调用，但这次方式很新颖
	//假设我们需要访问第四个元素(I=3)，我们将生成函数签名：
	//template<class W>
	//W f(void*, void*, void*, W*, ...);
	//然后传递给他一个类型列表：
	//L<T1, T2, T3, T4, T5, T6, T7>
	//最后评估(evaluate)表达式：
	//decltype(f((T1*)0, (T2*)0, (T3*)0, (T4*)0, (T5*)0, (T6*)0, (T7*)0))
	
	//基于此技术的实现：
	namespace mp_at_pack_expansion_function
	{
		template<class... T>
		struct mp_list
		{};

		//mp_repeat_c
		template<std::size_t N, class... T>
		struct mp_repeat_c_impl
		{
			using _l1 = typename mp_repeat_c_impl<N / 2, T...>::type;
			using _l2 = typename mp_repeat_c_impl<N % 2, T...>::type;

			using type = boost::mp11::mp_append<_l1, _l1, _l2>;
		};
		template<class... T>
		struct mp_repeat_c_impl<0, T...>
		{
			using type = mp_list<>;
		};
		template<class... T>
		struct mp_repeat_c_impl<1, T...>
		{
			using type = mp_list<T...>;
		};
		template<std::size_t N, class... T>
		using mp_repeat_c = typename mp_repeat_c_impl<N, T...>::type;

		//mp_at
		template<class L, class L2>
		struct mp_at_c_impl;
		template<template<class...> class L, class... T, template<class...> class L2, class... U>
		struct mp_at_c_impl<L<T...>, L2<U...>>
		{
			template<class W>
			static W f(U*..., W*, ...);

			using R = decltype(f((boost::mp11::mp_identity<T>*)0 ...));  //注意0后面有空格

			using type = typename R::type;
		};
		template<class L, std::size_t I>
		using map_at_c = typename mp_at_c_impl<L, mp_repeat_c<I, void>>::type;
		template<class L, class I>
		using mp_at = boost::mp11::mp_at_c<L, I::value>;
	}
	//性能提高了很多

	//再试试其他方法——将输入列表[T1, T2, T3]转换为map，即[[0, T1], [1, T2], [2, T3]]，然后使用mp_map_find进行查找：
	namespace mp_at_by_mp_map_find
	{
		//mp_map_from_list
		template<class L, class S>
		struct mp_map_from_list_impl;
		template<template<class...> class L, class... T, std::size_t... J>
		struct mp_map_from_list_impl<L<T...>, std::integer_sequence<std::size_t, J...>>
		{
			using type = boost::mp11::mp_list<boost::mp11::mp_list<boost::mp11::mp_size_t<J>, T>...>;
		};
		template<class L>
		using mp_map_from_list = typename mp_map_from_list_impl<L, std::make_index_sequence<boost::mp11::mp_size<L>::value>>::type;
		//mp_at
		template<class L, std::size_t I>
		struct mp_at_c_impl
		{
			using map = mp_map_from_list<L>;
			using type = boost::mp11::mp_second<boost::mp11::mp_map_find<map, boost::mp11::mp_size_t<I>>>;
		};
		template<class L, std::size_t I>
		using mp_at_c = typename mp_at_c_impl<L, I>::type;
		template<class L, class I>
		using mp_at = typename mp_at_c_impl<L, I::value>::type;
	}
	//这个性能最佳
}

namespace ns12
{//mp_drop

	//通过上面的分析，mp_at的实现其实不需要使用void*技巧，但在这里实现mp_drop，将使用这个技巧

	//mp_drop<L, N> 返回类型列表中除前N个元素外的其他元素组成的类型列表
	//template<class... W> mp_list<W> f(void*, void*, void*, W*...);

	//这里不在提供递归版本实现，性能太低

	//基于函数参数包扩展
	//namespace mp_drop_pack_expansion_function
	//{
	//	template<class L, class L2>
	//	struct mp_drop_c_impl;

	//	template<template<class...> class L, class... T,
	//		template<class...> class L2, class... U>
	//	struct mp_drop_c_impl<L<T...>, L2<U...>>
	//	{
	//		template<class... W>
	//		static boost::mp11::mp_identity<L<W...>> f(U*..., boost::mp11::mp_identity<W>*...);

	//		using R = decltype(f((boost::mp11::mp_identity<T>*)0 ...));

	//		using type = typename R::type;
	//	};

	//	template<class L, std::size_t N>
	//	using mp_drop_c = typename mp_drop_c_impl<L, boost::mp11::mp_repeat_c<N, void>>::type;

	//	template<class L, class N>
	//	using mp_drop = mp_drop_c<L, N::value>;
	//}
}

namespace ns13
{//mp_find_index

	//mp_find_index<L, V> returns an integral constant of type size_t with a value that is
	//the index of the first occurrence of V in L. If V is not in L, the return value is the size of L

	namespace mp_find_index_recursive
	{
		template<class L, class V>
		struct mp_find_index_impl;

		template<class L, class V>
		using mp_find_index = typename mp_find_index_impl<L, V>::type;

		template<template<class...> class L, class V>
		struct mp_find_index_impl<L<>, V>
		{
			using type = boost::mp11::mp_size_t<0>;
		};

		template<template<class...> class L, class... T, class V>
		struct mp_find_index_impl<L<V, T...>, V>
		{
			using type = boost::mp11::mp_size_t<0>;
		};

		template<template<class...> class L, class T1, class... T, class V>
		struct mp_find_index_impl<L<T1, T...>, V>
		{
			using type = boost::mp11::mp_size_t<1 + mp_find_index<L<T...>, V>::value>;
		};
	}
	//性能很差

	//回顾之前的 mp_contains 实现，找到其中的 m_count/cx_plus2 实现，他构建了一个constexpr的bool数组，并在一个constexpr函数中求和
	//在这里我们做同样的事，除了不求数组的和，可以找到第一个true值的索引
	namespace mp_find_index_constexpr_array
	{
		template<class L, class V>
		struct mp_find_index_impl;

		template<class L, class V>
		using mp_find_index = typename mp_find_index_impl<L, V>::type;

		template<template<class...> class L, class V>
		struct mp_find_index_impl<L<>, V>
		{
			using type = boost::mp11::mp_size_t<0>;
		};

		constexpr std::size_t cx_find_index(bool const* first, bool const* last)
		{
			return first == last || *first ? 0 : 1 + cx_find_index(first + 1, last);
		}

		template<template<class...> class L, class... T, class V>
		struct mp_find_index_impl<L<T...>, V>
		{
			static constexpr bool _v[] = { std::is_same<T, V>::value... };

			using type = boost::mp11::mp_size_t<cx_find_index(_v, _v + sizeof...(T))>;
		};
	}
	//性能虽然也不理想，但比递归版本好多了

	//因为vc++2013不支持constexpr函数，所以将其替换为普通元函数
	namespace mp_find_index_replace_constexpr
	{
		template<class L, class V>
		struct mp_find_index_impl;

		template<class L, class V>
		using mp_find_index = typename mp_find_index_impl<L, V>::type;

		template<template<class...> class L, class V>
		struct mp_find_index_impl<L<>, V>
		{
			using type = boost::mp11::mp_size_t<0>;
		};

		template<bool...>
		struct find_index_impl_;

		template<>
		struct find_index_impl_<>
		{
			static const std::size_t value = 0;
		};

		template<bool B1, bool... R>
		struct find_index_impl_<B1, R...>
		{
			static const std::size_t value = B1 ? 0 : 1 + find_index_impl_<R...>::value;
		};

		template<bool B1, bool B2, bool B3, bool B4, bool B5,
			bool B6, bool B7, bool B8, bool B9, bool B10, bool... R>
			struct find_index_impl_<B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, R...>
		{
			static const std::size_t value = B1 ? 0 : B2 ? 1 : B3 ? 2 : B4 ? 3 : B5 ? 4 :
				B6 ? 5 : B7 ? 6 : B8 ? 7 : B9 ? 8 : B10 ? 9 : 10 + find_index_impl_<R...>::value;
		};

		template<template<class...> class L, class... T, class V>
		struct mp_find_index_impl<L<T...>, V>
		{
			using type = boost::mp11::mp_size_t<find_index_impl_<std::is_same<T, V>::value...>::value>;
		};
	}
	//这种实现性能略差，但是不容易使编译期崩溃

	//一旦有了 mp_drop 和 mp_find_index，就可以实现 mp_find<L, V> 算法
}
