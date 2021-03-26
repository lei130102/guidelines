#pragma once

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>

#include <cstddef>  //std::size_t
#include <utility>  //std::integer_sequence

namespace test_template_custom_loop
{
	namespace loop_typelist
	{
		template<typename... Types>
		struct typelist {};

		template<std::size_t Index, typename T>
		struct loop;

		template<std::size_t Index, typename Head, typename... Tail>
		struct loop<Index, typelist<Head, Tail...>> : loop<Index - 1, typelist<Tail...>>
		{};
		//或者
		//	template<std::size_t Index, typename Head, typename... Tail>
		//	struct loop<Index, typelist<Head, Tail...>>
		//	{
		//		using type = typename loop<Index - 1, typelist<Tail...>>::type;
		//	};
		//这种方式更灵活

		template<typename Head, typename... Tail>
		struct loop<0, typelist<Head, Tail...>>   //因为每次循环都是typelist<Head, Tail...>，无法形成结束条件，所以需要借助Index实现
		{
			using type = Head;
		};
	}

	namespace loop_typelist_2
	{
		template<typename... Types>
		struct typelist {};

		template<typename T>
		struct loop;

		template<>
		struct loop<typelist<>>;

		template<typename T1>
		struct loop<typelist<T1>>
		{
			using type = T1;
		};

		template<typename T1, typename T2>
		struct loop<typelist<T1, T2>>
		{
			using type = typename T1::template pack<T2>;
		};

		template<typename... Others>
		struct loop<typelist<void, Others...>>
		{
			using type = typename loop<typelist<Others...>>::type;
		};

		template<typename T1, typename... Others>
		struct loop<typelist<T1, Others...>>
		{
			using type = typename T1::template pack<typename loop<typelist<Others...>>::type>;
		};

		//因为循环可以形成结束条件，所以不需要借助Index实现
	}

	namespace loop_intlist
	{
		template<int... Ints>
		struct intlist {};    //注意是定义不是声明，因为 func(loop<2>::type{}); 要求

		template<std::size_t Count, typename T = intlist<>>
		struct loop;

		template<std::size_t Count, int... Ints>
		struct loop<Count, intlist<Ints...>> : loop<Count - 1, intlist<Ints..., sizeof...(Ints)>>
		{};

		template<int... Ints>
		struct loop<0, intlist<Ints...>>     //因为每次循环都是intlist<Ints...>，无法形成结束条件，所以需要借助Count实现
		{
			using type = intlist<Ints...>;
		};

		////应用：2->loop<2>->intlist<0, 1>->0, 1
		template<int... Ints>
		void func(intlist<Ints...> int_list)
		{
			//这里使用萃取出的Ints...
		}
	}

	////综上，使用loop命名是不好的，应该按功能命名

	namespace intlist
	{
		//可以直接使用C++14开始有的std::integer_sequence
		template<int... Ints>
		using intlist = std::integer_sequence<int, Ints...>;

		////应用：2->std::make_integer_sequence<int, 2>->std::integer_sequence<int, 0, 1>->0, 1
		template<int... Ints>
		void func(intlist<Ints...> int_list)
		{
			//这里使用萃取出的Ints...
		}
	}
}


