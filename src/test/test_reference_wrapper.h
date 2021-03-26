#pragma once

#include <boost/ref.hpp>   //boost::reference_wrapper    真正的实现代码在<boost/core/ref.hpp>中

#include <vector>
#include <set>

#include <cmath>
#include <cassert>

#include <functional>

//C++标准库和Boost中的算法大量使用了函数对象作为判断式或谓词参数，而这些参数都是传值语义，算法或函数在内部保留函数对象的拷贝并使用

//一般情况下传值语义都是可行的，但也有很多特殊情况：
//1.作为参数的函数对象拷贝代价过高(具有复杂的内部状态)
//2.不希望拷贝对象(内部状态不应该被改变，比如随机数发生器)
//3.拷贝是不可行的(noncopyable、singleton)

//reference_wrapper是引用类型的包装器(接口是引用，实现是指针)
//template<class T>
//class reference_wrapper
//{
//public:
//    类型为T&，要求实参必须是左值，正好迎合了成员变量是指针，指向一个非临时对象或非将析构对象
//    explicit reference_wrapper(T& t) : t_(&t) {}    //因为被声明为explicit，因此必须在创建reference_wrapper对象时就赋值初始化，就像是使用一个引用类型的变量

//    //提供的接口用起来像是一个引用(能够在任何需要T出现的地方使用reference_wrapper)    //重点：提供引用语义
//    operator T&() const {return *t_;}
//    T& get() const {return *t_;}

//    T* get_pointer() const {return t_;}

//private:
//    //实现是通过指针实现
//    T* t_;
//};

//reference_wrapper只有在使用T的语境下才能够执行隐性转换，其他的情况下则需要调用类型转换函数或者get()函数才能真正操作被包装对象
//reference_wrapper支持拷贝构造和赋值(因为成员变量是指针(可见成员变量优先使用指针而非引用))，而引用类型是不可赋值的

namespace test_reference_wrapper
{
	void func()
	{
		{
			int i = 10;
			boost::reference_wrapper<int> rw(i);    //rw内部存储指向i的指针，成员函数提供引用语义

			assert(i == rw);                        //隐式转换为int类型
			(int&)rw = 100;                         //显式转换为int&类型
			assert(i == 100);

			boost::reference_wrapper<int> rw2(rw);  //拷贝构造
			assert(rw2.get() == 100);
		}

		{
			std::string str;
			boost::reference_wrapper<std::string> rw(str);
			*rw.get_pointer() = "test reference_wrapper";   //指针操作

			//std::cout << rw.size() << "\n";
			//错误：boost::reference_wrapper<std::string>不存在size()函数

			std::cout << rw.get().size() << "\n";
			//正确：以为这里不存在一个要求std::string类型的隐式转换，所以必须要使用get()获得被包装的真正对象
		}
	}
}

namespace test_ref_cref
{
	//reference_wrapper的名字过长，声明包装对象很不方便，因而ref库提供了两个便捷的工厂函数ref()和cref()，可以通过参数类型
	//推导很容易地构造reference_wrapper对象

	//template<class T> BOOST_FORCEINLINE reference_wrapper<T const> BOOST_REF_CONST cref(T const& t)
	//{
	//	return reference_wrapper<T const>(t);
	//}

	//template<class T> BOOST_FORCEINLINE reference_wrapper<T> BOOST_REF_CONST ref(T& t)
	//{
	//	return reference_wrapper<T>(t);
	//}


	using namespace boost;

	void func()
	{
		double d = 2.71828;
		auto rw_double_const = cref(d);   //std::reference_wrapper<double const>
		auto rw_double = ref(d);          //std::reference_wrapper<double>

		std::string str;
		auto rw_string_const = boost::cref(str);   //boost::不能省
		auto rw_string = boost::ref(str);          //boost::不能省
		//由于现在c++11 标准中也实现了ref/cref，为了避免ADL命名空间查找导致的名字冲突，有时候可能需要加上明确的boost命名空间限定

		std::cout << std::sqrt(ref(d)) << "\n";
		//因为reference_wrapper支持拷贝，因此ref()和cref()可以直接用在需要拷贝语义的函数参数中，
		//而不必专门使用一个reference_wrapper来暂存
	}
}

namespace test_is_reference_wrapper
{
	//boost::is_reference_wrapper<T>::value
	//可以判断T是否为一个reference_wrapper

	void func()
	{
		std::vector<int> vec(10, 2);
		auto rw = boost::cref(vec);

		assert(boost::is_reference_wrapper<decltype(rw)>::value);
		assert(!boost::is_reference_wrapper<decltype(vec)>::value);
	}
}

