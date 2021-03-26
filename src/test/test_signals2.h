#pragma once

//按照boost::reference_wrapper boost::bind boost::function boost::signals2::signal的顺序学习

#include <boost/signals2.hpp>

#include <boost/make_shared.hpp>
#include <boost/random.hpp>

#include <numeric>
#include <iostream>

//signals2库的核心是signal类，相当于C#语言中的event+delegate

//template<typename Signature,
//		 typename Combiner = boost::signals2::optional_last_value<R>,
//		 typename Group = int,
//		 typename GroupCompare = std::less<Group>>
//class signal : public boost::signals2::signal_base

//signal的模板参数列表相当长，总共有7个参数，这里仅列出了最重要的前4个，而且除第一个是必须的外，其他的都可以使用默认值
//第一个模板参数Signature的含义与function的相同，也是一个函数类型，表示可被signal调用的函数(插槽、事件处理handler)，例如：signal<void(int, double)>
//第二个模板参数Combiner是一个函数对象，他被称为“合并器”，用来组合所有插槽的调用结果，默认是optional_last_value<R>，他使用optional库返回最后一个被调用的插槽的返回值
//第三个模板参数Group是插槽编组的类型，默认使用int来标记组号，也可以改为std::string等类型，但通常没有必要
//第四个模板参数GroupCompare与Group配合使用，用来确定编组的排序准则，默认是升序(std::less<Group>)，因此要求Group必须定义了operator<

//signal继承自signal_base，而signal_base又继承自noncopyable，因此signal是不可拷贝的。如果把signal作为自定义类点的成员变量，那么自定义类也将是不可拷贝的，
//除非使用shared_ptr/ref来间接持有它



//成员函数connect()把插槽连接到信号上，相当于为信号（事件）增加了一个处理的handler

//插槽可以是任意的可调用对象，包括函数指针、函数对象，以及他们的bind/lambda表达式和function对象，signal内部使用function作为容器来保存这些可调用对象

//连接时可以指定组号也可以不指定组号，当信号发生时将依据组号的排序准则依次调用插槽函数。如果连接成功，connect()将返回一个connection对象，表示了信号与
//插槽之间的连接关系，他是一个轻量级的对象，可以处理两者间的连接，例如断开、重连接或者测试连接状态

//成员函数disconnect()可以断开插槽与信号的连接，他有两种形式：
//传递组号将断开该组的所有插槽
//传递一个插槽对象将仅断开该插槽

//成员函数disconnect_all_slots()可以一次性断开信号的所有插槽的连接

//成员函数num_slots()可以获得当前信号所连接的插槽数量
//成员函数empty()相当于num_slots() == 0，但他的执行效率比num_slots()高，disconnect_all_slots()的执行结果就是令empty()返回true

//signal提供operator()，可以接受最多9个参数。当operator()被外界调用时意味着产生了一个信号(事件)，从而导致信号所关联的所有插槽被调用，插槽调用的结果
//使用合并器处理后返回，默认情况下是一个optional对象

//成员函数combiner()和set_combiner()分别用于获取和设置合并器对象，通过signal的构造函数也可以在创建的时候就传入一个合并器的实例。但除非你想改用其他的
//合并方式，通常我们可以直接使用默认构造函数创建模板参数列表中指定的合并器对象

//当signal析构时，将自动断开所有插槽连接，相当于调用disconnect_all_slots()

namespace test_signals2_signal
{
	//定义两个插槽，可以是任意可放入boost::function中的东西
	void slot1()
	{
		std::cout << "slot1 called" << "\n";
	}

	void slot2()
	{
		std::cout << "slot2 called" << "\n";
	}

