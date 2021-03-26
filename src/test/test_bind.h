#pragma once

#include <boost/bind.hpp>

#include <boost/rational.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <algorithm>

//bind是对C++98标准中函数适配器bind1st/bind2nd的泛化和增强，可以适配任意的可调用对象，包括函数指针、函数引用、成员函数指针和函数对象

//bind远远地超越了STL中的函数绑定器bind1st/bind2nd，可以绑定最多9个函数参数，并且对被绑定对象的要求很低，可以在没有result_type内部类型定义的情况下
//完成对函数对象的绑定

//bind并不是一个单独的类或函数，而是非常庞大的家族，依据绑定的参数个数和要绑定的调用对象类型，总共有数十个不同的重载形式，但他们的名字都叫做bind，编译器
//会根据具体的绑定代码自动确定要使用的正确形式

//template<class F>
//    _bi::bind_t<_bi::unspecified, F, _bi::list0>
//    BOOST_BIND(F f)
//{
//    typedef _bi::list0 list_type;
//    return _bi::bind_t<_bi::unspecified, F, list_type> (f, list_type());
//}

//template<class F, class A1>
//    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_1<A1>::type>
//    BOOST_BIND(F f, A1 a1)
//{
//    typedef typename _bi::list_av_1<A1>::type list_type;
//    return _bi::bind_t<_bi::unspecified, F, list_type> (f, list_type(a1));
//}

//...

//namespace boost
//{
//namespace placeholders
//{
//    boost::arg<1> _1;
//    boost::arg<2> _2;
//    boost::arg<3> _3;
//    //...                其他6个占位符定义
//}
//}

//bind接受的第一个参数必须是一个可调用对象f(函数、函数指针(和成员函数指针)、函数对象)
//之后bind接受最多9个参数，参数的数量必须与f的参数数量相等，这些参数将被传递给f作为实参

//绑定完成后，bind会返回一个函数对象，他内部保存了f的拷贝，具有operator()，返回值类型被自定推导为f的返回值类型。在发生调用时，这个函数对象将把之前存储的参数转发给f完成调用

//例如

//func(a1,a2)
//等价于
//bind(func, a1, a2)()
//(bind表达式存储了func和a1、a2的拷贝，产生了一个临时函数对象。当operator()调用发生时函数对象把a1、a2的拷贝传递给func，完成真正的函数调用)

//bind的真正威力在于他的占位符，他们分别被定义为_1、_2、_3一直到_9，占位符可以取代bind中参数的位置，在发生函数调用时才接受真正的参数。
//占位符的名字表示他在调用式中的顺序，而在绑定表达式中没有顺序的要求

//例如

//bind(func, _2, _1)(a1, a2)
//(返回一个具有两个参数的函数对象，第一个参数将放在函数func的第二个位置，而第二个参数则放在第一个位置)
//等价于
//func(a2, a1)

namespace test_bind_not_member_function
{
	//绑定普通函数(可以使用函数名或者函数指针)

	int f(int a, int b)
	{
		return a - b;
	}

	int g(int a, int b, int c)
	{
		return a + b * c;
	}

