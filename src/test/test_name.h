#pragma once

namespace test_name
{
	class a
	{

	};

	class b
	{

	};

	class test_class
	{
		int a()   //可以重名
		{
			return 0;
		}

		b* b()    //可以重名
		{
			return nullptr;
		}
	};
}
