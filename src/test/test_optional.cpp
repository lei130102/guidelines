#include "test_optional.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <functional>
#include <iostream>
#include <map>
#include <complex>

//optional管理一个可选的容纳值，既可以存在也可以不存在的值

//optional常用于一个可能失败的函数的返回值
//(像boost.property_tree中就有很多optional接口，比如get_child_optional接口，返回一个optional<ptree>对象，外面需要判断他是否是一个有效值
//来确定是否取到了对应的子节点)

//任何一个optional实例在给定时间点要么含值，要么不含值

//若一个optional含值，则保证值作为optional对象所用空间的一部分分配，即不会发生动态内存分配。
//从而optional对象模拟一个对象，而非指针，尽管定义了operator*()和operator->()运算符

//当一个optional对象被按语境转换成bool时，若对象含值则转换返回true，若对象不含值则返回false

//1.optional对象在下列条件下含值：
//a.对象被以T类型值或另一含值的optional初始化赋值
//2.optional对象在下列条件下不含值：
//a.对象被默认初始化
//b.对象被以std::nullopt_t类型值或不含值的optional对象初始化赋值
//c.调用了成员函数reset()

//无引用的 optional ：若以引用类型实例化 optional 则程序非良构。不过，可用 T 类型的 std::reference_wrapper 的 optional 保有引用。
//另外，若以（可有 cv 限定的）标签类型 std::nullopt_t 或 std::in_place_t 实例化 optional 则程序非良构。

BOOST_AUTO_TEST_SUITE(s_optional)

BOOST_AUTO_TEST_CASE(c_pair_t_bool)
{
	//std::pair<T, bool>
}

#include <optional>

