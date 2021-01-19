#pragma once

#include <string>
#include <functional>
#include <iostream>
#include <type_traits>

#include <stdlib.h>


#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>



#include <optional>

namespace test_optional_std_optional
{
	////optional用途
	//1.optional可用作可能失败的工厂的返回类型

	std::optional<std::string> create(bool b)
	{
		if (b)
		{
			return "Godzilla";
		}
		else
		{
			return {};//注意，返回的是没有值的std::optional
		}
	}

	//能用std::nullopt创建任何(空的)std::optional
	auto create2(bool b)
	{
		return b ? std::optional<std::string>{"Godzilla"} : std::nullopt; //使用std::nullopt后返回的是无值的std::optional
	}

	//std::reference_wrapper可用于返回引用
	auto create_ref(bool b)
	{
		static std::string value = "Godzilla";
		return b ? std::optional<std::reference_wrapper<std::string>>{value} : std::nullopt;
	}

	std::optional<int> asInt(std::string const& s)
	{
		try
		{
			return std::stoi(s);
		}
		catch (...)
		{
			return std::nullopt;
		}
	}

	std::optional<int> asInt2(std::string const& s)
	{//与asInt同样功能
		std::optional<int> ret;
		try
		{
			ret = std::stoi(s);
		}
		catch (...)
		{
		}
		return ret;
	}

	//2.optional用作参数和数据成员
	class Name
	{
	private:
		std::string first;
		std::optional<std::string> middle;
		std::string last;

	public:
		Name(std::string f, std::optional<std::string> m, std::string l)
			:first{ std::move(f) }, middle{ std::move(m) }, last{ std::move(l) }
		{}

		friend std::ostream& operator<<(std::ostream& strm, const Name& n)
		{
			strm << n.first << ' ';
			if (n.middle)
			{
				strm << n.middle.value() << ' ';
			}
			return strm << n.last;
		}
	};
}

#include <boost/optional.hpp>

namespace test_optional_boost_optional
{
	boost::optional<int> convert(const std::string& text)
	{
		std::stringstream s(text);
		int i;
		if ((s >> i) && s.get() == std::char_traits<char>::eof())
		{
			return i;             //调用转换构造函数将T转为optional<T>
		}
		else
		{
			return boost::none;   //调用另一个转换构造函数
		}
	}
	//或者另一种实现方式，只有一个return
	boost::optional<int> convert2(const std::string& text)
	{
		boost::optional<int> ans;
		std::stringstream s(text);
		int i;
		if ((s >> i) && s.get() == std::char_traits<char>::eof())
		{
			ans = i;  //如果ans未初始化，那么调用int的拷贝构造，如果ans已初始化，那么调用int的赋值操作
		}
		return ans;
	}

	int fallback_to_default()
	{
		std::cerr << "could not convert; using -1 instead" << std::endl;
		return -1;
	}



	//optional作为数据成员
	//假设我们想实现一个缓式加载优化，因为我们不想执行一个昂贵的资源初始化，直到他被使用时
	class Resource
	{
	public:
		Resource(std::string const& key, std::string const& value)
		{}

	};

	class Widget
	{
	public:
		Widget() {}

		const Resource& getResource() const   //not thread-safe
		{
			if (resource_ == boost::none)   //也可以写成 if(!resource_)
			{
				//没有调用Resource的拷贝构造和移动构造
				resource_.emplace("resource", "arguments");//Function emplace is only available on compilers that support rvalue references and variadic templates. If your compiler does not support these features and you still need to avoid any move-constructions, use In-Place Factories.
			}

			return *resource_;
		}
	private:
		mutable boost::optional<const Resource> resource_;
	};


	////避免不必要的默认构造
	//假设要实现一个函数，该函数返回同一个自定义类型的两个对象，并且自定义类型没有默认构造函数
	//class Date    //没有默认构造函数
	//{
	//public:
	//	Date(int i)
	//	{}
	//};

	//boost::tuple<Date, Date> getPeriod()
	//{
	//	//...
	//}

	//错误使用：
	//Date begin, end;                         //Error: no default constructor
	//boost::tie(begin, end) = getPeriod();    //Ok   这行正确

	//正确使用：
	//boost::optional<Date> begin, end;
	//boost::tie(begin, end) = getPeriod();
	//It works because inside boost::tie a move - assignment from T is invoked on optional<T>, which internally calls a move - constructor of T.