	void func()
	{
		std::cout << boost::bind(f, 1, 2)() << "\n";    //输出-1
		//等价于
		std::cout << f(1, 2) << "\n";

		std::cout << boost::bind(g, 1, 2, 3)() << "\n";   //输出7
		//等价于
		std::cout << g(1, 2, 3) << "\n";

		std::cout << "\n";

		std::cout << boost::bind(f, boost::placeholders::_1, 9)(4) << "\n";   //输出-5     //占位符_1被调用式中的第1个(因为_1的名称)参数4替代后，形成"f(4,9)" 
		//等价于
		std::cout << boost::bind(f, 4, boost::placeholders::_1)(9) << "\n";               //占位符_1被调用式中的第1个参数9替代后，形成"f(4,9)"
		//等价于
		std::cout << f(4, 9) << "\n";

		std::cout << "\n";

		std::cout << boost::bind(f, boost::placeholders::_1, boost::placeholders::_2)(4, 9) << "\n"; //输出-5   //占位符_1被调用式中的第1个参数4替代，占位符_2被调用式中的第2个参数9替代，形成"f(4,9)"
		//等价于
		std::cout << boost::bind(f, boost::placeholders::_2, boost::placeholders::_1)(9, 4) << "\n";           //占位符_2被调用式中的第2个参数4替代，占位符_1被调用式中的第1个参数9替代，形成"f(4,9)     (占位符的名字表示他在调用式中的顺序，而在绑定表达式中没有顺序的要求)
		//等价于
		std::cout << f(4, 9) << "\n";

		std::cout << "\n";

		std::cout << boost::bind(f, boost::placeholders::_1, boost::placeholders::_1)(4, 9) << "\n";  //输出0    //占位符_1被调用式中的第1个参数4替代，第二个占位符_1被调用式中的第1个参数4替代，形成"f(4,4)"   9被忽略
		//等价于
		std::cout << f(4, 4) << "\n";

		//注意，必须在绑定表达式中提供函数要求的所有参数(且顺序一致)!!!无论是真实参数还是占位符均可以。占位符可以出现也可以不出现，出现的顺序和数量也没有限定，但不能使用超出函数参数数量的
		//占位符，比如在绑定f时不能使用_3，在绑定g时不能使用_4，也不能写bind(f, _1, _2, _2)这样的形式(因为f只允许2个参数)，否则会导致编译错误

	}

	//bind完全可以代替标准库中的bind1st和bind2nd，使用bind(f, N, _1)和bind(f, _1, N)。要注意的是他们均使用了一个占位符，bind1st把第一个参数用固定值代替，bind2nd把第二个参数用固定值代替

	void func2()
	{
		//bind同样可以绑定函数指针

		//定义函数指针类型
		using f_func_pointer_type = decltype(&f);
		using g_func_pointer_type = decltype(&g);

		//定义函数指针变量
		f_func_pointer_type pf = f;
		g_func_pointer_type pg = g;

		int x = 1, y = 2, z = 3;

		std::cout << boost::bind(pf, boost::placeholders::_1, 9)(x) << "\n";                                                         //(*pf)(x, 9)
		std::cout << boost::bind(pg, boost::placeholders::_3, boost::placeholders::_2, boost::placeholders::_2)(x, y, z) << "\n";    //(*pg)(z, y, y)
	}
}

namespace test_bind_member_function
{
	//绑定成员函数

	//类的成员函数不同于普通函数，因为成员函数指针不能直接调用operator()，他必须被绑定到一个对象或者指针，然后才能得到this指针进而调用成员函数。因此bind需要
	//“牺牲”一个占位符的位置，要求用户提供一个类的实例、引用或者指针，通过对象作为第一个参数来调用成员函数(实际上成员函数的第一个（隐含的）参数就是对象指针)
	
	//bind(&X::func, x, _1, _2, ...)
	//这意味着使用成员函数时只能最多绑定8个参数

	struct demo
	{
		int f(int a, int b)
		{
			return a + b;
		}
	};

	void func()
	{
		demo a, & ra = a;    //类对象和类引用
		demo* p = &a;        //类指针

		//注意，成员函数前要加上取地址操作符&，普通函数不用

		std::cout << boost::bind(&demo::f, a, _1, 20)(10) << "\n";                                                 //类对象
		std::cout << boost::bind(&demo::f, ra, boost::placeholders::_2, boost::placeholders::_1)(10, 20) << "\n";  //类引用
		std::cout << boost::bind(&demo::f, p, boost::placeholders::_1, boost::placeholders::_2)(10, 20) << "\n";   //类指针

		//注意!!!!!我们必须在成员函数前加上取地址操作符&，表明这是一个成员函数指针，否则会无法通过编译，这是与绑定函数的一个小小的不同
	}

	//bind可以绑定成员函数，这样可以替代标准库中令人迷惑的mem_fun和mem_fun_ref绑定器，用来配合标准算法操作容器中的对象
	//(也可以使用专门用来绑定成员函数的boost::mem_fn，他同样被收入了C++11标准)

	struct point
	{
		int x, y;

		point(int a = 0, int b = 0) :x(a), y(b) {}

		void print()
		{
			std::cout << "(" << x << "," << y << ")\n";
		}
	};

