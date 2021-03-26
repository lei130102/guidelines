#pragma once

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <type_traits>
#include <iostream>

namespace test_template_metafunction
{
	////元函数：在编译期被调用与执行的映射或变换(一定是无副作用的)

	//分为:
	//a.数值元函数
	//例如：
	constexpr int func(int a)    //constexpr为C++11中的关键字，表明这个函数可以在编译期被调用
	{
		return a + 1;
	}
	//b.类型元函数
	//例如：
	template<typename T>
	struct Func_
	{
		using type = T;
	};

	////元函数的操作数(参数和返回值)可以是：数值、类型和模板。他们统称为元数据

	//数值和类型很常见，这里介绍一些模板
	namespace input_template
	{
		//1.模板作为元函数的输入
		template<template<typename> class T1, typename T2>
		struct Func_
		{
			using type = typename T1<T2>::type;
		};

		//辅助函数(同样也是元函数，下同)
		template<template<typename> class T1, typename T2>
		using Func = typename Func_<T1, T2>::type;

		//使用方式
		//Func<std::remove_reference, int&> h = 3;
	}

	namespace output_template
	{
		//2.模板作为元函数的输出
		template<bool AddOrRemoveRef>
		struct Func_;

		template<>
		struct Func_<true>
		{
			template<typename T>
			using type = std::add_lvalue_reference<T>;
		};

		template<>
		struct Func_<false>
		{
			template<typename T>
			using type = std::remove_reference<T>;
		};

		//辅助函数
		template<typename T>
		template<bool AddOrRemove>
		using Func = typename Func_<AddOrRemove>::template type<T>;

		//使用方式
		//template<typename T>
		//using Res_ = Func<false>;

		//Res_<int&>::type h = 3;
	}

	//之前介绍了什么是元数据，这里进一步扩展其数组形式：数值数组、类型数组和模板数组   (注意这三种数组就可以满足大多数需求，不需要再有混合形式以增加复杂度)
	//a.数值数组
	template<int... values> struct IntContainer;                               //(C++17支持template<auto... values> struct Container;这样的容器，用于存储不同类型的数值)
	template<bool... values> struct BoolContainer;
	//b.类型数组
	template<typename... Types> struct TypeContainer;
	//c.模板数组
	template<template<typename> class... T> struct TemplateContainer;          //(C++17之前必须用class)
	template<template<typename...> class... T> struct Template2Container;

	//(以上只使用声明就可以了，不需要定义)

	////之前是数据结构的讨论，下面讨论算法，而算法最基本的是顺序、分支和循环
	//1.编译期顺序
	namespace sequential
	{
		template<typename T>
		struct RemoveReferenceConst_
		{
		private:
			using inter_type = typename std::remove_reference<T>::type;//先根据T计算出inter_type
		public:
			using type = typename std::remove_const<inter_type>::type;//再根据inter_type计算出type
		};
		//注意inter_type和type的定义顺序不能颠倒，否则因为inter_type未定义而无法编译!!

		//辅助函数
		template<typename T>
		using RemoveReferenceConst = typename RemoveReferenceConst_<T>::type;

		//使用方式
		//RemoveReferenceConst<const int&> h = 3;
	}

	//2.编译期分支   (与编译期顺序不同，编译期分支可以是纯粹的元函数，也可以与运行期的执行逻辑相结合(这时编译期分支往往用于运行期逻辑的选择))
	namespace conditional
	{
		//2.1 使用std::conditional与std::conditional_t实现编译期分支
		//优势在于使用比较简单，但缺点是表达能力不强：他只能实现二元分支(真假分支)，其行为更像运行期的问号表达式：x=B?T:F
		//对于多元分支(类似于switch功能)则支持起来就比较困难了，所以不建议使用这两个元函数

		//2.2 使用(部分)特化实现分支
		struct A;
		struct B;

		template<typename T>
		struct Func_
		{
			constexpr static size_t value = 0;
		};

		template<>
		struct Func_<A>
		{
			constexpr static size_t value = 1;
		};

		template<>
		struct Func_<B>
		{
			constexpr static size_t value = 2;
		};
		//引入了3个分支，分别对应输入参数为A、B与默认的情况，书写简单容易理解但代码一般比较长

		//使用方式
		//constexpr size_t h = Func_<B>::value;


		////C++14还可以使用变量模板实现上面，如果希望分支返回的结果是单一的数值，那么可以考虑这种方式
		namespace conditional_variable_template
		{
			struct A;
			struct B;

			template<typename T>
			constexpr size_t Func = 0;

			template<>
			constexpr size_t Func<A> = 1;

			template<>
			constexpr size_t Func<B> = 2;

			//使用方式
			//constexpr size_t h = Func<B>;
		}

