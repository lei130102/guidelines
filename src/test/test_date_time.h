#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>

namespace test_posix_time
{
	//Posix Time System

	//Introduction
	//Defines a non - adjusted time system with nano - second / micro - second resolution and stable calculation properties.The nano - second resolution option uses 96 bits of underlying storage for each ptime while the micro - second resolution uses 64 bits per ptime(see Build Options for details).This time system uses the Gregorian calendar to implement the date portion of the time representation.

	////介绍
	//定义了具有纳秒/微秒分辨率和稳定计算特性的非调整时间系统。纳秒分辨率选项为每个ptime使用96位底层存储，而微秒分辨率
	//为每个ptime使用64位底层存储，这个时间系统使用公历来实现时间表示的日期部分

	namespace test_ptime
	{
		//boost::posix_time::ptime 是时间点操作的主要接口
		
		//#include <boost/date_time/posix_time/posix_time.hpp>       包括所有的类型和I/O
		//或者
		//#include <boost/date_time/posix_time/posix_time_types.hpp> 仅包括所有类型
	}

	namespace test_time_duration
	{
		//boost::posix_time::time_duration 是时间段的基类，可正可负，通常由小时、分钟、秒和秒的小数部分构造

		//#include <boost/date_time/posix_time/posix_time.hpp>       包括所有的类型和I/O
		//或者
		//#include <boost/date_time/posix_time/posix_time_types.hpp> 仅包括所有类型

		//派生类:
		//boost::posix_time::hours
		//boost::posix_time::minutes
		//boost::posix_time::seconds
		//boost::posix_time::millisec
		//boost::posix_time::nanosec

		//Note that the existence of the higher resolution classes (eg: nanoseconds) depends on the installation of the library. 
		//See Build-Compiler Information for more information.

		//Another way to handle this is to utilize the ticks_per_second() method of time_duration to write code that is portable 
		//no matter how the library is compiled. The general equation for calculating a resolution independent count is as 
		//follows:
		//count*(time_duration_ticks_per_second / count_ticks_per_second)

		//For example, let's suppose we want to construct using a count that represents tenths of a second. That is, each tick 
		//is 0.1 second.

		//For example, let's suppose we want to construct using a count that represents tenths of a second. That is, each tick is 0.1 second.
		//int number_of_tenths = 5;
		//create a resolution independent count -- divide by 10 since there are 
		//10 tenths in a second.  
		//int count = number_of_tenths * (time_duration::ticks_per_second() / 10);
		//time_duration td(1, 2, 3, count); //01:02:03.5 //no matter the resolution settings
	}

	namespace test_time_period
	{
		//The class boost::posix_time::time_period provides direct representation for ranges between two times. Periods provide the ability to 
		//simplify some types of calculations by simplifying the conditional logic of the program.

		//A period that is created with beginning and end points being equal, or with a duration of zero, is known as a zero length period. Zero 
		//length periods are considered invalid (it is perfectly legal to construct an invalid period). For these periods, the last point will 
		//always be one unit less that the begin point.

		//#include <boost/date_time/posix_time/posix_time.hpp>       包括所有的类型和I/O
		//或者
		//#include <boost/date_time/posix_time/posix_time_types.hpp> 仅包括所有类型
	}

	namespace test_time_iterator
	{
		//Time iterators provide a mechanism for iteration through times. Time iterators are similar to Bidirectional Iterators. 
		//However, time_iterators are different than standard iterators in that there is no underlying sequence, just a calculation 
		//function. In addition, time_iterators are directly comparable against instances of class ptime. Thus a second iterator 
		//for the end point of the iteration is not required, but rather a point in time can be used directly.

		//#include <boost/date_time/posix_time/posix_time.hpp>       包括所有的类型和I/O
		//或者
		//#include <boost/date_time/posix_time/posix_time_types.hpp> 仅包括所有类型
	}

}

namespace test_last_day_of_the_month
{

}