#ifndef OPTIONS_H
#define OPTIONS_H

#include <pack_options.h>

namespace xll
{
	//为了方便，一般OPTION_NAME和CONSTANT_NAME相同
	XLL_OPTION_CONSTANT(custom_value_option_name, bool, Enabled, custom_value_option_name)
		//template<bool Enabled>
		//struct custom_value_option_name
		//{
		//	template<class Base>
		//	struct pack : Base
		//	{
		//		static const bool custom_value_option_name = Enabled;
		//	};
		//}

	//为了方便，一般OPTION_NAME和TYPEDEF_NAME相同
	XLL_OPTION_TYPE(custom_type_option_name, Custom_type_option_type, Custom_type_option_type, custom_type_option_name)
		//template<typename Custom_type_option_type>
		//struct custom_type_option_name
		//{
		//	template<typename Base>
		//	struct pack : Base
		//	{
		//		typedef Custom_type_option_type custom_type_option_name;
		//	};
		//}

	//设置默认值默认类型
	struct defaults
	{
		static const bool custom_value_option_name = false; //这里custom_value_option_name要跟上面的OPTION_NAME同名
		typedef void* custom_type_option_name;              //这里custom_type_option_name要跟上面的OPTION_NAME同名
	};
}

#endif //OPTIONS_H