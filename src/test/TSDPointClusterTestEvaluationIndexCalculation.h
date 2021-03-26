#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/Polygon_2.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/mpl/if.hpp>

#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

namespace TSDPointClusterTestEvaluationIndexCalculation
{
	//标准数据
	struct standard
	{};

	//被检数据
	struct test
	{};

	template<typename T>
	class point_2d_imp;

	//记录数据
	template<typename T>
	class record_data
	{
		friend class point_2d_imp<T>;

	public:
		using data_type = T;

		record_data() :m_salinity(0.0), m_temperature(0.0), m_depth(0.0)
		{}

		record_data(double salinity, double temperature, double depth) :
			m_salinity(salinity), m_temperature(temperature), m_depth(depth)
		{}

		double salinity() const { return m_salinity; }
		double temperature() const { return m_temperature; }

	private:
		double m_salinity;
		double m_temperature;
		double m_depth;
	};

	//站位数据
	template<typename T>
	class station_data
	{
	public:
		using data_type = T;
		using record_type = record_data<T>;

		using value_type = record_type;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = value_type*;
		using const_pointer = value_type const*;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		template<bool is_const>
		class iter
		{
			friend class iter<!is_const>;
			friend class station_data<T>;

		public:
			using difference_type = station_data<T>::difference_type;
			using value_type = station_data<T>::value_type;
			using reference = typename boost::mpl::if_c<is_const, station_data<T>::const_reference, station_data<T>::reference>::type;
			using pointer = typename boost::mpl::if_c<is_const, station_data<T>::const_pointer, station_data<T>::pointer>::type;
			using iterator_category = std::forward_iterator_tag;

			using iter_type = typename boost::mpl::if_c<is_const, typename std::vector<record_type>::const_iterator, typename std::vector<record_type>::iterator>::type;

			iter(iter_type it):m_iter(it)
			{}

			iter(iter<false> const& that) noexcept:m_iter(that.m_iter)
			{}

			pointer operator->() const noexcept
			{
				return m_iter.operator->();
			}

			reference operator*() const noexcept
			{
				return m_iter.operator*();
			}

			iter& operator++() noexcept
			{
				++m_iter;
				return *this;
			}

			iter operator++(int) noexcept
			{
				iter tmp(*this);
				++m_iter;
				return tmp;
			}

			template<bool ic>
			bool operator==(iter<ic> const& that) const noexcept
			{
				return (m_iter == that.m_iter);
			}

			template<bool ic>
			bool operator!=(iter<ic> const& that) const noexcept
			{
				return (m_iter != that.m_iter);
			}

			template<bool ic>
			iter& operator=(iter<ic> const& that)
			{
				m_iter = that.m_iter;
				return *this;
			}

			iter_type get_iter_type()
			{
				return m_iter;
			}

		private:
			iter_type m_iter;
		};

	public:
		using iterator = iter<false>;
		using const_iterator = iter<true>;

		iterator begin() noexcept
		{
			return iterator(m_record_data_set.begin());
		}

		const_iterator cbegin() const noexcept
		{
			return const_iterator(m_record_data_set.cbegin());
		}

		iterator end() noexcept
		{
			return iterator(m_record_data_set.end());
		}

		const_iterator cend() const noexcept
		{
			return const_iterator(m_record_data_set.cend());
		}

		size_type size() const noexcept
		{
			return m_record_data_set.size();
		}

		bool empty() const noexcept
		{
			return m_record_data_set.empty();
		}

		iterator insert(const_reference value)
		{
			return iterator(m_record_data_set.insert(m_record_data_set.end(), value));
		}

		template<typename ForwardIt>
		void insert(ForwardIt first, ForwardIt last)
		{
			for (; first != last; ++first)
			{
				this->insert(*first);
			}
		}

		void erase(iterator it) noexcept
		{
			m_record_data_set.erase(it.get_iter_type());
		}

		void erase(iterator begin, iterator end) noexcept
		{
			m_record_data_set.erase(begin.get_iter_type(), end.get_iter_type());
		}

		void clear() noexcept
		{
			m_record_data_set.clear();
		}

	private:
		std::vector<record_type> m_record_data_set;
	};

