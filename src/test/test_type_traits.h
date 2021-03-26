#pragma once

#undef max
#undef min

#include <algorithm>

#include <type_traits>

namespace test_type_traits
{
	//integral_constant

	template<class T, T v>
	struct integral_constant
	{
		static constexpr T value = v;  //核心!!  静态常量(static constexpr)

		using value_type = T;
		using type = integral_constant;//使用注入类名

		constexpr operator value_type() const noexcept
		{
			return (value);
		}

		constexpr value_type operator()() const noexcept
		{
			return (value);
		}
	};

	//integral_constant存在的意义是：为了C++编译的时候能够使用模板初编译来确定其中的值

	//同理true_type 和 false_type

	template<bool _Val>
	using bool_constant = integral_constant<bool, _Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	//(注意上面不是通过继承实现的)

	namespace primary_type_categories
	{
		//检查T是否数组类型
		template<class T> struct is_array : false_type {};

		template<class T> struct is_array<T[]> : public true_type {};
		template<class T> struct is_array<T const[]> : public true_type {};
		template<class T> struct is_array<T volatile[]> : public true_type {};
		template<class T> struct is_array<T const volatile[]> : public true_type {};

		template<class T, std::size_t N> struct is_array<T[N]> : public true_type {};
		template<class T, std::size_t N> struct is_array<T const[N]> : public true_type {};
		template<class T, std::size_t N> struct is_array<T volatile[N]> : public true_type {};
		template<class T, std::size_t N> struct is_array<T const volatile[N]> : public true_type {};

		template<class T>
		inline constexpr bool is_array_v = is_array<T>::value;










		//检查T是否为联合体类型     (union)
		namespace is_union_by_is_union
		{
			template<class T> struct is_union : public integral_constant<bool, __is_union(T)> {};

			template<class T> struct is_union<T const> : public is_union<T> {};
			template<class T> struct is_union<T volatile const> : public is_union<T> {};
			template<class T> struct is_union<T volatile> : public is_union<T> {};

			template<class T>
			inline constexpr bool is_union_v = is_union<T>::value;
		}












		//检查T是否为非联合类类型   (struct是类，enum class和基本类型比如int不是类)
		//利用编译器提供的__is_union(T)
		namespace is_class_by_is_union
		{
			namespace detail
			{
				template<class T>
				integral_constant<bool, !__is_union(T)> test(int T::*);

				template<class>
				false_type test(...);
			}

			template<class T>
			struct is_class : decltype(detail::test<T>(nullptr))
			{};

			template<class T>
			inline constexpr bool is_class_v = is_class<T>::value;
		}
		//利用编译器提供的__is_class(T)
		namespace is_class_by_is_class
		{
			namespace detail
			{
				template<typename T>
				struct is_class_impl
				{
					static const bool value = __is_class(T);
				};
			}

			template<class T> struct is_class : public integral_constant<bool, detail::is_class_impl<T>::value> {};
			template<class T> struct is_class<const T> : public is_class<T> {};
			template<class T> struct is_class<const volatile T> : public is_class<T> {};
			template<class T> struct is_class<volatile T> : public is_class<T> {};

			template<class T>
			inline constexpr bool is_class_v = is_class<T>::value;
		}
		



		//检查T是否为枚举类型
		//利用编译器提供的__is_enum(T)
		namespace is_enum_by_is_enum
		{
			template<class T> struct is_enum : public integral_constant<bool, __is_enum(T)> {};

			template<class T>
			inline constexpr bool is_enum_v = is_enum<T>::value;
		}





		//检查T是否为浮点类型         (float、double、long double和对应cv限定变体)
		template<class T> struct is_floating_point : public false_type{};
		template<class T> struct is_floating_point<const T> : public is_floating_point<T> {};
		template<class T> struct is_floating_point<volatile const T> : public is_floating_point<T> {};
		template<class T> struct is_floating_point<volatile T> : public is_floating_point<T> {};
		template<> struct is_floating_point<float> : public true_type {};
		template<> struct is_floating_point<double> : public true_type {};
		template<> struct is_floating_point<long double> : public true_type {};

		template<class T>
		inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

		namespace is_floating_point_by_is_same_and_remove_cv
		{
			template<class T>
			struct is_floating_point
				: std::integral_constant<
				bool,
				std::is_same<float, typename std::remove_cv<T>::type>::value ||
				std::is_same<double, typename std::remove_cv<T>::type>::value ||
				std::is_same<long double, typename std::remove_cv<T>::type>::value
				> {};

			template<class T>
			inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
		}





		//检查T是否为函数类型      std::function、lambda、有重载operator()的类和指向函数指针都不认为是函数类型
		//std::is_function 能以更简单的方式实现。 libc++ 、 libstdc++ 及 MS STL 的新版本使用类似以下的实现
		template<class T>
		struct is_function : std::integral_constant<
			bool,
			!std::is_const<const T>::value && !std::is_reference<T>::value
		>{};
		template<class T>
		inline constexpr bool is_function_v = is_function<T>::value;
		//boost库中的实现和https://zh.cppreference.com/w/cpp/types/is_function中的实现是一样的，比较多，这里省略，主要是教学目的，展现了大量函数类型种类






		//检查T是否为整数类型    (bool、char、char8_t、char16_t、char32_t、wchar_t、short、int、long、long long类型，或任何实现定义的扩展整数类型，包含任何有符号、无符号及cv限定的变体) (注意enum不是)
		template<class T> struct is_integral : public false_type {};

		template<class T> struct is_integral<const T> : public is_integral<T> {};
		template<class T> struct is_integral<volatile const T> : public is_integral<T> {};
		template<class T> struct is_integral<volatile T> : public is_integral<T> {};

		template<> struct is_integral<unsigned char> : public true_type {};
		template<> struct is_integral<unsigned short> : public true_type {};
		template<> struct is_integral<unsigned int> : public true_type {};
		template<> struct is_integral<unsigned long> : public true_type {};

		template<> struct is_integral<signed char> : public true_type {};
		template<> struct is_integral<short> : public true_type {};
		template<> struct is_integral<int> : public true_type {};
		template<> struct is_integral<long> : public true_type {};

		template<> struct is_integral<char> : public true_type {};
		template<> struct is_integral<bool> : public true_type {};

		template<> struct is_integral<wchar_t> : public true_type {};

		template<> struct is_integral<unsigned long long> : public true_type {};
		template<> struct is_integral<long long> : public true_type {};

		template<> struct is_integral<char16_t> : public true_type {};
		template<> struct is_integral<char32_t> : public true_type {};

		template<class T>
		inline constexpr bool is_integral_v = is_integral<T>::value;









		//检查T是否为指向对象指针或指向函数指针(但不是指向成员/成员函数指针)    (引用、int[10]、std::nullptr_t不是)
		namespace is_pointer_by_remove_cv
		{
			template<class T>
			struct is_pointer_helper : false_type {};

			template<class T>
			struct is_pointer_helper<T*> : true_type {};

			template<class T>
			struct is_pointer : is_pointer_helper<typename std::remove_cv<T>::type> {};

			template<class T>
			inline constexpr bool is_pointer_v = is_pointer<T>::value;
		}
		template<class T> struct is_pointer : public false_type {};

		template<class T> struct is_pointer<T*> : public true_type {};
		template<class T> struct is_pointer<T* const> : public true_type {};
		template<class T> struct is_pointer<T* const volatile> : public true_type {};
		template<class T> struct is_pointer<T* volatile> : public true_type {};

		template<class T> struct is_pointer<T const> : public is_pointer<T> {};
		template<class T> struct is_pointer<T const volatile> : public is_pointer<T> {};
		template<class T> struct is_pointer<T volatile> : public is_pointer<T> {};

		template<class T>
		inline constexpr bool is_pointer_v = is_pointer<T>::value;








		//检查T是否为void类型    (void、const void、volatile void或者const volatile void)
		namespace is_void_by_is_same_and_remove_cv
		{
			template<class T>
			struct is_void : std::is_same<void, typename std::remove_cv<T>::type> {};

			template<class T>
			inline constexpr bool is_void_v = is_void<T>::value;
		}

		template<class T>
		struct is_void : public false_type {};

		template<> struct is_void<void> : public true_type {};

		template<> struct is_void<const void> : public true_type {};
		template<> struct is_void<const volatile void> : public true_type {};
		template<> struct is_void<volatile void> : public true_type {};

		template<class T>
		inline constexpr bool is_void_v = is_void<T>::value;








		//检查T是否为左值引用类型
		template<class T> struct is_lvalue_reference : false_type {};
		template<class T> struct is_lvalue_reference<T&> : true_type {};

		template<class T>
		inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;







		//检查T是否为右值引用类型
		template<class T> struct is_rvalue_reference : false_type {};
		template<class T> struct is_rvalue_reference<T&&> : true_type {};

		template<class T>
		inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;










		//检查T是否为引用类型（左值引用或者右值引用）
		template<class T> struct is_reference : std::false_type {};
		template<class T> struct is_reference<T&> : std::true_type {};
		template<class T> struct is_reference<T&&> : std::true_type {};

		template<class T>
		inline constexpr bool is_reference_v = is_reference<T>::value;










		//检查T是否为执行非静态成员对象或非静态成员函数的指针
		template<class T>
		struct is_member_pointer_helper : std::false_type {};

		template<class T, class U>
		struct is_member_pointer_helper<T U::*> : std::true_type{};

		template<class T>
		struct is_member_pointer : is_member_pointer_helper<typename std::remove_cv<T>::type> {};

		template<class T>
		inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;








		//检查T是否为非静态成员函数指针类型
		namespace is_member_function_pointer_helper_by_is_function
		{
			template<class T>
			struct is_member_function_pointer_helper : false_type {};

			template<class T, class U>
			struct is_member_function_pointer_helper<T U::*> : std::is_function<T> {};

			template<class T>
			struct is_member_function_pointer : is_member_function_pointer_helper<typename std::remove_cv<T>::type> {};

			template<class T>
			inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;
		}






		//检查T是否为std::nullptr_t类型  (std::nullptr_t、const std::nullptr_t、volatile std::nullptr_t或者const volatile std::nullptr_t)     注意is_pointer<std::nullptr_t>::value为false，因为std::nullptr_t不是内建指针类型
		template<class T>
		struct is_null_pointer : std::is_same<std::nullptr_t, std::remove_cv_t<T>> {};

		template<class T>
		inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;
	}


	namespace type_relationships
	{
		//template<class Base, class Derived>
		//struct is_base_of;

		//检查是否Derived派生自Base或为同一非联合类(均忽略cv限定)
		//(若Base和Derived都是非联合类类型，且他们不是同一类型(忽略cv限定)，则Derived应是完整类型；否则行为未定义)
		
		//注意，即使A是B的私有、受保护或有歧义基类，std::is_base_of<A, B>::value亦为true，许多情况下，std::is_convertible<B*, A*>是更适合的测试
	
		//注意，尽管没有类是其自身的基类，std::is_base_of<T, T>::value亦为true，因为此特性的目的是模拟“是(is-a)”关系，而T是T。
		//尽管如此，std::is_base_of<int, int>::value为false，因为仅类参与此特性所模拟的关系

		//cppreference
		namespace is_base_of_by_cppreference
		{
			namespace detail
			{
				template<typename B>
				std::true_type test_pre_ptr_convertible(const volatile B*);
				template<typename>
				std::false_type test_pre_ptr_convertible(const volatile void*);

				template<typename, typename>
				auto test_pre_is_base_of(...)->std::true_type;
				template<typename B, typename D>
				auto test_pre_is_base_of(int)->decltype(test_pre_ptr_convertible<B>(static_cast<D*>(nullptr)));
			}

			template<typename Base, typename Derived>
			struct is_base_of : std::integral_constant<
				bool,
				std::is_class<Base>::value&& std::is_class<Derived>::value&&
				decltype(detail::test_pre_is_base_of<Base, Derived>(0))::value> {};

			template<typename Base, typename Derived>
			inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
		}
		//boost
		namespace is_base_of_by_boost
		{
			//is_base_and_derived
			namespace detail
			{
				template<typename B, typename D>
				struct is_base_and_derived_impl
				{
					using ncvB = std::remove_cv<B>;
					using ncvD = std::remove_cv<D>;

					static const bool value = ((__is_base_of(B, D) && !std::is_same<B, D>::value) && !std::is_same<ncvB, ncvD>::value);
				};
				template<class Base, class Derived>
				struct is_base_and_derived : public integral_constant<bool, (detail::is_base_and_derived_impl<Base, Derived>::value)> {};
				template<class Base, class Derived>
				struct is_base_and_derived<Base&, Derived> : public false_type {};
				template<class Base, class Derived>
				struct is_base_and_derived<Base, Derived&> : public false_type {};
				template<class Base, class Derived>
				struct is_base_and_derived<Base&, Derived&> : public false_type {};

				template<class B, class D>
				struct is_base_of_imp
				{
					using ncvB = typename std::remove_cv<B>::type;
					using ncvD = typename std::remove_cv<D>::type;

