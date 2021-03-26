#ifndef NUMERIC_RANGE_H
#define NUMERIC_RANGE_H

#include <exception>
#include <iterator>
#include <type_traits>

template<typename T>
class numeric_iterator;

template<typename T>
class numeric_range
{
    //因为T不是任意类型都可以，所以这里最好有一个static_assert
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "numeric_range type argument must be numeric.");

    friend class numeric_iterator<T>;

public:
    explicit numeric_range(T start = 0, T step = 1, size_t count = 2)
        :m_start(start),m_step(step),m_count(count)
    {}

    //定义begin接口
    numeric_iterator<T> begin()
    {
        return numeric_iterator<T>(this);
    }

    //定义end接口
    numeric_iterator<T> end()
    {
        numeric_iterator<T> end_iter;
        end_iter.m_value = m_start + m_count * m_step;

        return end_iter;
    }

private:
    T m_start;
    T m_step;
    size_t m_count;
};



//numeric_iterator的成员函数分两类：
//1.对numeric_range提供
//目的是为了实现numeric_range的接口，比如begin()和end()。
//2.对外(非numeric_range)提供
//目的是为了满足迭代器类别的要求

//为了简化和防止混淆，第一类成员函数不再提供，而是使用友元类方式，numeric_iterator可以作为嵌套类，但是因为嵌套类有限制(比如在Qt中)，所以尽量不要用嵌套类
template<typename T>
class numeric_iterator : public std::iterator<std::forward_iterator_tag, T> //借助iterator模板定义自定义迭代器类，指定为前向迭代器
{
    friend class numeric_range<T>;

public:
    explicit numeric_iterator(numeric_range<T>* a_range)
        :m_range(a_range), m_value{a_range->m_start}
    {}

    //iter()        (默认构造函数)
    numeric_iterator()
        :m_range(nullptr), m_value{}
    {}

    //iter(iter const&）(复制构造函数)
    //编译器自动生成

    //*iter  读取实际元素
    T& operator*()
    {
        if(m_value == static_cast<T>(m_range->m_start + m_range->m_count * m_range->m_step))
        {
            throw std::logic_error("cannot dereference an end iterator.");
        }
        return m_value;
    }

    //iter->member   读取实际元素(如果有的话)

    //++iter
    numeric_iterator& operator++()
    {
        if(m_value == static_cast<T>(m_range->m_start + m_range->m_count * m_range->m_step))
        {
            throw std::logic_error("cannot increment an end iterator.");
        }
        m_value += m_range->m_step;
        return *this;
    }

    //iter++
    numeric_iterator operator++(int)
    {
        if(m_value == static_cast<T>(m_range->m_start + m_range->m_count * m_range->m_step))
        {
            throw std::logic_error("cannot increment an end iterator.");
        }
        auto temp = *this;
        m_value += m_range->m_step;
        return temp;
    }


    //iter1 == iter2
    bool operator==(numeric_iterator const& iter) const
    {
        return m_value == iter.m_value;
    }

    //iter1 != iter2
    bool operator!=(numeric_iterator const& iter) const
    {
        return m_value != iter.m_value;
    }

    //iter1 = iter2
    numeric_iterator& operator=(numeric_iterator const& src)
    {
        m_range = src.m_range;
        m_value = src.m_value;
    }

private:
    numeric_range<T>* m_range;
    T m_value;
};

#endif // NUMERIC_RANGE_H