		//注意：在非完全特化的类模板中引入完全特化的分支代码是非法的
		namespace conditional_error
		{
			//错误代码：
			//template<typename TW>
			//struct Wrapper
			//{
			//	//...

			//	template<typename T>
			//	struct Func_
			//	{
			//		constexpr static size_t value = 0;
			//	};

			//	template<>
			//	struct Func_<int>
			//	{
			//		constexpr static size_t value = 1;
			//	};

			//	//...
			//};
			//Wrapper是一个未完全特化的类模板，但在其内部包含了一个模板的完全特化Func_<int>，会产生编译错误

			//为解决这个问题，可以使用部分特化来代替完全特化

			//正确代码：
			template<typename TW>
			struct Wrapper
			{
				template<typename T, typename TDummy = void>
				struct Func_
				{
					constexpr static size_t value = 0;
				};

				template<typename TDummy>   //注意，默认的void也会应用到这里，也就是说这里不应该写为template<typename TDummy = void>
				struct Func_<int, TDummy>
				{
					constexpr static size_t value = 1;
				};
			};
			//引入了一个伪参数TDummy，用于将原有的完全特化修改为部分特化。这个参数有一个默认值void，这样就可直接以Func_<int>的形式
			//调用这个元函数，无需为伪参数赋值了
		}

		//2.3 使用std::enable_if与std::enable_if_t和SFINAE特性实现分支
		//对于实现分支，std::enable_if中的T并不重要，重要的是当B为true时，std::enable_if元函数可以返回结果type
		template<bool IsFeedbackOut, typename T, std::enable_if_t<IsFeedbackOut>* = nullptr>
		auto FeedbackOut_(T&&)
		{
			//...
		}
		template<bool IsFeedbackOut, typename T, std::enable_if_t<!IsFeedbackOut>* = nullptr>
		auto FeedbackOut_(T&&)
		{
			//...
		}
		//注意，只要C++中支持SFINAE的地方，都可以引入enable_if或enable_if_t，不限于上面的例子

		//这里的例子是一个典型的编译期与运行期结合的使用方式。FeedbackOut_中包含了运行期的逻辑，而选择哪个FeedbackOut_则是
		//通过编译期的分支来实现的

		//顺便说一句，当需要重载函数但又无法通过参数类型加以区分时，也可以使用std::enable_if和std::enable_if_t

		

		//编译期分支与多种返回类型

		//1.运行期分支无法有多种返回类型：
		//错误举例：
		//auto return_kinds_of_value(bool check)
		//{
		//	if (check)
		//	{
		//		return (int)0;
		//	}
		//	else
		//	{
		//		return (double)0;
		//	}
		//}
		//在C++14中，函数声明中可以不用显式指明其返回类型，编译器可以根据函数体中的return语句来自动推导其返回类型，但要求函数体中的所有return语句所返回
		//的类型均相同
		
		//2.编译期分支可以有多种返回类型：
		template<bool Check, std::enable_if_t<Check>* = nullptr>
		auto return_kinds_of_value_()
		{
			return (int)0;
		}
		template<bool Check, std::enable_if_t<!Check>* = nullptr>
		auto return_kinds_of_value_()
		{
			return (double)0;
		}
		template<bool Check>
		auto return_kinds_of_value()
		{
			return return_kinds_of_value_<Check>();
		}
		//为了执行这个函数，还需要在编译期指定模板参数值，从而将这个编译期的返回多种类型的函数蜕化为运行期的返回单一类型的函数

		//同样，这里的例子也是一个典型的编译期与运行期结合的使用方式。因此C++17专门引入了一种新的语法if constexpr来简化代码的编写
		template<bool Check>
		auto return_kinds_of_value_simple()
		{
			if constexpr (Check)   //if constexpr必须接收一个常量表达式，即编译期常量
			{//编译期分支1

				//运行期分支可以在这里实现
				return (int)0;
			}
			else
			{//编译期分支2

				//运行期分支可以在这里实现
				return (double)0;
			}
		}
		//if constexpr的优点是产生的模板实例少，从而减少编译所需要的资源以及编译产出的文件大小，缺点是使用场景相对较窄
	}

	//3.编译期循环  (因为元数据是常量、类型和模板(还有相应的数组形式)，所以一般使用递归的形式来实现循环，还需要引入一个分支来结束循环(递归))
	namespace loop
	{
		//编译期的循环，本质上是通过分支对递归代码进行控制的。因此，之前所讨论的很多分支编写方法也可以衍生并编写相应的循环代码(典型的，可以使用
		//if constexpr来编写分支)



		//例子：给定一个无符号整数，求该整数所对应的二进制表示中1的个数
		template<size_t Input>
		constexpr size_t OnesCount = (Input % 2) + OnesCount<(Input / 2)>;

		template<>
		constexpr size_t OnesCount<0> = 0;