	void func2()
	{
		std::vector<point> vec(10);
		for_each(vec.begin(), vec.end(), boost::bind(&point::print, boost::placeholders::_1));
	}

	//注意，bind同样支持绑定虚拟成员函数，用法与非虚函数相同，虚函数的行为将由实际调用发生时的实例来决定
}

namespace test_bind_member_variable
{
	//绑定成员变量(看做一个传入对象返回对象成员变量的函数对象)

	//bind可以绑定public成员变量，就像是一个选择器，用法与绑定成员函数类似，只需要把成员变量名像一个成员函数一样去使用

	struct point
	{
		int x, y;

		point(int a = 0, int b = 0) :x(a), y(b) {}

		void print()
		{
			std::cout << "(" << x << "," << y << ")\n";
		}
	};

	void func()
	{
		{
			std::vector<point> vec_point(10);
			std::vector<int> vec_int(10);

			std::transform(vec_point.begin(), vec_point.end(), vec_int.begin(), bind(&point::x, boost::placeholders::_1));

			for (auto x : vec_int)
			{
				std::cout << x << ",";
			}

			//bind(&point::x, _1)取出point对象的成员变量x，transform算法调用bind表达式操作容器v，逐个把成员变量填入到v2中
		}

		{
			//bind可以实现SGISTL/STLport中的非标准函数适配器select1st和select2nd的功能，直接选择出pair对象的first和second成员
			//但bind显然要比他们功能更强，可以选择类中任意名字的public成员变量

			using pair_type = std::pair<int, std::string>;
			pair_type p(123, "string");

			std::cout << boost::bind(&pair_type::first, p)() << "\n";
			std::cout << boost::bind(&pair_type::second, p)() << "\n";
		}
	}
}

namespace test_bind_function_object
{
	//绑定函数对象

	//如果函数对象有内部类型定义result_type，那么bind可以自动推导出返回值类型，用法与绑定普通函数一样，但如果函数对象没有定义result_type，则需要在绑定形式上做一点改动，
	//用模板参数指明返回类型
	//bind<result_type>(functor, ...);   //注意模板参数

	//标准库和Boost库中的大部分函数对象都具有result_type定义，因此不需要特别的形式就可以直接使用bind
	//bind(std::greater<int>(), _1, 10);    //检查x>10
	//bind(std::plus<int>(), _1, _2);       //执行x+y
	//bind(std::modulus<int>(), _1, 3);     //执行x%3

	struct func_object
	{
		int operator()(int a, int b)
		{
			return a + b;
		}
	};

	void func()
	{
		std::cout << boost::bind<int>(func_object(), boost::placeholders::_1, boost::placeholders::_2)(10, 20) << "\n";
		//因为func_object没有提供result_type，所以用int指名返回类型

		//这种写法多少会有些不方便，因此，在编写自己的函数对象时，最好遵循规范为他们增加内部typedef result_type，这将使函数对象与许多其他标准库和Boost库组件良好配合工作
	}
}

namespace test_bind_reference_wrapper
{
	//bind采用拷贝的方式存储绑定对象和参数，这意味着绑定表达式中的每个变量都会由一份拷贝，如果函数对象或值参数很大、拷贝代价很高，或者无法拷贝，那么bind的使用就会受到限制

	//因此bind库可以搭配reference_wrapper使用，reference_wrapper包装了对象的引用，可以让bind存储对象引用的拷贝，从而降低了拷贝的代价，但这也带来了一个隐患，因为有时候bind的调用可能会延后很久，
	//程序员必须保证bind被调用时引用是有效的，如果调用时引用的变量或者函数对象被销毁了，那么就会发生未定义行为

	int f(int a, int b)
	{
		return a - b;
	}

	int g(int a, int b, int c)
	{
		return a + b * c;
	}

	struct func_object
	{
		int operator()(int a, int b)
		{
			return a + b;
		}
	};

