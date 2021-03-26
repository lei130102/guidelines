#pragma once

#include <any>
#include <string>
#include <functional>

namespace test_any_string_type
{
	struct typeinfo
	{
		std::string type_name;
		std::function<std::any()> type_creator_function;
		std::function<void(std::any)> type_deleter_function;
	};
}

namespace test_any_movable
{
	//resource类模板只能移动，不可拷贝
	template<typename T>
	class resource
	{
	public:
		resource()
			:m_own(true)
			, m_object(new T)
		{}

		explicit resource(T* object)
			: m_own(true)
			, m_object(object)
		{}

		resource(resource const& rhs) = delete;
		resource& operator=(resource const& rhs) = delete;

		resource(resource&& rhs)
			:m_own(rhs.m_own)
			, m_object(const_cast<T*>(rhs.release()))
		{}

		resource& operator=(resource&& rhs)
		{
			if (&rhs == this)
			{
				return *this;
			}

			if (m_object != rhs.get())
			{
				destroy_if_owned();
				m_own = rhs.m_own;
			}
			else if (!m_own)
			{
				m_own = rhs.m_own;
			}
			m_object = const_cast<T*>(rhs.release());
		}

		virtual ~resource()
		{
			destroy_if_owned();
		}

		T const* get() const
		{
			return m_object;
		}

		T* get()
		{
			return m_object;
		}

		T const* operator->() const
		{
			return get();
		}

		T* operator->()
		{
			return get();
		}

		T const& operator*() const
		{
			return *get();
		}

		T& operator*()
		{
			return *get();
		}

		T const& operator[](int index) const
		{
			return get()[index];
		}

		T& operator[](int index)
		{
			return get()[index];
		}

		T const* release() const
		{
			m_own = false;
			return get();
		}

		T* release()
		{
			m_own = false;
			return get();
		}

	protected:
		void destroy_if_owned()
		{
			if (m_object != nullptr && m_own)
			{
				delete m_object;
				m_object = nullptr;
				m_own = false;
			}
		}

	private:
		T* m_object;
		mutable bool m_own;
	};

	template<typename T>
	T* create_object(std::string const& str)
	{
		if (str == "int")
		{
			return new int;
		}
		else if (str == "double")
		{
			return new double;
		}
		else if (str == "std::string")
		{
			return new std::string;
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	resource<T> create_resource(std::string const& str)
	{
		return resource<T>(create_object<T>(str));
	}
}