	void func()
	{
		//signal就像是一个增强的function对象，他可以容纳(使用connect()连接)多个符合模板参数中函数签名类型的函数(插槽)，
		//形成一个插槽链表，然后在信号发生时一起调用

		//除了类名称不同，signal的声明语法与function几乎一模一样

		boost::signals2::signal<void()> signal_;     //指定插槽类型void()，其他模板参数使用默认值

		//然后就可以使用connect()来连接插槽，最后用operator()来产生信号
		signal_.connect(&slot1);
		signal_.connect(&slot2);

		//调用operator()，产生信号(事件)，触发插槽调用
		signal_();       

		//注意:
		//在连接插槽时我们省略了connect()的第二个参数connect_position，他的默认值是at_back，表示插槽将插入到信号插槽链表的尾部，因此slots2将在slots1之后被调用，运行结果为：
		//slot1 called
		//slot2 called

		//如果在连接slots2的时候不使用默认参数，而是明确地传入at_front位置标志，即
		//sig.connect(&slots2, at_front);
		//那么slots2将在slots1之前被调用
	}
}

namespace test_signals2_slot_group_of_signal
{
	//类模板，可以生成一系列的插槽
	template<int N>
	struct slot
	{
		void operator()()
		{
			std::cout << "slot" << N << " called" << "\n";
		}
	};

	void func()
	{
		boost::signals2::signal<void()> signal_;

		signal_.connect(slot<1>(), boost::signals2::at_back);        //默认，放在链表末尾
		signal_.connect(slot<100>(), boost::signals2::at_front);     //放在链表开头

		//放到组号为5的组中
		signal_.connect(5, slot<51>(), boost::signals2::at_back);    
		signal_.connect(5, slot<55>(), boost::signals2::at_front);    

		//输出：
		//slot100 called
		//slot55 called
		//slot51 called
		//slot1 called

		//注意：
		//组与组的顺序是signal的GroupCompare模板参数决定的
	}
}

namespace test_signals2_return_value_of_signal
{
	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	void func()
	{
		boost::signals2::signal<int(int)> signal_;

		signal_.connect(slot<10>());
		signal_.connect(slot<20>());
		signal_.connect(slot<50>());

		std::cout << *signal_(20) << "\n";     //20是传入的输入参数

		//输出:
		//50

		//如function一样，signal不仅可以把输入参数转发给所有插槽，也可以传回插槽的返回值。
		//默认情况下signal使用合并器optional_last_value<R>，他将
		//使用optional对象返回最后被调用的插槽的返回值

		//注意：
		//signal的operator()调用这是需要传入一个整数参数，这个参数会被signal存储一个拷贝，然后转发给各个插槽。
		//最后signal将返回插槽链表末尾slots<50>()的计算结果，他是一个optional对象，必须用解引用操作符*来获得值
	}
}

namespace test_signals2_custom_combiner
{
	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	//缺省的合并器optional_last_value<R>并没有太多的意义，他通常用在我们不关心插槽返回值或者返回值是void的时候，
	//但大多数时候，插槽的返回值都是有意义的，需要以某种方式处理多个插槽的返回值

	//signal允许用户自定义合并器来处理插槽的返回值，把多个插槽的返回值合并为一个结果返回给用户。
	//合并器应该是一个函数对象(不是函数或者函数指针)

	template<typename T>
	class combiner                //自定义合并器
	{
		T v;

	public:

		using result_type = std::pair<T, T>;

		combiner(T t = T{}) :v(t) {}

		//operator()的返回值类型可以是任意类型，完全由用户指定，不一定必须是optional或者是插槽的返回值类型。
		//operator()的模板参数InputIterator是插槽链表的返回值迭代器，可以使用他来遍历所有插槽的返回值
		template<typename InputIterator>
		result_type operator()(InputIterator begin, InputIterator end) const
		{
			if (begin == end)
			{
				return result_type();     
			}

			std::vector<T> vec(begin, end);

			T sum = std::accumulate(vec.begin(), vec.end(), v);   //#include <numeric>
			T max = *std::max_element(vec.begin(), vec.end());

			//注意！！
			//没有直接使用标准库的算法accumulate和max_element来操作迭代器，因为那样会遍历两次插槽列表，导致所有插槽被调用两次

			return result_type(sum, max);
		}
	};

