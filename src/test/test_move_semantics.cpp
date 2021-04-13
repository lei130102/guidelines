#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <test_move_semantics.h>

#include <iostream>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(s_move_semantics)

////移动语义的动机

//支持移动语义之前(使用不支持移动语义的编译器)：
std::vector<std::string> createAndInsert()
{
	std::vector<std::string> coll; //在栈上创建一个对象，他有0个元素并且没有为元素分配内存
	coll.reserve(3);               //在堆上保留三个元素的内存空间，分配的没有被初始化，因为元素个数仍然是0
	std::string s = "data";        //string相当于有char元素的vector。即在栈上创建一个对象，其中一个成员表示字符数，另一个成员表示指向字符内存的指针。(在内部，字符串还存储一个终止的空字符，避免在调用成员函数c_str()以请求C风格字符串时分配内存)

	//到此，程序有如下状态：
	//1.有三个对象在栈上：v、coll、s
	//2.coll和s在堆上分配了内存    (当使用小字符串优化(SSO)时，字符串s可以将其整个值存储在栈上，前提是该值不是太长。然而，对于一般情况，让我们假设我们没有小字符串优化或字符串的值足够长，因此不会发生小字符串优化)

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  0 |------------->|    |    |    |
	//+----+              +----+----+----+
	//
	//
	//
	//s:
	//+----+
	//|  4 |------------->d a t a \0
	//+----+
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	coll.push_back(s);             //C++标准库中的所有容器都有值语义(可拷贝且拷贝结果跟原对象完全独立)，这意味着他们创建传递给他们的值的副本，结果我们得到vector中的第一个元素，他是传递的值/对象的完整(深)副本

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  1 |------------->|  4 |    |    |
	//+----+              +----+----+----+
	//                       |
	//                       |
	//                       d a t a \0
	//s:
	//+----+
	//|  4 |------------->d a t a \0
	//+----+
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	coll.push_back(s + s);        //创建一个新的临时string

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  2 |------------->|  4 |  8 |    |                  //b.创建临时string：s+s的深拷贝，并插入到coll中
	//+----+              +----+----+----+
	//                       |    |
	//                       |    d a t a d a t a \0
	//                       d a t a \0
	//s:
	//+----+
	//|  4 |------------->d a t a \0
	//+----+
	//
	//s+s:                                                  //a.首先创建临时string：s+s
	//+----+                                                //c.临时string：s+s不再使用，所以他被销毁
	//|  8 |------------->d a t a d a t a \0
	//+----+
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	//这里可以看出产生了性能不佳的代码：创建一个副本然后立即销毁副本的源，这意味着不必要地分配和释放内存，本可以从源移动到副本

	coll.push_back(s);            //再次把s插入到coll中

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  3 |------------->|  4 |  8 |  4 |                 
	//+----+              +----+----+----+
	//                       |    |    d a t a \0
	//                       |    d a t a d a t a \0
	//                       d a t a \0
	//s:
	//+----+
	//|  4 |------------->d a t a \0
	//+----+
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	//这里可以看出产生了性能不佳的代码：到这里s不再使用，所以可以进行一些优化使用s的内存作为vector中新元素的内存

	return coll;

	//分析：
	//return返回的是coll的副本(因为coll不是引用，需要按值返回),创建副本意味这必须创建一个包含所有元素的整个vector的深拷贝。因此必须为vector的中元素数组分配堆内存，并为每个值分配堆内存字符串分配以保存其值。在这里，我们需要分配4次内存(1+3)
	//然而，因为同时coll被销毁了，所以允许编译器执行命名返回值优化(named return value optimization,NRVO)。这意味着编译器可以生成代码，以便coll仅用作返回值
	//也就是说这里coll是可能没有被拷贝，优化改变了程序的功能行为，并且是否进行该优化是编译器决定的

	//假设这里进行了优化，那么coll作为返回值，然后调用s的析构函数，释放他在声明时分配的内存

	//stack:              heap:
	//retval:
	//+----+              +----+----+----+
	//|  3 |------------->|  4 |  8 |  4 |                 
	//+----+              +----+----+----+
	//                       |    |    d a t a \0
	//                       |    d a t a d a t a \0
	//                       d a t a \0
	//
	//v:
	//+----+
	//|  0 |
	//+----+
}


BOOST_AUTO_TEST_CASE(c_before_move_semantics_cpp03)
{
	std::vector<std::string> v;   //在栈上创建一个对象，他有0个元素并且没有为元素分配内存

	v = createAndInsert();        //最后，将返回值分配给v

	//stack:              heap:
	//retval:                                               //b.销毁retval
	//+----+              +----+----+----+
	//|  3 |------------->|  4 |  8 |  4 |                 
	//+----+              +----+----+----+
	//                       |    |    d a t a \0
	//                       |    d a t a d a t a \0
	//                       d a t a \0
	//
	//v:                                                    //a.深拷贝retval到v
	//+----+              +----+----+----+
	//|  3 |------------->|  4 |  8 |  4 |                 
	//+----+              +----+----+----+
	//                       |    |    d a t a \0
	//                       |    d a t a d a t a \0
	//                       d a t a \0

	//这里也有改进的地方：通常赋值运算符的作用是给v与指定的源值相同的值，通常不应修改指定的源值，也就是说赋值运算符将创建整个返回值的深拷贝。然而之后指定的源值不再使用，从而被销毁
	//同样，创建一个临时对象的拷贝，然后再销毁拷贝源，即非必要的分配和销毁内存，这里是4次内存分配(1+3)

	//题外话：虽然可以以类似 createAndInsert(v) 或者 createAndInsert().swap(v) 避免性能损耗，但这样的写法丑陋，不自然
}