	////////////////
	class json_operator
	{
	public:
		static bool get_station_data(std::string const& input_json, station_data<standard>& standard_station_data, station_data<test>& test_station_data)
		{
			char* json = (char*)input_json.c_str();
			rapidjson::Document document;
			document.Parse(json);
			//
			rapidjson::Value::ConstMemberIterator standard_station_data_citer = document.FindMember("standard_station_data");
			if (standard_station_data_citer == document.MemberEnd())
			{
				return false;
			}
			if (!standard_station_data_citer->value.IsArray())
			{
				return false;
			}
			for (rapidjson::SizeType i = 0; i < standard_station_data_citer->value.Size(); ++i)
			{
				if (!standard_station_data_citer->value[i].IsObject())
				{
					return false;
				}

				rapidjson::Value::ConstMemberIterator salinity_citer = standard_station_data_citer->value[i].FindMember("salinity");
				if (salinity_citer == standard_station_data_citer->value[i].MemberEnd() && salinity_citer->value.IsDouble())
				{
					return false;
				}
				rapidjson::Value::ConstMemberIterator temperature_citer = standard_station_data_citer->value[i].FindMember("temperature");
				if (temperature_citer == standard_station_data_citer->value[i].MemberEnd() && temperature_citer->value.IsDouble())
				{
					return false;
				}
				rapidjson::Value::ConstMemberIterator depth_citer = standard_station_data_citer->value[i].FindMember("depth");
				if (depth_citer == standard_station_data_citer->value[i].MemberEnd() && depth_citer->value.IsDouble())
				{
					return false;
				}

				record_data<standard> record(salinity_citer->value.GetDouble(), temperature_citer->value.GetDouble(), depth_citer->value.GetDouble());
				standard_station_data.insert(record);
			}
			//
			rapidjson::Value::ConstMemberIterator test_station_data_citer = document.FindMember("test_station_data");
			if (test_station_data_citer == document.MemberEnd())
			{
				return false;
			}
			if (!test_station_data_citer->value.IsArray())
			{
				return false;
			}
			for (rapidjson::SizeType i = 0; i < test_station_data_citer->value.Size(); ++i)
			{
				if (!test_station_data_citer->value[i].IsObject())
				{
					return false;
				}

				rapidjson::Value::ConstMemberIterator salinity_citer = test_station_data_citer->value[i].FindMember("salinity");
				if (salinity_citer == test_station_data_citer->value[i].MemberEnd() && salinity_citer->value.IsDouble())
				{
					return false;
				}
				rapidjson::Value::ConstMemberIterator temperature_citer = test_station_data_citer->value[i].FindMember("temperature");
				if (temperature_citer == test_station_data_citer->value[i].MemberEnd() && temperature_citer->value.IsDouble())
				{
					return false;
				}
				rapidjson::Value::ConstMemberIterator depth_citer = test_station_data_citer->value[i].FindMember("depth");
				if (depth_citer == test_station_data_citer->value[i].MemberEnd() && depth_citer->value.IsDouble())
				{
					return false;
				}

				record_data<test> record(salinity_citer->value.GetDouble(), temperature_citer->value.GetDouble(), depth_citer->value.GetDouble());
				test_station_data.insert(record);
			}

			return true;
		}

		static bool set_n(double n, std::string& output_json)
		{
			rapidjson::Document document(rapidjson::kObjectType);
			document.AddMember("N", n, document.GetAllocator());

			rapidjson::StringBuffer strBuffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
			document.Accept(writer);
			output_json = strBuffer.GetString();

			return true;
		}
	};

	////////////////

	//二维平面点
	class point_2d_imp_interface
	{
	public:
		virtual double x() const = 0;
		virtual double y() const = 0;
		virtual point_2d_imp_interface* clone() = 0;
	};

	template<typename T>
	class point_2d_imp : public point_2d_imp_interface
	{
	public:
		using data_type = T;
		using record_type = record_data<T>;

		point_2d_imp(record_type const& record):
			m_x(record.m_salinity),
			m_y(record.m_temperature)
		{}

		point_2d_imp(point_2d_imp const& rhs)
		{
			m_x = rhs.m_x;
			m_y = rhs.m_y;
		}

		point_2d_imp_interface* clone() override { return new point_2d_imp(*this); }

		double x() const override {return m_x;}
		double y() const override {return m_y;}

	private:
		double m_x;
		double m_y;
	};

	class point_2d
	{
	public:
		point_2d():m_imp(nullptr)
		{}

		point_2d(point_2d const& rhs)
		{
			m_imp = nullptr;
			if (rhs.m_imp != nullptr)
			{
				m_imp = rhs.m_imp->clone();
			}
		}