	void func()
	{
		boost::signals2::signal<int(int), combiner<int>> signal_;  //指定合并器
		
		//注意：
		//这里没有向构造函数传递合并器的实例，因为signal的构造函数会默认构造出一个实例(即内部combiner<int>())

		signal_.connect(slot<10>());
		signal_.connect(slot<20>());
		signal_.connect(slot<30>(), boost::signals2::at_front);

		auto result = signal_(2);  //不再是默认的boost::optional对象
		std::cout << result.first << "," << result.second;

		//当信号被调用时，signal会自动把解引用操作转换为插槽调用，使用给定的合并器的operator()逐个处理插槽的返回值，
		//并最终返回合并器operator()的结果
	}

	void func2()
	{
		boost::signals2::signal<int(int), combiner<int>> signal_(combiner<int>(100));  //指定合并器
		
		//注意：
		//如果我们不使用signal的默认构造函数，而是在构造signal时传入一个合并器的实例，那么signal将使用这个合并器(的拷贝)处理返回值

		signal_.connect(slot<10>());
		signal_.connect(slot<20>());
		signal_.connect(slot<30>(), boost::signals2::at_front);

		auto result = signal_(2); 
		std::cout << result.first << "," << result.second;
	}

	//因为signal会自动把解引用操作转换为插槽调用，所以自定义合并器某种程度上也相当于一个插槽调用器，程序可以不要求所有的插槽被调用，只选择那些符合
	//条件的插槽，比如当一个插槽的返回值满足要求后就终止迭代，不再调用剩余的插槽

	//下面的合并器当得到一个大于100的返回值就停止插槽调用:
	class combiner2
	{
	public:
		using result_type = bool;

		template<typename InputIterator>
		result_type operator()(InputIterator begin, InputIterator end) const
		{
			for (; begin != end; ++begin)
			{
				if (*begin > 100)
				{
					return true;
				}
			}
			return false;
		}
	};
}

namespace test_signals2_disconnect
{
	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	template<int N>
	bool operator==(const slot<N>&, const slot<N>&)         //因为slot<N>没有状态，所以他的等价比较非常简单
	{
		return true;
	}

	void func()
	{
		boost::signals2::signal<int(int)> signal_;

		//连接到组号0
		signal_.connect(0, slot<10>());   
		signal_.connect(0, slot<20>());
		//连接到组号1
		signal_.connect(1, slot<30>());

		//信号与插槽的连接并不要求是永久的，当信号调用完插槽后，有可能需要把插槽从信号中断开

		//成员函数disconnect()断开一个或者一组插槽
		signal_.disconnect(0);
		signal_.disconnect(slot<30>());   //注意：要断开一个插槽，插槽必须能够进行等价比较，对于函数对象来说就是重载一个等价语义的operator==

		//成员函数disconnect_all_slots()断开所有插槽连接
		signal_.disconnect_all_slots();

		//成员函数函数empty()和成员函数num_slots()用来检查信号当前插槽的连接状态
		assert(signal_.empty());
		assert(signal_.num_slots() == 0);
	}
}

namespace test_signals2_connection_object
{
	//另外一种较为灵活的连接管理方式是使用connection对象
	//每当signal使用connect()连接插槽时，他就会返回一个connection对象。connection对象像是信号与插槽连接关系的一个句柄(handle)，可以管理连接
	//class connection
	//{
	//public:
	//    //构造函数与析构函数
	//    connection();
	//    connection(const connection&);
	//    connection& operator=(const connection&);
	
	//    //插槽连接管理
	//    void disconnect() const;
	//
	//    //判断是否连接
	//    bool connected() const;
	
	//    //判断是否阻塞
	//    bool blocked() const;
	
	//    //交换
	//    void swap(const connection&);
	
	//    //比较操作
	//    bool operator==(const connection&) const;
	//    bool operator!=(const connection&) const;
	//    bool operator<(const connection&) const;
	//};

