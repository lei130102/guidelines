#pragma once

#include <boost/functional.hpp>

//function是一个函数的“容器”，概念上像是C/C++中泛化的函数指针
//只要符合他声明中的函数类型，任何普通函数、成员函数、函数对象都可以存储在function对象中，然后在任何需要的时候被调用
//因此他可以被用于回调机制，暂时保管函数或者函数对象，在之后需要的时机在调用，使回调机制拥有更多的弹性

//function可以配合bind/lambda使用，存储bind/lambda表达式的返回值(一种复杂的函数对象)，使bind/lambda可以被多次调用

//bind是函数模板家族
//function是类模板家族，function可以容纳0到10个参数的函数，因此也就有多达11个类，命名分别是function0到function10。但我们通常不直接使用他们，而是使用一个更通用的function类:
//template<typename Signature>
//class function : public functionN<R, T1, T2, ..., TN>
//{
//public:
//    //内部类型定义
//    typedef R result_type;
//    typedef TN argN_type;

//    //参数个数常量
//    static const int arity = N;

//    //构造函数
//    function();

//    template<typename F>
//    function(F);

//    //基本操作
//    void swap(const function&);
//    void clear();
//    bool empty() const;

//    //bool转型
//    bool safe_bool() const;
//    bool operator!() const;

//    //访问内部元素
//    template<typename Functor>
//    Functor* target();
//    template<typename Functor>
//    bool contains(const Functor&) const;
//    const std:type_info& target_type() const;

//    //调用操作符
//    result_type operator()(arg1_type, ...) const;
//};


//与原始的函数指针相比，function对象的体积要稍微大一点(3个指针的大小)，速度要稍微慢一点(10%左右的性能差距)，但这与他带给程序的巨大好处相比是无足轻重的


namespace test_function_simple
{
	int f(int a, int b)
	{
		return a + b;
	}

	void func()
	{
		{
			//声明了一个可以容纳 返回值为int、无参函数 的function对象
			boost::function<int()> function_;   //"int()"就是一个没有函数名的函数原型     (函数引用:"int(&)()"，函数指针:"int(*)()")

			//无参构造一个function对象，此时function不持有任何对象
			assert(!function_);
		}

		{
			//声明了一个可以容纳 返回值为int、有两个int类型参数函数 的function对象
			boost::function<int(int, int)> function_;
			//等价于
			boost::function<int(int a, int b)> function_2;
			//等价于
			boost::function<decltype(f)> function_3;    //可以用C++11的关键字decltype来直接获取函数类型
		}

		{
			{
				boost::function<int(int, int)> function_(f);
			}
			//function的构造函数可以接受任意符合模板中声明的函数类型的可调用对象，如函数指针和函数对象，或者是另一个function对象的引用，之后在内部存储一份他的拷贝

			{
				boost::function<int(int, int)> function_;

				function_ = f;
				assert(function_.contains(&f));
				//contains()  可以检测function是否持有一个Functor对象

				function_(1, 2);
				//operator()  把传入的参数转交给内部保存的可调用物，完成真正的函数调用


				//这样写不对  function_ = nullptr;

				//std::function没有clear()成员函数，所以boost::function也尽量不用
				//function_.clear();
				////clear()     可以直接将function对象置空，与使用operator=赋值0具有相同的效果

				//std::function没有empty()成员函数，所以boost::function也尽量不用
				//assert(function_.empty());
			}
		}

		{
			//这样写不对  boost::function<int(int, int)> function_(nullptr);
			boost::function<int(int, int)> function_;

			if (!function_.empty())
			{

			}

			//operator!
			if (!function_)
			{

			}

			//operator bool
			if (function_)
			{

			}
			//无参的构造函数或者传入空指针构造将创建一个空的function对象，不持有任何可调用物，调用空的function对象将抛出bad_function_call异常，因此在使用function
			//前最好检测一下他的有效性。可以用empty()测试function是否为空，或者用重载操作符operator!来测试，function对象也可以在一个bool语境中直接测试他是否为空
		}

		{
			//target()    可以返回function对象内部持有的可调用物Functor的指针，如果function为空则返回空指针nullptr
		}

		{

			//比较操作

			//function重载了比较操作符operator==和operator!=，可以与被包装的函数或函数对象进行比较。

			//(注意，两个function对象不能使用==和!=直接比较，这是特意的。因为function存在bool的隐式转换，function定义了两个function对象的operator==，但
			//没有实现，因为企图比较两个function对象会导致编译错误)

			//a.如果function存储的是函数指针，那么比较相当于
			//function.target<Functor>() == func_pointer

			//例如
			//function<int(int, int)> func(f);
			//assert(func == f);

			//b.如果function存储的是函数对象，那么要求函数对象必须重载了operator==
		}
	}
}