	void func()
	{
		{
			int i = 10;
			std::cout << boost::bind(g, boost::placeholders::_1, boost::cref(i), boost::ref(i))(10) << "\n";

			func_object func_object_;  //没有定义public的result_type类型的函数对象
			std::cout << boost::bind<int>(boost::ref(func_object_), boost::placeholders::_1, boost::placeholders::_2)(10, 20) << "\n";
		}

		{
			int i = 10;
			auto r = boost::ref(i);
			{
				int* pi = new int(20);
				r = boost::ref(*pi);
				std::cout << r << "\n";
				std::cout << boost::bind(g, r, 1, 1)() << "\n";    //正常工作
				delete pi;
			}
			//std::cout << boost::bind(g, r, 1, 1)() << "\n";      //未定义行为
		}
	}
}

//对比C++11标准

//C++11标准使用可变参数模板和完美转发简化了bind的定义，支持绑定任意数量的参数

//template<class R ,class F, class... BoundArgs>
//unspecified bind(F&&, BoundArgs&&...);               //bind函数

//namespace std::placeholders         //占位符命名空间
//{
//    extern unspecified _1;
//    extern unspecified _2;
//    //...
//};

//std::bind的用法与boost::bind完全相同，但为了避免冲突，标准占位符位于std::placeholder命名空间，代码里需要用std::placeholders::_1的形式(略微不太方便)
//std::cout << std::bind(f, std::placeholders::_1, 20)(10) << "\n";

//注意，优先使用std::bind，而不是boost::bind

namespace test_std_bind
{
	int f(int a, int b)
	{
		return a - b;
	}

	void func()
	{
		std::cout << std::bind(f, std::placeholders::_1, 20)(10) << "\n";
	}
}

namespace test_lambda
{
	//C++11标准还提供了语言级别的lambda表达式，他可以就地声明匿名函数对象，用法非常灵活。
	//lambda表达式某种程度上也可以代替bind，捕获列表[]相当于绑定的变量，函数参数列表()则相当于bind的占位符

	int f(int a, int b)
	{
		return a - b;
	}

	void func()
	{
		std::cout << std::bind(f, std::placeholders::_1, 20)(10) << "\n";

		//等价于

		auto l = [](int x)
		{
			return f(x, 20);
		};

		std::cout << l(10) << "\n";
	}
}

namespace test_placeholders_rename
{
	//为原占位符使用引用创建别名

	int f(int a, int b)
	{
		return a - b;
	}

	void func()
	{
		{
			boost::arg<1> const& _a = boost::placeholders::_1;
			std::cout << boost::bind(f, _a, 20)(10) << "\n";
		}

		{
			auto& _a = boost::placeholders::_1;
			std::cout << boost::bind(f, _a, 20)(10) << "\n";
		}

		{
			decltype(boost::placeholders::_1)& _a = boost::placeholders::_1;
			std::cout << boost::bind(f, _a, 20)(10) << "\n";
		}
	}
}

namespace test_store_bind
{
	//存储bind

	//bind生成的函数对象类型声明非常复杂，通常无法写出正确的类型

	//两种方法：1.auto 2.boost::function

	struct test_class
	{
		int add(int a, int b)
		{
			return a + b;
		}
	};

	void func()
	{
		//使用auto
		{
			//错误，std::greater没有result_type类型
			//auto bind_object = boost::bind(std::greater<int>(), boost::placeholders::_1, boost::placeholders::_2);

			//正确
			auto bind_object = boost::bind<bool>(std::greater<int>(), boost::placeholders::_1, boost::placeholders::_2);

			std::cout << bind_object(10, 20) << "\n";
		}

		////使用function
		{
			{
				test_class tc;

				boost::function<int(test_class&, int, int)> function_;     //注意是test_class&，不是test_class*
				//3个参数，导致bind必须有3个占位符

				function_ = boost::bind(&test_class::add, boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);
				std::cout << function_(tc, 10, 20);      
			}

			//或者

			{
				test_class tc;

				boost::function<int(int, int)> function_;
				//2个参数，导致bind必须有2个占位符

				function_ = boost::bind(&test_class::add, &tc, boost::placeholders::_1, boost::placeholders::_2);  //tc是类对象，类引用和类指针也都可以，见上面关于bind绑定成员函数的说明
				std::cout << function_(10, 20);
			}

			//可见
			//function对于成员函数，可以有两种函数签名形式，这两种形式导致两种参数数量，从而导致bind占位符个数不同，即也有两种形式
		}
	}
}