	//connection是可拷贝可赋值的，他也重载了比较操作符，因而可以被安全地放入标准序列容器或者关联容器中，成员函数disconnect()和connected()分别用来与信号断开连接和
	//检测连接状态

	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	void func()
	{
		boost::signals2::signal<int(int)> signal_;

		boost::signals2::connection connection_10 = signal_.connect(0, slot<10>());
		boost::signals2::connection connection_20 = signal_.connect(0, slot<20>());
		boost::signals2::connection connection_30 = signal_.connect(1, slot<30>());

		connection_10.disconnect();                    //断开第一个连接
		assert(signal_.num_slots() == 2);              //sig现在连接两个插槽
		assert(!connection_10.connected());            //c1不再连接信号
		assert(connection_20.connected());             //c2仍然连接
	}

	void func2()
	{
		//另一种连接管理对象是scoped_connection，他是connection的子类，提供类似scoped_ptr的RAII功能：插槽与信号的连接仅在作用域内生效，当离开作用域时连接就会自动断开
		//当需要临时连接信号时，scoped_connection会非常有用

		boost::signals2::signal<int(int)> signal_;

		{
			boost::signals2::scoped_connection scoped_connection_ = signal_.connect(0, slot<10>());
		}
	}

	void func3()
	{
		//插槽与信号的连接一旦断开就不能再连接起来，connection不提供reconnect()这样的函数，但可以暂时地阻塞插槽与信号的连接，
		//当信号发生时被阻塞的插槽将不会被调用

		//connection对象的blocked()函数可以检测插槽是否被阻塞，但被阻塞的插槽并没有断开与信号的连接，
		//在需要的时候可以随时解除阻塞

		//connection自身没有阻塞的功能(只有判断是否阻塞)，我们需要用一个辅助类shared_connection_block来实现。shared_connection_block可以阻塞connection对象，直到他被析构或者
		//显式调用unblock()函数

		boost::signals2::signal<int(int)> signal_;

		boost::signals2::connection connection_10 = signal_.connect(0, slot<10>());
		boost::signals2::connection connection_20 = signal_.connect(0, slot<20>());
		signal_(2);                //调用两个插槽

		std::cout << "begin blocking..." << "\n";
		{
			boost::signals2::shared_connection_block shared_connection_block_(connection_10);  //阻塞connection_10连接
			assert(signal_.num_slots() == 2);                               //仍然有两个连接
			assert(connection_10.blocked());                                //connection_10被阻塞
			signal_(2);                                                     //只有一个插槽会被调用
		}
		//离开shared_connection_block_作用域，阻塞自动解除
		std::cout << "end blocking..." << "\n";

		assert(!connection_10.blocked());
		signal_(2);                                                         //可以调用两个插槽
	}
}

namespace test_signals2_auto_connection_manager
{
	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	//之前的槽函数都是通过拷贝实例的方式传递给signal，实际也可以利用boost::ref变相传入一个引用，但这样存在一个问题：当触发信号
	//调用槽时，槽如果已被销毁，那么将会发生未定义行为

	//错误演示
	//void func()
	//{
	//	boost::signals2::signal<int(int)> signal_;

	//	signal_.connect(slot<10>());

	//	auto p = new slot<20>;
	//	signal_.connect(boost::ref(*p));//连接到引用

	//	delete p;

	//	signal_(1); //将发生未定义行为
	//}

	//正确做法

	//使用slot类提供了自动连接管理的功能，能够自动跟踪插槽的生命周期，当插槽失败时会自动断开连接，以保证程序不会发行运行错误

	//template<typename Signature,                                    //Function type R(T1,...,TN)
	//		 typename SlotFunction = boost::function<R(T1,...,TN)>>
	//class slot : public boost::signals2::slot_base
	//{
	//public:
	//	template<typename Func, typename Arg...>
	//	slot(const Func&, const Arg&...);            //重点，用slot封装函数对象

	//	template<typename Slot>
	//	slot(const Slot&);
	
	//	result_type operator()(...);
	
	//	//跟踪功能
	//	slot& track(const boost::weak_ptr<void>&);  //重点，调用track函数实现跟踪功能，注意参数类型为boost::weak_ptr<void>，不能使用std::weak_ptr
	//};