BOOST_AUTO_TEST_CASE(c_std_optional)
{
	//std::optional C++17

	//template<class T>
	//class optional;

	//T必须满足可析构的要求(特别是不允许数组类型)

	using namespace test_optional_std_optional;

	//std::optional<T>::value_or
	//若*this拥有值则返回其所包含的值，否则返回default_value
	std::cout << "create(false) returned " << create(false).value_or("empty") << '\n';

	//返回optional的工厂函数可用作while和if的条件
	if (auto str = create2(true))
	{
		std::cout << "create2(true) returned " << *str << '\n';
	}

	if (auto str = create_ref(true))
	{
		//用get()访问reference_wrapper的值
		std::cout << "create_ref(true) returned " << str->get() << '\n';
		str->get() == "Mothra";
		std::cout << "modifying it changed it to " << str->get() << '\n';
	}

	for (auto s : { "42", "077", "hello", "0x33" })
	{
		std::optional<int> oi = asInt(s);
		if (oi)
		{
			std::cout << "convert '" << s << "' to int: " << *oi << "\n";
		}
		else
		{
			std::cout << "can't convert '" << s << "' to int\n";
		}
	}

	for (auto s : { "42", "077", "hello", "0x33" })
	{
		std::optional<int> oi = asInt(s);
		if (oi.has_value())
		{
			//value()比operator*更安全，没有值而调用该接口的话会抛出异常std::bad_optional_access，派生自std::exception。operator*只有确认有值的情况下才能使用，否则程序会出现未定义行为
			std::cout << "convert '" << s << "' to int: " << oi.value() << "\n";
		}
	}

	Name n{ "Jim", std::nullopt, "Knopf" };
	std::cout << n << '\n';
	Name m{ "Donald", "Ervin", "Knuth" };
	std::cout << m << '\n';

	////具体使用
	////1.使用optional构造函数
	{
		////创建没有值的optional
		//必须指定包含的类型，比如这里是int
		//不会为所包含的类型调用任何构造函数
		std::optional<int> o1;
		std::optional<int> o2(std::nullopt);

		////传递一个值来初始化所包含的类型
		//根据推导指南，不必指定所包含的类型
		std::optional o3{ 42 };        //推导为optional<int>
		std::optional<std::string> o4{ "hello" };
		std::optional o5{ "hello" };   //推导为optional<const char*>

		////要初始化一个具有多个参数的可选对象，必须创建该对象或将std::in_place添加为第一个参数(所包含类型无法推断)
		std::optional o6{ std::complex{3.0, 4.0} };
		std::optional<std::complex<double>> o7{ std::in_place, 3.0, 4.0 };//这种方式可以避免创建临时对象。通过使用这种形式，甚至可以传递初始化器列表和附加参数
		
		auto sc = [](int x, int y) //initialize set with lambda as sorting criterion:
		{
			return std::abs(x) < std::abs(y);
		};
		std::optional<std::set<int, decltype(sc)>> o8{ std::in_place, {4, 8, -7, -2, 0, 5}, sc };

		////拷贝构造
		//支持类型转换
		std::optional<std::string> o9{ o5 };   //o5是std::optional<const char*>类型

		////make_optional<>()函数
		//使用他可以使用单个或多个参数初始化(不需要in_place参数)   (函数推导)
		auto o10 = std::make_optional(3.0);     //std::optional<double>
		auto o11 = std::make_optional("hello"); //std::optional<const char*>
		auto o12 = std::make_optional<std::complex<double>>(3.0, 4.0);

		////注意构造函数不支持接收一个布尔值并根据他的值来决定是使用值初始化一个可选值还是使用std::nullopt
		//可以使用条件表达式达到这种效果
		bool b = true;
		auto o13 = b ? std::optional{ "Godzilla" } : std::nullopt;
		//o13初始化为std::optional<const char*>类型，这是由于std::optional{"Godzilla"}进行的类模板参数推导，而对于std::nullopt，类模板参数推导不起作用，但是
		//运算符?:在推导表达式的结果类型时也将其转换为这种类型

	}

	////2.访问值
	{
		std::optional o{ 42 };

		////检查optional对象是否有值

		//1.
		if (o)
		{
			//...
		}

		if (!o)
		{
			//...
		}

		//2.
		if (o.has_value())
		{
			//...
		}

		////访问值

		//1.operator*  (operator->用来访问值的成员)
		//std::optional o2{ std::pair{42, "hello"} };
		//auto p = *o2;                                //initializes p as pair<int, string>
		//std::cout << o2->first;                      //prints 42
		//(注意这些操作符要求optional包含一个值，在没有值的情况下使用它们是未定义的行为)
		//所以正确使用方式是使用前先检查
		std::optional o2{ std::pair{42, "hello"} };
		if (o2)
		{
			auto p = *o2;
		}
		if (o2)
		{
			std::cout << o2->first;
		}

		//2.value()   如果没有包含值，他会抛出一个std::bad_optional_access异常
		auto p2 = o2.value();
		std::cout << o2.value().first;

		//3.value_or(...)  如果optional对象没有值，那么可以请求该值
		std::cout << o.value_or(5);
		//回退参数作为rvalue引用传递，因此，如果不使用回退，则不需要花费任何代价，如果使用回退，则支持move语义
	}

	////3.比较操作
	{
		//如果两个操作数都是具有值的optional，则使用所包含类型的对应操作符
		//如果两个操作数都是没有值的optional，则认为他们是相等的 (==产生true，所有其他比较产生false)
		//如果只有一个操作数是有值的optional，则没有值的操作数被认为小于另一个操作数

		std::optional<int> o0;
		std::optional<int> o1{ 42 };
		o0 == std::nullopt;   //true
		o0 == 42;             //false
		o0 < 42;              //true
		o0 > 42;              //false
		o1 == 42;             //true
		o0 < o1;              //true

		//这意味着
		//对于无符号整型，没有值的optional对象相当于有一个小于0的值
		//对于布尔类型，没有值的optional对象相当于有一个小于0的值
		std::optional<unsigned> uo;
		uo < 0;               //true
		std::optional<bool> bo;
		bo < false;           //true

		//同样，支持底层类型的隐式类型转换：
		std::optional<int> o2{ 42 };
		std::optional<double> o3{ 42.0 };
		o3 == 42;             //true
		o2 == o3;             //true
	}

	////4.修改值
	{
		//通过赋值和emplace来修改值
		std::optional<std::complex<double>> o; //has no value
		std::optional ox{ 77 };                //optional<int> with value 77
		o = 42;                                //value becomes complex(42.0, 0.0)
		o = { 9.9, 4.4 };                      //value becomes complex(9.9, 4.4)
		o = ox;                                //OK, because int converts to complex<double>
		o.emplace(5.5, 7.7);                   //value becomes complex(5.5, 7.7)

		//赋值std::nullopt将删除该值，如果之前有值，该值将调用所包含类型的析构函数，也可以通过调用reset()获得相同的效果，或者赋值为空花括号
		o = std::nullopt;                      //o no longer has a value
		o.reset();                             //o no longer has a value
		o = {};                                //o no longer has a value


		
		//使用operator*修改存在的值
		std::optional < std::complex<double>> o2;
		//...
		if (o2)
		{
			*o2 = 88;                           //OK: value becomes complex(88.0, 0.0)
			*o2 = { 1.2, 3.4 };                 //OK: value becomes complex(1.2, 3.4)
		}
	}

	////5.移动语义
	{
		//std::optional<>也支持移动语义，如果将optional对象作为一个整体移动，则会复制状态并移动所包含的对象(如果有的话)。
		//因此，一个从对象中移出的对象仍然具有相同的状态，但是移动后的值会变成是未指定的，但是也可以将一个值移动到或移出
		//所包含的对象
		std::optional<std::string> os;
		std::string s = "a very very very long string";
		os = std::move(s);                                 //OK, moves
		std::string s2 = *os;                              //OK, copies
		std::string s3 = std::move(*os);                   //OK, moves
		//注意，在最后一次调用之后os仍然有一个字符串值，但是对于os对象这个值未指定。因此，不能对移动后的对象的值做任何假设就去使用他，
		//当然，可以给移动后的对象分配一个新的值
	}

	//bool或者原始指针的optional
	{
		//注意，使用比较操作符与使用optional对象作为布尔值具有不同的语义，如果包含的类型是bool或者指针类型，这就会变得很混乱
		std::optional<bool> ob{ false };           //has value, which is false
		if (!ob)           //false
		{
			//...
		}
		if (ob == false)   //true
		{
			//...
		}

		std::optional<int*> op{ nullptr };
		if (!op)           //false
		{
			//...
		}
		if (op == nullptr) //true
		{
			//...         
		}
	}

	//optional的optional
	{
		//可以定义一个optional的optional值
		std::optional<std::optional<std::string>> oos1;
		std::optional<std::optional<std::string>> oos2 = "hello";
		std::optional<std::optional<std::string>> oos3(std::in_place, std::in_place, "hello");
		std::optional<std::optional<std::complex<double>>> ooc{ std::in_place, std::in_place, 4.2, 5.3 };

		//也可以分配新的值，即使隐式转换
		oos1 = "hello";                  //OK: assign new value
		ooc.emplace(std::in_place, 7.2, 8.3);

		//由于optional的无值有两个层次，optional的optional使“无值”出现在外部或内部，可以有不同的语义含义：
		*oos1 = std::nullopt;            //inner optional has no value
		oos1 = std::nullopt;             //outer optional has no value

		//但必须特别注意处理optional值
		if (!oos1)
		{
			std::cout << "no value\n";
		}
		if (oos1 && !*oos1)
		{
			std::cout << "no inner value\n";
		}
		if (oos1 && *oos1)
		{
			std::cout << "value: " << **oos1 << '\n';
		}
	}
}