		constexpr size_t res1 = OnesCount<45>;


		//编译期循环使用更多的一类情况则是处理数组元素
		//例子：
		template<size_t... Inputs>
		constexpr size_t Accumulate = 0;

		template<size_t CurInput, size_t... Inputs>
		constexpr size_t Accumulate<CurInput, Inputs...> = CurInput + Accumulate<Inputs...>;

		constexpr size_t res2 = Accumulate<1, 2, 3, 4, 5>;

		//此例在C++17中可以有更简单的代码编写方法，即使用其所提供的fold expression技术
		template<size_t... values>
		constexpr size_t fun()
		{
			return (0 + ... + values);
		}

		constexpr size_t res3 = fun<1, 2, 3, 4, 5>();
	}

	//将循环拆分出来解决实例爆炸
	namespace instantiation_explosion_loop_split
	{
		//实例爆炸举例：
		//template<size_t A>
		//struct Wrap_
		//{
		//	template<size_t ID, typename TDummy = void>
		//	struct imp
		//	{
		//		constexpr static size_t value = ID + imp<ID - 1>::value;
		//	};

		//	template<typename TDummy>
		//	struct imp<0, TDummy>
		//	{
		//		constexpr static size_t value = 0;
		//	};

		//	template<size_t ID>
		//	constexpr static size_t value = imp<A + ID>::value;
		//};

		//int main()
		//{
			//std::cerr << Wrap_<3>::value<2> << std::endl;
			//std::cerr << Wrap_<10>::value<2> << std::endl;
		//}
		
		//编译std::cerr << Wrap_<3>::value<2> << std::endl;时，编译期会因为这条语句产生Wrap_<3>::imp的一系列实例，不幸的是，在编译std::cerr << Wrap_<10>::value<2> << std::endl;时,
		//这与Wrap_<3>::imp系列并不同名。因此，我们无法使用编译期已经编译好的实例来提升编译速度，而且由此产生的实例将随循环层数的增加呈指数的速度增长——这些内容都会被保存在编译期中，
		//如果编译过程中保存了大量的实例，那么可能会导致编译器的内存超限，从而出现编译失败甚至崩溃的情况


		//解决：将循环拆分出来
		template<size_t ID>
		struct imp
		{
			constexpr static size_t value = ID + imp<ID - 1>::value;
		};

		template<>
		struct imp<0>
		{
			constexpr static size_t value = 0;
		};

		template<size_t A>
		struct Wrap_
		{
			template<size_t ID>
			constexpr static size_t value = imp<A + ID>::value;
		};
		//不足之处：imp污染Wrap_外围的命名空间

		//总之，权衡
	}

	//利用短路逻辑解决实例爆炸
	namespace instantiation_explosion_short_circuit_logic
	{
		//template<size_t N>
		//constexpr bool is_odd = ((N % 2) == 1);

		//template<size_t N>
		//struct AllOdd_
		//{
		//	constexpr static bool is_cur_odd = is_odd<N>;
		//	constexpr static bool is_pre_odd = AllOdd_<N - 1>::value;
		//	constexpr static bool value = is_cur_odd && is_pre_odd;
		//};

		//template<>
		//struct AllOdd_<0>
		//{
		//	constexpr static bool value = is_odd<0>;
		//};
		//无论is_cur_odd的值是什么，AllOdd_都会对is_pre_odd进行求值，这会间接产生若干实例化的结果，虽然这些实例化可能对系统最终的求值没有什么作用

		//解决：短路逻辑
		template<size_t N>
		constexpr bool is_odd = ((N % 2) == 1);

		template<bool cur, typename TNext>
		constexpr static bool AndValue = false;

		template<typename TNext>
		constexpr static bool AndValue<true, TNext> = TNext::value;

		template<size_t N>
		struct AllOdd_
		{
			constexpr static bool is_cur_odd = is_odd<N>;
			constexpr static bool value = AndValue<is_cur_odd, AllOdd_<N - 1>>;
		};

		template<>
		struct AllOdd_<0>
		{
			constexpr static bool value = is_odd<0>;
		};
		//这里引入了一个辅助元函数AndValue：只有当该元函数的第一个操作数为true时，他才会实例化第二个操作数；否则将直接返回false
		//(在C++中，只有访问了模板内部的具体元素时，相应的元素才会被实例化)
	}

	//奇特的递归模板模式(CRTP)
	namespace crtp
	{
		//如果要实现类似运行期的多态特性，那么可以考虑使用CRTP。比如函数模板和静态函数不能是虚函数，为了让他们像虚函数一样使用，可以使用CRTP实现
		template<typename D>
		struct Base
		{
			template<typename TI>
			void func(const TI& input)
			{
				//
				D* ptr = static_cast<D*>(this);
				ptr->func_template(input);

				//
				D::func_static();
			}
		};