	//signals2::slot模板类可以自动管理插槽的连接，但通常我们不直接使用他，而是使用signal的内部typedef slot_type，他已经定义好了该signal所使用的slot的模板参数

	void func()
	{
		boost::signals2::signal<int(int)> signal_;

		////连接一个普通槽
		signal_.connect(slot<10>());          

		////连接一个当槽对象销毁时自动断开连接的槽
		//如果要使用自动管理连接的功能，那么在信号连接时我们就不能直接连接插槽，而是要用slot的构造函数包装插槽，然后再调用成员函数track()来跟踪插槽使用的资源。
		//(注意！！！track()函数不支持C++11标准的std::weak_ptr，只能使用boost.weak_ptr)
		auto p_slot_20 = boost::make_shared<slot<20>>();    //因为下面要用boost::weak_ptr，所以这里不能用std::make_shared
		signal_.connect(boost::signals2::signal<int(int)>::slot_type(boost::ref(*p_slot_20)).track(p_slot_20));

		p_slot_20.reset();   //这里故意销毁槽

		assert(signal_.num_slots() == 1);    //一个插槽被自动断开
		signal_(1);                          //将只有一个插槽被调用

		//注意：
		//在track(p_slot_20)中，存在boost::shared_ptr到boost::weak_ptr的隐式转换
	}

	//注意slot的构造函数，他支持与bind表达式相同的语法
	void func2()
	{
		boost::signals2::signal<int(int)> signal_;

		auto p_slot_10 = boost::make_shared<slot<10>>();
		auto p_slot_20 = boost::make_shared<slot<20>>();

		boost::function<int(int)> function_ = boost::ref(*p_slot_10);  //function存储引用

		signal_.connect(boost::signals2::signal<int(int)>::slot_type(function_).track(p_slot_10));
		//构造函数实参是boost::function<int(int)>对象

		signal_.connect(boost::signals2::signal<int(int)>::slot_type(&slot<20>::operator(), p_slot_20.get(), boost::placeholders::_1).track(p_slot_20));
		//构造函数实参使用bind语法
		//注意：必须传递给slot类(即slot_type所表示的类)原始指针，否则slot类会持有一个shared_ptr的拷贝，导致引用计数增加，妨碍了shared_ptr的资源管理

		p_slot_10.reset();    //销毁两个指针对象
		p_slot_20.reset();    //销毁两个指针对象

		assert(signal_.num_slots() == 0);   //此时已经自动断开了所有连接

		signal_(1);           //不发生任何插槽调用
	}
}

namespace test_signal2_observer_pattern
{
	//使用signals2开发一个完整的观察者模式示例程序，用来演示信号/插槽的用法
	//这个程序将模拟一个日常生活情景：访客按门铃，门铃响，护士开门，婴儿哭闹

	//门铃
	//拥有一个signal对象，当按门铃时就会发出信号
	//注意，因为signal不可拷贝，所以ring不可拷贝
	class ring
	{
		using signal_t = boost::signals2::signal<void()>;
		using slot_t = signal_t::slot_type;

	public:
		boost::signals2::connection connect(const slot_t& s)      //连接插槽操作
		{
			return alarm.connect(s);
		}

		void press()                                              //按门铃动作
		{
			std::cout << "Ring alarm..." << "\n";
			alarm();                                              //调用signal，触发信号，引发插槽调用
		}

	private:
		signal_t alarm;                                           //信号对象
	};


	//决定采用随机数来让护士和婴儿的行为具有不确定性，这样程序会更有趣些。随机数的产生使用random库，为了方便使用我们把随机数发生器定义为全局变量
	using bool_rand = boost::random::variate_generator<boost::random::rand48, boost::uniform_smallint<>>;
	bool_rand g_rand(boost::random::rand48(std::time(0)), boost::uniform_smallint<>(0, 100));

	//护士类
	//action()函数，根据随机数决定是惊醒开门还是继续睡觉
	//(注意他的模板参数，使用了char const*作为护士的名字，因此实例化时字符串必须被声明成extern(也可以用boost.mpl库里的编译期字符串类mpl::string))