namespace test_bind_nest
{
	//bind可以嵌套，一个bind表达式生成的函数对象可以被另一个bind再绑定，从而实现类似f(g(x))的形式

	//如果我们有f(x)和g(x)两个函数，那么f(g(x))的bind表达式就是
	//bind(f, bind(g, _1))(x)
	
	//注意，bind的嵌套用法不太容易一次写正确，也不太容易理解，超过两个以上的bind表达式通常只能被编译器读懂，必须配合良好的注释才能够使用bind嵌套用法
}

namespace test_bind_operator
{
	//bind生成的函数对象重载了比较操作符和逻辑非操作符

	//使用bind的操作符重载可以构建出复杂的逻辑表达式，虽然好用，但不熟悉bind用法的读者通常很难理解代码，应当慎重使用


	void func()
	{
		using ri_type = boost::rational<int>;        //有理数

		std::vector<ri_type> vec = { ri_type(1, 2), ri_type(3, 4), ri_type(5, 6) };

		//删除所有分子为1的有理数
		std::remove_if(vec.begin(), vec.end(), boost::bind(&ri_type::numerator, boost::placeholders::_1) == 1);
		assert(vec[0].numerator() == 3);           //有理数1/2被删除

		//使用find_if算法查找分子是1的有理数，不存在
		assert(std::find_if(vec.begin(), vec.end(), boost::bind(&ri_type::numerator, boost::placeholders::_1) == 1) == vec.end());

		//查找分子大于3且分母小于8的有理数
		auto pos = std::find_if(vec.begin(), vec.end(), boost::bind(&ri_type::numerator, boost::placeholders::_1) > 3 && boost::bind(&ri_type::denominator, boost::placeholders::_1) < 8);

		std::cout << *pos << "\n";  //输出5/6

		//很多情况下复杂的逻辑判断可以使用C++11的lambda表达式来就地定义，形式上要比bind组合清晰

		auto pos2 = std::find_if(vec.begin(), vec.end(),
			[](ri_type& r)
			{
				return r.numerator() > 3 && r.denominator() < 8;
			});
	}
}

namespace test_overloaded_function
{
	//绑定重载函数

	//直接使用函数名的绑定方式存在一点局限，如果代码里有若干个同名的重载函数，那么bind就无法确定要绑定的具体函数，导致编译错误

	int f(int a, int b)
	{
		return a - b;
	}

	int f(double a, double b)
	{
		return a * b;
	}

	void func()
	{
		//std::cout << boost::bind(f, 1, 2)() << "\n";
		//错误：bind失败，无法确定绑定函数

		{
			//方法一：定义函数指针类型，再使用函数指针变量明确要绑定的函数
			//但这种方法对于函数模板重载无效，因为很难写出一个准确的模板函数指针类型

			using f_type1 = int(*)(int, int);
			using f_type2 = int(*)(double, double);

			f_type1 pf1 = f;
			f_type2 pf2 = f;
			//虽然写的都是f，但实际是不同的两个函数名

			std::cout << boost::bind(pf1, 1, 2)() << "\n";
			std::cout << boost::bind(pf2, 1, 2)() << "\n";
		}

		{
			//方法二：使用lambda来变通地“绑定”

			auto l = [](const std::string& x)
			{
				return boost::contains(x, "a");   //boost::algorithm::contains是重载函数模板
			};
		}
	}

}

namespace test_bind_nonstandard_function
{
	//绑定非标准函数

	//有些非标准函数bind无法自动推导出返回值类型，典型的就是C中的可变参数函数printf()。必须用bind<int>(printf, ...)(...)的形式
	//bind<int>(printf, "%d+%d=%d\n", _1, 1, _2)(6, 7);
	//bind的标准形式也不能支持使用了不同的调用方式(如__stdcall、__fastcall、extern "C")的函数，通常bind把他们看作函数对象，需要显式地指定bind的返回值类型才能绑定

	//也可以在头文件<boost/bind.hpp>之前加上BOOST_BIND_ENABLE_STDCALL、BOOST_BIND_ENABLE_FASTCALL或BOOST_BIND_ENABLE_PASCAL等宏，明确地告诉bind支持这些
	//调用方式
}