		struct Derive : public Base<Derive>
		{
			//
			template<typename TI>
			void func_template(const TI& input)
			{
				std::cout << input << std::endl;
			}

			//
			static void func_static()
			{
				std::cout << "...";
			}
		};
	}
}

namespace test_template_value
{
	template<std::size_t S>
	struct ls_zeros
	{
		static const std::size_t value = (S & std::size_t(1)) ? 0 : (1 + ls_zeros<(S >> 1u)>::value);
	};

	template<>
	struct ls_zeros<0>
	{
		static const std::size_t value = 0;
	};

	template<>
	struct ls_zeros<1>
	{
		static const std::size_t value = 0;
	};
}

#include <pack_options.h>
#include <options.h>
#include <pointer_rebind.h>

namespace test_template_pack_options
{
	using namespace xll;

	////实际应用
	template<typename... Options>
	class my_class
	{
		using packed_options = typename pack_options<defaults, Options...>::type;

		//...
		//packed_options::custom_type_option_name
		//packed_options::custom_value_option_name
	};
}

#include <pointer_rebind.h>

namespace test_template_pointer_rebind
{
	struct my_dont_have_rebindU
	{

	};

	struct my_have_rebindU_dont_have_rebindU_other
	{
		template<typename T>
		struct rebind
		{

		};
	};

	struct my_have_rebindU_have_rebindU_other
	{
		template<typename T>
		struct rebind
		{
			struct other
			{

			};
		};
	};

	template<typename A, typename... An>
	struct my_dont_have_rebindU_but_have_A_An
	{

	};

	template<typename A>
	struct my_dont_have_rebindU_but_have_A
	{

	};

}


namespace test_template_pointer_traits
{
	//yes_type no_type
	using yes_type = char;

	struct no_type
	{
		char dummy[2];
	};

	////使用宏，节省定义多次类模板