	extern char const nurse1[] = "Mary";
	extern char const nurse2[] = "Kate";

	template<char const* name>
	class nurse
	{
	private:
		bool_rand& rand;                  //随机数发生器

	public:
		nurse() :rand(g_rand) {}            //构造函数

		void action()
		{
			std::cout << name;
			if (rand() > 30)               //70%的概率惊醒
			{
				std::cout << "wakeup and open the door." << "\n";
			}
			else                          //30%的概率继续睡觉
			{
				std::cout << " is sleeping..." << "\n";
			}
		}
	};

	//婴儿类(与护士类的实现差不多)

	extern char const baby1[] = "Tom";
	extern char const baby2[] = "Jerry";

	template<char const* name>
	class baby
	{
	private:
		bool_rand& rand;
	public:
		baby() :rand(g_rand) {}

		void action()
		{
			std::cout << "Baby" << name;
			if (rand() > 50)
			{
				std::cout << " wakeup and crying loudly..." << "\n";
			}
			else
			{
				std::cout << "is sleeping sweetly..." << "\n";
			}
		}
	};

	//最后我们还需要一个访客类，他的唯一动作就是按门铃触发press事件

	class guest
	{
	public:
		void press(ring& r)
		{
			std::cout << "A guest press the ring." << "\n";
			r.press();
		}
	};

	void func()
	{
		//声明门铃、护士、婴儿、访客等类的实例
		ring r;
		nurse<nurse1> n1;
		nurse<nurse2> n2;
		baby<baby1> b1;
		baby<baby2> b2;
		guest g;

		r.connect(boost::bind(&nurse<nurse1>::action, n1));
		r.connect(boost::bind(&nurse<nurse2>::action, n2));
		r.connect(boost::bind(&baby<baby1>::action, b1));
		r.connect(boost::bind(&baby<baby2>::action, b2));

		g.press(r);

		//可能输出：
		//A guest press the ring.
		//Ring alarm...
		//Mary is sleeping...
		//Kate is sleeping...
		//BabyTomis sleeping sweetly...
		//BabyJerryis sleeping sweetly...
	}
}

namespace test_signals2_copyable_signal
{
	//让signal支持拷贝

	//如果出于某种理由，确实需要在多个对象之间共享signal对象，那么可以考虑使用shared_ptr<signal>作为类的成员，shared_ptr可以很好地管理signal的共享语义

	//可以是boost::shared_ptr或者是std::shared_ptr
}

namespace test_signals2_thread_safe
{
	//线程安全

	//signal模板参数列表的最后一个类型参数是互斥量Mutex，默认值是signals2::mutex，他会自动检测编译器的线程支持程度，根据操作系统自动决定要使用的
	//系统互斥量对象。通常mutex都工作得很好，我们不需要改变他
	
	//signal对象在创建时会自动创建一个mutex保护内部状态，每一个插槽连接时也会创建出一个新的mutex，当信号或插槽被调用时mutex都会自动锁定，因此signal
	//可以很好地工作于多线程环境
	//同样的，connection和shared_connection_block也是线程安全的，但用于自动连接管理的slot类不是线程安全的
	
	//signals2库中还有一个dummy_mutex，他是一个空的mutex类，把他作为模板参数可以使signals2变成非线程安全的版本，由于不使用锁定速度会稍微快一些
	
	//由于mutex是signal的最后一个模板参数，要指定他需要写出很多默认的类型，signals2使用模板元函数的方式可以便利地完成这个工作
	using signal_t =  boost::signals2::signal_type<int(int), boost::signals2::keywords::mutex_type<boost::signals2::dummy_mutex>>::type;
}

namespace test_signals2_slot_self_control
{
	//默认情况下插槽与信号signal、连接connection都是无关的，他自己无法处理与信号的连接，只能由信号或者连接时返回的connection对象来管理，
	//但有的时候我们又必须让插槽自己管理连接

	//connect_extended()函数和extended_slot_type类型可以让插槽接受一个额外的connection对象以处理连接。为了使用
	//这个功能，需要修改插槽的声明，使他能够接受connection对象