		point_2d& operator=(point_2d const& rhs)
		{
			if (this != &rhs)
			{
				if (m_imp != nullptr)
				{
					delete m_imp;
					m_imp = nullptr;
				}

				if (rhs.m_imp != nullptr)
				{
					m_imp = rhs.m_imp->clone();
				}
			}
			return *this;
		}

		template<typename T>
		point_2d(record_data<T> const& record):
			m_imp(new point_2d_imp<T>(record))
		{}

		~point_2d(){ delete m_imp;}

		double x() const { return m_imp->x(); }
		double y() const { return m_imp->y(); }

	private:
		point_2d_imp_interface* m_imp;
	};

	//省略了定义point_2d的容器类型

	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef CGAL::Point_2<K> Point_2;
	typedef CGAL::Convex_hull_traits_adapter_2<K, CGAL::Pointer_property_map<Point_2>::type> Convex_hull_traits_2;
	typedef CGAL::Polygon_2<K> Polygon_2;

	//求面积
	class area
	{
	public:
		template<typename ForwardIt>
		double operator()(ForwardIt first_point_2d, ForwardIt last_point_2d)
		{
			//先计算凸包
			std::vector<Point_2> points;
			std::transform(first_point_2d, last_point_2d, std::back_inserter(points), [](point_2d const& value) {
				return Point_2(value.x(), value.y());
				});
			std::vector<std::size_t> indices(points.size()), convex_hull_indices;
			std::iota(indices.begin(), indices.end(), 0);
			CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(convex_hull_indices), Convex_hull_traits_2(CGAL::make_property_map(points)));

			//然后计算面积
			Polygon_2 p;
			std::transform(convex_hull_indices.begin(), convex_hull_indices.end(), std::back_inserter(p), [&points](std::size_t indice) {
					return points[indice];
				});
			return std::fabs(p.area());//注意p.area()计算的面积可能为正也可能为负，所以这里取绝对值
		}
	};

	class calculation
	{
	public:

		template<typename StandardRecordForwardIt, typename TestRecordForwardIt>
		static double N(StandardRecordForwardIt first_standard_record, StandardRecordForwardIt last_standard_record,
			TestRecordForwardIt first_test_record, TestRecordForwardIt last_test_record)
		{
			if (std::distance(first_standard_record, last_standard_record) == 0)
			{
				return 0;
			}

			int count = 0;
			if ((count = std::distance(first_test_record, last_test_record)) == 0)
			{
				return 0;
			}


			std::vector<point_2d> standard_point_2d_set;
			std::transform(first_standard_record, last_standard_record, std::back_inserter(standard_point_2d_set), [](std::iterator_traits<StandardRecordForwardIt>::value_type const& value) {
					return point_2d(value);
				});

			area get_area;
			double s0 = get_area(standard_point_2d_set.begin(), standard_point_2d_set.end());

			double init = 0;
			return std::accumulate(first_test_record, last_test_record, init, [s0, first_standard_record, last_standard_record, count](double init, std::iterator_traits<TestRecordForwardIt>::value_type value)
				{
					return init + N(s0, first_standard_record, last_standard_record, value) * 1.0 / count;
				});
		}

		template<typename StandardRecordForwardIt>
		static double N(double s0, StandardRecordForwardIt first_standard_record, StandardRecordForwardIt last_standard_record, record_data<test> test_record)
		{
			std::vector<point_2d> test_point_2d_set;
			std::transform(first_standard_record, last_standard_record, std::back_inserter(test_point_2d_set), [](std::iterator_traits<StandardRecordForwardIt>::value_type const& value) {
					return point_2d(value);
				});
			test_point_2d_set.push_back(point_2d(test_record));

			area get_area;
			double s1 = get_area(test_point_2d_set.begin(), test_point_2d_set.end());

			return (s1 <= s0 ? 1 : (1 - (s1 - s0) / s0));
		}
	};

	bool N(std::string input_json, std::string& output_json)
	{
		station_data<standard> standard_station_data;
		station_data<test> test_station_data;
		if (!json_operator::get_station_data(input_json, standard_station_data, test_station_data))
		{
			return false;
		}
		double n = calculation::N(standard_station_data.begin(), standard_station_data.end(), test_station_data.begin(), test_station_data.end());
		if (!json_operator::set_n(n, output_json))
		{
			return false;
		}
		return true;
	}
}
