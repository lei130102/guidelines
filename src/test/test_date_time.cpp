#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "test_date_time.h"

BOOST_AUTO_TEST_SUITE(s_date_time)

BOOST_AUTO_TEST_CASE(c_posix_time)
{

}

BOOST_AUTO_TEST_CASE(c_posix_time_ptime)
{
	////创建ptime

	//default constructor
	//creates a ptime object initialized to not_a_date_time
	boost::posix_time::ptime t0;

	//construct from a date and offset
	//2002.01.10 00:00:00 + 1h + 2m + 3s
	boost::posix_time::ptime t1(boost::gregorian::date(2002, boost::gregorian::Jan, 10), boost::posix_time::time_duration(1, 2, 3));   //gregorian 公历；阳历

	//constructor for infinities, not-a-date-time, max_date_time, and min_date_time
	boost::posix_time::ptime t2_neg_infin(boost::posix_time::neg_infin);
	boost::posix_time::ptime t2_pos_infin(boost::posix_time::pos_infin);
	boost::posix_time::ptime t2_not_a_date_time(boost::posix_time::not_a_date_time);
	boost::posix_time::ptime t2_max_date_time(boost::posix_time::max_date_time);
	boost::posix_time::ptime t2_min_date_time(boost::posix_time::min_date_time);

	//copy constructor
	boost::posix_time::ptime t3(t1);

	//from delimited string
	//NOTE: Excess digits in fractional seconds will be dropped. Ex: "1:02:03.123456999" => 1:02:03.123456. This behavior is affected by the precision the library is compiled with (see Build-Compiler Information.
	std::string ts4("2002-01-20 23:59:59.000");
	boost::posix_time::ptime t4(boost::posix_time::time_from_string(ts4));

	//from non delimited iso form string
	std::string ts5("20020131T235959");
	boost::posix_time::ptime t5(boost::posix_time::from_iso_string(ts5));

	//from delimited iso form string
	std::string ts6("2020-01-31T23:59:59.123");
	boost::posix_time::ptime t6(boost::posix_time::from_iso_extended_string(ts6));

	//Get the local time, second level resolution, based on the time zone settings of the computer.
	//本地时区的当前时间，秒分辨率
	boost::posix_time::ptime t7(boost::posix_time::second_clock::local_time());
	boost::posix_time::ptime now_second(t7);

	//Get the UTC time.
	boost::posix_time::ptime t8(boost::posix_time::second_clock::universal_time());

	//Get the local time using a sub second resolution clock. 
	//On Unix systems this is implemented using gettimeofday. 
	//On most Win32 platforms it is implemented using ftime. 
	//Win32 systems often do not achieve microsecond resolution via this API. 
	//If higher resolution is critical to your application test your platform to see the achieved resolution.
	boost::posix_time::ptime t9(boost::posix_time::microsec_clock::local_time());
	boost::posix_time::ptime now_microsec(t9);//一般不会达到微秒分辨率

	//Get the UTC time using a sub second resolution clock. 
	//On Unix systems this is implemented using gettimeofday. 
	//On most Win32 platforms it is implemented using ftime. 
	//Win32 systems often do not achieve microsecond resolution via this API. 
	//If higher resolution is critical to your application test your platform to see the achieved resolution.
	boost::posix_time::ptime t10(boost::posix_time::microsec_clock::universal_time());

	//time_t => boost::posix_time::ptime
	time_t tt;
	time(&tt);//获取time_t类型的当前时间
	boost::posix_time::ptime t11 = boost::posix_time::from_time_t(tt);

	//FILETIME => boost::posix_time::ptime
	//FILETIME ft;
	//boost::posix_time::ptime t12 = boost::posix_time::from_ftime<boost::posix_time::ptime>(ft);

	//tm => boost::posix_time::ptime
	tm pt_tm;
	pt_tm.tm_year = 105;
	pt_tm.tm_mon = 0;
	pt_tm.tm_mday = 1;
	pt_tm.tm_hour = 1;
	pt_tm.tm_min = 2;
	pt_tm.tm_sec = 3;
	boost::posix_time::ptime t13 = boost::posix_time::ptime_from_tm(pt_tm);

	////使用ptime
	{
		//Get the date part of a time
		//t1:2002.01.10 01:02:03
		// d:2002.01.10
		boost::gregorian::date d = t1.date();
	}

	{
		//Get the time offset in the day
		//t1:2002.01.10 01:02:03
		//td:01:02:03
		boost::posix_time::time_duration td = t1.time_of_day();
	}

	{
		//Returns true if ptime is either positive or negative infinity
		BOOST_CHECK(t2_neg_infin.is_infinity());
		BOOST_CHECK(t2_pos_infin.is_infinity());
	}

	{
		//Returns true if ptime is negative infinity
		BOOST_CHECK(t2_neg_infin.is_neg_infinity());
	}

	{
		//Returns true if ptime is positive infinity
		BOOST_CHECK(t2_pos_infin.is_pos_infinity());
	}

	{
		//Returns true if value is not a ptime
		BOOST_CHECK(t2_not_a_date_time.is_not_a_date_time());
	}

	{
		//Returns true if ptime is any special_value
		BOOST_CHECK(t2_neg_infin.is_special());
		BOOST_CHECK(t2_pos_infin.is_special());
		BOOST_CHECK(t2_not_a_date_time.is_special());

		BOOST_CHECK(!t2_max_date_time.is_special());
		BOOST_CHECK(!t2_min_date_time.is_special());
		BOOST_CHECK(!t1.is_special());
	}

	{
		//转为字符串



		//To YYYY-mmm-DD HH:MM:SS.fffffffff string where mmm 3 char month name.Fractional seconds only included if non-zero.
		//比如：2002-Jan-01 10:00:01.123456789

		//std::cout << boost::posix_time::to_simple_string(t1);
		BOOST_CHECK(boost::posix_time::to_simple_string(t1) == std::string("2002-Jan-10 01:02:03"));





		//Convert to form YYYYMMDDTHHMMSS,fffffffff where T is the date-time separator
		//比如：20020131T100001,123456789

		//std::cout << boost::posix_time::to_iso_string(t1) << std::endl;
		BOOST_CHECK(boost::posix_time::to_iso_string(t1) == std::string("20020110T010203"));





		//Convert to form YYYY-MM-DDTHH:MM:SS,fffffffff where T is the date-time separator
		//比如：2002-01-31T10:00:01,123456789

		//std::cout << boost::posix_time::to_iso_extended_string(t1);
		BOOST_CHECK(boost::posix_time::to_iso_extended_string(t1) == std::string("2002-01-10T01:02:03"));
	}

	{
		//operator<<  operator>>
		//Streaming operators. Note: As of version 1.33, streaming operations have been greatly improved.
		//See Date Time IO System for more details (including exceptions and error conditions).
		//略
		boost::posix_time::ptime t_from_stream(boost::posix_time::not_a_date_time);
		std::stringstream ss("2002-Jan-01 14:23:11");
		ss >> t_from_stream;
		BOOST_CHECK(boost::posix_time::to_simple_string(t_from_stream) == std::string("2002-Jan-01 14:23:11"));
	}

	{
		//operator== operator!= operator> operator< operator>= operator<=
		//略
	}

	{
		//boost::posix_time::ptime operator+(boost::gregorian::days)
		//Return a ptime adding a day offset

		//boost::posix_time::ptime operator-(boost::gregorian::days)
		//Return a ptime subtracting a day offset

		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::minutes m(5);

		boost::posix_time::ptime t(d, m);

		boost::gregorian::days ds(1);

		boost::posix_time::ptime t2 = t + ds;
		boost::posix_time::ptime t3 = t - ds;
	}
	
	{
		//boost::posix_time::ptime operator+(boost::posix_time::time_duration)
		//Return a ptime adding a time duration

		//boost::posix_time::ptime operator-(boost::posix_time::time_duration)
		//Return a ptime subtracting a time duration

		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::minutes m(5);

		boost::posix_time::ptime t(d, m);

		boost::posix_time::ptime t2 = t - boost::posix_time::minutes(2);
	}

	{
		//boost::posix_time::time_duration operator-(boost::posix_time::ptime)
		//Take the difference between two times

		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::minutes m(5);
		boost::posix_time::seconds s(5);

		boost::posix_time::ptime t1(d, m);
		boost::posix_time::ptime t2(d, s);

		boost::posix_time::time_duration t3 = t2 - t1;//negative result
	}

	{
		//boost::posix_time::ptime => tm

		boost::posix_time::ptime t(boost::gregorian::date(2005, boost::gregorian::Jan, 1), boost::posix_time::time_duration(1, 2, 3));
		tm pt_tm = to_tm(t);
		BOOST_CHECK(pt_tm.tm_year == 105);
		BOOST_CHECK(pt_tm.tm_mon == 0);
		BOOST_CHECK(pt_tm.tm_mday == 1);
		BOOST_CHECK(pt_tm.tm_wday == 6);   //Saturday
		BOOST_CHECK(pt_tm.tm_yday == 0);
		BOOST_CHECK(pt_tm.tm_hour == 1);
		BOOST_CHECK(pt_tm.tm_min == 2);
		BOOST_CHECK(pt_tm.tm_sec == 3);
		BOOST_CHECK(pt_tm.tm_isdst == -1);  //The tm_isdst field is set to -1
	}
}