	template<int N>
	struct slot
	{
		int operator()(const boost::signals2::connection& conn, int x)      //检查连接状态
		{
			std::cout << "conn = " << conn.connected() << "\n";
			return x * N;
		}
	};

	void func()
	{
		boost::signals2::signal<int(int)> signal_;

		//_1是connection对象，_2是插槽实际使用的参数
		signal_.connect_extended(boost::signals2::signal<int(int)>::extended_slot_type(&slot<10>::operator(), slot<10>(), boost::placeholders::_1, boost::placeholders::_2));
		signal_.connect_extended(boost::signals2::signal<int(int)>::extended_slot_type(&slot<20>::operator(), slot<20>(), boost::placeholders::_1, boost::placeholders::_2));

		signal_(5); //整数5将作为_2的实际参数传递给插槽
	}
}


namespace test_signals2_signal_operator
{
	template<int N>
	struct slot
	{
		//有返回值
		int operator()(int x)
		{
			std::cout << "slot" << N << " called" << "\n";
			return N;
		}
	};

	//signal不支持operator+=，虽然有很多合理的理由，但还是有人喜欢这种自然简单的写法。如果真的很需要，那么可以对signal做一层简单的包装，为他添加这个功能

	//下面实现了一个示范性质的signal包装类signal_ex    (这里使用的是对象适配器模式，也可以改用类适配器模式，使用继承实现)

	//operator+=直接调用connect()函数，两者是等价的操作，为了支持从0到任意数量参数的operator()调用，我们使用C++11的可变参数模板和完美转发

	template<typename Signature>
	class signal_ex
	{
	public:
		using signal_type = boost::signals2::signal<Signature>;
		using slot_type = typename signal_type::slot_type;

		boost::signals2::connection connect(const slot_type& slot_)
		{
			return m_signal.connect(s);
		}

		//添加对operator+=的支持
		boost::signals2::connection operator+=(const slot_type& slot_)
		{
			return connect(s);
		}

		template<typename... Args>
		typename signal_type::result_type operator()(Args&&... args)
		{
			return m_signal(std::forward<Args>(args)...);
		}

	private:
		signal_type m_signal;
	};

	void func()
	{
		signal_ex<int(int)> signal_;

		signal_ += slot<10>();

		signal_();
	}
}




/////对比function

//signal内部使用function来存储可调用物，他的声明也与function很像，同样提供了operator()。在signal只连接了一个插槽的时候基本上可以与function替换

//void f()
//{ std::cout << "func called" << "\n";}

//boost::function<void()> func;
//func = f;
//func();
//等价于
//boost::signals2::signal<void()> sig;
//sig.connect(&f);
//sig();

//注意他们的返回值，function对象直接返回被包装函数的返回值，而signal则使用optional对象作为返回值，真正的返回值需要使用解引用操作符*才能取得

//(signal比function提供了更多用于回调的灵活性，但也使得他的用法比较复杂，较难掌握)






/////对比boost.signals

//signals是boost库中的另一个信号/插槽库，实际上signals2的实现是基于signals的，signals2与signals最大的区别是具有线程安全，能够用于多线程环境，而且不需要编译就可以使用

//signals2在结构、接口、设计原理等许多方面都模仿了signals，但也有一些变化，例如默认信号调用返回的是一个optional对象、combiner()函数改为set_combiner()、connection对象
//不再拥有block()方法等等
//(在boost1.56版里signals库已经被正式废弃，我们应当使用signals2库(也许将来的版本中signals2的命名空间会改为boost::signals))






/////对比C#

//signals2中的信号/插槽机制原理上类似于C#语言的event/delegate机制

//但C#的delegate的功能要比signals2弱，他要求精确的类型匹配，也没有合并器的概念，只能返回一个结果

//C#使用operator+=来连接event与delegate，signals2则使用connect()函数。这是因为signals2在设计
//时认为operator+=并没有带来太多的好处，反而会导致连续使用+=连接、operator-=等其他语义问题