namespace test_unwrap_reference
{
	//boost::unwrap_reference<T>::type
	//返回T的真实类型，无论他是否经过reference_wrapper包装
	
	void func()
	{
		std::string str;
		auto rw_string = boost::ref(str);

		std::cout << typeid(boost::unwrap_reference<decltype(rw_string)>::type).name() << "\n";
		std::cout << typeid(boost::unwrap_reference<decltype(str)>::type).name() << "\n";
	}
}

namespace test_unwrap_ref
{
	//boost::unwrap_ref函数
	//返回T的引用，无论他是否经过reference_wrapper保证(实际内部依赖boost::unwrap_reference<T>)

	void func()
	{
		std::set<int> set_;
		auto rw_set = boost::ref(set_);
		boost::unwrap_ref(rw_set).insert(12);

		std::string str("test");
		auto rw_string_const = boost::cref(str);
		std::cout << boost::unwrap_ref(rw_string_const) << "\n";

		std::cout << boost::unwrap_ref(str) << "\n";               //直接对一个未包装的对象使用unwrap_ref()。unwrap_ref()的这个功能很有用，可以把unwrap_ref()安全地用在泛型代码中，从而不必关心对象的包装特性，总能够正确地操作对象
	}
}

namespace test_big_class
{
	//假设有一个很大的类big_class，他具有复杂的内部状态，构造、拷贝都具有很高的代价
	class big_class
	{
	private:
		int x;     //假设是很复杂的内部状态，这里从简

	public:
		big_class() :x(0) {}

		void print()     //改变内部状态的成员函数
		{
			std::cout << "big_class " << ++x << "\n";
		}
	};

	template<typename T>
	void print(T a)
	{
		for (int i = 0; i < 2; ++i)
		{
			//考虑到类型可能会使用reference_wrapper包装，这里使用unwrap_ref()函数

			//(如果使用reference_wrapper包装且直接a.print()，那么a.prin()不会隐式转换为T&并且reference_wrapper没有print函数，导致编译失败)

			boost::unwrap_ref(a).print();    //解包装
		}
	}

	void func()
	{
		big_class big_class_;

		auto rw = boost::ref(big_class_);

		big_class_.print();                  //输出1

		print(big_class_);                   //拷贝实参big_class_，输出2，3，big_class_内部状态不改变

		print(rw);                           //拷贝实参，引用big_class_，输出2，3，big_class_内部状态不改变

		print(big_class_);                   //拷贝实参big_class_，输出4，5，big_class_内部状态不改变

		big_class_.print();                  //输出4
	}
}

//对比C++11标准

//C++11标准里的std::reference_wrapper
//template<class T>
//class reference_wrapper
//{
//public:
//    reference_wrapper(T&) noexcept;
//    reference_wrapper(T&&) = delete;               //不能包装右值即临时对象

//    reference_wrapper(const reference_wrapper<T>& x) noexcept;
//    reference_wrapper& operator=(const reference_wrapper<T>& x) noexcept;

//    operator T&() const noexcept;                  //隐式转型
//    T& get() const noexcept;

//    template<class... ArgTypes>
//    typename result_of<T&(ArgTypes&&...)>::type operator()(ArgTypes&&...) const;    //调用操作符(函数对象具有调用操作符，所以std::reference_wrapper<T>可以作为函数对象的引用)
//};
//std::reference_wrapper是由boost.ref库演变而来的，所以两者非常相似，但也有少量的不同，
//最大的区别是std::reference_wrapper支持调用操作符——这使得我们可以包装一个函数对象的引用并传递给标准库算法

//所以优先选用std::reference_wrapper，而不是boost::reference_wrapper！！！

namespace test_std_reference_wrapper_has_operator
{
	struct square
	{
		using result_type = void;

		result_type operator()(int& x)
		{
			x = x * x;
		}
	};

	void func()
	{
		using pfunc = double(*)(double);    //pfunc是函数指针类型

		pfunc pf = std::sqrt;

		std::cout << std::ref(pf)(5.0);         //std::reference_wrapper可以包装函数指针，而boost::reference_wrapper不能

		square square_;
		int x = 5;
		std::ref(square_)(x);                   //std::reference_wrapper可以包装函数对象，而boost::reference_wrapper不能
		std::cout << x;

		//主要，这要求square类必须有public的result_type类型定义

		std::vector<int> vec = { 1, 2, 3, 4, 5 };
		std::for_each(vec.begin(), vec.end(), std::ref(square_));
	}
}

