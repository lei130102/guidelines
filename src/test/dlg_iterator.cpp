#include "dlg_iterator.h"
#include "ui_dlg_iterator.h"

#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <list>
#include <set>

#include <numeric_range.h>

template<class Iter>
constexpr std::move_iterator<Iter> make_move_iterator(Iter i)
{
    return std::move_iterator<Iter>(std::move(i));
}

dlg_iterator::dlg_iterator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_iterator)
{
    ui->setupUi(this);
}

dlg_iterator::~dlg_iterator()
{
    delete ui;
}

void dlg_iterator::on_pbNumericRangeCopy_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    auto first = range.begin();
    auto last = range.end();
    std::copy(first, last, std::ostream_iterator<double>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbNumericRangeAccumulate_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    std::cout << "Sum = " << std::accumulate(std::begin(range), std::end(range), 0.0) << "\n";
}

void dlg_iterator::on_pbNumericRangeFor_clicked()
{
    numeric_range<double> range{1.5, 0.5, 5};
    for(auto n : range)
    {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

void dlg_iterator::on_pbVectorArrayStringIterator_clicked()
{
    std::vector<int> v{5, 4, 3, 2, 1};
    std::sort(++v.begin(), v.end());

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void dlg_iterator::on_pbReverseIterator_clicked()
{
    std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    std::copy(v.rbegin(), v.rend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbIteratorToReverseIterator_clicked()
{
    std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int>::iterator pos = std::find(v.begin(), v.end(), 2);
    std::cout << "pos:" << *pos << "\n";
    //iterator转reverse_iterator
    std::vector<int>::reverse_iterator pos_left(pos);//注意reverse_iterator的构造方式
    std::cout << "pos_left:" << *pos_left << "\n";
}

void dlg_iterator::on_pbReverseIteratorToIterator_clicked()
{
    std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int>::reverse_iterator pos = std::find(v.rbegin(), v.rend(), 2);//注意参数类型决定返回值类型
    std::cout << "pos:" << *pos << "\n";
    //reverse_iterator转iterator
    std::vector<int>::iterator pos_right(pos.base());
    std::cout << "pos_right:" << *pos_right << "\n";
}

void dlg_iterator::on_pbBackInsertIterator_clicked()
{
    std::vector<int> v;

    ////使用std::back_insert_iterator模板类
    std::back_insert_iterator<std::vector<int>> bii(v);
    //调用作为输出迭代器的接口
    *bii++ = 1;
    *bii++ = 2;
    *bii++ = 3;
    //插入迭代器的operator*和operator++、operator++(int)都是无操作，所以可以
    bii = 4;
    bii = 5;
    bii = 6;

    ////使用std::back_inserter辅助函数
    std::back_inserter(v) = 7;
    std::back_inserter(v) = 8;

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbBackInsertIteratorGenerate_n_clicked()
{
    std::vector<int> v;
    int n = 0;

    std::generate_n(std::back_insert_iterator<std::vector<int>>(v), 10, [n]() mutable{return ++n;});

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbBackInsertIteratorCopy_clicked()
{
    std::vector<int> v{1,2,3,4,5};

    v.reserve(2 * v.size()); //注意！一定要在调用copy之前确保有足够大的空间，因为插入元素时可能会造成指向v的其他迭代器失效
    std::copy(v.begin(), v.end(), std::back_inserter(v));
//    template<class InputIt, class OutputIt>
//    OutputIt copy(InputIt first, InputIt last,
//                  OutputIt d_first)
//    {
//        while (first != last) {
//            *d_first++ = *first++;
//        }
//        return d_first;
//    }

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbFrontInsertIterator_clicked()
{
    std::list<int> l;

    ////使用std::front_insert_iterator模板类
    std::front_insert_iterator<std::list<int>> fii(l);
    //调用作为输出迭代器的接口
    *fii++ = 1;
    *fii++ = 2;
    *fii++ = 3;
    //插入迭代器的operator*和operator++、operator++(int)都是无操作，所以可以
    fii = 4;
    fii = 5;
    fii = 6;

    ////使用std::front_inserter辅助函数
    std::front_inserter(l) = 7;
    std::front_inserter(l) = 8;

    std::copy(l.begin(), l.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbInsertIterator_clicked()
{
    std::set<int> s;

    ////使用std::insert_iterator模板类
    std::insert_iterator<std::set<int>> ii(s, s.begin());
    //调用作为输出迭代器的接口
    *ii++ = 1;
    *ii++ = 2;
    *ii++ = 3;
    //插入迭代器的operator*和operator++、operator++(int)都是无操作，所以可以
    ii = 4;
    ii = 5;
    ii = 6;

    ////使用std::inserter辅助函数
    std::inserter(s, s.end()) = 7;
    std::inserter(s, s.end()) = 8;

    std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

void dlg_iterator::on_pbOstreamIterator_clicked()
{
    std::ostream_iterator<int> writer(std::cout, " ");
    //调用作为输出迭代器的接口
    *writer++ = 1;
    *writer++ = 2;
    *writer++ = 3;
    //输出流迭代器的operator*和operator++、operator++(int)都是无操作，所以可以
    writer = 4;
    writer = 5;
    writer = 6;

    std::cout << "\n";

    std::vector<int> v{1,2,3,4,5,6,7,8,9};
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout));
    //    template<class InputIt, class OutputIt>
    //    OutputIt copy(InputIt first, InputIt last,
    //                  OutputIt d_first)
    //    {
    //        while (first != last) {
    //            *d_first++ = *first++;
    //        }
    //        return d_first;
    //    }

    std::cout << "\n";
}

void dlg_iterator::on_pbIstreamIterator_clicked()
{
    std::istream_iterator<int> reader(std::cin);
    std::istream_iterator<int> readerEOF;//默认构造函数创建end()

    while(reader != readerEOF)
    {
        //调用作为输入迭代器的接口
        std::cout << *reader++ << "\n";
        //输入流迭代器的operator*和operator++、operator++(int)都是无操作，所以可以
        //std::cout << reader << "\n";
    }
}

void dlg_iterator::on_pbOstreamIteratorIstreamIterator_clicked()
{
    std::istream_iterator<std::string> reader(std::cin);
    std::ostream_iterator<std::string> writer(std::cout , " ");

    while(reader != std::istream_iterator<std::string>())
    {
        //每次跳过前两个字符串
        std::advance(reader, 2);

        //重新判断
        if(reader != std::istream_iterator<std::string>())
        {
            *writer++ = *reader++;
        }
    }
}

void dlg_iterator::on_pbMoveIterator_clicked()
{
    std::vector<std::string> v{"this", "is", "an", "example"};

    typedef std::vector<std::string>::iterator iter_t;
    std::string result = std::accumulate(
                            std::move_iterator<iter_t>(v.begin()),
                            std::move_iterator<iter_t>(v.end()),
                            std::string()
                );

    std::cout << "result:" << result << "\n";
    std::cout << "v:";
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, " "));
}

void dlg_iterator::on_pbMakeMoveIterator_clicked()
{
    std::vector<std::string> v{"this", "is", "an", "example"};

    typedef std::vector<std::string>::iterator iter_t;
    std::string result = std::accumulate(
                            std::make_move_iterator(v.begin()),
                            std::make_move_iterator(v.end()),
                            std::string()
                );

    std::cout << "result:" << result << "\n";
    std::cout << "v:";
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, " "));
}

void dlg_iterator::on_pbOfstreamOstreamIterator_clicked()
{
    std::string filepath = "123.txt";

    std::vector<std::string> v{"1", "2", "3"};

    std::ofstream of(filepath);//of创建时自动执行open，of析构时自动执行close
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(of, "\n"));
}

void dlg_iterator::on_pbIfstreamIstreamIterator_clicked()
{
    std::string filepath = "123.txt";

    std::ifstream ifs(filepath);
    ifs.unsetf(std::ios::skipws);//进制忽略文件中的空白字符

    //std::string filedata(std::istream_iterator<char>(ifs), std::istream_iterator<char>());//编译器会认为是函数声明
    std::string filedata((std::istream_iterator<char>(ifs)), (std::istream_iterator<char>()));

    std::cout << filedata << "\n";

    //std::istream_iterator<char>对象使用operator>>从输入流中读取单个字符，而istreambuf_iterator<char>
    //则直接从流的缓冲区中读取下一个字符
    //std::istreambuf_iterator<char>对象从一个输入流istream s中读取下一个字符的操作是通过s.rdbuf()->sgetc()来完成的
}

void dlg_iterator::on_pbIfstreamIstreambufIterator_clicked()
{
    std::string filepath = "123.txt";

    std::ifstream ifs(filepath);

    //std::string filedata(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());//编译器会认为是函数声明
    std::string filedata((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    std::cout << filedata << "\n";

}