BOOST_AUTO_TEST_CASE(c_boost_optional)
{
	using namespace test_optional_boost_optional;

	std::string text = "ceshi";

	//可以：
	//boost::optional<int> oi = convert(text);    //move-construct
	//if (oi)                                     //contextual conversion to bool
	//{
	//	int i = *oi;                            //operator*
	//}
	//更好：
	if (boost::optional<int> oi = convert(text))
	{
		int i = *oi;
	}


	//尝试解引用一未初始化的optional对象会造成未定义行为(undefined behaviour,UB)，所以下面的代码是"reasonably UB-safe"
	int i = *convert("100");
	//更好的方式是：
	try
	{
		int j = convert(text).value();
	}
	catch (const boost::bad_optional_access&)
	{
		//deal with it
	}
	//更好的方式是：
	int k = convert(text).value_or(0);
	//更好的方式是：
	int l = convert(text).value_or_eval(fallback_to_default);
	//This will call the provided callback and return whatever the callback returns. The callback can have side effects: they will only be observed when the optional object does not contain a value.


	//Quite a lot of people expect that when an object that contains a value is moved from, its contained value should be destroyed.
	//This is not so, for performance reasons. Current semantics allow the implementation of boost::opiotnal<T> to be trivially copyable when T is trivial.
	boost::optional<std::unique_ptr<int>> opi{ std::make_unique<int>(1) };
	boost::optional<std::unique_ptr<int>> opj = std::move(opi);
	BOOST_CHECK(opi);
	BOOST_CHECK(*opi == nullptr);



	for (unsigned i = 0; i < 10; ++i)
	{
		boost::optional<locked_device> t = locked_device::try_lock_device();
		if (t)
		{
			t->use();   //解引用 t-> *t
		}
		else
		{
			std::cout << "... tring again\n";
		}
	}

	//boost::optional<T>可以转换为bool类型
	//boost::optional<locked_device> t;可以转换为一个保存了false的布尔变量，并且不得解引用，因为他没有构造一个基本类型
}

BOOST_AUTO_TEST_CASE(c_qicosmos_optional)
{
	using namespace test_optional_qicosmos_optional;

	optional<std::string> a("ok");
	optional<std::string> b("ok");
	optional<std::string> c("aa");

	c = a;
	if (c < a)
	{
		std::cout << "<" << std::endl;
	}

	if (a == b)
	{
		std::cout << "=" << std::endl;
	}

	std::map<optional<std::string>, int> mymap;
	mymap.insert(std::make_pair(a, 1));
	mymap.insert(std::make_pair(c, 2));

	auto it = mymap.find(a);
	std::cout << it->second << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()