	//Infrastructure for providing a default type for T::TNAME if absent
#define INSTANTIATE_DEFAULT_TYPE_TMPLT(TNAME)                      \
	template<typename T>                                           \
	struct has_type_ ## TNAME                                      \
	{                                                              \
		template<typename X>                                       \
		static char test(int, typename X::TNAME*);                 \
                                                                   \
		template<typename X>                                       \
		static int test(...);                                      \
                                                                   \
		static const bool value = (1 == sizeof(test<T>(0, 0)));    \
	};                                                             \
	template<typename T, typename DefaultType>                     \
	struct default_type_ ## TNAME                                  \
	{                                                              \
		struct DefaultWrap { using TNAME = DefaultType; };          \
                                                                   \
		using type = typename boost::mpl::if_c                     \
			<has_type_ ## TNAME<T>::value, T, DefaultWrap>         \
			::type::TNAME;                                         \
	};                                                             \
	//
#define OBTAIN_TYPE_WITH_DEFAULT(INSTANTIATION_NS_PREFIX, T, TNAME, TIMPL)   \
	typename INSTANTIATION_NS_PREFIX default_type_ ## TNAME<T, TIMPL>::type  \
	//
#define HAS_TYPE(INSTANTIATION_NS_PREFIX, T, TNAME)                \
	INSTANTIATION_NS_PREFIX has_type_ ## TNAME<T>::value           \
	//
#define INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(TNAME)                 \
	template<typename T, typename DefaultType>                     \
	struct eval_default_type_ ## TNAME                             \
	{                                                              \
		template<typename X>                                       \
		static char test(int, typename X::TNAME*);                 \
                                                                   \
		template<typename X>                                       \
		static int test(...);                                      \
                                                                   \
		struct DefaultWrap                                         \
		{                                                          \
			using TNAME = typename DefaultType::type;              \
		};                                                         \
                                                                   \
		static const bool value = (1 == sizeof(test<T>(0, 0)));    \
                                                                   \
		using type = typename ::boost::mpl::eval_if_c              \
			<value                                                 \
			, ::boost::mpl::identity<T>                            \
			, ::boost::mpl::identity<DefaultWrap>>::type::TNAME;   \
	};                                                             \
	//
#define OBTAIN_TYPE_WITH_EVAL_DEFAULT(INSTANTIATION_NS_PREFIX, T, TNAME, TIMPL)  \
	typename INSTANTIATION_NS_PREFIX eval_default_type_##TNAME<T, TIMPL>::type \
	//
#define INTERNAL_STATIC_BOOL_IS_TRUE(TRAITS_PREFIX, TYPEDEF_TO_FIND)      \
	template<typename T>                                                  \
	struct TRAITS_PREFIX##_bool                                           \
	{																	  \
		template<bool Add>                                                \
		struct two_or_three                                               \
		{                                                                 \
			yes_type _[2 + Add];                                          \
		};                                                                \
															              \
		template<typename U>                                              \
		static yes_type test(...);                                        \
                                                                          \
		template<typename U>                                              \
		static two_or_three<U::TYPEDEF_TO_FIND> test(int);                \
                                                                          \
		static const std::size_t value = sizeof(test<T>(0));              \
	};                                                                    \
	template<typename T>                                                  \
	struct TRAITS_PREFIX##_bool_is_true                                   \
	{	                                                                  \
		static const bool value =                                         \
			TRAITS_PREFIX##_bool<T>::value > sizeof(yes_type) * 2;        \
	};                                                                    \
	//
#define HAS_STATIC_MEMBER_FUNC_SIGNATURE(TRAITS_NAME, FUNC_NAME)          \
	template<typename U, typename Signature>                              \
	class TRAITS_NAME                                                     \
	{                                                                     \
	private:                                                              \
		template<Signature>                                               \
		struct helper;                                                    \
                                                                          \
		template<typename T>                                              \
		static yes_type test(helper<&T::FUNC_NAME>*);                     \
                                                                          \
		template<typename T>                                              \
		static no_type test(...);                                         \
	public:                                                               \
		static const bool value =                                         \
			sizeof(test<U>(0)) == sizeof(yes_type);                       \
	};                                                                    \
	//
#define HAS_MEMBER_FUNC_CALLED(TRAITS_NAME, FUNC_NAME)                    \
	template<typename Type>                                               \
	struct TRAITS_NAME                                                    \
	{																      \
		struct BaseMixin                                                  \
		{                                                                 \
			void FUNC_NAME();                                             \
		};                                                                \
                                                                          \
		struct Base : public Type, public BaseMixin                       \
		{                                                                 \
			Base();                                                       \
		};                                                                \
																	      \
		template<typename T, T t>                                         \
		class Helper{};                                                   \
																          \
		template<typename U>                                              \
		static no_type test(U*, Helper<void (BaseMixin::*)()              \
								, &U::FUNC_NAME>* = 0);                   \
		                                                                  \
		static yes_type test(...);                                        \
                                                                          \
		static const bool value                                           \
			= sizeof(yes_type) == sizeof(test((Base*)(0)));               \
	};                                                                    \
	//
#define HAS_MEMBER_FUNC_CALLED_IGNORE_SIGNATURE(TRAITS_NAME, FUNC_NAME)   \
	HAS_MEMBER_FUNC_CALLED(TRAITS_NAME##_ignore_signature, FUNC_NAME)     \
																	      \
	template<typename Type, class>                                        \
	struct TRAITS_NAME                                                    \
		: public TRAITS_NAME##_ignore_signature<Type>                     \
	{};                                                                   \
	//

	namespace detail
	{
		HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_pointer_to, point_to)
			HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_dynamic_cast_from, dynamic_cast_from)
			HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_static_cast_from, static_cast_from)
			HAS_STATIC_MEMBER_FUNC_SIGNATURE(has_member_function_callable_with_const_cast_from, const_cast_from)

			INSTANTIATE_EVAL_DEFAULT_TYPE_TMPLT(element_type)
			INSTANTIATE_DEFAULT_TYPE_TMPLT(difference_type)
			INSTANTIATE_DEFAULT_TYPE_TMPLT(reference)
			INSTANTIATE_DEFAULT_TYPE_TMPLT(value_traits_ptr)

			//meta_utils.hpp
			template<bool C_>
		struct bool_ : std::integral_constant<bool, C_>
		{
			operator bool() const
			{
				return C_;
			}

			bool operator()() const
			{
				return C_;
			}
		};
		using true_ = bool_<true>;
		using false_ = bool_<false>;

		template<typename T>
		struct addr_impl_ref
		{
			T& v_;
			inline addr_impl_ref(T& v) :v_(v) {}
			inline operator T& () const { return v_; }

		private:
			addr_impl_ref& operator=(const addr_impl_ref&);
		};

		template<typename T>
		struct addressof_impl
		{
			inline static T* f(T& v, long)
			{
				return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(v)));
			}

			inline static T* f(T* v, int)
			{
				return v;
			}
		};

		template<typename T>
		inline T* addressof(T& v)
		{
			return detail::addressof_impl<T>::f(detail::addr_impl_ref<T>(v), 0);
		}



		//type_traits.hpp
		template<typename T>
		struct unvoid_ref : std::add_lvalue_reference<T>
		{};
		template<>
		struct unvoid_ref<void>
		{
			using type = unvoid_ref&;
		};
		template<>
		struct unvoid_ref<const void>
		{
			using type = unvoid_ref&;
		};
		template<>
		struct unvoid_ref<volatile void>
		{
			using type = unvoid_ref&;
		};
		template<>
		struct unvoid_ref<const volatile void>
		{
			using type = unvoid_ref&;
		};

		//
		template<typename T>
		struct first_param
		{
			using type = void;
		};

		template<template<typename, typename...> class TemplateClass, typename T, typename... Args>
		struct first_param<TemplateClass<T, Args...>>
		{
			using type = T;
		};
	}

	//pointer_traits is the implementation of C++11 std::pointer_traits class with some extensions like castings

	//pointer_traits supplies a uniform interface to certain attributes of pointer-like types.

	//Note:When defining a custom family of pointers or references to be used with BI library,make sure
	//the public static conversion functions accessed through the "pointer_traits" interface ("_cast_from" and "pointer_to")
	//can properly convert between const and nonconst referred member types without the use of implicit constructor calls.
	//It is suggested these conversions be implemented as function templates,where the template argument is the type of the 
	//object being converted from.
	template<typename Ptr>
	struct pointer_traits
	{
		using pointer = Ptr;
		//缺省值不确定用OBTAIN_TYPE_WITH_EVAL_DEFAULT宏
		using element_type = OBTAIN_TYPE_WITH_EVAL_DEFAULT(test_template_pointer_traits::detail::, Ptr, element_type, test_template_pointer_traits::detail::first_param<Ptr>);
		//缺省值确定用OBTAIN_TYPE_WITH_DEFAULT宏
		using difference_type = OBTAIN_TYPE_WITH_DEFAULT(test_template_pointer_traits::detail::, Ptr, difference_type, std::ptrdiff_t);

		using reference = OBTAIN_TYPE_WITH_DEFAULT(test_template_pointer_traits::detail::, Ptr, reference, typename test_template_pointer_traits::detail::unvoid_ref<element_type>::type);

		template<class U>
		struct rebind_pointer
		{
			using type = typename xll::pointer_rebind<Ptr, U>::type;
		};
		template<class U>
		using rebind = typename xll::pointer_rebind<Ptr, U>::type;

		//Remark: If element_type is (possibly cv-qualified) void, r type is unspecified; otherwise,it is element_type&.
		//
		//Returns: A dereferenceable pointer to r obtained by calling Ptr::pointer_to(reference).
		//         Non-standard extension:If such function does not exist,returns pointer(addressof(r));
		//Note: For non-conforming compilers only the existence of a member function called "pointer_to" is checked.
		inline static pointer pointer_to(reference r)
		{
			//Non-standard extension,it does not require Ptr::pointer_to.If not present tries to converts &r to pointer.
			const bool value = detail::has_member_function_callable_with_pointer_to<Ptr, Ptr(*)(reference)>::value;
			detail::bool_<value> flag;
			return pointer_traits::priv_pointer_to(flag, r);
		}

		//Remark: Non-standard extension.
		//
		//Returns: A dereferenceable pointer to r obtained by calling the static template function Ptr::static_cast_from(UPpr/const UPpr &).
		//         If such function does not exist, returns pointer_to(static_cast<element_type&>(*uptr))
		//
		//Note:For non-conforming compilers only the existence of a member function called "static_cast_from" is checked
		template<class UPtr>
		inline static pointer static_cast_from(const UPtr& uptr)
		{
			using RefArg = const UPtr&;
			const bool value =
				detail::has_member_function_callable_with_static_cast_from<pointer, pointer(*)(RefArg)>::value ||
				detail::has_member_function_callable_with_static_cast_from<pointer, pointer(*)(UPtr)>::value;
			return pointer_traits::priv_static_cast_from(detail::bool_<value>(), uptr);
		}

		//Remark: Non-standard extension
		//
		//Returns: A dereferenceable pointer to r obtained by calling the static template function Ptr::const_cast_from<UPtr>(UPpr/const UPpr &).
		//		If such function does not exist, returns pointer_to(const_cast<element_type&>(*uptr))
		//
		//Note: For non-conforming compilers only the existence of a member function called "const_cast_from" is checked.
		template<class UPtr>
		inline static pointer const_cast_from(const UPtr& uptr)
		{
			using RefArg = const UPtr&;
			const bool value =
				detail::has_member_function_callable_with_const_cast_from<pointer, pointer(*)(RefArg)>::value ||
				detail::has_member_function_callable_with_const_cast_from<pointer, pointer(*)(UPtr)>::value;
			return pointer_traits::priv_const_cast_from(detail::bool_<value>(), uptr);
		}

		//Remark: Non-standard extension.
		//
		//Returns: A dereferenceable pointer to r obtained by calling the static template function Ptr::dynamic_cast_from<UPtr>(UPpr/const UPpr &).
		//        If such function does not exist, returns pointer_to(*dynamic_cast<element_type*>(&*uptr))
		//
		//Note: For non-conforming compilers only the existence of a member function called "dynamic_cast_from" is checked.
		template<class UPtr>
		inline static pointer dynamic_cast_from(const UPtr& uptr)
		{
			using RefArg = const UPtr&;
			const bool value =
				detail::has_member_function_callable_with_dynamic_cast_from<pointer, pointer(*)(RefArg)>::value ||
				detail::has_member_function_callable_with_dynamic_cast_from<pointer, pointer(*)(UPtr)>::value;
			return pointer_traits::priv_dynamic_cast_from(detail::bool_<value>(), uptr);
		}

	private:
		//to_raw_pointer
		template<typename T>
		inline static T* to_raw_pointer(T* p)
		{
			return p;
		}
		template<typename Pointer>
		inline static typename pointer_traits<Pointer>::element_type* to_raw_pointer(const Pointer& p)
		{
			return pointer_traits::to_raw_pointer(p.operator->());
		}

		//priv_pointer_to
		inline static pointer priv_pointer_to(detail::true_, reference r)
		{
			return Ptr::pointer_to(r);
		}
		inline static pointer priv_pointer_to(detail::false_, reference r)
		{
			return pointer(detail::addressof(r));
		}

		//priv_static_cast_from
		template<typename UPtr>
		inline static pointer priv_static_cast_from(detail::true_, const UPtr& uptr)
		{
			return Ptr::static_cast_from(uptr);
		}
		template<typename UPtr>
		inline static pointer priv_static_cast_from(detail::false_, const UPtr& uptr)
		{
			return uptr ? pointer_to(*static_cast<element_type*>(to_raw_pointer(uptr))) : pointer();
		}

		//priv_const_cast_from
		template<typename UPtr>
		inline static pointer priv_const_cast_from(detail::true_, const UPtr& uptr)
		{
			return Ptr::const_cast_from(uptr);
		}
		template<typename UPtr>
		inline static pointer priv_const_cast_from(detail::false_, const UPtr& uptr)
		{
			return uptr ? pointer_to(const_cast<element_type&>(*uptr)) : pointer();
		}

		//priv_dynamic_cast_from
		template<typename UPtr>
		inline static pointer priv_dynamic_cast_from(detail::true_, const UPtr& uptr)
		{
			return Ptr::dynamic_cast_from(uptr);
		}

		template<typename UPtr>
		inline static pointer priv_dynamic_cast_from(detail::false_, const UPtr& uptr)
		{
			return uptr ? pointer_to(dynamic_cast<element_type&>(*uptr)) : pointer();
		}
	};

	//Remove cv qualification from Ptr parameter to pointer_traits:
	template<typename Ptr>
	struct pointer_traits<const Ptr> : pointer_traits<Ptr> {};

	template<typename Ptr>
	struct pointer_traits<volatile Ptr> : pointer_traits<Ptr> {};

	template<typename Ptr>
	struct pointer_traits<const volatile Ptr> : pointer_traits<Ptr> {};

	//Remove reference from Ptr parameter to pointer_traits:
	template<typename Ptr>
	struct pointer_traits<Ptr&> : pointer_traits<Ptr> {};

	//Specialization of pointer_traits for raw pointers
	template<typename T>
	struct pointer_traits<T*>
	{
		using element_type = T;
		using pointer = T*;
		using difference_type = std::ptrdiff_t;
		using reference = typename test_template_pointer_traits::detail::unvoid_ref<element_type>::type;

		template<typename U>
		using rebind = U*;

		template<typename U>
		struct rebind_pointer
		{
			using type = U*;
		};

		//Returns: addressof(r)
		inline static pointer pointer_to(reference r)
		{
			return detail::addressof(r);
		}

		//Returns: static_cast<pointer>(uptr)
		template<typename U>
		inline static pointer static_cast_from(U* uptr)
		{
			return static_cast<pointer>(uptr);
		}

		//Returns: const_cast<pointer>(uptr)
		template<typename U>
		inline static pointer const_cast_from(U* uptr)
		{
			return const_cast<pointer>(uptr);
		}

		//Returns: dynamic_cast<pointer>(uptr)
		template<typename U>
		inline static pointer dynamic_cast_from(U* uptr)
		{
			return dynamic_cast<pointer>(uptr);
		}
	};
}