//综上，不必要的内存分配是由下面引起的：
//a.向集合中插入一个临时对象。
//b.向集合中插入一个不再使用的对象。(插入的是对象的拷贝)
//c.将一个临时集合赋值给另一个集合

//支持移动语义之后(使用支持移动语义的编译器)：
std::vector<std::string> createAndInsert2()
{
	std::vector<std::string> coll;     //在栈上创建空vector对象coll
	coll.reserve(3);                   //在堆上保留三个元素的未初始化内存
	std::string s = "data";

	coll.push_back(s);

	//到此，跟之前使用不支持移动语义的都是一样的

	coll.push_back(s + s);

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  1 |------------->|  4 |    |    |                  //b.偷取s+s的内存并将他移动到coll的新元素(修改了s+s)
	//+----+              +----+----+----+
	//                       |    +------
	//                       |          |
	//                       d a t a \0 |
	//s:                                |
	//+----+                            |
	//|  4 |------------->d a t a \0    |
	//+----+                            |
	//                                  |
	//s+s:                              |                    //a.首先创建临时string：s+s
	//+----+                            |                    //c.临时string：s+s不再使用，所以他被销毁，但是由于他不再是初始内存的所有者，因此析构函数无法释放此内存
	//|  8 |                            d a t a d a t a \0
	//+----+
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	coll.push_back(std::move(s));

	//这里使用std::move，表示这里不再使用s了

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  1 |------------->|  4 |    |    |----+                  //b.偷取s+s的内存并将他移动到coll的新元素(修改了s+s)
	//+----+              +----+----+----+    |
	//                       |    +------     |
	//                       |          |     |
	//                       d a t a \0 |     |
	//s:                                |     |
	//+----+                            |     |
	//|  4 |                            |     d a t a \0    
	//+----+                            |
	//                                  |
	//                                  |                    //a.首先创建临时string：s+s
	//                                  |                    //c.临时string：s+s不再使用，所以他被销毁，但是由于他不再是初始内存的所有者，因此析构函数无法释放此内存
	//                                  d a t a d a t a \0
	//
	//
	//v:
	//+----+
	//|  0 |
	//+----+

	return coll;

	//是否使用返回值优化仍然取决于编译器，但如果不使用返回值优化，那么就会使用移动语义，这意味着返回值从coll中窃取值，最坏的情况是从源到返回值复制成员，比如size、capacity和指向内存的指针，然后为源的这些成员赋值新值

	//注意此时会析构s，但不会释放他的内存，因为他的初始内存已经被移动到coll中
}

BOOST_AUTO_TEST_CASE(c_after_move_semantics_cpp11)
{
	std::vector<std::string> v;        //在栈上创建空vector对象v

	v = createAndInsert2();

	//stack:              heap:
	//coll:
	//+----+              +----+----+----+
	//|  1 |    +-------->|  4 |    |    |----+                  //b.偷取s+s的内存并将他移动到coll的新元素(修改了s+s)
	//+----+    |         +----+----+----+    |
	//          |            |    +------     |
	//          |            |          |     |
	//          |            d a t a \0 |     |
	//          |                       |     |
	//          |                       |     |
	//          |                       |     d a t a \0    
	//          |                       |
	//          |                       |
	//          |                       |                    //a.首先创建临时string：s+s
	//          |                       |                    //c.临时string：s+s不再使用，所以他被销毁，但是由于他不再是初始内存的所有者，因此析构函数无法释放此内存
	//          |                       d a t a d a t a \0
	//          |
	//          |
	//v:        |
	//+----+    |
	//|  3 |----+
	//+----+

}

////实现移动语义

//移动语义之前，std::vector<>只有一个push_back()的实现

//template<typename T>
//class vector
//{
//public:
//	//...
//
//	//insert a copy of elem:
//	void push_back(const T& elem);
//
//	//...
//};

//形参是常量左值引用，可以绑定左值和右值的实参。原理是在push_back函数内部拷贝elem并且不会修改elem，实参没有被修改仍然可以被继续使用

//移动语义之后，又增加一个push_back()的重载

//template<typename T>
//class vector
//{
//public:
//	//...
//
//	//insert a copy of elem:
//	void push_back(const T& elem);
//
//	//insert elem when the value of elem is no longer needed:
//	void push_back(T&& elem);
//
//	//...
//};

