#pragma once

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <numeric>
#include <iterator>

template<typename T>
inline bool equal(T t1, T t2)
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
inline bool equal(double d1, double d2)
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


/* 插值算法 */
namespace interpolation
{
    /* 反距离加权法（Inverse Distance Weighted）插值 https://www.jianshu.com/p/b38c5e464d16 */
    inline bool idw(std::map<std::pair<double, double>, double> const& discrete_point, double invalid_value, std::pair<std::pair<double, double> const, double>& interpolation_point, double p = 1)
    {
        auto get_distance_reciprocal = [&](std::pair<double, double> const& point1, std::pair<double, double> const& point2){
            return std::pow(std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2)), (-1)*p); //如果p为1那么是距离的倒数
        };

        double distance_reciprocal_all = std::accumulate(discrete_point.begin(), discrete_point.end(), 0.0,
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

        double interpolation_point_value = std::accumulate(discrete_point.begin(), discrete_point.end(), 0.0,
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

    inline bool idw(std::map<std::pair<double, double>, double> const& discrete_point, double invalid_value, std::map<std::pair<double, double>, double>& interpolation_point, double p = 1)
    {
        std::vector<bool> success;
        std::transform(interpolation_point.begin(), interpolation_point.end(), std::back_inserter(success),
                       [&](std::map<std::pair<double, double>, double>::value_type& a_interpolation_point)
        {
            return idw(discrete_point, invalid_value, a_interpolation_point, p);
        });

        return std::accumulate(success.begin(), success.end(), true,
                               [](bool init, bool first)
        {
            return init && first;
        });
    }
}