//只要函数签名式一致，function也可以存储 成员函数 函数对象 bind/lambda表达式(的返回值)

namespace test_function_member_function
{
	struct test_class
	{
		int add(int a, int b)
		{
			return a + b;
		}
	};

	void func()
	{
		{
			test_class tc;

			//function用于成员函数方式1：
			boost::function<int(test_class&, int, int)> function_;  //注意是test_class&，不是test_class*

			//function规定了bind中必须有3个占位符
			function_ = boost::bind(&test_class::add, boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

			std::cout << function_(tc, 10, 20);
		}

		{
			test_class tc;

			//function用于成员函数方式2：                           //也可以在函数类型中仅写出成员函数的签名，在bind时直接绑定类的实例：
			boost::function<int(int, int)> function_;

			function_ = boost::bind(&test_class::add, &tc, boost::placeholders::_1, boost::placeholders::_2); //"&tc"这个参数使用类对象、类引用和类指针都可以

			std::cout << function_(10, 20);
		}
	}
}

namespace test_function_reference_wrapper
{
	//function使用拷贝语义保存参数，当参数很大时拷贝的代价往往很高，或者有时候不能拷贝参数，这时需要用ref库，允许以引用的方式传递参数，能够降低function拷贝的代价

	struct function_object
	{
		int operator()(int x) const
		{
			return x * x;
		}
	};

	void func()
	{
		function_object fo;

		boost::function<int(int)> function_;

		function_ = boost::cref(fo);
		//boost::cref(fo)返回的是boost::reference_wrapper<function_object const>对象
		//该对象没有调用操作(operator())，即不是函数对象，但boost::function可以自动识别它并调用get()方法获得被包装的函数对象
		//从而获得function_object const

		std::cout << function_(10);
		//因为获得的是function_object const，所以只能调用const成员函数
	}
}

namespace test_function_function_object_with_state
{
	//带有内部状态的函数对象
	template<typename T>
	struct summary                          //功能：计算总和
	{
		using result_type = void;

		T sum;                              //内部状态，总和

		summary(T v = T{}) : sum(v) {}

		void operator()(T const& x)
		{
			sum += x;
		}
	};

	void func()
	{
		std::vector<int> vec = { 1, 3, 5, 7, 9 };

		//错误写法
		{
			summary<int> s;       //有状态的函数对象

			//std::for_each(vec.begin(), vec.end(), boost::ref(s));
			//编译错误：
			//标准库算法std::for_each总使用拷贝语义，所以需要使用reference_wrapper，但reference_wrapper不提供operator()操作，所以编译失败
		}

		//借助boost::function包装boost::reference_wrapper，实现正确写法
		{
			summary<int> s;

			boost::function<void(int const&)> function_(boost::ref(s));
			std::for_each(vec.begin(), vec.end(), function_);
			//正确：
			//用boost::function包装boost::reference_wrapper
			//对内，boost::function可以自动识别boost::reference_wrapper中的包装
			//对外，boost::function提供了operator()

			std::cout << s.sum << "\n";    //函数对象的状态被改变
		}

		//借助有些标准库算法的返回值是对应函数对象的拷贝，实现正确写法
		{
			//不过有的标准库算法(比如std::for_each，查看文档关于他的返回值)可以返回内部函数对象的拷贝，他
			//持有被改变的内部状态，但大对象的拷贝仍然是个问题
			summary<int> s;

			std::cout << (std::for_each(vec.begin(), vec.end(), s)).sum << "\n";
		}
	}
}

namespace test_function_callback
{
	//boost::function可以容纳任意符合函数签名式的可调用物，因此他非常适合代替函数指针，存储用于回调的函数，
	//而且他的强大功能会使代码更加灵活且更富有弹性