//形参是万能引用，可以绑定左值和右值的实参。原理是在push_back函数内部直接使用和修改实参，实参被修改所以不可以被继续使用(实际可以使用，只是不能假设他的值现在是什么)

//然而，vector不知道怎样拷贝或移动一个元素，在确保vector有足够的内存盛放新元素时，vector将此工作委托给元素类型，这里就是std::string

//1.
//push_back(const T&)是传统的拷贝语义，他会调用std::string类的拷贝构造函数，该函数使用string对象(不会修改它)创建一个新的string对象

//(简化实现)
//class string
//{
//private:
//	int len;                                       //current number of characters
//	char* data;                                    //dynamic array of characters
//
//public:
//	//copy constructor:create a full copy of s:
//	string(const string& s)
//		:len{ s.len }                              //copy number of characters
//	{
//		if (len > 0)                               //if not empty
//		{
//			data = new char[len + 1];              //-allocate new memory
//			memcpy(data, s.data, len + 1);         //-and copy the characters
//		}
//	}
//};

//使用拷贝构造函数:

//std::string a = "data";

//a:
//+---------+
//| len:  4 |
//| data:   |----->d a t a \0
//+---------+

//std::string b = a; //调用拷贝构造函数

//a:
//+---------+
//| len:  4 |
//| data:   |----->d a t a \0
//+---------+

//b:
//+---------+
//| len:  4 |
//| data:   |----->d a t a \0
//+---------+

//std::string的拷贝构造函数提供了data的深拷贝实现

//2.
//push_back(T&&)是新的移动语义，他会调用std::string类的移动构造函数，该函数使用不再需要的string对象(会修改他)创建一个新的string对象

//(简化实现)
//class string
//{
//private:
//	int len;                               //current number of characters
//	char* data;                            //dynamic array of characters
//
//public:
//	//...
//
//	//move constructor:initialize the new string from s(stealing the value):
//	string(string&& s)
//		:len{ s.len }, data{ s.data }      //copy number of characters and pointer to memory
//	{
//		s.data = nullptr;                  //release the memory for the source value
//		s.len = 0;                         //and adjust number of characters accordingly
//	}
//
//  //...
//};

//std::string c = std::move(b);   //调用移动构造函数 init c with the value of b(no longer needing its value here)

//b:
//+---------+
//| len:  4 |
//| data:   |   -->d a t a \0
//+---------+   |
//              |
//c:            |
//+---------+   |
//| len:  4 |   |
//| data:   |---+
//+---------+

//std::string的移动构造函数提供了data的浅拷贝实现，然后修改源data(如果不修改的话，b的析构函数会释放内存)，从而实现所有权的转移






//从上面可以看出，提供push_back(T&& elem)函数后，使用临时对象或者std::move()可以调用他，从而使用了移动语义。如果没有提供这个函数，那么使用临时对象或者std::move()时，使用的是拷贝语义
//比如，假设一个类似vector的容器没有push_back(T&& elem)版本的重载：
//template<typename T>
//class MyVector
//{
//public:
//	//...
//	void push_back(const T& elem);       //insert a copy of elem
//	//...                                //no other push_back() declared
//};

//MyVector<std::string> coll;
//std::string s{"data"};
//...
//coll.push_back(std::move(s));//仍然可以使用临时对象或者std::move()，使用的是拷贝语义

//这样的好处：
//a.在移动语义支持之前或者不支持移动语义的函数或类实现在以临时对象或者std::move()方式使用时，仍能正常工作
//b.没有需要优化的

//对于泛型代码，我们总能使用std::move标记不再使用的对象，即使该对象类型不支持移动语义
//同样，甚至可以标记基本数据类型的对象，例如int，仍会使用通常的值语义拷贝，例如：

//std::vector<int> coll;
//int x{42};
//...
//coll.push_back(std::move(x));   //Ok,but copies x (std::move() has no effect)



//注意，以const声明的对象无法移动，因为任意优化实现都需要修改被传递的参数，以const声明的对象只适合push_back(const T& elem)，也就是说对以const声明的对象使用std::move仍只是调用push_back(const T& elem)，即std::move没有效果

//std::vector<std::string> coll;
//const std::string s{"data"};
//...
//coll.push_back(std::move(s));    //OK,calls push_back(const std::string&)  (std::move() has no effect)

//原则上，我们可以通过声明一个形参为常量右值引用的重载函数push_back(const T&& elem)，但是这在语义上毫无意义。这里再次说明了无法使用移动语义时会使用拷贝语义





//(重点!!!)

//之前讨论了，以const声明的对象，被禁用了移动语义，这同样适用于以const声明的返回值
//因此，从C++11开始，以const的按值返回的不再是好的编码风格(以前曾作为一个风格指南被推荐过)，例如

//(错误示范)
//const std::string getValue();

//std::vector<std::string> coll;
//...
//coll.push_back(getValue());      //copies(because the return value is const)

//以const声明返回值时，只适用于返回的指针类型或引用类型的部分，例如

//const std::string& getRef();
//const std::string* getPtr();

BOOST_AUTO_TEST_SUITE_END()