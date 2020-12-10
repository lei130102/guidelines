#pragma once

#include <boost/intrusive/list.hpp>

#include <iostream>

namespace test_intrusive_test
{
	//boost::intrusive::list_base_hook
	//带有 boost::intrusive::list_base_hook<缺省tag> 的元素类型
	class my_element : public boost::intrusive::list_base_hook<>
	{};

	//boost::intrusive::list
	//针对父类为 boost::intrusive::list_base_hook<缺省tag> 的my_element类型元素的容器
	using my_list = boost::intrusive::list<
											my_element
											>;
}

namespace test_intrusive_test2
{
	struct tag_for_my_list_a;//使用不完整类型就可以
	struct tag_for_my_list_b;//同上

	class my_element : public boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>>, public boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>>
	{};

	//针对父类为 boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>> 的my_element类型元素的容器
	using my_list_a = boost::intrusive::list<
											my_element,
											boost::intrusive::base_hook<//注意下面写法
																		boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_a>>
																		>
											>;
	//针对父类为 boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>> 的my_element类型元素的容器
	using my_list_b = boost::intrusive::list<
											my_element,
											boost::intrusive::base_hook<//注意下面写法
																		boost::intrusive::list_base_hook<boost::intrusive::tag<tag_for_my_list_b>>
																		>
											>;
}

namespace test_intrusive_test3
{
	class my_element
	{
	public:
		boost::intrusive::list_member_hook<> m_list_hook;//public成员变量
	};

	using my_list = boost::intrusive::list<
										   my_element,
										   boost::intrusive::member_hook<//注意下面写法
																		 my_element, boost::intrusive::list_member_hook<>, &my_element::m_list_hook
																		>
										  >;

	//  boost::intrusive::list_base_hook缺点:会破坏继承结果，违反"is a"含义
	//boost::intrusive::list_member_hook缺点:有些情况下不能使用
}

namespace test_intrusive_test4
{
	//同时存在

	class my_element : public boost::intrusive::list_base_hook<>
	{
	public:
		my_element(int i) :m_int(i) {}

	public:
		boost::intrusive::list_member_hook<> m_list_hook;
	private:
		int m_int;
	};

	using my_base_list = boost::intrusive::list<my_element>;
	using my_member_list = boost::intrusive::list<my_element, boost::intrusive::member_hook<my_element, boost::intrusive::list_member_hook<>, &my_element::m_list_hook>>;
}

namespace test_intrusive_test5
{
	//抽象基类
	class window : public boost::intrusive::list_base_hook<>
	{
	public:
		using window_list = boost::intrusive::list<window>;//window是抽象类，std::list无法使用，除非是window*

		window()
		{
			m_window_list.push_back(*this);
		}

		virtual ~window()
		{
			m_window_list.erase(window_list::s_iterator_to(*this));
		}

		virtual void paint() = 0;

		static window_list m_window_list;
	};

	window::window_list window::m_window_list;//这个定义应该放到源文件中，避免重复定义，这里暂且忽略这个问题

	class framewindow : public window
	{
	private:
		void paint() override
		{
			std::cout << "framewindow" << "\n";
		}
	};

	class editwindow : public window
	{
		void paint() override
		{
			std::cout << "editwindow" << "\n";
		}
	};

	class canvaswindow : public window
	{
		void paint()
		{
			std::cout << "canvaswindow" << "\n";
		}
	};

	//...

	class mainwindow : public window
	{
	private:
		void paint() override
		{
			std::cout << "mainwindow" << "\n";
		}

	private:
		framewindow m_frame;
		editwindow m_edit;
		canvaswindow m_canvas;
	};

	void paint_all_windows()
	{
		for (window::window_list::iterator iter = window::m_window_list.begin(); iter != window::m_window_list.end(); ++iter)
		{
			iter->paint();
		}
	}

}

namespace test_intrusive_test6
{
	//link mode为auto unlink的hook

	class my_element : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>
	{
	public:
		using base = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;

		my_element(int i = 0) :m_int(i) {}

		int get_int() { return m_int; }

		void unlink()
		{
			base::unlink();
		}

		bool is_linked()
		{
			return base::is_linked();
		}

	private:
		int m_int;
	};

	//Boost.Intrusive auto-unlink hooks are incompatible with containers that have constant-time size()
	using my_list = boost::intrusive::list<my_element, boost::intrusive::constant_time_size<false>>;

}