					static const bool value =
						is_base_and_derived_impl<ncvB, ncvD>::value ||
						(std::is_same<ncvB, ncvD>::value && std::is_class<ncvB>::value);
				};
			}
			template<class Base, class Derived> struct is_base_of
				: public integral_constant<bool, (detail::is_base_of_imp<Base, Derived>::value)> {};

			template<class Base, class Derived> struct is_base_of<Base, Derived&> : false_type {};
			template<class Base, class Derived> struct is_base_of<Base&, Derived&> : false_type {};
			template<class Base, class Derived> struct is_base_of<Base&, Derived> : false_type {};

			template<typename Base, typename Derived>
			inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
		}












		//is_convertible
		//c++11
		//template<class From, class To>
		//struct is_convertible;
		//c++20
		//template<class From, class To>
		//struct is_nothrow_convertible;

		//若虚构函数定义 To test(){return std::declval<From>();}为良构，（即std::declval<From>()能用隐式转换
		//转换为To，或From和To均为可有cv限定的void），则为true，否则为false
		//(为此检查的目的，不认为 return 语句中的 std::declval 是 odr 使用。)
		//( 如同从无关乎任一类型的语境中进行访问检查。仅考虑 return 语句中的表达式（包含转换到返回类型）的立即语境的合法性。)

		//is_nothrow_convertible同is_convertible，但转换亦为noexcept

		//From与To应均为完整类型、（可为cv限定的）void，或未知边界数组。否则行为未定义
		//(若上述模板的实例化直接或间接的依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义)

		//对引用类型、 void 类型、数组类型和函数类型给出良好定义的结果。
		//当前标准未指定是否认为转换所产生的对象（结果对象或绑定到引用的临时量）的析构是转换的一部分。此为 LWG 问题 3400 。
		//所有已知实现都将析构当作转换的一部分，如 P0758R1 中提议。

		namespace is_convertible_by_cppreference
		{
			namespace detail
			{
				template<class T>
				auto test_returnable(int)->decltype(
					void(static_cast<T(*)()>(nullptr)), std::true_type{}
				);
				template<class>
				auto test_returnable(...)->std::false_type;

				template<class From, class To>
				auto test_implicitly_convertible(int)->decltype(
					void(std::declval<void(&)(To)>()(std::declval<From>())), std::true_type{}
					);
				template<class, class>
				auto test_implicitly_convertible(...)->std::false_type;
			}

			template<class From, class To>
			struct is_convertible : std::integral_constant<bool,
				(decltype(detail::test_returnable<To>(0))::value&&
					decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
				(std::is_void<From>::value && std::is_void<To>::value)> {};

			template<class From, class To>
			struct is_nothrow_convertible : std::conjunction<std::is_void<From>, std::is_void<To>> {};

			template<class From, class To>
			requires
				requires{
				static_cast<To(*)()>(nullptr);
				{std::declval<void(&)(To) noexcept>()(std::declval<From>())} noexcept;
			}
			struct is_nothrow_convertible<From, To> :std::true_type {};

			//c++17
			template<class From, class To>
			inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
			//c++20
			template<class From, class To>
			inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;
		}

		namespace is_convertible_by_boost
		{
			template<class From, class To>
			struct is_convertible : public integral_constant<bool, (__is_convertible_to(From, To) || (std::is_same<From, To>::value && !std::is_function<To>::value)) && !__is_abstract(To)>{};

			template<class From, class To>
			inline constexpr bool is_convertible_v = is_convertible<From, To>::value;
		}







		//template<class T, class U>
		//struct is_same;
		//若T与U指名同一类型(考虑const/volatile限定)，则为true，否则为false
		//(满足交换律，即对于任何两个类型T与U，is_same<T,U>::value == true当且仅当is_same<U,T>::value == true)

		template<class T, class U>
		struct is_same : std::false_type {};
		template<class T>
		struct is_same<T, T> : std::true_type {};

		template<class T, class U>
		inline constexpr bool is_same_v = is_same<T, U>::value;
	}

	namespace property_queries
	{
		//template<class T>
		//struct alignment_of;
		//提供等于T类型对齐要求的成员常量value，如同用alignof表达式获得。
		//若T是数组类型，则返回元素类型的对齐要求
		//若T是引用类型，则返回备用类型的对齐要求

		//若alignof(T)不是合法表达式，则行为未定义

		//注意，此类型特征遭遇alignof关键字出现，该关键字能用于较简明地获得相同值

		template<class T>
		struct alignment_of : std::integral_constant<std::size_t, alignof(T)> {};

		template<class T>
		inline constexpr std::size_t alignment_of_v = alignment_of<T>::value;







		template<class T, unsigned N = 0>
		struct extent;
		//若T是数组类型，则提供等于数组第N维元素数量的成员常量value，若N在[0, std::rank<T>::value)中

		//对于任何其他类型，或若T是在其首维度未知边界数组且N为0，则value为0

		template<class T, unsigned N = 0>
		struct extent : std::integral_constant<std::size_t, 0> {};

		template<class T>
		struct extent<T[], 0> : std::integral_constant<std::size_t, 0> {};

		template<class T, unsigned N>
		struct extent<T[], N> : std::extent<T, N - 1> {};

		template<class T, std::size_t I>
		struct extent<T[I], 0> : std::integral_constant<std::size_t, I> {};

		template<class T, std::size_t I, unsigned N>
		struct extent<T[I], N> : std::extent<T, N - 1> {};

		template<class T, unsigned N = 0>
		inline constexpr std::size_t extent_v = extent<T, N>::value;






		//template<class T>
		//struct rank;

		//若T是数组类型，则提供等于该数组维数的成员常量value。对于任何其他类型，value为0

		template<class T>
		struct rank : public std::integral_constant<std::size_t, 0> {};
		template<class T>
		struct rank<T[]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
		template<class T, std::size_t N>
		struct rank<T[N]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};

		template<class T>
		inline constexpr std::size_t rank_v = rank<T>::value;
	}

	namespace type_transformations
	{
		namespace const_volatile_qualifications
		{
			//template<class T>
			//struct add_cv;
			//template<class T>
			//struct add_const;
			//template<class T>
			//struct add_volatile;

			//提供同T的成员typedef type，除了他拥有添加的cv限定符(除非T是函数、引用或已拥有cv限定符)
			//add_cv：添加const和volatile
			//add_const：添加const
			//add_volatile：添加volatile

			//注意，这些变换特征能用于在模板实参推导中建立非推导语境  (模板实参推导 https://zh.cppreference.com/w/cpp/language/template_argument_deduction#.E9.9D.9E.E6.8E.A8.E5.AF.BC.E8.AF.AD.E5.A2.83)
			//template<class T>
			//void f(const T&, const T&);
			//f(4.2, 0);//错误：对'T'推导出冲突的类型
			//template<class T>
			//void g(const T&, std::add_const_t<T>&);
			//g(4.2, 0);//OK:调用g<double>

			template<class T>
			struct add_cv
			{
				using type = const volatile T;
			};
			template<class T>
			struct add_const
			{
				using type = const T;
			};
			template<class T>
			struct add_volatile
			{
				using type = volatile T;
			};

			template<class T>
			using add_cv_t = typename add_cv<T>::type;
			template<class T>
			using add_const_t = typename add_const<T>::type;
			template<class T>
			using add_volatile_t = typename add_volatile<T>::type;








			//template<class T>
			//struct remove_cv;
			//template<class T>
			//struct remove_const;
			//template<class T>
			//struct remove_volatile;
			//提供与T相同的成员typedef type，除了其最顶层cv限定符被移除
			//remove_cv：移除最顶层const、最顶层volatile或两者，若存在
			//remove_const：移除最顶层const
			//remove_volatile：移除最顶层volatile

			template<class T>
			struct remove_cv
			{
				using type = T;
			};
			template<class T>
			struct remove_cv<const T>
			{
				using type = T;
			};
			template<class T>
			struct remove_cv<volatile T>
			{
				using type = T;
			};
			template<class T>
			struct remove_cv<const volatile T>
			{
				using type = T;
			};

			template<class T>
			struct remove_const
			{
				using type = T;
			};
			template<class T>
			struct remove_const<const T>
			{
				using type = T;
			};

			template<class T>
			struct remove_volatile
			{
				using type = T;
			};
			template<class T>
			struct remove_volatile<volatile T>
			{
				using type = T;
			};

			template<class T>
			using remove_cv_t = typename remove_cv<T>::type;
			template<class T>
			using remove_const_t = typename remove_const<T>::type;
			template<class T>
			using remove_volatile_t = typename remove_volatile<T>::type;
		}

		namespace compound_type_alterations
		{
			//C++20    (注意C++20才开始有)
			//template<class T>
			//struct type_identity;
			//提供指名T的成员typedef type（即恒等变换）

			//注意，type_identity能用于在模板实参推导中建立非推导语境
			//template<class T>
			//void f(T, T);
			//f(4.2, 0);  //错误：对'T'推导出冲突的类型
			//template<class T>
			//void g(T, std::type_identity_t<T>);
			//g(4.2, 0);  //OK：调用g<double>

			template<class T>
			struct type_identity
			{
				using type = T;
			};

			template<class T>
			using type_identity_t = typename type_identity<T>::type;








			//之前介绍了is_pointer

			template<class T>
			struct add_pointer;
			//若T为引用类型，则提供成员typedef type，其为指向被引用类型的指针
			//否则，若T指名对象类型、无cv或引用限定的函数类型或（可有cv限定的）void类型，则提供成员typedef type，其为类型T*

			//否则（若T为cv或引用限定的函数类型），提供成员typedef type，其为类型T
			namespace add_pointer_by_cppreference
			{
				namespace detail
				{
					template<class T>
					auto try_add_pointer(int)->std::type_identity<typename std::remove_reference<T>::type*>;
					template<class T>
					auto try_add_pointer(...)->std::type_identity<T>;
				}
				template<class T>
				struct add_pointer : decltype(detail::try_add_pointer<T>(0)){};

				template<class T>
				using add_pointer_t = typename add_pointer<T>::type;
			}
			namespace add_pointer_by_boost
			{
				template<typename T>
				struct add_pointer
				{
					using no_ref_type = typename std::remove_reference<T>::type;
					using type = no_ref_type*;
				};

				template<class T>
				using add_pointer_t = typename add_pointer<T>::type;
			}






			template<class T>
			struct remove_pointer;
			//提供成员typedef type，其为T所指向的类型，或若T不是指针，则type与T相同

			template<class T>
			struct remove_pointer
			{
				using type = T;
			};
			template<class T>
			struct remove_pointer<T*>
			{
				using type = T;
			};
			template<class T>
			struct remove_pointer<T* const>
			{
				using type = T;
			};
			template<class T>
			struct remove_pointer<T* volatile>
			{
				using type = T;
			};
			template<class T>
			struct remove_pointer<T* const volatile>
			{
				using type = T;
			};

			template<class T>
			using remove_pointer_t = typename remove_pointer<T>::type;








			//template<class T>
			//struct remove_reference;
			//若类型T为引用类型，则提供成员typedef type，其为T所引用的类型，否则type为T

			template<class T>
			struct remove_reference
			{
				using type = T;
			};
			template<class T>
			struct remove_reference<T&>
			{
				using type = T;
			};
			template<class T>
			struct remove_reference<T&&>
			{
				using type = T;
			};

			template<class T>
			using remove_reference_t = typename remove_reference<T>::type;







			////创建T的左值或者右值引用类型

			//template<class T>
			//struct add_lvalue_reference;
			//若T是对象或无cv或引用限定符的函数类型，则提供成员typedef type，其为T&，若T是到某类型U的右值引用，则
			//type为U&。否则，type为T

			//template<class T>
			//struct add_rvalue_reference;
			//若T是对象或无cv或引用限定符的函数类型，则提供成员typedef type，其为T&&，否则type为T。

			//这些类型变换遵从折叠规则：
			//std::add_lvalue_reference<T&>::type是T&
			//std::add_lvalue_reference<T&&>::type是T&
			//std::add_rvalue_reference<T&>::type是T&
			//std::add_rvalue_reference<T&&>::type是T&&

			//与直接使用T&的主要区别是std::add_lvalue_reference<void>::type为void，而void&导致编译错误

			namespace add_lvalue_reference_add_rvalue_reference_by_cppreference
			{
				namespace detail
				{
					template<class T>
					auto try_add_lvalue_reference(int)->type_identity<T&>;
					template<class T>
					auto try_add_lvalue_reference(...)->type_identity<T>;

					template<class T>
					auto try_add_rvalue_reference(int)->type_identity<T&&>;
					template<class T>
					auto try_add_rvalue_reference(...)->type_identity<T>;
				}
				template<class T>
				struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0)){};