namespace test_template_hook
{
	using namespace test_template_pointer_traits;

	//标签，一般只需要声明就可以了
	struct dft_tag;       //缺省标签
	struct member_tag;    //成员hook标签


	//data_holder
	template<typename Data, typename Tag>
	struct data_holder
		: Data
	{};

	template<typename DataTraits>
	struct algo
	{
		using data = typename DataTraits::data;
		using data_ptr = typename DataTraits::data_ptr;
		using const_data_ptr = typename DataTraits::const_data_ptr;
		using data_traits = DataTraits;

		//...
		inline static void init(data_ptr this_data)
		{
			//DataTraits::set_next(this_data, node_ptr());
		}
	};

	template<typename DataTraits>
	struct get_algo
	{
		using type = algo<DataTraits>;
	};


	template
		<class DataTraits
		, class Tag              //标签，默认是dft_tag，基类hook可以有多个不同类型的，成员hook只能是member_hook
		>  //这里还可以添加其他有用模板参数
		class generic_hook
		//If the hook is a base hook, derive generic hook from node_holder so that a unique base class is created to convert from
		//the data to the type.This mechanism will be used by bhtraits.
		//(如果hook是基类hook，那么generic_hook的基类是data_holder模板实例，以便创建一个唯一的基类来将数据转换为类型，这个机制将被bhtraits使用)
		//
		//If the hook is a member hook, generic hook will directly derive from the hook.
		//(如果hook是成员hook，那么generic_hook的基类是DataTraits::data)
		: public boost::mpl::if_c
		<std::is_same_v<Tag, member_tag>
		, typename DataTraits::data
		, data_holder<typename DataTraits::data, Tag>
		>::type
	{
		//定义data、data_ptr和const_data_ptr类型     //私有，外界若想访问可以通过DataTraits
		//实际上就是DataTraits中定义的同名类型
		using data_algorithms = typename get_algo<DataTraits>::type;
		using data = typename data_algorithms::data;
		using data_ptr = typename data_algorithms::data_ptr;
		using const_data_ptr = typename data_algorithms::const_data_ptr;

	public:

		inline data_ptr this_ptr()
		{
			return pointer_traits<data_ptr>::pointer_to(static_cast<data&>(*this));
		}

		inline const_data_ptr this_ptr() const
		{
			return pointer_traits<const_data_ptr>::pointer_to(static_cast<const data&>(*this));
		}

	public:
		inline generic_hook()
		{
			data_algorithms::init(this->this_ptr());
		}
		inline generic_hook(const generic_hook&)
		{
			data_algorithms::init(this->this_ptr());
		}
		inline generic_hook& operator=(const generic_hook&)
		{
			return *this;
		}
		inline ~generic_hook()
		{
			//...
		}
		inline void swap_datas(generic_hook& other)
		{
			//data_algorithms::swap_datas(this->this_ptr(), other.this_ptr());
		}

		//...
	};



