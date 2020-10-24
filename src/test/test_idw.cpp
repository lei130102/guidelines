#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

/* 插值算法 */

namespace interpolation
{
	template<typename T>
	bool equal(T t1, T t2)
	{
		if (t1 != t2)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	template<>
	bool equal(double d1, double d2)
	{
		if (((d1 - d2) > -0.000001) && ((d1 - d2) < 0.000001))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/* 反距离加权法（Inverse Distance Weighted）插值 https://www.jianshu.com/p/b38c5e464d16 */
	bool idw(std::map<std::pair<double, double>, double>& discrete_point, double invalid_value, std::pair<std::pair<double, double>, double>& interpolation_point, double p = 1)
	{
		auto get_distance_reciprocal = [&](std::pair<double, double> const& point1, std::pair<double, double> const& point2){
			return std::pow(std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2)), (-1)*p); //如果p为1那么是距离的倒数
		};

		double distance_reciprocal_all = 0.0;
		distance_reciprocal_all = std::accumulate(discrete_point.begin(), discrete_point.end(), distance_reciprocal_all,
			[&](double init_value, std::map<std::pair<double, double>, double>::value_type const& iter_value) {
			if (!equal(iter_value.second, invalid_value))
			{
				return std::move(init_value) + get_distance_reciprocal(iter_value.first, interpolation_point.first);
			}
			else
			{
				return std::move(init_value);
			}
		});

		double interpolation_point_value = 0.0;
		interpolation_point_value = std::accumulate(discrete_point.begin(), discrete_point.end(), interpolation_point_value,
			[&](double init_value, std::map<std::pair<double, double>, double>::value_type const& iter_value) {
			if (!equal(iter_value.second, invalid_value))
			{
				double weight = get_distance_reciprocal(iter_value.first, interpolation_point.first) / distance_reciprocal_all;
				return std::move(init_value) + iter_value.second * weight;
			}
			else
			{
				return std::move(init_value);
			}
		});
		interpolation_point.second = interpolation_point_value;

		return true;
	}
}





int wmain(int argc, wchar_t* argv[])
{
	std::map<std::pair<double, double>, double> values;
	values.insert(std::make_pair(std::make_pair(0, 0), 0));
	values.insert(std::make_pair(std::make_pair(0, 1), 1));
	values.insert(std::make_pair(std::make_pair(0, 2), 2));
	values.insert(std::make_pair(std::make_pair(0, 3), 3));
	values.insert(std::make_pair(std::make_pair(0, 4), 4));
	values.insert(std::make_pair(std::make_pair(0, 5), 5));

	std::pair<std::pair<double, double>, double> point;
	point = std::make_pair(std::make_pair(0, 2.5), 0);
	interpolation::idw(values, 99999, point, 2);

	double value = point.second;

	std::cout << value;
}