	//optional作为容器元素
	//Suppose you want to ask users to choose some number (an int). One of the valid responses is to choose nothing, which is represented by an uninitialized optional<int>. You want to make a histogram showing how many times each choice was made. You can use an std::map:

	//std::map<boost::optional<int>, int> choices;
	//for (int i = 0; i < LIMIT; ++i)
	//{
	//	boost::optional<int> choice = readChoice();
	//	++choices[choice];
	//}
	//This works because optional<T> is LessThanComparable whenever T is LessThanComparable. In this case the state of being uninitialized is treated as a yet another value of T, which is compared less than any value of T.





	//In-Place Factories
	//包装器和容器的一个典型问题是，他们的接口通常提供一个操作来初始化包含的对象或将其指定为其他对象的副本。这不仅要求基础类型是可复制的，而且还要求
	//存在一个完全构造的对象(通常是临时的)，仅从以下位置复制：
	//struct X
	//{
	//	X(int, std::string);
	//};

	//class W
	//{
	//public:
	//	W(X const& x) : wrapped_(x) {}

	//private:
	//	X wrapped_;
	//};

	//void foo()
	//{
	//	//Temporary object created
	//	W(X(123, "hello"));
	//}
	//解决这个问题的一个方法是支持在容器的存储中直接构造包含的对象。在这个方案中，用户只需要向构造函数提供参数，以便在包装对象构造中使用
	//class W
	//{
	//public:
	//	W(X const& x) : wrapped_(x) {}
	//	W(int a0, std::string a1) : wrapped_(a0, a1) {}

	//private:
	//	X wrapped_;
	//};

	//void foo()
	//{
	//	//Wrapped object constructed in-place
	//	//No temporary created.
	//	W(123, "hello");
	//}
	//这种方法的一个限制是，如果构造函数重载未知，他不能很好地扩展到包含多个构造函数的包装对象，也不能扩展到泛型代码
	//optional库中提供的解决方案是InPlaceFactories和TypedInPlaceFactories家族，这些工厂是一系列类，他们封装了越来越多
	//的任意构造函数参数，并提供了一种方法，在用户通过放置new指定的地址处使用这些参数来构造给定类型的对象

	//比如家族中的一个成员:
	//template<class T, class A0, class A1>
	//class TypedInPlaceFactory2
	//{
	//public:
	//	TypedInPlaceFactory2(A0 const& a0, A1 const& a1) : m_a0(a0), m_a1(a1) {}
	//	void construct(void* p) { new (p)T(m_a0, m_a1); }
	//private:
	//	A0 m_a0, A1 m_a1;
	//};
	//对应的包装类为：
	//class W
	//{
	//public:
	//	W(X const& x) : wrapped_(x) {}
	//	W(TypedInPlaceFactory2 const& fac) { fac.construct(&wrapped_); }
	//private:
	//	X wrapped_;
	//};
	//
	//void foo()
	//{
	//	//Wrapped object constructed in-place via a TypedInPlaceFactory
	//	//No temporary created.
	//	W(TypedInPlaceFactory2<X, int, std::string>(123, "hello"));
	//}

	//工厂分为两组：
	//1.TypedInPlaceFactories:   将目标类型作为主模板参数的工厂
	//2.InPlaceFactories:        带有目标类型的"construct(void*)"成员函数模板的工厂
	//在每个组中，家族成员只在允许的参数数量上有所不同
	//本库提供了一组重载的帮助器模板函数来构造这些工厂，而不需要不必要的模板参数

	//template<class A0, ..., class AN>
	//InPlaceFactoryN<A0, ..., AN> in_place(A0 const& a0, ..., AN const& aN);
	//template<class T, class A0, ..., class AN>
	//TypedInPlaceFactoryN<T, A0, ..., AN> in_place(T const& t, A0 const& a0, ..., AN const& aN);

	//包装器和用户通常可以以如下方式使用In-place factories:
	//class W
	//{
	//public:
	//	W(X const& x) :wrapped_(x) {}

	//	template<class InPlaceFactory>
	//	W(InPlaceFactory const& fac)
	//	{
	//		fac.template<X>construct(&wrapped_);
	//	}
	//private:
	//	X wrapped_;
	//};
	//void foo()
	//{
	//	//Wrapped object constructed in-place via a InPlaceFactory.
	//	//No temporary created.
	//	W(in_place(123, "hello"));
	//}