BOOST_AUTO_TEST_CASE(c_posix_time_time_duration)
{
	////创建boost::posix_time::time_duration

	//boost::posix_time::time_duration::time_duration(boost::posix_time::hours, boost::posix_time::minutes, boost::posix_time::seconds, boost::posix_time::fractional_seconds)

	//Construct a duration from the counts. The fractional_second parameter is a number of units 
	//and is therefore affected by the resolution the application is compiled with (see 
	//Build-Compiler Information). If the fractional_seconds argument exceeds the limit of the
	//compiled precision, the excess value will be "carried over" into the seconds field. See 
	//above for techniques to creating a resolution independent count.
	
	boost::posix_time::time_duration td(1, 2, 3, 9);
	//1 hr 2 min 3 sec 9 nanoseconds

	boost::posix_time::time_duration td2(1, 2, 3, 123456789);
	boost::posix_time::time_duration td3(1, 2, 3, 1000);
	//with microsecond resolution (6 digits)
	//td2 => "01:04:06.456789"    //这里是不是有错？
	//td3 => "01:02:03.001000"
	//with nanosecond resolution (9 digits)
	//td2 => "01:02:03.123456789"
	//td3 => "01:02:03.000000000"







	//boost::posix_time::time_duration(boost::posix_time::special_value sv)
	//Special values constructor. Important note: When a time_duration is a special value, either by 
	//construction or other means, the following accessor functions will give unpredictable results:
	//hours()
	//minutes()
	//seconds()
	//ticks()
	//fractional_seconds()
	//total_nanoseconds()
	//total_microseconds()
	//total_milliseconds()
	//total_seconds()







	boost::posix_time::time_duration td_hours = boost::posix_time::hours(3);
	boost::posix_time::time_duration td_minutes = boost::posix_time::minutes(3);
	boost::posix_time::time_duration td_seconds = boost::posix_time::seconds(3);
	boost::posix_time::time_duration td_milliseconds = boost::posix_time::milliseconds(3); //毫秒
	boost::posix_time::time_duration td_microseconds = boost::posix_time::microseconds(3); //微秒
	//boost::posix_time::time_duration td_nanoseconds = boost::posix_time::nanoseconds(3);   //纳秒






	//From delimited string. NOTE: Excess digits in fractional seconds will be dropped. Ex: "1:02:03.123456999" => 1:02:03.123456. 
	//This behavior is affected by the precision the library is compiled with (see Build-Compiler Information.
	std::string ts("23:59:59.000");
	boost::posix_time::time_duration td_s(boost::posix_time::duration_from_string(ts));








	////使用time_duration

	{
		//Get the number of normalized hours (will give unpredictable results if calling time_duration is a special_value).

		boost::posix_time::time_duration td(1, 2, 3);
		boost::int64_t hours = td.hours();
		BOOST_CHECK(hours == 1);

		boost::posix_time::time_duration neg_td(-1, 2, 3);
		boost::int64_t neg_hours = neg_td.hours();
		BOOST_CHECK(neg_hours == -1);
	}

	{
		//Get the number of minutes normalized +/-(0..59) (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3);
		boost::int64_t minutes = td.minutes();
		BOOST_CHECK(minutes == 2);

		boost::posix_time::time_duration neg_td(-1, 2, 3);
		boost::int64_t neg_minutes = neg_td.minutes();
		BOOST_CHECK(neg_minutes == -2);
	}

	{
		//Get the normalized number of second +/-(0..59) (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3);
		boost::int64_t seconds = td.seconds();
		BOOST_CHECK(seconds == 3);

		boost::posix_time::time_duration neg_td(-1, 2, 3);
		boost::int64_t neg_seconds = neg_td.seconds();
		BOOST_CHECK(neg_seconds == -3);
	}

	{
		//Get the total number of seconds truncating any fractional seconds (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3, 10);
		boost::int64_t total_seconds = td.total_seconds();
		BOOST_CHECK(total_seconds == ((1 * 3600) + (2 * 60) + 3));
	}

	{
		//Get the total number of milliseconds truncating any remaining digits (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3, 123456789);
		boost::int64_t total_milliseconds = td.total_milliseconds();
		//HMS->(1*3600) + (2*60) + 3 = 3723 seconds
		//milliseconds is 3 decimal places
		//(3723 * 1000) + 123 = 3723123
		BOOST_CHECK(total_milliseconds == 3846456);  //与预计不同，疑问？
	}

	{
		//Get the total number of microseconds truncating any remaining digits (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3, 123456789);
		boost::int64_t total_microseconds = td.total_microseconds();
		//HMS->(1*3600) + (2*60) + 3 = 3723 seconds
		//microseconds is 6 decimal places
		//(3723*1000000) + 123456 == 3723123456
		BOOST_CHECK(total_microseconds == 3846456789);  //与预计不同，疑问？
	}

	{
		//Get the total number of nanoseconds truncating any remaining digits (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3, 123456789);
		boost::int64_t total_nanoseconds = td.total_nanoseconds();
		// HMS --> (1*3600) + (2*60) + 3 == 3723 seconds
		// nanoseconds is 9 decimal places
		// (3723 * 1000000000) + 123456789 == 3723123456789
		BOOST_CHECK(total_nanoseconds == 3846456789000);  //与预计不同，疑问？
	}

	{
		//Get the number of fractional seconds (will give unpredictable results if calling time_duration is a special_value).
		boost::posix_time::time_duration td(1, 2, 3, 1000);
		boost::int64_t fractional_seconds = td.fractional_seconds();
		BOOST_CHECK(fractional_seconds == 1000);
	}

	{
		//True if and only if duration is negative.
		boost::posix_time::time_duration td(-1, 0, 0);
		BOOST_CHECK(td.is_negative());
	}

	{
		//True if and only if duration is zero
		boost::posix_time::time_duration td(0, 0, 0);
		BOOST_CHECK(td.is_zero());
	}

	{
		//True if and only if duration is positive
		boost::posix_time::time_duration td(1, 0, 0);
		BOOST_CHECK(td.is_positive());
	}

	{
		//Generate a new duration with the sign inverted.
		boost::posix_time::time_duration td(-1, 0, 0);
		boost::posix_time::time_duration invert_sign_td = td.invert_sign(); //01:00:00
	}

	{
		//Generate a new duration with the absolute value of the time duration.
		boost::posix_time::time_duration td(-1, 0, 0);
		boost::posix_time::time_duration abs_td = td.abs();   //01:00:00
	}

	{
		//Describes the resolution capability of the time_duration class.time_resolutions is an enum of resolution
		//possibilities ranging from seconds to nanoseconds.
		boost::date_time::time_resolutions tr = boost::posix_time::time_duration::resolution();
		BOOST_CHECK(tr == boost::date_time::time_resolutions::micro);
	}

	{
		//Returns the number of fractional digits the time resolution has.
		unsigned short secs = boost::posix_time::time_duration::num_fractional_digits();
		BOOST_CHECK(secs == 6);
		//9 for nano, 6 for micro, etc
	}

	{
		//Return the number of ticks in a second.For example, if the duration supports nanoseconds then the returned result
		//will be 1,000,000,000 (1e+9).
		boost::int64_t ticks_per_second = boost::posix_time::time_duration::ticks_per_second();
		BOOST_CHECK(ticks_per_second == 1000000);
	}

	{
		//Return the raw count of the duration type(will give unpredictable results if calling time_duration is a special_value)
		boost::posix_time::time_duration td(0, 0, 0, 1000);
		BOOST_CHECK(td.ticks() == 1000);
	}

	{
		//Return smallest possible unit of duration type(1 nanosecond)
		BOOST_CHECK(boost::posix_time::time_duration::unit() == boost::posix_time::time_duration(0, 0, 0, 1));
	}

	{
		//Returns true if time_duration is negative infinity
		boost::posix_time::time_duration td(boost::posix_time::neg_infin);
		BOOST_CHECK(td.is_neg_infinity() == true);
	}

	{
		//Returns true if time_duration is positive infinity
		boost::posix_time::time_duration td(boost::posix_time::pos_infin);
		BOOST_CHECK(td.is_pos_infinity() == true);
	}

	{
		//Returns true if value is not a time
		boost::posix_time::time_duration td(boost::posix_time::not_a_date_time);
		BOOST_CHECK(td.is_not_a_date_time());
	}

	{
		//Returns true if time_duration is any special_value
		boost::posix_time::time_duration td_neg_infin(boost::posix_time::neg_infin);
		boost::posix_time::time_duration td_pos_infin(boost::posix_time::pos_infin);
		boost::posix_time::time_duration td_not_a_date_time(boost::posix_time::not_a_date_time);

		BOOST_CHECK(td_neg_infin.is_special());
		BOOST_CHECK(td_pos_infin.is_special());
		BOOST_CHECK(td_not_a_date_time.is_special());

		boost::posix_time::time_duration td(2, 5, 10);

		BOOST_CHECK(!td.is_special());
	}

	{
		//To HH:MM:SS.fffffffff were fff is fractional seconds that are only included if non-zero.
		//比如：10:00:01.123456789
		boost::posix_time::time_duration td(1, 2, 3, 9);
		//std::string s = boost::posix_time::to_simple_string(td);
		BOOST_CHECK(boost::posix_time::to_simple_string(td) == std::string("01:02:03.000009"));
	}

	{
		//Convert to form HHMMSS,fffffffff.
		//比如：100001,123456789
		boost::posix_time::time_duration td(1, 2, 3, 9);
		std::string s = boost::posix_time::to_iso_string(td);
		BOOST_CHECK(boost::posix_time::to_iso_string(td) == std::string("010203.000009"));
	}

	{
		//operator<<
		//operator>>

		//Streaming operators. Note: As of version 1.33, streaming operations have been greatly improved. 
		//See Date Time IO System for more details (including exceptions and error conditions).

		boost::posix_time::time_duration td(0, 0, 0);
		std::stringstream ss("14:23:11.345678");
		ss >> td;
		std::cout << td; // "14:23:11.345678"
	}

	{
		//operator==
		//operator!=
		//operator>
		//operator<
		//operator>=
		//operator<=

		//略
	}

	{
		//operator+
		//operator-

		boost::posix_time::time_duration td1(boost::posix_time::hours(1) + boost::posix_time::minutes(2));
		boost::posix_time::time_duration td2(boost::posix_time::seconds(10));
		boost::posix_time::time_duration td3 = td1 + td2;
		boost::posix_time::time_duration td4 = td1 - td2;
	}

	{
		//operator/
		//Divide the length of a duration by an integer value. Discards any remainder.

		//operator*
		//Multiply the length of a duration by an integer value
		
		BOOST_CHECK(boost::posix_time::hours(3) / 2 == boost::posix_time::time_duration(1, 30, 0));
		//BOOST_CHECK(boost::posix_time::nanosecond(3) / 2 == boost::posix_time::nanosecond(1));

		BOOST_CHECK(boost::posix_time::hours(3) * 2 == boost::posix_time::hours(6));
	}

	{
		//boost::posix_time::time_duration => tm

		//The fields:tm_year,tm_mday,tm_wday,tm_yday are set to zero
		//The tm_isdst field is set to -1
		boost::posix_time::time_duration td(1, 2, 3);
		tm td_tm = to_tm(td);
		BOOST_CHECK(td_tm.tm_year == 0);
		BOOST_CHECK(td_tm.tm_mon == 0);
		BOOST_CHECK(td_tm.tm_mday == 0);
		BOOST_CHECK(td_tm.tm_wday == 0);
		BOOST_CHECK(td_tm.tm_yday == 0);
		BOOST_CHECK(td_tm.tm_hour == 1);
		BOOST_CHECK(td_tm.tm_min == 2);
		BOOST_CHECK(td_tm.tm_sec == 3);
		BOOST_CHECK(td_tm.tm_isdst == -1);
	}
}