	class test_class
	{
	private:
		using function_type = boost::function<void(int)>;     //用boost::function代替函数指针

		function_type m_function;        //boost::function对象 (使用了boost::function作为内部可调用物的存储，因此不用做任何改变，即可接受函数指针也可接受函数对象)
		int m_n;

	public:
		test_class(int n) :m_n(n) {}

		//存储回调函数
		//(注意！之所以使用函数模板，是因为这种形式更加灵活，用户可以在不知道也不关心内部存储形式的情况下传递任何可调用对象，包括函数指针和函数对象)
		template<typename CallBack>
		void accept(CallBack f)
		{
			m_function = f;
		}

		//调用回调函数
		void run()
		{
			m_function(m_n);
		}
	};

	//普通函数作为回调函数       (一般不能体现boost::function的好处)
	void callback_function(int i)
	{
		std::cout << "callback_function:";
		std::cout << i * 2 << "\n";
	}

	//带状态的函数对象作为回调函数   (因为是带状态的，所以要借助boost::reference_wrapper)
	class callback_object
	{
	private:
		int m_i;            //内部状态

	public:
		callback_object(int i) :m_i(i) {}

		void operator()(int i)
		{
			std::cout << "callback_object:";
			std::cout << i * m_i++ << "\n";     //先做乘法，然后递增
		}
	};

	//boost::function可以搭配boost::bind，好处是：
	//1.可以接受类成员函数
	//2.把不符合函数签名的函数转为可接受的形式
	class callback_factory
	{
	public:
		void callback_member_function_1param(int i)         //1个参数
		{
			std::cout << "callback_member_function_1param:";
			std::cout << i * 2 << "\n";
		}

		void callback_member_function_2param(int i, int j)  //2个参数
		{
			std::cout << "callback_member_function_2param:";
			std::cout << i * j * 2 << "\n";
		}
	};

	void func()
	{
		//使用callback_function
		{
			test_class tc(10);

			tc.accept(callback_function);   //接受回调函数

			tc.run();                       //调用回调函数
		}

		//使用callback_object
		{
			test_class tc(10);

			callback_object co(2);
			tc.accept(boost::ref(co));

			tc.run();
			tc.run();
		}

		//
		{
			test_class tc(10);

			callback_factory cf;
			tc.accept(boost::bind(&callback_factory::callback_member_function_1param, cf, boost::placeholders::_1));
			tc.run();
			tc.accept(boost::bind(&callback_factory::callback_member_function_2param, cf, boost::placeholders::_1, 5));
			tc.run();
		}
	}
}

//对比C++11的auto

//auto func = &f;                                    //auto推导出某普通函数指针类型
//std::cout << func(10, 20) << "\n";

//demo_class sc;
//auto func2 = bind(&demo_class::add, &sc, _1, _2);  //auto推导出某函数对象类型

//function类似一个容器，可以容纳任意有operator()的类型(函数指针、函数对象、lambda表达式)，他是运行时的，可以任意拷贝、赋值、存储其他可调用物
//而auto仅是在编译期推导出的一个静态类型变量，他很难再赋予其他值，也无法容纳其他的类型，不能用于泛型编程

//当需要存储一个可调用物用于回调的时候，最好使用function，他具有更多的灵活性，特别是把回调作为类的一个成员的时候我们只能使用function

//auto也有他的优点，他的类型是在编译期推导的，没有运行时的开销，效率上要比function略高一点。但他声明的变量不能存储其他类型的可调用物，不具有灵活性，只能用
//于有限范围的延后回调








//对比std::function

//C++11标准定义了std::function

//std::function与boost::function基本相同，只有少量的区别
//1.没有clear()和empty()成员函数
//2.提供assign()成员函数
//3.explicit显式bool转型
//因此，同shared_ptr一样，在函数返回值或者函数参数等语境里转型bool需要使用static_cast<bool>(f)或者!!f的形式