	//这个就是要注入的数据
	template<typename VoidPointer>
	struct a_data
	{
		using data_ptr = typename xll::pointer_rebind<VoidPointer, a_data>::type;

		data_ptr next_;
		data_ptr prev_;
	};

	//这个是实际注入的数据，是数据的特征
	template<typename VoidPointer>    //注意a_data和a_data_traits是怎么配合的，肯定不是通过继承 -->a_data提供数据 a_data_traits提供(静态非成员)函数
	struct a_data_traits
	{
		using data = a_data<VoidPointer>;
		using data_ptr = typename data::data_ptr;
		using const_data_ptr = typename xll::pointer_rebind<VoidPointer, const data>::type;

		inline static data_ptr get_previous(const const_data_ptr& n)
		{
			return n->prev_;
		}

		inline static data_ptr get_previous(const data_ptr& n)
		{
			return n->prev_;
		}

		inline static void set_previous(data_ptr n, data_ptr prev)    //注意这里prev不是const_data_ptr类型，提高了灵活性
		{
			n->prev_ = prev;
		}

		inline static data_ptr get_next(const const_data_ptr& n)
		{
			return n->next_;
		}

		inline static data_ptr get_next(const data_ptr& n)
		{
			return n->text;
		}

		inline static void set_next(data_ptr n, data_ptr next)  //注意这里next不是const_data_ptr类型，提高了灵活性
		{
			n->next_ = next;
		}
	};











	template<typename... Options>
	struct make_a_base_hook
	{
		//... 应用Options

		using implementation_defined = generic_hook<a_data_traits<void*>, dft_tag>;

		using type = implementation_defined;
	};

	template<typename... Options>
	class a_base_hook : public make_a_base_hook<Options...>::type
	{};

	template<typename... Options>
	struct make_a_member_hook
	{
		//... 应用Options

		using implementation_defined = generic_hook<a_data_traits<void*>, member_tag>;

		using type = implementation_defined;
	};

	template<typename... Options>
	class a_member_hook : public make_a_member_hook<Options...>::type
	{};
}

namespace test_template_const_char
{
	extern const char a[] = "a";     //用数组而非指针是为了保证是编译期的
	extern const char b[] = "b";

	//还可以用 boost::mpl::string

	//别名模板不支持部分特化和全特化

	//字符串转类型
	template<const char*>
	struct to_type;

	template<>
	struct to_type<a>
	{
		using type = int;
	};

	template<>
	struct to_type<b>
	{
		using type = double;
	};
}
