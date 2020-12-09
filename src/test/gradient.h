#pragma once

#include <iterator>
#include <vector>
#include <numeric>
#include <algorithm>

namespace gradient
{
    //低通滤波(7点2次线性平滑)
    template<typename ForwardIt, typename OutputIt>
    OutputIt low_pass_filter(
            ForwardIt first1,
            ForwardIt last1,
            OutputIt first2)
    {
        if(std::distance(first1, last1) >= 7)
        {
            auto i = first1;
            auto i1 = first1;
            auto i2 = first1;
            std::advance(i2, 1);
            auto i3 = first1;
            std::advance(i3, 2);
            auto i4 = first1;
            std::advance(i4, 3);
            auto i5 = first1;
            std::advance(i5, 4);
            auto i6 = first1;
            std::advance(i6, 5);
            auto i7 = first1;
            std::advance(i7, 6);

            while(i != last1)
            {
                if(std::distance(first1, i) == 0)
                {
                    *first2++ = (32.0 * *i1 + 15.0 * *i2 + 3.0 * *i3 - 4.0 * *i4 - 6.0 * *i5 - 3.0 * *i6 + 5.0 * *i7) / 42.0;
                }
                else if(std::distance(first1, i) == 1)
                {
                    *first2++ = (5.0 * *i1 + 4.0 * *i2 + 3.0 * *i3 + 2.0 * *i4 + 1.0 * *i5 + 0.0 * *i6 - 1.0 * *i7) / 14.0;
                }
                else if(std::distance(first1, i) == 2)
                {
                    *first2++ = (1.0 * *i1 + 3.0 * *i2 + 4.0 * *i3 + 4.0 * *i4 + 3.0 * *i5 + 1.0 * *i6 - 2.0 * *i7) / 14.0;
                }
                else if(std::distance(i, last1) == 3)
                {
                    *first2++ = (-2.0 * *i1 + 1.0 * *i2 + 3.0 * *i3 + 4.0 * *i4 + 4.0 * *i5 + 3.0 * *i6 + 1.0 * *i7) / 14.0;
                }
                else if(std::distance(i, last1) == 2)
                {
                    *first2++ = (-1.0 * *i1 + 0 * *i2 + 1.0 * *i3 + 2.0 * *i4 + 3.0 * *i5 + 4.0 * *i6 + 5.0 * *i7) / 14.0;
                }
                else if(std::distance(i, last1) == 1)
                {
                    *first2++ = (5.0 * *i1 - 3.0 * *i2 - 6.0 * *i3 - 4.0 * *i4 + 3.0 * *i5 + 15.0 * *i6 + 32.0 * *i7) / 42.0;
                }
                else
                {
                    *first2++ = (-2.0 * *i1 + 3.0 * *i2 + 6.0 * *i3 + 7.0 * *i4 + 6.0 * *i5 + 3.0 * *i6 - 2.0 * *i7) / 21.0;
                    if(std::distance(i7, last1) > 1)
                    {
                        ++i1;
                        ++i2;
                        ++i3;
                        ++i4;
                        ++i5;
                        ++i6;
                        ++i7;
                    }
                }
                ++i;

            }
        }
        else
        {
            while(first1 != last1)
            {
                *first2++ = *first1++;
            }
        }
        return first2;
    }

    //计算梯度
    template<typename ValueForwardIt, typename DepthForwardIt, typename GradientOutputIt>
    GradientOutputIt compute_gradient(
            ValueForwardIt first1,                 //值 要求值容器中至少有3个元素，都是有效值
            ValueForwardIt last1,
            DepthForwardIt first2,                 //深度 要求深度容器中至少有3个元素
            DepthForwardIt last2,
            GradientOutputIt gradient_first        //返回的梯度
            )
    {
        if(std::distance(first1, last1) < 3)
        {
            throw std::logic_error("value container's size must greator than or equal to 3");
        }
        if(std::distance(first2, last2) < 3)
        {
            throw std::logic_error("depth container's size must greator than or equal to 3");
        }
        if(std::distance(first1, last1) != std::distance(first2, last2))
        {
            throw std::logic_error("value container's size must equal to depth container's size");
        }

        std::vector<double> tmp_v;

        auto value_i = first1;
        auto value_i1 = first1;
        auto value_i2 = first1;
        std::advance(value_i2, 1);
        auto value_i3 = first1;
        std::advance(value_i3, 2);

        auto depth_i1 = first2;
        auto depth_i2 = first2;
        std::advance(depth_i2, 1);
        auto depth_i3 = first2;
        std::advance(depth_i3, 2);

        while(value_i != last1)
        {
            if(std::distance(first1, value_i) == 0)
            {
                *std::back_inserter(tmp_v)++ = (*value_i2 - *value_i1) * 1.0 / (*depth_i2 - *depth_i1);
            }
            else if(std::distance(value_i, last1) == 1)
            {
                *std::back_inserter(tmp_v)++ = (*value_i3 - *value_i2) * 1.0 / (*depth_i3 - *depth_i2);
            }
            else
            {
                *std::back_inserter(tmp_v)++ = (*value_i3 - *value_i1) * 1.0 / (*depth_i3 - *depth_i1);

                if(std::distance(value_i3, last1) > 1)
                {
                    ++value_i1;
                    ++value_i2;
                    ++value_i3;
                    ++depth_i1;
                    ++depth_i2;
                    ++depth_i3;
                }
            }
            ++value_i;
        }

        return low_pass_filter(tmp_v.begin(), tmp_v.end(), gradient_first);
    }
}