				template<class T>
				struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0)){};

				template<class T>
				using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

				template<class T>
				using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
			}
			namespace add_lvalue_reference_add_rvalue_reference_by_boost
			{
				namespace detail
				{
					//
					// We can't filter out rvalue_references at the same level as
					// references or we get ambiguities from msvc:
					//

					template<typename T>
					struct add_reference_impl
					{
						using type = T&;
					};

					template<typename T>
					struct add_reference_impl<T&&>
					{
						using type = T&&;
					};
				}

				template<class T>
				struct add_reference
				{
					using type = typename detail::add_reference_impl<T>::type;
				};

				template<class T>
				struct add_reference<T&>
				{
					using type = T&;
				};

				//void&会导致编译期报错

				template<>
				struct add_reference<void>
				{
					using type = void;
				};

				template<>
				struct add_reference<const void>
				{
					using type = const void;
				};

				template<>
				struct add_reference<const volatile void>
				{
					using type = const volatile void;
				};

				template<>
				struct add_reference<volatile void>
				{
					using type = volatile void;
				};

				////

				template<class T>
				struct add_lvalue_reference
				{
					using type = typename add_reference<T>::type;
				};

				template<class T>
				struct add_lvalue_reference<T&&>
				{
					using type = T&;
				};

				template<class T>
				using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

				namespace detail
				{
					template<typename T, bool b>
					struct add_rvalue_reference_helper
					{
						using type = T;
					};

					template<typename T>
					struct add_rvalue_reference_helper<T, true>
					{
						using type = T&&;
					};

					template<typename T>
					struct add_rvalue_reference_imp
					{
						using type = typename add_rvalue_reference_helper<T, (std::is_void<T>::value == false && std::is_reference<T>::value == false)>::type;
					};
				}

				template<class T>
				struct add_rvalue_reference
				{
					using type = typename detail::add_rvalue_reference_imp<T>::type;
				};

				template<class T>
				using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
			}











			//template<class T>
			//struct remove_extent;

			//若T是某类型X的数组，则提供等于X的成员typedef type，否则type为T。注意若T是多维数组，则只移除第一维

			//添加remove_extent的特化的程序行为未定义

			template<class T>
			struct remove_extent
			{
				typedef T type;
			};
			template<class T>
			struct remove_extent<T[]>
			{
				typedef T type;
			};
			template<class T, std::size_t N>
			struct remove_extent<T[N]>
			{
				typedef T type;
			};

			template<class T>
			using remove_extent_t = typename remove_extent<T>::type;











			//template<class T>
			//struct remove_all_extents;

			//若T是某类型X的多维数组，则提供等于X的成员typedef type，否则type为T

			//添加remove_all_extents的特化的程序行为未定义

			template<class T>
			struct remove_all_extents
			{
				typedef T type;
			};
			template<class T>
			struct remove_all_extents<T[]>
			{
				typedef typename remove_all_extents<T>::type type;
			};
			template<class T, std::size_t N>
			struct remove_all_extents<T[N]>
			{
				typedef typename remove_all_extents<T>::type type;
			};
			template<class T>
			using remove_all_extents_t = typename remove_all_extents<T>::type;

















			//template<class T>
			//struct decay

			//对类型T应用左值到右值、数组到指针及函数到指针隐式转换，移除cv限定符，并定义结果类型为成员typedef type

			//若T指名“U的数组”或“到U的数组的引用”类型，则成员typedef type为U*
			//否则，若T为函数类型F或到他的引用，则成员typedef type为std::add_pointer<F>::type
			//否则，成员typedef type为std::remove_cv<std::remove_reference<T>::type>::type

			//这些转换模仿在以值传递时，应用到所有函数参数的类型转换
			namespace decay_by_cppreference
			{
				template<class T>
				struct decay
				{
				private:
					typedef typename std::remove_reference<T>::type U;
				public:
					typedef typename std::conditional<
						std::is_array<U>::value,
						typename std::remove_extent<U>::type*,
						typename std::conditional<
						std::is_function<U>::value,
						typename std::add_pointer<U>::type,
						typename std::remove_cv<U>::type
						>::type
					>::type type;
				};

				template<class T> using decay_t = typename decay<T>::type;
			}

			namespace decay_by_boost
			{
				//boost库中remove_bounds就是remove_extent   这里为了方便用std::remove_extent代替boost::remove_extent
				template<class T>
				struct remove_bounds : public std::remove_extent<T> {};

				template<class T>
				using remove_bounds_t = typename remove_bounds<T>::type;


				namespace detail
				{
					template<class T, bool Array, bool Function> struct decay_imp { typedef typename std::remove_cv<T>::type type; };
					template<class T> struct decay_imp<T, true, false> { typedef typename remove_bounds<T>::type* type; };
					template<class T> struct decay_imp<T, false, true> { typedef T* type; };
				}

				template<class T>
				struct decay
				{
				private:
					typedef typename remove_reference<T>::type Ty;
				public:
					typedef typename detail::decay_imp<Ty, std::is_array<Ty>::value, std::is_function<Ty>::value>::type type;
				};

				template<class T> using decay_t = typename decay<T>::type;
			}







			//template<class T>
			//struct make_signed;
			
			//若T是整数(除bool)或枚举类型，则提供是对应T的有符号整数类型的成员typedef type，他拥有相同的cv限定符
			//若T为有符号或无符号的char、short、int、long、long long；则提供来自此列表的对应T的有符号类型
			//若T为枚举类型或char、wchar_t、char8_t（C++20）、char16_t、char32_t；则提供与T有相同sizeof的有最小等级的有符号整数类型
			//否则，行为未定义(C++20前)
			//否则，程序为非良构(C++20后)

			//添加make_signed的特化的程序行为未定义

			//实现略












			//template<class T>
			//struct make_unsigned;
			//若T是整数(除bool)或枚举类型，则提供成员typedef type，他是对应T的拥有相同cv限定符的无符号整数类型。
			//若T为有符号或无符号的char、short、int、long、long long；则提供来自此列表的对应T的无符号类型。
			//若T为枚举类型或char、wchar_t、char8_t(C++20)、char16_t、char32_t；则提供与T有相同sizeof的有最小等级的无符号整数类型
			//否则，行为未定义(C++20前)
			//否则，程序为非良构(C++20后)

			//添加make_unsigned的特化的程序行为未定义

			//实现略













			//template<class T>
			//struct underlying_type;

			//若T是完整枚举类型，则提供指名T底层类型的成员typedef type
			//否则行为未定义(C++20前)
			//否则，若T不是枚举类型，则无成员type。否则(T为不完整枚举类型)程序为谬构(C++20后)

			//添加underlying_type的特化的程序行为未定义

			//每个枚举类型都有底层类型，他们是：
			//1.显式指定（有作用域和无作用域枚举均可）
			//2.省略，该情况下对于有作用域枚举是int，或（对于无作用域枚举）是足以表示枚举所有值的实现定义的整数类型

		}



		namespace other_type_generators
		{
			//template<bool B, class T, class F>
			//struct conditional;
			//提供成员typedef type，若B在编译时为true则定义为T，或若B为false则定义为F
			template<bool B, class T, class F>
			struct conditional { typedef T type; };
			template<class T, class F>
			struct conditional<false, T, F> { typedef F type; };
			template<bool B, class T, class F>
			using conditional_t = typename conditional<B, T, F>::type;

			//添加conditional的特化的程序行为未定义











			//template<class... T>
			//struct common_type;
			//确定所有类型T...的共用类型，即所有T...都能隐式转换到的类型。若这种类型（根据后述规则确定）存在，则成员
			//type指名该类型。否则，无成员type

			//1.若sizeof...(T)为零，则无成员type
			//2.若sizeof...(T)为一(即T...只含一个类型T0)，则成员type指名与std::common_type<T0,T0>::type相同的类型，若他存在；否则无成员type
			//3.若sizeof...(T)为二(即T...含恰好二个成员T1与T2)，则
			//	若应用std::decay到至少T1与T2中至少一个类型后产生相异类型，则成员type指名与std::common_type<std::decay<T1>::type, std::decay<T2>::type>::type相同的类型，若他存在；若不存在，则无成员type
			//	否则，若有对std::common_type<T1, T2>的用户定义特化，则使用该特化
			//	否则，若std::decay<decltype(false ? std::declval<T1>() : std::declval<T2>())>::type是合法类型，则成员type代表该类型
			//	(C++20)否则，若std::decay<decltype(false ? std::declval<const T1&>() : std::declval<const T2&>())>::type为合法类型，则成员type代表该类型
			//	否则，无成员type
			//4.若sizeof...(T)大于二(即T...由类型T1，T2，R...组成)，则若std::common_type<T1, T2>::type存在，则成员type指代std::common_type<std::common_type<T1, T2>::type, R...>::type，
			//若存在这种类型。其他所有情况下，无成员type

			//参数包T中的类型应均为完整类型、(可为cv限定的)void，或未知边界数组，否则行为未定义

			//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

			//特化
			//用户可为类型T1与T2特化common_type，只要

			//1.T1与T2至少一个依赖于用户定义类型，且
			//2.std::decay对于T1和T2都是同等变换

			//若这种特化拥有名为type的成员，则他必须是公开且无歧义的成员类型，并指名一个T1和T2都能显式转换到的，无cv限定且非引用的类型。
			//而且，std::common_type<T1, T2>::type与std::common_type<T2, T1>::type必须指代同一类型

			//以违背这些规则的方式添加common_type特化的程序拥有未定义行为
			//注意添加特化到来自<type_traits>的任何其他模板的行为是未定义的

			//标准库已提供下列特化：
			//std::common_type<std::chrono::duration>
			//std::common_type<std::chrono::time_point>

			namespace common_type_by_cppreference
			{
				template<typename...>
				using void_t = void;

				//初等模板(用于零个类型)
				template<class...>
				struct common_type {};

				//一个类型
				template<class T>
				struct common_type<T> : common_type<T, T> {};

				//二个类型
				template<class T1, class T2>
				using cond_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

				template<class T1, class T2, class=void>
				struct common_type_2_impl {};

				template<class T1, class T2>
				struct common_type_2_impl<T1, T2, void_t<cond_t<T1, T2>>>
				{
					using type = typename std::decay<cond_t<T1, T2>>::type;
				};

				template<class T1, class T2>
				struct common_type<T1, T2> : common_type_2_impl<typename std::decay<T1>::type, typename std::decay<T2>::type>
				{};

				//三个以上类型
				template<class AlwaysVoid, class T1, class T2, class... R>
				struct common_type_multi_impl {};

				template<class T1, class T2, class... R>
				struct common_type_multi_impl<
					void_t<typename common_type<T1, T2>::type>, T1, T2, R...>
					: common_type<typename common_type<T1, T2>::type, R...> {};

				template<class T1, class T2, class... R>
				struct common_type<T1, T2, R...>
					: common_type_multi_impl<void, T1, T2, R...> {};

				template<class... T>
				using common_type_t = typename common_type<T...>::type;
			}

			namespace common_type_by_boost
			{
				//略
			}






			//template<std::size_t Len, std::size_t Align = /*default-alignment*/ >
			//struct aligned_storage;
			//提供嵌套类型type，其为平凡的标准布局类型，适于作为任何大小至多为Len且对齐要求为Align的因数的对象的未初始化存储
			//Align的默认值是任何大小至多为Len的对象的最强(最大)对齐要求。若不使用默认值，则Align对于某类型T必须为alignof(T)的值，否则行为未定义

			//若Len==0则行为未定义

			//是否支持任何扩展对齐是实现定义的

			//添加aligned_storage的特化的程序行为未定义

			//std::aligned_storage<>::type所定义的类型能用于创建适合保有给定类型对象的未初始化内存块，可选地对齐严格于其自然对齐要求，例如在缓存或页边界上

			//同任何其他未初始化存储，通过使用布置new创建对象，并以显式的析构函数调用销毁
			namespace aligned_storage_by_cppreference
			{
				//除了默认实参，aligned_storage能通过alignas表达
				template<std::size_t Len, std::size_t Align /*未实现默认对齐*/>
				struct aligned_storage
				{
					struct type
					{
						alignas(Align) unsigned char data[Len];
					};
				};

				template<std::size_t Len, std::size_t Align>
				using aligned_storage_t = typename aligned_storage<Len, Align>::type;
			}

			namespace aligned_storage_by_boost
			{
				//略
			}








			//template<std::size_t Len, class... Types>
			//struct aligned_union;
			//提供嵌套类型type，他是平凡的标准布局类型，且其大小和对齐适合用作任何列于Types的类型的一个对象的未初始化存储。
			//存储的大小至少为Len。

			//std::aligned_union亦确定所有Types中最严格（最大）的对齐要求，使之可用作常量alignment_value

			//若sizeof...(Types) == 0 或若Types中的任何类型不是完整对象类型，则行为未定义

			//是否支持任何扩展对齐是实现定义的

			//添加aligned_union的特化的程序行为未定义

			namespace aligned_union_by_cppreference
			{
				//#include <algorithm>     std::max
				template<std::size_t Len, class... Types>
				struct aligned_union
				{
					static constexpr std::size_t alignment_value = std::max({ alignof(Types)... });

					struct type
					{
						alignas(alignment_value) char _s[std::max({ Len, sizeof(Types)... })];
					};
				};

				template<std::size_t Len, class... Types>
				using aligned_union_t = typename aligned_union<Len, Types...>::type;
			}









			//result_of
			//C++11起
			//C++17中弃用
			//C++20中移除
			//在 C++17 中被 std::invoke_result 取代而被弃用

			//template<class F, class... ArgTypes>
			//class invoke_result;

			//template<class F, class... Args>
			//std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) noexcept

			//template<class F, class... Args>
			//constexpr std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) noexcept;

			//std::invoke函数模板等价于INVOKE(std::forward<F>(f), std::forward<Args>(args)...) 。其中 INVOKE(f, t1, t2, ..., tN) 定义如下
			//1.若f是类T的指向成员函数的指针：
			//	1.1.若std::is_base_of<T, std::decay_t<decltype(t1)>>::value为true，则相当于(t1.*f)(t2,...,tN)
			//	1.2.若std::decay_t<decltype(t1)>是std::reference_wrapper的特化，则相当于(t1.get().*f)(t2,...,tN)
			//	1.3.若t1不满足前述项目，则相当于((*t1).*f)(t2,...,tN)
			//否则
			//2.若N==1，且f是类T的指向数据成员的指针：
			//	2.1.若std::is_base_of<T, std::decay_t<decltype(t1)>>::value为true，则相当于t1.*f
			//	2.2.若std::decay_t<decltype(t1)>是std::reference_wrapper的特化，则相当于t1.get().*f
			//	2.3.若t1不满足前述项目，则相当于(*t1).*f
			//否则
			//3.INVOKE(f, t1, t2, ..., tN) 等价于 f(t1, t2, ..., tN) （即 f 是一个函数对象）


			//std::invoke_result类模板在编译时推导 INVOKE 表达式的返回类型
			//(C++11起C++14前)F 必须是可调用类型、到函数引用或到可调用类型的引用。以 ArgTypes... 调用 F 必须是良构表达式。
			//(C++14起)F 和 ArgTypes 中的所有类型可以是任何完整类型、未知边界数组或（可有 cv 限定的） void

			namespace invoke_result_invoke_by_cppreference
			{
				//用std::enable_if实现INVOKE
				namespace detail
				{
					template<class T>
					struct is_reference_wrapper : std::false_type {};
					template<class U>
					struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

					template<class Base, class T, class Derived, class... Args>
					auto INVOKE(T Base::* pmf, Derived&& ref, Args&&... args) -> typename std::enable_if<std::is_function<T>::value&& std::is_base_of<Base, typename std::decay<Derived>::type>::value, decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>::type;

					template<class Base, class T, class RefWrap, class... Args>
					auto INVOKE(T Base::* pmf, RefWrap&& ref, Args&&... args) -> typename std::enable_if<std::is_function<T>::value&& is_reference_wrapper<typename std::decay<RefWrap>::type>::value, decltype((ref.get().*pmf)(std::forward<Args>(args)...))>::type;

					template<class Base, class T, class Pointer, class... Args>
					auto INVOKE(T Base::* pmf, Pointer&& ptr, Args&&... args) -> typename std::enable_if<std::is_function<T>::value && !is_reference_wrapper<typename std::decay<Pointer>::type>::value && !std::is_base_of<Base, typename std::decay<Pointer>::type>::value, decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>::type;

					//三个无参数版本(不知道为什么Args&&... args可以匹配0个参数，但还需要下面三个无参数版本)

					template<class Base, class T, class Derived>
					auto INVOKE(T Base::* pmd, Derived&& ref) -> typename std::enable_if<!std::is_function<T>::value&& std::is_base_of<Base, typename std::decay<Derived>::type>::value, decltype(std::forward<Derived>(ref).*pmd)>::type;

					template<class Base, class T, class RefWrap>
					auto INVOKE(T Base::* pmd, RefWrap&& ref) -> typename std::enable_if<!std::is_function<T>::value&& is_reference_wrapper<typename std::decay<RefWrap>::type>::value, decltype(ref.get().*pmd)>::type;

					template<class Base, class T, class Pointer>
					auto INVOKE(T Base::* pmd, Pointer&& ptr) -> typename std::enable_if<!std::is_function<T>::value && !is_reference_wrapper<typename std::decay<Pointer>::type>::value && !std::is_base_of<Base, typename std::decay<Pointer>::type>::value, decltype((*std::forward<Pointer>(ptr)).*pmd)>::type;

					//

					template<class F, class... Args>
					auto INVOKE(F&& f, Args&&... args) -> typename std::enable_if<!std::is_member_pointer<typename std::decay<F>::type>::value, decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type;
				}

				//用if constexpr实现INVOKE
				namespace detail2
				{
					template<class T, class Type, class T1, class... Args>
					constexpr decltype(auto) INVOKE(Type T::* f, T1&& t1, Args&&... args)
					{
						if constexpr (std::is_member_function_pointer_v<decltype(f)>)
						{
							if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
							{
								return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
							}
							else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
							{
								return (t1.get().*f)(std::forward<Args>(args)...);
							}
							else
							{
								return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
							}
						}
						else
						{
							static_assert(std::is_member_object_pointer_v<decltype(f)>);
							static_assert(sizeof...(args) == 0);
							if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
							{
								return std::forward<T1>(t1).*f;
							}
							else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
							{
								return t1.get().*f;
							}
							else
							{
								return (*std::forward<T1>(t1)).*f;
							}
						}
					}

					template<class F, class... Args>
					decltype(auto) INVOKE(F&& f, Args&&... args)
					{
						return std::forward<F>(f)(std::forward<Args>(args)...);
					}
				}

				//invoke_result类模板的实现
				namespace detail
				{
					template<typename AlwaysVoid, typename, typename...>
					struct invoke_result {};

					template<typename F, typename... Args>
					struct invoke_result<decltype(void(INVOKE(std::declval<F>(), std::declval<Args>()...))), F, Args...>
					{
						using type = decltype(INVOKE(std::declval<F>(), std::declval<Args>()...));
					};
				}

				template<class F, class... ArgTypes>
				struct invoke_result : detail::invoke_result<void, F, ArgTypes...> {};

				//invoke函数模板的实现
				template<class F, class... Args>
				constexpr std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args) noexcept(std::is_nothrow_invocable_v<F, Args...>)
				{
					return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
				}
			}










			//#include <boost/type_traits/enable_if.hpp>
			//#include <boost/utility/enable_if.hpp>


			//template<bool B, class T = void>
			//struct enable_if;

			//若B为true，则std::enable_if拥有等同于T的公开成员typedef type；否则，无该成员typedef

			//此元函数是活用SFINAE，基于类型特征条件性地从重载决议移除函数，并对不同类型特征提供分离的函数重载与特化的便利方法。

			//std::enable_if可用作额外的函数参数(不可应用于运算符重载)、返回类型(不可应用于构造函数与析构函数)，或类模板或函数模板形参

			//添加enable_if的特化的程序行为未定义

			//注意：
			//1.常见错误是声明两个函数模板，而他们仅于其默认模板实参相异。这是无效的，因为这些函数声明被当做同一函数模板的再声明(默认模板实参不为函数模板等价所考虑)
			//下面代码是错误的，因为不会发生重载:
			//struct T
			//{
			//	enum {int_t, float_t} m_type;

			//	template<typename Integer, typename = std::enable_if_t<std::is_integral_v<Integer>>>
			//	T(Integer) : m_type(int_t) {}

			//	template<typename Floating, typename = std::enable_if_t<std::is_floating_point_v<Floating>>>
			//	T(Floating) : m_type(float_t) {}
			//};
			//正确方式如下：
			//struct T
			//{
			//	enum {int_t, float_t} m_type;

			//	template<typename Integer, std::enable_if_t<std::is_integral_v<Integer>, int> = 0>
			//	T(Integer) : m_type(int_t) {}

			//	template<typename Floating, std::enable_if_t<std::is_floating_point_v<Floating>, int> = 0>
			//	T(Floating) : m_type(float_t) {}
			//};

			//2.于命名空间函数模板作用域的模板非类型形参中使用enable_if时需留意。某些ABI规范，如Itanium ABI，不于重整中包含
			//非类型模板形参的实例化依赖部分，这表示可能已相同重整名归纳二个相异的函数模板特化，并且错误地相互链接
			
			////第一个编译单元
			//struct X
			//{
			//	enum {value1 = true, value2 = true};
			//};

			//template<class T, std::enable_if_t<T::value1, int> = 0>
			//void func() {}          //#1

			//template void func<X>();//#2

			////第二个编译单元
			//struct X
			//{
			//	enum {value1 = true, value2 = true};
			//};

			//template<class T, std::enable_if_t<T::value2, int> = 0>
			//void func() {}          //#3

			//template void func<X>();//#4
			//函数模板#1与#3拥有不同签名，且为不同的模板，不过#2与#4尽管是不同函数模板的实例化，却于Itaninum C++ ABI拥有相同
			//的重整名(_Z4funcI1XLi0EEvv)，这表示链接器会错误地认为他们是同一实体

			namespace enable_if_by_cppreference
			{
				template<bool B, class T = void>
				struct enable_if {};

				template<class T>
				struct enable_if<true, T> { typedef T type; };
			}

		}
	}

	namespace composite_type_categories
	{
		//检查T是否为非静态成员对象指针类型
		template<class T>
		struct is_member_object_pointer : std::integral_constant<
			bool,
			std::is_member_pointer<T>::value &&
			!std::is_member_function_pointer<T>::value
		> {};

		template<class T>
		inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;









		//检查T是否为算术类型(即整数类型或浮点类型)或其cv限定版本
		template<class T>
		struct is_arithmetic : std::integral_constant<bool,
			std::is_integral<T>::value ||
			std::is_floating_point<T>::value> {};

		template<class T>
		inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;









		//检查T是否为基础类型(即算术类型、void或者nullptr_t)
		template<class T>
		struct is_fundamental : std::integral_constant<bool,
			std::is_arithmetic<T>::value ||
			std::is_void<T>::value ||
			std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value> {};

		template<class T>
		inline constexpr bool is_fundamental_v = is_fundamental<T>::value;








		//检查T是否为符合复合类型，就是非基础类型(即数组、函数、对象指针、函数指针、成员对象指针、成员函数指针、引用、类、联合体
		//或枚举，含任何cv限定变体)
		template<class T>
		struct is_compound : std::integral_constant<bool, !std::is_fundamental<T>::value> {};

		template<class T>
		inline constexpr bool is_compound_v = is_compound<T>::value;











		//检查T是否为标量类型(即可有cv限定的算术、指针、指向成员指针、枚举或std::nullptr_t)

		//(C++ 内存模型中的每个独立内存位置，包括语言特性所用的隐藏内存位置（如虚表指针），
		//都拥有标量类型（或为相邻的零长位域序列）。表达式求值中的副效应排序、线程间同步和
		//依赖顺序全部都基于标量对象定义。)
		template<class T>
		struct is_scalar : std::integral_constant<bool,
			std::is_arithmetic<T>::value ||
			std::is_enum<T>::value ||
			std::is_pointer<T>::value ||
			std::is_member_pointer<T>::value ||
			std::is_null_pointer<T>::value> {};

		template<class T>
		inline constexpr bool is_scalar_v = is_scalar<T>::value;










		//检查T是否为对象类型(即任何函数、引用或void类型外可有cv限定的类型)
		template<class T>
		struct is_object : std::integral_constant<bool,
			std::is_scalar<T>::value ||
			std::is_array<T>::value ||
			std::is_union<T>::value ||
			std::is_class<T>::value> {};

		template<class T>
		inline constexpr bool is_object_v = is_object<T>::value;






	}

	namespace type_properties
	{
		//检查T是否为抽象类(即声明或继承至少一个纯虚函数的非联合类类型)
		namespace is_abstract_by_is_abstract
		{
			template<class T>
			struct is_abstract_imp
			{
				static const bool value = __is_abstract(T);
			};
			template<class T> struct is_abstract : public integral_constant<bool, is_abstract_imp<T>::value>{};

			template<class T>
			inline constexpr bool is_abstract_v = is_abstract<T>::value;
		}





		//检查T是否为const限定类型(即const或const volatile)
		template<class T>
		struct is_const : false_type {};
		
		template<class T>
		struct is_const<const T> : true_type {};

		template<class T>
		inline constexpr bool is_const_v = is_const<T>::value;








		//检查T是否为volatile限定类型(即volatile或const volatile)，对于任何其他类型为false
		template<class T>
		struct is_volatile : std::false_type {};

		template<class T>
		struct is_volatile<volatile T> : std::true_type {};

		template<class T>
		inline constexpr bool is_volatile_v = is_volatile<T>::value;












		//std::is_empty
		//检查T是否为空类类型(即无异于0大小位域的非静态数据成员、无虚函数、无虚基类，且无非空基类的非联合类类型)
		//若T是非联合类类型，则T应该完整类型，否则行为未定义
		//(注意空基类优化，从空类类型继承通常不增加类的大小。std::is_empty<T>及所有其他类型特征均为空类)

		//目前不知道怎么自定义他








		//is_literal_type
		//检查T是否满足所有字面类型的要求
		//注意:
		//C++11起
		//C++17中弃用
		//C++20中移除
		//略










		//is_pod
		//检查T是否是简旧数据类型，即既是平凡又是标准布局
		//注意:
		//C++11起
		//C++20弃用
		//(略)







		//is_polymorphic
		//检查T是否为多态类(即声明或继承至少一个虚函数的非联合类)
		//若T是非联合类类型，则T应为完整类型，否则行为未定义
		namespace is_polymorphic_by_is_polymorphic
		{
			template<class T> struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)> {};

			template<class T>
			inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;
		}

		template<class T>
		true_type detect_is_polymorphic(
			decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr)))
		);
		template<class T>
		false_type detect_is_polymorphic(...);

		template<class T>
		struct is_polymorphic : decltype(detect_is_polymorphic<T>(nullptr)){};

		template<class T>
		inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;









		//检查T是否为算术类型且T(-1) < T(0)
		//(这对于浮点类型和有符号整数类型产生true，并对无符号整数类型和bool类型产生false)
		//(对于任何其他类型为false)
		namespace detail
		{
			template<typename T, bool = std::is_arithmetic<T>::value>
			struct is_signed : std::integral_constant<bool, T(-1) < T(0) > {};

			template<typename T>
			struct is_signed<T, false> : std::false_type {};
		}

		template<typename T>
		struct is_signed : detail::is_signed<T>::type {};

		template<typename T>
		inline constexpr bool is_signed_v = is_signed<T>::value;

		//boost库中提供的实现比较复杂，这里省略










		//检查T是否为算术类型且T(0)<T(-1)
		//(这对于无符号整数类型和bool类型产生true并对有符号整数类型和浮点类型产生false)
		//(对于任何其他类型为false)
		namespace detail
		{
			template<typename T, bool = std::is_arithmetic<T>::value>
			struct is_unsigned : std::integral_constant<bool, T(0) < T(-1) > {};

			template<typename T>
			struct is_unsigned<T, false> : std::false_type {};
		}

		template<typename T>
		struct is_unsigned : detail::is_unsigned<T>::type {};

		template<class T>
		inline constexpr bool is_unsigned_v = is_unsigned<T>::value;







		//检查T是否为标准布局类型(即标量类型、标准布局类、或这种类型/类的数组，可以有cv限定)
		//标准布局类是满足标准布局类型(StandardLayoutType)的类 https://zh.cppreference.com/w/cpp/named_req/StandardLayoutType
		//(若std::remove_all_extents_t<T>是不完整类型且非(可有cv限定的)void则行为未定义)

		//指向标准布局类的指针能转换(以reinterpret_cast)成指向其首个非静态数据成员的指针，反之亦然
		//若标准布局联合体保有二个或多个标准布局结构体，则允许查看他们的公共前导部分
		//宏offsetof只保证能用于标准布局类





		//is_trivially_copyable
		//检查T是否为可平凡复制类型，对于任何其他类型为false
		//(仅有的可平凡复制类型是标量类型、可平凡复制类及这些类型/类的数组(可以为cv限定))
		//(若std::remove_all_extents_t<T>是非(可有cv限定的)void的不完整类型，则行为未定义)

		//可平凡复制类型的，非潜在重叠子对象的对象是仅有的能以std::memcpy安全复制或以std::ofstream::write()/std::ifstream::read()
		//序列化自/到二进制文件的对象






		//检查T是否为平凡类型(即标量类型、有平凡默认构造函数的可平凡复制类，或这些类/类型的数组，可有cv限定)
		//(若std::remove_all_extents_t<T>是(可有cv限定)void以外的不完整类型，则行为未定义)
		template<class T>
		struct is_trivial : std::integral_constant<
			bool,
			std::is_trivially_copyable<T>::value &&
			std::is_trivially_default_constructible<T>::value> {};

		template<class T>
		inline constexpr bool is_trivial_v = is_trivial<T>::value;
	}

	namespace type_features
	{
		//检查T是否为拥有虚析构函数的类型
		//(若T是非联合类类型，则T应为完整类型，否则行为未定义)

		//若类拥有公开虚析构函数，则可以从他派生，且可以通过指向基类的指针安全地删除派生类
		template<class T> struct has_virtual_destructor : public integral_constant<bool, __has_virtual_destructor(T)>{};
		
		template<class T>
		inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;






		//is_assignable
		//is_trivially_assignable
		//is_nothrow_assignable

		//检查表达式std::declval<T>() = std::declval<U>()在不求值语境为良态
		
		//is_trivially_assignable同is_assignable，但赋值表达式的求值将不调用任何非平凡的操作。为此检查的目的，对std::declval的调用
		//被认为是平凡，且不是std::declval的odr使用

		//is_nothrow_assignable同is_assignable，但复制表达式的求值将不调用任何不是noexcept的操作

		//T与U应均为完整类型、（可为cv限定的）void，或未知边界数组，否则行为未定义
		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//注意，他不检查任何赋值表达式立即语境之外的任何事物：若T或U的使用会触发模板特化，生成隐式定义的成员函数，而他们拥有
		//错误，则实际赋值可能无法编译，即使std::is_assignable<T, U>::value通过编译并求值为true

		//declval
		template<typename T>
		typename std::add_rvalue_reference<T>::type declval() noexcept;  //as unevaluated operand  作为未赋值的操作数

		//is_complete
		namespace detail
		{
			template<unsigned N>
			struct ok_tag
			{
				double d;
				char c[N];
			};

			template<class T>
			ok_tag<sizeof(T)> check_is_complete(int);
			template<class T>
			char check_is_complete(...);
		}
		template<class T>
		struct is_complete : public integral_constant<bool,
			std::is_function<typename std::remove_reference<T>::type>::value ||
			(sizeof(detail::check_is_complete<T>(0)) != sizeof(char))> {};

		namespace detail
		{
			//yes_type
			using yes_type = char;

			//no_type
			struct no_type
			{
				char padding[8];
			};

			//is_assignable_imp
			struct is_assignable_imp
			{
				template<typename T, typename U, typename = decltype(declval<T>() = declval<U>())>
				static yes_type test(int);

				template<typename, typename>
				static no_type test(...);
			};
		}

		//is_assignable
		template<class T, class U>
		struct is_assignable : public integral_constant<bool, sizeof(detail::is_assignable_imp::test<T, U>(0)) == sizeof(detail::yes_type)>
		{
			static_assert(is_complete<T>::value, "Arguments to is_assignable must be complete types");
		};
		template<class T, std::size_t N, class U>
		struct is_assignable<T[N], U> : public is_assignable<T, U> {};
		template<class T, std::size_t N, class U>
		struct is_assignable<T(&)[N], U> : public is_assignable<T&, U> {};
		template<class T, class U>
		struct is_assignable<T[], U> : public is_assignable<T, U> {};
		template<class T, class U>
		struct is_assignable<T(&)[], U> : public is_assignable<T&, U> {};
		template<class U>
		struct is_assignable<void, U> : public integral_constant<bool, false> {};
		template<class U>
		struct is_assignable<void const, U> : public integral_constant<bool, false> {};
		template<class U>
		struct is_assignable<void volatile, U> : public integral_constant<bool, false> {};
		template<class U>
		struct is_assignable<void const volatile, U> : public integral_constant<bool, false> {};

		//is_assignable_v
		template<class T, class U>
		inline constexpr bool is_assignable_v = is_assignable<T, U>::value;













		//is_copy_assignable
		//is_trivially_copy_assignable
		//is_nothrow_copy_assignable

		//若T不是可引用类型（即可为cv限定的void或带cv-qualifier-seq或ref-qualifier的函数类型），则为false。否则与std::is_assignable<T&, constT&>一致

		//is_trivially_copy_assignable同is_copy_assignable，但使用std::is_trivially_assignable<T&, const T&>

		//is_nothrow_copy_assignable同is_copy_assignable，但使用std::is_nothrow_assignable<T&, const T&>

		//T应为完整类型、(可为cv限定的)void，或未知边界数组，否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//注意，特征std::is_copy_assignable不如可复制赋值(CopyAssignable)严格，因为他不检查赋值的结果类型（对于可复制赋值(CopyAssignable)类型必须是T类型的左值）
		//且不检查赋值表达式保留未更改的语义要求。他亦不检查T是否满足对所有可复制赋值(CopyAssignable)类型要求的可移动赋值(MoveAssignable)

		//template<class T>
		//struct is_copy_assignable
		//	: std::is_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_lvalue_reference<const T>::type
		//	> {};

		//template<class T>
		//struct is_trivially_copy_assignable
		//	: std::is_trivially_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_lvalue_reference<const T>::type
		//	> {};

		//template<class T>
		//struct is_nothrow_copy_assignable
		//	: std::is_nothrow_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_lvalue_reference<const T>::type
		//	> {};

		//template<class T>
		//inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

		//template<class T>
		//inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

		//template<class T>
		//inline constexpr bool is_nothrow_copy_assignable = is_nothrow_copy_assignable<T>::value;

		//is_base_and_derived
		namespace detail
		{
			template<typename B, typename D>
			struct is_base_and_derived_impl
			{
				using ncvB = std::remove_cv<B>;
				using ncvD = std::remove_cv<D>;

				static const bool value = ((__is_base_of(B, D) && !std::is_same<B, D>::value) && !std::is_same<ncvB, ncvD>::value);
			};
		}
		template<class Base, class Derived>
		struct is_base_and_derived : public integral_constant<bool, (detail::is_base_and_derived_impl<Base, Derived>::value)> {};
		template<class Base, class Derived>
		struct is_base_and_derived<Base&, Derived> : public false_type {};
		template<class Base, class Derived>
		struct is_base_and_derived<Base, Derived&> : public false_type {};
		template<class Base, class Derived>
		struct is_base_and_derived<Base&, Derived&> : public false_type {};

		//  Private copy constructor and copy assignment ensure classes derived from
		//  class noncopyable cannot be copied.
		namespace noncopyable_
		{
			// noncopyable derives from base_token to enable Type Traits to detect
			// whether a type derives from noncopyable without needing the definition
			// of noncopyable itself.
			//
			// The definition of base_token is macro-guarded so that Type Trais can
			// define it locally without including this header, to avoid a dependency
			// on Core.  (boost/core/noncopyable.hpp)

			struct base_token {};

			class noncopyable : base_token
			{
				constexpr noncopyable() = default;
				~noncopyable() = default;

				noncopyable(const noncopyable&) = delete;
				noncopyable& operator=(const noncopyable&) = delete;
			};
		}
		using noncopyable = noncopyable_::noncopyable;


		// boost::noncopyable derives from noncopyable_::base_token to enable us
		// to recognize it. The definition is macro-guarded so that we can replicate
		// it here without including boost/core/noncopyable.hpp, which is in Core.
		//namespace noncopyable_
		//{
		//	struct base_token {};
		//}
		template<class T>
		struct is_noncopyable : is_base_and_derived<noncopyable_::base_token, T> {};

		namespace detail
		{
			template<bool DerivedFromNoncopyable, class T>
			struct is_copy_assignable_impl2
			{
				template<class U>
				static decltype(declval<U&>() = declval<const U&>(), yes_type()) test(int);

				template<class>
				static no_type test(...);

				static const bool value = sizeof(test<T>(0)) == sizeof(yes_type);
			};

			template<class T>
			struct is_copy_assignable_impl2<true, T>
			{
				static const bool value = false;
			};

			template<class T>
			struct is_copy_assignable_impl
			{
				static const bool value = is_copy_assignable_impl2<is_noncopyable<T>::value, T>::value;
			};
		}
		template<class T>
		struct is_copy_assignable : public integral_constant<bool, detail::is_copy_assignable_impl<T>::value> {};
		template<> struct is_copy_assignable<void> : public false_type {};
		template<> struct is_copy_assignable<void const> : public false_type {};
		template<> struct is_copy_assignable<void const volatile> : public false_type {};
		template<> struct is_copy_assignable<void volatile> : public false_type {};

		template<typename T>
		inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;








		//is_move_assignable
		//is_trivially_move_assignable
		//is_nothrow_move_assignable

		//若T不是可引用类型(即可为cv限定的void或带cv-qualifier-seq或ref-qualifier的函数类型)，则为false。否则等效于std::is_assignable<T&, T&&>
		//is_trivially_move_assignable同is_move_assignable，但使用std::is_trivially_assignable<T&, T&&>
		//is_nothrow_move_assignable同is_move_assignable，但使用std::is_nothrow_assignable<T&, T&&>

		//T应为完整类型、（可为cv限定的）void，或未知边界数组，否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//特征std::is_move_assignable不如可移动赋值(MoveAssignable)严格，因为他既不检查赋值的结果类型(对于可移动赋值(MoveAssignable)类型
		//必须是T&)，也不检查赋值后目标的值等于赋值前源的值的语义要求

		//为满足此特征，类型不必实现移动赋值运算符，细节见可移动赋值(MoveAssignable)

		//template<class T>
		//struct is_move_assignable
		//	: std::is_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_rvalue_reference<T>::type
		//	> {};
		//template<class T>
		//struct is_trivially_move_assignable
		//	: std::is_trivially_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_rvalue_reference<T>::type
		//	> {};
		//template<class T>
		//struct is_nothrow_move_assignable
		//	: std::is_nothrow_assignable<
		//	typename std::add_lvalue_reference<T>::type,
		//	typename std::add_rvalue_reference<T>::type
		//	> {};

		//template<class T>
		//inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
		//template<class T>
		//inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assinable<T>::value;
		//template<class T>
		//inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;


		template<class T>
		struct is_nothrow_move_assignable : public integral_constant<bool, __is_nothrow_assignable(T&, T&&)>
		{
			static_assert(is_complete<T>::value, "Arguments to is_nothrow_move_assignable must be complete types");
		};
		template<class T> struct is_nothrow_move_assignable<T const> : public false_type {};
		template<class T> struct is_nothrow_move_assignable<T volatile> : public false_type {};
		template<class T> struct is_nothrow_move_assignable<T const volatile> : public false_type {};
		template<class T> struct is_nothrow_move_assignable<T&> : public false_type {};
		template<class T> struct is_nothrow_move_assignable<T&&> : public false_type {};
		template<> struct is_nothrow_move_assignable<void> : public false_type {};
		template<> struct is_nothrow_move_assignable<void const> : public false_type {};
		template<> struct is_nothrow_move_assignable<void const volatile> : public false_type {};
		template<> struct is_nothrow_move_assignable<void volatile> : public false_type {};

		template<typename T>
		inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;









		//is_destructible
		//is_trivially_destructible
		//is_nothrow_destructible
		//若T是引用类型，则为true
		//若T是不完整类型（包含void）或函数类型，则为false
		//若T是对象类型，则对于等于std::remove_all_extents<T>::type的类型U，若表达式std::declval<U&>().~U()在不求值语境合法，则为true，否则为false

		//is_trivially_destructible同is_destructible，并且std::remove_all_extents<T>::type要么是非类类型，要么是拥有平凡析构函数的类类型

		//is_nothrow_destructible同is_destructible，但析构函数为noexcept

		//T应为完整类型、（可为cv限定的）void，或未知边界数组。否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假设使该类型完整，实例化就会产生不同的结果，则行为未定义

		//因为若析构函数在栈回溯中抛出异常则C++程序终止（通常是无法预期的），所有实用的析构函数均为不抛出，即使他们不声明为noexcept。
		//所有C++标准库中能找到的析构函数都是不抛出的
		//可平凡析构对象所占用的存储可以重用而无需调用析构函数

		namespace detail
		{
			struct is_destructible_imp
			{
				template<typename T, typename = decltype(declval<T&>().~T())>
				static yes_type test(int);
				template<typename>
				static no_type test(...);
			};
		}
		template<class T> struct is_destructible : public integral_constant<bool, sizeof(detail::is_destructible_imp::test<T>(0)) == sizeof(detail::yes_type)>
		{
			static_assert(is_complete<T>::value, "Arguments to is_destructible must be complete types");
		};

		template<class T>
		inline constexpr bool is_destructible_v = is_destructible<T>::value;







		//is_default_constructible
		//is_trivially_default_constructible
		//is_nothrow_default_constructible
		//对于is_default_constructible，若std::is_constructible<T>为true，则为true，否则为false
		//对于is_trivially_default_constructible，若std::is_trivially_constructible<T>为true，则为true，否则为false
		//对于is_nothrow_default_constructible，若std::is_nothrow_constructible<T>为true，则为true，否则为false

		//T应为完整类型、(可为cv限定的)void，或未知边界数组，否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//多数实现中，is_nothrow_default_constructible亦检查析构函数是否抛出，因为他等效于noexcept(T())。同样的情况应用于
		//is_trivially_default_constructible，他在这些实现中亦要求析构函数为平凡

		//template<class T>
		//struct is_default_constructible : std::is_constructible<T> {};
		//template<class T>
		//struct is_trivially_default_constructible : std::is_trivially_constructible<T> {};
		//template<class T>
		//struct is_nothrow_default_constructible : std::is_nothrow_constructible<T> {};

		//template<class T>
		//inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

		namespace detail
		{
			struct is_default_constructible_imp
			{
				template<typename _Tp, typename = decltype(_Tp())>
				static yes_type test(int);

				template<typename>
				static no_type test(...);
			};
		}
		template<class T> struct is_default_constructible : public integral_constant<bool, sizeof(detail::is_default_constructible_imp::test<T>(0)) == sizeof(detail::yes_type)>
		{
			static_assert(is_complete<T>::value, "Arguments to is_default_constructible must be complete types");
		};
		template<class T, std::size_t N> struct is_default_constructible<T[N]> : public is_default_constructible<T> {};
		template<class T> struct is_default_constructible<T[]> : public is_default_constructible<T> {};
		template<class T> struct is_default_constructible<T&> : public integral_constant<bool, false> {};
		template<class T> struct is_default_constructible<T&&> : public integral_constant<bool, false> {};
		template<> struct is_default_constructible<void> : public integral_constant<bool, false> {};
		template<> struct is_default_constructible<void const> : public integral_constant<bool, false> {};
		template<> struct is_default_constructible<void volatile> : public integral_constant<bool, false> {};
		template<> struct is_default_constructible<void const volatile> : public integral_constant<bool, false> {};

		template<typename T>
		inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;










		//is_constructible
		//is_trivially_constructible
		//is_nothrow_constructible

		//检查T是对象或引用类型，且变量定义T obj(std::declval<Args>()...);为良构，所有其他情况下为false

		//为此检查的目的，决不转译变量定义为函数声明，且不认为std::declval是odr使用。如同从与T和任何Args中类型无关语境
		//进行访问检查，仅考虑变量定义的立即语境的合法性

		//is_trivially_constructible同is_constructible，但变量定义不调用任何非平凡操作，为此检查的目的，对std::declval的调用被认为是平凡的

		//is_nothrow_constructible同is_constructible，但变量定义为noexcept

		//T与参数包Args中的所有类型应均为完整类型、（可为cv限定的）void，或未知边界数组，否则行为未定义
		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//多数实现中，is_nothrow_constructible亦检查析构函数是否抛出，因为他等效于noexcept(T(arg))，同样的情况应用于is_trivially_constructible，
		//在这些实现中亦要求析构函数为平凡

		namespace detail
		{
			struct is_constructible_imp
			{
				template<typename T, typename... TheArgs, typename = decltype(T(declval<TheArgs>()...))>
				static yes_type test(int);
				template<typename, typename...>
				static no_type test(...);

				template<typename T, typename Arg, typename = decltype(::new T(boost::declval<Arg>()))>
				static yes_type test1(int);
				template<typename, typename>
				static no_type test1(...);

				template<typename T>
				static yes_type ref_test(T);
				template<typename T>
				static no_type ref_test(...);
			};
		}

		template<class T, class... Args> struct is_constructible : public integral_constant<bool, sizeof(detail::is_constructible_imp::test<T, Args...>(0)) == sizeof(detail::yes_type)>
		{
			static_assert(is_complete<T>::value, "The target type must be complete in order to test for constructibility");
		};
		template<class T, class Arg> struct is_constructible<T, Arg> : public integral_constant<bool,
			is_destructible<T>::value && sizeof(detail::is_constructible_imp::test1<T, Arg>(0)) == sizeof(detail::yes_type)>
		{
			static_assert(is_complete<T>::value, "The target type must be complete in order to test for constructibility");
		};
		template<class Ref, class Arg> struct is_constructible<Ref&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&>(declval<Arg>())) == sizeof(detail::yes_type)> {};
		template<class Ref, class Arg> struct is_constructible<Ref&&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&&>(declval<Arg>())) == sizeof(detail::yes_type)> {};
		template<> struct is_constructible<void> : public false_type {};
		template<> struct is_constructible<void const> : public false_type {};
		template<> struct is_constructible<void const volatile> : public false_type {};
		template<> struct is_constructible<void volatile> : public false_type {};

		template<class T> struct is_constructible<T> : public is_default_constructible<T> {};

		template<class T, class... Args>
		inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;









		//is_copy_constructible
		//is_trivially_copy_constructible
		//is_nothrow_copy_contructible
		//若T不是可引用类型（即可为cv限定的void或带cv-qualifier-seq或ref-qualifier的函数类型），则为false。否则等效于std::is_constructible<T, const T&>

		//is_trivially_copy_constructible同is_copy_constructible，但使用std::is_trivially_constructible<T, const T&>

		//is_nothrow_copy_contructible同is_copy_constructible，但使用std::is_nothrow_constructible<T, const T&>

		//T应为完整类型、(可为cv限定的)void，或未知边界数组，否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//多数实现中，is_nothrow_copy_constructible亦检查析构函数是否抛出，因为他等效于noexcept(T(arg))。同样的情况应用于
		//is_trivially_copy_constructible，他在这些实现中亦要求析构函数为平凡
		//template<class T>
		//struct is_copy_constructible :
		//	std::is_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type> {};
		//template<class T>
		//struct is_trivially_copy_constructible :
		//	std::is_trivially_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type> {};
		//template<class T>
		//struct is_nothrow_copy_constructible :
		//	std::is_nothrow_constructible<T, typename std::add_lvalue_reference<typename std::add_const<T>::type>::type> {};

		//template<class T>
		//inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

		//boost中的实现
		template<class T> struct is_copy_constructible : public is_constructible<T, const T&> {};
		
		template<> struct is_copy_constructible<void> : public false_type {};
		template<> struct is_copy_constructible<void const> : public false_type {};
		template<> struct is_copy_constructible<void const volatile> : public false_type {};
		template<> struct is_copy_constructible<void volatile> : public false_type {};

		template<typename T>
		inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;












		//is_move_constructible
		//is_trivially_move_constructible
		//is_nothrow_move_constructible
		//若T不是可引用类型（即可为cv限定的void或带cv-qualifier-seq或ref-qualifier的函数类型），则为false，否则等效于std::is_constructible<T, T&&>
		//is_trivially_move_constructible同is_move_constructible，但使用std::is_trivially_constructible<T, T&&>
		//is_nothrow_move_constructible同is_move_constructible，但使用std::is_nothrow_constructible<T, T&&>

		//T应为完整类型、（可为cv限定的）void，或未知边界数组，否则行为未定义

		//若上述模板的实例化直接或间接地依赖于不完整类型，并且如果假如使该类型完整，实例化就会产生不同的结果，则行为未定义

		//无移动构造函数的但有接受const T&参数的复制构造函数对的类型，满足std::is_move_constructible
		//移动构造函数通常是noexcept，因为否则他们就不可用于任何提供强异常保证的代码中
		//多数实现中，is_nothrow_move_constructible亦检查析构函数是否抛出，因为他等效于noexcept(T(arg))。同样的情况应用于
		//is_trivially_move_constructible，他在这些实现中亦要求析构函数为平凡
		//template<class T>
		//struct is_move_constructible :
		//	std::is_constructible<T, typename std::add_rvalue_reference<T>::type> {};
		//template<class T>
		//struct is_trivially_move_constructible :
		//	std::is_trivially_constructible<T, typename std::add_rvalue_reference<T>::type> {};
		//template<class T>
		//struct is_nothrow_move_constructible :
		//	std::is_nothrow_constructible<T, typename std::add_rvalue_reference<T>::type> {};

		//template<class T>
		//inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;
		//template<class T>
		//inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

		template<class T>
		struct is_nothrow_move_constructible : public integral_constant<bool, __is_nothrow_constructible(T, T&&) >
		{
			static_assert(is_complete<T>::value, "Arguments to is_nothrow_move_constructible must be complete types");
		};
		template<class T> struct is_nothrow_move_constructible<volatile T> : public false_type {};
		template<class T> struct is_nothrow_move_constructible<const volatile T> : public false_type {};

		template<> struct is_nothrow_move_constructible<void> : false_type {};
		template<> struct is_nothrow_move_constructible<void const> : false_type {};
		template<> struct is_nothrow_move_constructible<void volatile> : false_type {};
		template<> struct is_nothrow_move_constructible<void const volatile> : false_type {};
		template<class T> struct is_nothrow_move_constructible<T&> : public true_type {};
		template<class T> struct is_nothrow_move_constructible<T&&> : public true_type {};

		template<typename T>
		inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;
	}


	namespace c_std_is_array
	{
		class A
		{};
	}
	namespace c_boost_is_array
	{
		class A
		{};
	}
	namespace c_test_is_array
	{
		class A
		{};
	}
	namespace c_std_is_union
	{
		struct A {};

		typedef union {
			int a;
			float b;
		}B;

		struct C {
			B d;
		};
	}
	namespace c_boost_is_union
	{
		struct A {};

		typedef union {
			int a;
			float b;
		}B;

		struct C {
			B d;
		};
	}
	namespace c_test_is_union
	{
		struct A {};

		typedef union {
			int a;
			float b;
		}B;

		struct C {
			B d;
		};
	}
	namespace c_std_is_class
	{
		struct A {};
		class B {};
		enum class C {};
	}
	namespace c_boost_is_class
	{
		struct A {};
		class B {};
		enum class C {};
	}
	namespace c_test_is_class
	{
		struct A {};
		class B {};
		enum class C {};
	}
	namespace c_std_is_enum
	{
		class A {};
		enum E {};
		enum class Ec : int {};
	}
	namespace c_boost_is_enum
	{
		class A {};
		enum E {};
		enum class Ec : int {};
	}
	namespace c_test_is_enum
	{
		class A {};
		enum E {};
		enum class Ec : int {};
	}
	namespace c_std_is_floating_point
	{
		class A {};
	}
	namespace c_boost_is_floating_point
	{
		class A {};
	}
	namespace c_test_is_floating_point
	{
		class A {};
	}
	namespace c_std_is_function
	{
		struct A
		{
			int fun() const&;
		};

		template<typename>
		struct PM_traits {};

		template<class T, class U>
		struct PM_traits<U T::*>
		{
			using member_type = U;
		};

		int f();
	}
	namespace c_boost_is_function
	{
		struct A
		{
			int fun() const&;
		};

		template<typename>
		struct PM_traits {};

		template<class T, class U>
		struct PM_traits<U T::*>
		{
			using member_type = U;
		};

		int f();
	}
	namespace c_test_is_function
	{
		struct A
		{
			int fun() const&;
		};

		template<typename>
		struct PM_traits {};

		template<class T, class U>
		struct PM_traits<U T::*>
		{
			using member_type = U;
		};

		int f();
	}
	namespace c_std_is_integral
	{
		class A {};

		enum E : int {};

		template<class T>
		T f(T i)
		{
			static_assert(std::is_integral<T>::value, "Integral required.");
			return i;
		}
	}
	namespace c_boost_is_integral
	{
		class A {};

		enum E : int {};

		template<class T>
		T f(T i)
		{
			static_assert(std::is_integral<T>::value, "Integral required.");
			return i;
		}
	}
	namespace c_test_is_integral
	{
		class A {};

		enum E : int {};

		template<class T>
		T f(T i)
		{
			static_assert(std::is_integral<T>::value, "Integral required.");
			return i;
		}
	}
	namespace c_std_is_pointer
	{
		class A {};
	}
	namespace c_boost_is_pointer
	{
		class A {};
	}
	namespace c_test_is_pointer
	{
		class A {};
	}
	namespace c_std_is_lvalue_reference
	{
		class A {};
	}
	namespace c_boost_is_lvalue_reference
	{
		class A {};
	}
	namespace c_test_is_lvalue_reference
	{
		class A {};
	}
	namespace c_std_is_rvalue_reference
	{
		class A {};
	}
	namespace c_boost_is_rvalue_reference
	{
		class A {};
	}
	namespace c_test_is_rvalue_reference
	{
		class A {};
	}
	namespace c_std_is_reference
	{
		class A {};
	}
	namespace c_boost_is_reference
	{
		class A {};
	}
	namespace c_test_is_reference
	{
		class A {};
	}
	namespace c_std_is_member_pointer
	{
		class cls {};
	}
	namespace c_boost_is_member_pointer
	{
		class cls {};
	}
	namespace c_test_is_member_pointer
	{
		class cls {};
	}
	namespace c_std_is_member_function_pointer
	{
		class A
		{
		public:
			void member() {}
		};
	}
	namespace c_boost_is_member_function_pointer
	{
		class A
		{
		public:
			void member() {}
		};
	}
	namespace c_test_is_member_function_pointer
	{
		class A
		{
		public:
			void member() {}
		};
	}
	namespace c_std_is_base_of
	{
		class A {};
		class B : A {};
		class C {};
	}
	namespace c_boost_is_base_of
	{
		class A {};
		class B : A {};
		class C {};
	}
	namespace c_test_is_base_of
	{
		class A {};
		class B : A {};
		class C {};
	}
	namespace c_std_is_convertible
	{
		class E
		{
		public:
			template<class T>
			E(T&&) {}
		};

		class A {};
		class B : public A {};
		class C {};
		class D { public: operator C() { return c; } C c; };
	}
	namespace c_boost_is_convertible
	{
		class E
		{
		public:
			template<class T>
			E(T&&) {}
		};

		class A {};
		class B : public A {};
		class C {};
		class D { public: operator C() { return c; } C c; };
	}
	namespace c_test_is_convertible
	{
		class E
		{
		public:
			template<class T>
			E(T&&) {}
		};

		class A {};
		class B : public A {};
		class C {};
		class D { public: operator C() { return c; } C c; };
	}
	namespace c_std_alignment_of
	{
		class A {};
	}
	namespace c_boost_alignment_of
	{
		class A {};
	}
	namespace c_test_alignment_of
	{
		class A {};
	}
	namespace c_std_add_cv
	{
		struct foo
		{
			void m()
			{
				std::cout << "non-cv\n";
			}
			void m() const
			{
				std::cout << "const\n";
			}
			void m() volatile
			{
				std::cout << "volatile\n";
			}
			void m() const volatile
			{
				std::cout << "const-volatile\n";
			}
		};
	}
	namespace c_boost_add_cv
	{
		struct foo
		{
			void m()
			{
				std::cout << "non-cv\n";
			}
			void m() const
			{
				std::cout << "const\n";
			}
			void m() volatile
			{
				std::cout << "volatile\n";
			}
			void m() const volatile
			{
				std::cout << "const-volatile\n";
			}
		};
	}
	namespace c_test_add_cv
	{
		struct foo
		{
			void m()
			{
				std::cout << "non-cv\n";
			}
			void m() const
			{
				std::cout << "const\n";
			}
			void m() volatile
			{
				std::cout << "volatile\n";
			}
			void m() const volatile
			{
				std::cout << "const-volatile\n";
			}
		};
	}
	namespace c_std_remove_extent
	{
		template<class A>
		typename std::enable_if<std::rank<A>::value == 1>::type print(const A& a)
		{
			copy(a, a + std::extent<A>::value, std::ostream_iterator<typename std::remove_extent<A>::type>(std::cout, " "));
			std::cout << '\n';
		}
	}
	namespace c_boost_remove_extent
	{
		template<class A>
		typename std::enable_if<std::rank<A>::value == 1>::type print(const A& a)
		{
			copy(a, a + std::extent<A>::value, std::ostream_iterator<typename std::remove_extent<A>::type>(std::cout, " "));
			std::cout << '\n';
		}
	}
	namespace c_test_remove_extent
	{
		template<class A>
		typename std::enable_if<std::rank<A>::value == 1>::type print(const A& a)
		{
			copy(a, a + std::extent<A>::value, std::ostream_iterator<typename std::remove_extent<A>::type>(std::cout, " "));
			std::cout << '\n';
		}
	}
	namespace c_std_remove_all_extents
	{
		template<class A>
		void foo(const A&)
		{
			typedef typename std::remove_all_extents<A>::type Type;
			std::cout << "underlying type:" << typeid(Type).name() << '\n';
		}
	}
	namespace c_boost_remove_all_extents
	{
		template<class A>
		void foo(const A&)
		{
			typedef typename std::remove_all_extents<A>::type Type;
			std::cout << "underlying type:" << typeid(Type).name() << '\n';
		}
	}
	namespace c_test_remove_all_extents
	{
		template<class A>
		void foo(const A&)
		{
			typedef typename std::remove_all_extents<A>::type Type;
			std::cout << "underlying type:" << typeid(Type).name() << '\n';
		}
	}
	namespace c_std_decay
	{
		template<typename T, typename U>
		struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type {};

		template<typename T, typename U>
		inline constexpr bool decay_equiv_v = std::is_same_v<typename std::decay_t<T>, U>;
	}
	namespace c_boost_decay
	{
		template<typename T, typename U>
		struct decay_equiv : std::is_same<typename boost::decay<T>::type, U>::type {};
	}
	namespace c_test_decay
	{
		template<typename T, typename U>
		struct decay_equiv : std::is_same<typename test_type_traits::type_transformations::compound_type_alterations::decay_by_cppreference::decay<T>::type, U>::type {};

		template<typename T, typename U>
		struct decay_equiv_boost : std::is_same<typename test_type_traits::type_transformations::compound_type_alterations::decay_by_boost::decay<T>::type, U>::type {};
	}
	namespace c_std_underlying_type
	{
		enum e1 {};
		enum class e2 :int {};
	}
	namespace c_boost_underlying_type
	{
		enum e1 {};
		enum class e2 :int {};
	}
	namespace c_test_underlying_type
	{
		enum e1 {};
		enum class e2 :int {};
	}
	namespace c_std_common_type
	{
		template<class T>
		struct Number
		{
			T n;
		};

		template<class T, class U>
		Number<typename std::common_type<T, U>::type> operator+(const Number<T>& lhs, const Number<U>& rhs)
		{
			return { lhs.n + rhs.n };
		}
	}
	namespace c_boost_common_type
	{
		template<class T>
		struct Number
		{
			T n;
		};

		template<class T, class U>
		Number<typename boost::common_type<T, U>::type> operator+(const Number<T>& lhs, const Number<U>& rhs)
		{
			return { lhs.n + rhs.n };
		}
	}
	namespace c_test_common_type
	{
		template<class T>
		struct Number
		{
			T n;
		};

		template<class T, class U>
		Number<typename test_type_traits::type_transformations::other_type_generators::common_type_by_cppreference::common_type<T, U>::type> operator+(const Number<T>& lhs, const Number<U>& rhs)
		{
			return { lhs.n + rhs.n };
		}
	}
	namespace c_std_aligned_storage
	{
		//原始的静态vector类，演示于对齐存储中创建、访问及析构对象
		template<class T, std::size_t N>
		class static_vector
		{
			//N个T的正确对齐的未初始化存储
			typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
			std::size_t m_size = 0;

		public:
			//于对齐存储创建对象
			template<typename... Args>
			void emplace_back(Args&&... args)
			{
				if (m_size >= N)//可行的错误处理
					throw std::bad_alloc{};
				new (data + m_size) T(std::forward<Args>(args)...);
				++m_size;
			}

			//访问对齐存储中的对象
			const T& operator[](std::size_t pos) const
			{
				//注意：C++17起需要std::launder
				return *reinterpret_cast<const T*>(data + pos);
			}

			//从对齐存储删除对象
			~static_vector()
			{
				for (std::size_t pos = 0; pos < m_size; ++pos)
				{
					//注意：C++17起需要std::launder
					reinterpret_cast<T*>(data + pos)->~T();
				}
			}
		};
	}
	namespace c_boost_aligned_storage
	{
		//原始的静态vector类，演示于对齐存储中创建、访问及析构对象
		template<class T, std::size_t N>
		class static_vector
		{
			//N个T的正确对齐的未初始化存储
			typename boost::aligned_storage<sizeof(T), alignof(T)>::type data[N];
			std::size_t m_size = 0;

		public:
			//于对齐存储创建对象
			template<typename... Args>
			void emplace_back(Args&&... args)
			{
				if (m_size >= N)//可行的错误处理
					throw std::bad_alloc{};
				new (data + m_size) T(std::forward<Args>(args)...);
				++m_size;
			}

			//访问对齐存储中的对象
			const T& operator[](std::size_t pos) const
			{
				//注意：C++17起需要std::launder
				return *reinterpret_cast<const T*>(data + pos);
			}

			//从对齐存储删除对象
			~static_vector()
			{
				for (std::size_t pos = 0; pos < m_size; ++pos)
				{
					//注意：C++17起需要std::launder
					reinterpret_cast<T*>(data + pos)->~T();
				}
			}
		};
	}
	namespace c_test_aligned_storage
	{
		//原始的静态vector类，演示于对齐存储中创建、访问及析构对象
		template<class T, std::size_t N>
		class static_vector
		{
			//N个T的正确对齐的未初始化存储
			typename test_type_traits::type_transformations::other_type_generators::aligned_storage_by_cppreference::aligned_storage<sizeof(T), alignof(T)>::type data[N];
			std::size_t m_size = 0;

		public:
			//于对齐存储创建对象
			template<typename... Args>
			void emplace_back(Args&&... args)
			{
				if (m_size >= N)//可行的错误处理
					throw std::bad_alloc{};
				new (data + m_size) T(std::forward<Args>(args)...);
				++m_size;
			}

			//访问对齐存储中的对象
			const T& operator[](std::size_t pos) const
			{
				//注意：C++17起需要std::launder
				return *reinterpret_cast<const T*>(data + pos);
			}

			//从对齐存储删除对象
			~static_vector()
			{
				for (std::size_t pos = 0; pos < m_size; ++pos)
				{
					//注意：C++17起需要std::launder
					reinterpret_cast<T*>(data + pos)->~T();
				}
			}
		};
	}
	namespace c_std_invoke_result
	{
		struct Foo
		{
			Foo(int num) : num_(num) {}

			void print_add(int i) const
			{
				std::cout << num_ + i << '\n';
			}

			int num_;
		};

		void print_num(int i)
		{
			std::cout << i << '\n';
		}

		struct PrintNum
		{
			void operator()(int i) const
			{
				std::cout << i << '\n';
			}
		};
	}
	namespace c_boost_invoke_result
	{
		struct Foo
		{
			Foo(int num) : num_(num) {}

			void print_add(int i) const
			{
				std::cout << num_ + i << '\n';
			}

			int num_;
		};

		void print_num(int i)
		{
			std::cout << i << '\n';
		}

		struct PrintNum
		{
			void operator()(int i) const
			{
				std::cout << i << '\n';
			}
		};
	}
	namespace c_test_invoke_result
	{
		struct Foo
		{
			Foo(int num) : num_(num) {}

			void print_add(int i) const
			{
				std::cout << num_ + i << '\n';
			}

			int num_;
		};

		void print_num(int i)
		{
			std::cout << i << '\n';
		}

		struct PrintNum
		{
			void operator()(int i) const
			{
				std::cout << i << '\n';
			}
		};
	}
	namespace c_std_enable_if
	{
		namespace detail
		{
			void* voidify(const volatile void* ptr) noexcept { return const_cast<void*>(ptr); }
		}

		//经由返回类型启用
		template<class T>
		typename std::enable_if<std::is_trivially_default_constructible<T>::value>::type construct(T* p)
		{
			std::cout << "default constructing trivially default constructible T\n";
		}
		template<class T>
		typename std::enable_if<!std::is_trivially_default_constructible<T>::value>::type construct(T* p)
		{
			std::cout << "default constructing non-trivially default constructible T\n";
			::new(detail::voidify(p)) T;
		}
		template<class T, class... Args>
		typename std::enable_if<std::is_constructible<T, Args&&...>::value>::type construct(T* p, Args&&... args)
		{
			std::cout << "constructing T with operation\n";
			::new(detail::voidify(p))T(static_cast<Args&&>(args)...);
		}

		//经由参数启用
		template<class T>
		void destroy(T*, typename std::enable_if<std::is_trivially_destructible<T>::value>::type* = 0)
		{
			std::cout << "destroying trivially destructible T\n";
		}

		//经由非类型模板形参启用
		template<class T, typename std::enable_if < !std::is_trivially_destructible<T>{} && (std::is_class<T>{} || std::is_union<T>{}), int >::type = 0>
		void destroy(T * t)
		{
			std::cout << "destroying non-trivially destructible T\n";
			t->~T();
		}

		//经由类型模板形参启用
		template<class T, typename = typename std::enable_if<std::is_array<T>::value>::type>
		void destroy(T* t)    //#5
		{
			for (std::size_t i = 0; i < std::extent<T>::value; ++i)
			{
				destroy((*t)[i]);
			}
		}

		//错误：函数签名与#5相同
		//template<class T, typename = typename std::enable_if<std::is_void<T>::value>::type>
		//void destroy(T* t) {}



		//经由模板形参启用A的偏特化

		//初等模板
		template<class T, class Enable = void>
		class A {};

		//浮点类型的偏特化
		template<class T>
		class A<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {};
	}
	namespace c_boost_enable_if
	{
		//略
	}
	namespace c_test_enable_if
	{
		//略
	}
	namespace c_std_is_member_object_pointer
	{
		class cls {};
	}
	namespace c_boost_is_member_object_pointer
	{
		class cls {};
	}
	namespace c_test_is_member_object_pointer
	{
		class cls {};
	}
	namespace c_std_is_arithmetic
	{
		class A {};
	}
	namespace c_boost_is_arithmetic
	{
		class A {};
	}
	namespace c_test_is_arithmetic
	{
		class A {};
	}
	namespace c_std_is_fundamental
	{
		class A {};
	}
	namespace c_boost_is_fundamental
	{
		class A {};
	}
	namespace c_test_is_fundamental
	{
		class A {};
	}
	namespace c_std_is_compound
	{
		class cls {};
	}
	namespace c_boost_is_compound
	{
		class cls {};
	}
	namespace c_test_is_compound
	{
		class cls {};
	}
	namespace c_std_is_scalar
	{
		class cls {};
	}
	namespace c_boost_is_scalar
	{
		class cls {};
	}
	namespace c_test_is_scalar
	{
		class cls {};
	}
	namespace c_std_is_object
	{
		class cls {};
	}
	namespace c_boost_is_object
	{
		class cls {};
	}
	namespace c_test_is_object
	{
		class cls {};
	}
	namespace c_std_is_abstract
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C
		{
			virtual void foo() = 0;
		};

		struct D : C {};
	}
	namespace c_boost_is_abstract
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C
		{
			virtual void foo() = 0;
		};

		struct D : C {};
	}
	namespace c_test_is_abstract
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C
		{
			virtual void foo() = 0;
		};

		struct D : C {};
	}
	namespace c_std_is_empty
	{
		struct A {};

		struct B {
			int m;
		};

		struct C
		{
			virtual ~C() {}
		};

		union D {};

		//struct E {
		//	[[no_unique_address]] D d;
		//};
	}
	namespace c_boost_is_empty
	{
		struct A {};

		struct B {
			int m;
		};

		struct C
		{
			virtual ~C() {}
		};

		union D {};

		//struct E {
		//	[[no_unique_address]] D d;
		//};
	}
	namespace c_test_is_empty
	{
		struct A {};

		struct B {
			int m;
		};

		struct C
		{
			virtual ~C() {}
		};

		union D {};

		//struct E {
		//	[[no_unique_address]] D d;
		//};
	}
	namespace c_std_is_polymorphic
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C : B {};
	}
	namespace c_boost_is_polymorphic
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C : B {};
	}
	namespace c_test_is_polymorphic
	{
		struct A
		{
			int m;
		};

		struct B
		{
			virtual void foo() {}
		};

		struct C : B {};
	}
	namespace c_std_is_signed
	{
		class A {};
		enum B : int {};
		enum class C : int {};
	}
	namespace c_boost_is_signed
	{
		class A {};
		enum B : int {};
		enum class C : int {};
	}
	namespace c_test_is_signed
	{
		class A {};
		enum B : int {};
		enum class C : int {};
	}
	namespace c_std_is_unsigned
	{
		class A {};
		enum B : unsigned {};
		enum class C : unsigned {};
	}
	namespace c_boost_is_unsigned
	{
		class A {};
		enum B : unsigned {};
		enum class C : unsigned {};
	}
	namespace c_test_is_unsigned
	{
		class A {};
		enum B : unsigned {};
		enum class C : unsigned {};
	}
	namespace c_std_is_standard_layout
	{
		struct A
		{
			int m;
		};

		struct B
		{
			int m1;
		private:
			int m2;
		};

		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_boost_is_standard_layout
	{
		struct A
		{
			int m;
		};

		struct B
		{
			int m1;
		private:
			int m2;
		};

		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_test_is_standard_layout
	{
		struct A
		{
			int m;
		};

		struct B
		{
			int m1;
		private:
			int m2;
		};

		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_std_is_trivially_copyable
	{
		struct A
		{
			int m;
		};
		struct B
		{
			B(const B&) {}
		};
		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_boost_is_trivially_copyable
	{
		struct A
		{
			int m;
		};
		struct B
		{
			B(const B&) {}
		};
		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_test_is_trivially_copyable
	{
		struct A
		{
			int m;
		};
		struct B
		{
			B(const B&) {}
		};
		struct C
		{
			virtual void foo() {}
		};
	}
	namespace c_std_is_trivial
	{
		struct A
		{
			int m;
		};

		struct B
		{
			B(){}
		};
	}
	namespace c_boost_is_trivial
	{
		struct A
		{
			int m;
		};

		struct B
		{
			B(){}
		};
	}
	namespace c_test_is_trivial
	{
		struct A
		{
			int m;
		};

		struct B
		{
			B(){}
		};
	}
	namespace c_std_has_virtual_destructor
	{
		struct A
		{
			virtual ~A() {}
		};

		struct B : A
		{
		};

		class C
		{
		private:
			virtual ~C() {}
		};
	}
	namespace c_boost_has_virtual_destructor
	{
		struct A
		{
			virtual ~A() {}
		};

		struct B : A
		{
		};

		class C
		{
		private:
			virtual ~C() {}
		};
	}
	namespace c_test_has_virtual_destructor
	{
		struct A
		{
			virtual ~A() {}
		};

		struct B : A
		{
		};

		class C
		{
		private:
			virtual ~C() {}
		};
	}
	namespace c_std_is_assignable
	{
		struct Exl { int n; };
	}
	namespace c_boost_is_assignable
	{
		struct Exl { int n; };
	}
	namespace c_test_is_assignable
	{
		struct Exl { int n; };
	}
	namespace c_std_is_copy_assignable
	{
		struct Foo { int n; };
	}
	namespace c_boost_is_copy_assignable
	{
		struct Foo { int n; };
	}
	namespace c_test_is_copy_assignable
	{
		struct Foo { int n; };
	}
	namespace c_std_is_move_assignable
	{
		struct Foo { int n; };
		
		struct NoMove
		{
			//避免默认的移动赋值运算符的隐式定义
			//然而，该类仍然可移动赋值，因为其复制
			//赋值运算符能绑定到右值参数

			NoMove& operator=(const NoMove&)
			{
				return *this;
			}
		};
	}
	namespace c_boost_is_move_assignable
	{
		struct Foo { int n; };
		
		struct NoMove
		{
			//避免默认的移动赋值运算符的隐式定义
			//然而，该类仍然可移动赋值，因为其复制
			//赋值运算符能绑定到右值参数

			NoMove& operator=(const NoMove&)
			{
				return *this;
			}
		};
	}
	namespace c_test_is_move_assignable
	{
		struct Foo { int n; };
		
		struct NoMove
		{
			//避免默认的移动赋值运算符的隐式定义
			//然而，该类仍然可移动赋值，因为其复制
			//赋值运算符能绑定到右值参数

			NoMove& operator=(const NoMove&)
			{
				return *this;
			}
		};
	}
	namespace c_std_is_destructible
	{
		struct Foo
		{
			std::string str;
			~Foo() noexcept {};
		};

		struct Bar
		{
			~Bar() = default;
		};
	}
	namespace c_boost_is_destructible
	{
		struct Foo
		{
			std::string str;
			~Foo() noexcept {};
		};

		struct Bar
		{
			~Bar() = default;
		};
	}
	namespace c_test_is_destructible
	{
		struct Foo
		{
			std::string str;
			~Foo() noexcept {};
		};

		struct Bar
		{
			~Bar() = default;
		};
	}
	namespace c_std_is_default_constructible
	{
		struct Ex1
		{
			std::string str;  //成员拥有非平凡默认构造函数
		};
		struct Ex2
		{
			int n;
			Ex2() = default;   //平凡且不抛出
		};
	}
	namespace c_boost_is_default_constructible
	{
		struct Ex1
		{
			std::string str;  //成员拥有非平凡默认构造函数
		};
		struct Ex2
		{
			int n;
			Ex2() = default;   //平凡且不抛出
		};
	}
	namespace c_test_is_default_constructible
	{
		struct Ex1
		{
			std::string str;  //成员拥有非平凡默认构造函数
		};
		struct Ex2
		{
			int n;
			Ex2() = default;   //平凡且不抛出
		};
	}
	namespace c_std_is_constructible
	{
		class Foo
		{
			int v1;
			double v2;
		public:
			Foo(int n) : v1(n), v2() {}
			Foo(int n, double f) noexcept : v1(n), v2(f) {}
		};
	}
	namespace c_boost_is_constructible
	{
		class Foo
		{
			int v1;
			double v2;
		public:
			Foo(int n) : v1(n), v2() {}
			Foo(int n, double f) noexcept : v1(n), v2(f) {}
		};
	}
	namespace c_test_is_constructible
	{
		class Foo
		{
			int v1;
			double v2;
		public:
			Foo(int n) : v1(n), v2() {}
			Foo(int n, double f) noexcept : v1(n), v2(f) {}
		};
	}
	namespace c_std_is_copy_constructible
	{
		struct Ex1
		{
			std::string str;//成员拥有非平凡复制构造函数
		};
		struct Ex2
		{
			int n;
			Ex2(const Ex2&) = default;//平凡且不抛出
		};
	}
	namespace c_boost_is_copy_constructible
	{
		struct Ex1
		{
			std::string str;//成员拥有非平凡复制构造函数
		};
		struct Ex2
		{
			int n;
			Ex2(const Ex2&) = default;//平凡且不抛出
		};
	}
	namespace c_test_is_copy_constructible
	{
		struct Ex1
		{
			std::string str;//成员拥有非平凡复制构造函数
		};
		struct Ex2
		{
			int n;
			Ex2(const Ex2&) = default;//平凡且不抛出
		};
	}
	namespace c_std_is_move_constructible
	{
		struct Ex1
		{
			std::string str;
		};
		struct Ex2
		{
			int n;
			Ex2(Ex2&&) = default;//平凡且不抛出
		};
		struct NoMove
		{
			//避免默认移动构造函数的隐式声明
			//然而，该类仍为可移动构造因为复制构造函数能绑定到右值参数
			NoMove(const NoMove&) {}
		};
	}
	namespace c_boost_is_move_constructible
	{
		struct Ex1
		{
			std::string str;
		};
		struct Ex2
		{
			int n;
			Ex2(Ex2&&) = default;//平凡且不抛出
		};
		struct NoMove
		{
			//避免默认移动构造函数的隐式声明
			//然而，该类仍为可移动构造因为复制构造函数能绑定到右值参数
			NoMove(const NoMove&) {}
		};
	}
	namespace c_test_is_move_constructible
	{
		struct Ex1
		{
			std::string str;
		};
		struct Ex2
		{
			int n;
			Ex2(Ex2&&) = default;//平凡且不抛出
		};
		struct NoMove
		{
			//避免默认移动构造函数的隐式声明
			//然而，该类仍为可移动构造因为复制构造函数能绑定到右值参数
			NoMove(const NoMove&) {}
		};
	}
}
