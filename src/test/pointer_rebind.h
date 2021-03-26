#ifndef POINTER_REBIND_H
#define POINTER_REBIND_H

namespace xll
{
	////pointer_rebind_mode

	template<typename Ptr, typename U>
	struct pointer_has_rebind
	{
		template<typename V>
		struct any
		{
			any(const V&)
			{}
		};

		//函数模板重载
		template<typename X>
		static char test(int, typename X::template rebind<U>*);   //不需要定义

		template<typename X>
		static int test(any<int>, void*);

		static const bool value = (1 == sizeof(test<Ptr>(0, 0))); //如果Ptr存在rebind成员模板，那么返回的指针类型肯定比void*优先，这里随便实参给一个零
	};

	template<typename Ptr, typename U>
	struct pointer_has_rebind_other
	{
		template<typename V>
		struct any
		{
			any(const V&)
			{}
		};

		template<typename X>
		static char test(int, typename X::template rebind<U>::other*);

		template<typename X>
		static int test(any<int>, void*);

		static const bool value = (1 == sizeof(test<Ptr>(0, 0)));
	};

	template<typename Ptr, typename U>
	struct pointer_rebind_mode
	{
		//(先看有没有rebind，有rebind的情况下才有可能有rebind<U>::other)
		static const unsigned int rebind = (unsigned int)pointer_has_rebind<Ptr, U>::value;
		static const unsigned int rebind_other = (unsigned int)pointer_has_rebind_other<Ptr, U>::value;

		//rebind 0u     rebind_other 0u       mode 0u
		//rebind 0u     rebind_other 1u       mode 0u
		//rebind 1u     rebind_other 0u       mode 1u
		//rebind 1u     rebind_other 1u       mode 2u
		static const unsigned int mode = rebind + rebind * rebind_other;
	};



	////pointer_rebinder

	template<typename Ptr, typename U, unsigned int RebindMode>
	struct pointer_rebinder;

	//Implementation of pointer_rebinder<U>::type if Ptr has its own rebind<U>::other type (C++03)
	template<typename Ptr, typename U>
	struct pointer_rebinder<Ptr, U, 2u>
	{
		using type = typename Ptr::template rebind<U>::other;
	};

	//Implementation of pointer_rebinder<U>::type if Ptr has its own rebind template
	template<typename Ptr, typename U>
	struct pointer_rebinder<Ptr, U, 1u>
	{
		using type = typename Ptr::template rebind<U>;
	};

	//Specialization of pointer_rebinder if Ptr does not have its own rebind template but has a the form Ptr<A, An...>,
	//where An... comprises zero or more type parameters.Many types fit this form, hence many pointers will get a reasonable
	//default for rebind.
	template<template<typename, typename...> typename Ptr, typename A, typename... An, typename U>
	struct pointer_rebinder<Ptr<A, An...>, U, 0u>
	{
		using type = Ptr<A, An...>;
	};

	//Needed for non-conforming compilers like GCC 4.3
	template<template<typename> typename Ptr, typename A, typename U>
	struct pointer_rebinder<Ptr<A>, U, 0u>
	{
		using type = Ptr<U>;
	};

	////pointer_rebind

	template<typename Ptr, typename U>
	struct pointer_rebind : public pointer_rebinder<Ptr, U, pointer_rebind_mode<Ptr, U>::mode>
	{};

	template<typename T, typename U>
	struct pointer_rebind<T*, U>
	{
		using type = U*;
	};

}

#endif //POINTER_REBIND_H