BOOST_AUTO_TEST_CASE(c_time_period)
{
	////创建time_period
	
	////time_period(ptime, ptime)
	//Create a period as [begin, end). If end is <= begin then the period will be defined as invalid.
	boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
	boost::posix_time::seconds s(10);
	boost::posix_time::hours h(10);

	boost::posix_time::ptime t1(d, s); //10 sec after midnight
	boost::posix_time::ptime t2(d, h); //10 hours after midnight

	boost::posix_time::time_period tp(t1, t2);

	////time_period(ptime, time_duration)
	//Create a period as [begin, begin+len] where end would be begin+len.If len is <= zero then the period will be
	//defined as invalid.
	boost::posix_time::time_period tp2(t1, h);

	////time_period(time_period rhs)
	//Copy constructor
	boost::posix_time::time_period tp3(tp2);

	////void shift(time_duration)
	//Add duration to both begin and end
	boost::posix_time::time_period tp4(
		boost::posix_time::ptime(boost::gregorian::date(2005, boost::gregorian::Jan, 1), boost::posix_time::hours(1))
		, boost::posix_time::hours(2));
	tp4.shift(boost::posix_time::minutes(5));
	//tp4 == 2005-Jan-01 01:05:00 to 2005-Jan-01 03:05:00

	////void expand(time_duration)
	//Subtract duration from begin and add duration to end
	boost::posix_time::time_period tp5(
		boost::posix_time::ptime(boost::gregorian::date(2005, boost::gregorian::Jan, 1), boost::posix_time::hours(1))
		, boost::posix_time::hours(2));
	tp5.shift(boost::posix_time::minutes(5));
	//tp5 == 2005-Jan-01 00:55:00 to 2005-Jan-01 03:05:00


	////使用time_period
	{
		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
		boost::posix_time::ptime t1(d, boost::posix_time::seconds(10)); //10 sec after midnight
		boost::posix_time::ptime t2(d, boost::posix_time::hours(10));   //10 hours after midnight
		boost::posix_time::time_period tp(t1, t2);

		//ptime begin()
		//Return first time of period
		tp.begin();  //->2002-Jan-01 00:00:10

		//ptime last()
		//Return last time of period
		tp.last();   //->2002-Jan-01 09:59:59.999999999

		//ptime end()
		//Return one past the last in period
		tp.end();    //->2002-Jan-01 10:00:00
	}

	{
		//time_duration length()
		//Return the length of the time period

		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
		boost::posix_time::ptime t1(d);//midnight
		boost::posix_time::time_period tp(t1, boost::posix_time::hours(1));
		tp.length(); //-> 1 hour
	}

	{
		//bool is_null()
		//True if period is not well formed. eg: end is less than or equal to begin.

		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
		boost::posix_time::ptime t1(d, boost::posix_time::hours(12));  //noon on Jan 1st
		boost::posix_time::ptime t2(d, boost::posix_time::hours(9));   //9am on Jan 1st
		boost::posix_time::time_period tp(t1, t2);
		BOOST_CHECK(tp.is_null());
	}

	{
		////bool contains(ptime)
		//True if ptime is within the period. Zero length periods cannot contain any points.
		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
		boost::posix_time::ptime t1(d, boost::posix_time::seconds(10));  //10 sec after midnight
		boost::posix_time::ptime t2(d, boost::posix_time::hours(10));    //10 hours after midnight
		boost::posix_time::ptime t3(d, boost::posix_time::hours(2));     //2 hours after midnight
		boost::posix_time::time_period tp(t1, t2);
		BOOST_CHECK(tp.contains(t3));
		boost::posix_time::time_period tp2(t1, t1);
		BOOST_CHECK(!tp2.contains(t1));
	}

	{
		////bool contains(time_period)
		//True if period is within the period
		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);
		boost::posix_time::time_period tp1(
			boost::posix_time::ptime(d, boost::posix_time::hours(1))
			, boost::posix_time::ptime(d, boost::posix_time::hours(12)));
		boost::posix_time::time_period tp2(
			boost::posix_time::ptime(d, boost::posix_time::hours(2))
			, boost::posix_time::ptime(d, boost::posix_time::hours(4)));
		BOOST_CHECK(tp1.contains(tp2));
		BOOST_CHECK(!tp2.contains(tp1));
	}

	{
		////bool intersects(time_period)
		//True if periods overlap
		boost::gregorian::date d(2002, boost::gregorian::Jan, 01);

		boost::posix_time::time_period tp1(
			boost::posix_time::ptime(d, boost::posix_time::hours(1))
			, boost::posix_time::ptime(d, boost::posix_time::hours(12)));

		boost::posix_time::time_period tp2(
			boost::posix_time::ptime(d, boost::posix_time::hours(2))
			, boost::posix_time::ptime(d, boost::posix_time::hours(4)));

		BOOST_CHECK(tp2.intersects(tp1));
	}

	{
		////time_period intersection(time_period)
		//Calculate the intersection of 2 periods. Null if no intersection.
	}

	{
		////time_period merge(time_period)
		//Returns union of two periods. Null if no intersection
	}

	{
		////time_period span(time_period)
		//Combines two periods and any gap between them such that begin = min(p1.begin, p2.begin) and end = max(p1.end, p2.end)
	}

	{
		////std::string to_simple_string(time_period dp)
		//To [YYYY-mmm-DD hh:mm:ss.fffffffff/YYYY-mmm-DD hh:mm:ss.fffffffff] string where mmm is 3 char month name.

		//[2002-Jan-01 01:25:10.000000001/2002-Jan-31 01:25:10.123456789]
		// string occupies one line
	}

	{
		////operator<<
		//Output streaming operator for time duration. Uses facet to output [date time_of_day/date time_of_day]. 
		//The default is format is [YYYY-mmm-DD hh:mm:ss.fffffffff/YYYY-mmm-DD hh:mm:ss.fffffffff] string where 
		//mmm is 3 char month name and the fractional seconds are left out when zero.
		//[2002-Jan-01 01:25:10.000000001/ \2002-Jan-31 01:25:10.123456789]

		////operator>>
		//Input streaming operator for time duration. Uses facet to read [date time_of_day/date time_of_day]. 
		//The default is format is [YYYY-mmm-DD hh:mm:ss.fffffffff/YYYY-mmm-DD hh:mm:ss.fffffffff] string where 
		//mmm is 3 char month name and the fractional seconds are left out when zero.
		//[2002-Jan-01 01:25:10.000000001/ \2002-Jan-31 01:25:10.123456789]
	}

	{
		////operator==
		////operator!=
		//Equality operators. Periods are equal if p1.begin == p2.begin && p1.last == p2.last

		//if(tp1 == tp2) ...
	}

	{
		////operator<
		//Ordering with no overlap. True if tp1.end() less than tp2.begin()
		//if(tp1 < tp2) ...

		////operator>
		//Ordering with no overlap. True if tp1.begin() greater than tp2.end()
		//if(tp1 > tp2) ...
	}

	{
		////operator<=
		////operator>=
		//Defined in terms of the other operators.
	}
}