	//使用boost::optional<bool>时应特别小心和考虑，功能上类似三态布尔值，优先使用boost::tribool




	class locked_device
	{
		explicit locked_device(const char* param)
		{
			//我们拥有对设备的独占式访问
			std::cout << "Device is locked\n";//设备被锁定
		}

	public:
		~locked_device()
		{
			//释放设备锁
		}

		void use()
		{
			std::cout << "Success!\n";//成功
		}

		static boost::optional<locked_device> try_lock_device()
		{
			//返回值为boost::optional<locked_device>
			//成功则返回locked_device，失败则返回boost::none
			if (rand() % 2)
			{
				//未能锁定设备
				return boost::none;
			}
			//成功
			return locked_device("device name");//设备名
		}
	};

}

namespace test_optional_qicosmos_optional
{
	//qicosmos用C++11实现的optional
	//用法与boost.optional的用法保持一致，实现起来也比较简单

	//1.内部存储空间
	//由于optional<T>需要容纳T的值，所以需要一个缓冲区保存这个T，这个缓冲区不能用普通的char数组，需要用内存对齐的缓冲区，
	//这里采用std::aligned_storage

	//2.拷贝构造函数和赋值运算函数
	//需要注意拷贝和赋值时，内部状态和缓冲区销毁的问题。内部状态用来表示该optional是否被初始化，当已经初始化时需要先将缓冲区清理一下。
	//需要增加右值版本优化效率

	template<typename T>
	class optional
	{
		using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

	public:

		////构造函数

		optional() : m_has_init(false) {}

		optional(const T& v)
		{
			create(v);
		}

		optional(T&& v) : m_has_init(false)    //为什么这里有"m_has_init(false)"而上面没有?
		{
			create(std::move(v));
		}

		////析构函数

		~optional()
		{
			destroy();
		}

		////拷贝构造函数

		optional(const optional& other) : m_has_init(false)
		{
			if (other.is_init())
			{
				assign(other);
			}
		}

		////移动构造函数

		optional(optional&& other) : m_has_init(false)
		{
			if (other.is_init())
			{
				assign(std::move(other));
				other.destroy();            //对other来说，确保语义正确
			}
		}

		////赋值运算函数

		optional& operator=(const optional& other)
		{
			assign(other);
			return *this;
		}

		optional& operator=(optional&& other)
		{
			assign(std::move(other));
			other.destroy();                //对other来说，确保语义正确   (这个是我自己补充上的)
			return *this;
		}

		////operator*函数(const 非const)

		T& operator*()
		{
			return *((T*)(&m_data));
		}

		T const& operator*() const
		{
			//为什么非const中没有判断？
			if (is_init())
			{
				return *((T*)(&m_data));
			}

			throw std::exception("");
		}

		////operator== operator< operator!=

		bool operator==(const optional<T>& rhs) const
		{
			return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
		}

		bool operator!=(const optional<T>& rhs)
		{
			return !(*this == (rhs));
		}

		bool operator<(const optional<T>& rhs) const
		{
			return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
		}

		////其他辅助函数

		template<class... args>
		void emplace(args&&... args_)
		{
			destroy();
			create(std::forward<args>(args_)...);
		}

		bool is_init() const
		{
			return m_has_init;
		}

		explicit operator bool() const           //需要用explicit吗？
		{
			return is_init();
		}

	private:

		template<class... args>
		void create(args&&... args_)
		{
			new (&m_data)T(std::forward<args>(args_)...);
			m_has_init = true;
		}

		void destroy()
		{
			if (m_has_init)
			{
				m_has_init = false;
				((T*)(&m_data))->~T();
			}
		}

		void assign(const optional& other)
		{
			if (other.is_init())
			{
				copy(other.m_data);

				m_has_init = true;
			}
			else
			{
				destroy();
			}
		}

		void assign(optional&& other)
		{
			if (other.is_init())
			{
				move(std::move(other.m_data));
				other.destroy();

				m_has_init = true;
			}
			else
			{
				destroy();
			}
		}

		void move(data_t&& val)
		{
			destroy();
			new (&m_data)T(std::move(*((T*)(&val))));
		}

		void copy(data_t const& val)
		{
			destroy();
			new (&m_data)T(*((T*)(&val)));
		}

	private:
		bool m_has_init;
		data_t m_data;
	};
}