BOOST_AUTO_TEST_CASE(c_time_iterator)
{
	//time_iterator类
	//Iterate incrementing by the specified duration.
	//ptime start_time, time_duration increment

	{
		//operator==(const ptime& rhs),
		//operator!=(const ptime& rhs),
		//operator>,
		//operator<,
		//operator>=,
		//operator<=

		//A full complement of comparison operators

		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::ptime start_time(d, boost::posix_time::hours(1));

		boost::posix_time::minutes increment(10);

		boost::posix_time::time_iterator titr(start_time, increment);

		boost::posix_time::ptime end_time = start_time + boost::posix_time::hours(2);

		BOOST_CHECK(titr != end_time);
		BOOST_CHECK(titr < end_time);
	}

	{
		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::ptime start_time(d, boost::posix_time::hours(1));

		boost::posix_time::milliseconds increment(10);

		//prefix increment
		//Increment the iterator by the specified duration.
		boost::posix_time::time_iterator titr(start_time, increment);
		++titr;  // == start_time + 10 milliseconds
	}

	{
		boost::gregorian::date d(2002, boost::gregorian::Jan, 1);
		boost::posix_time::ptime start_time(d, boost::posix_time::hours(1));

		boost::posix_time::time_duration increment(1, 2, 3);

		//prefix decrement
		//Decrement the iterator by the specified time duration.
		boost::posix_time::time_iterator titr(start_time, increment);
		--titr;// == start_time - 01:02:03
	}
}

BOOST_AUTO_TEST_CASE(c_dates_as_strings)
{
	//Various parsing and output of strings

	//shows conversion of dates to and from a std::string


}

//获取某年某月的最后一天
BOOST_AUTO_TEST_CASE(c_last_day_of_the_month)
{
	boost::gregorian::greg_year year(1400);
	boost::gregorian::greg_month month(1);

	boost::gregorian::date d(year, month, 1);

	//1400-Jan-31
	std::string s = boost::gregorian::to_simple_string(d.end_of_month());
	//1400-01-31
	std::string s2 = boost::gregorian::to_sql_string(d.end_of_month());

	std::cout << s << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()