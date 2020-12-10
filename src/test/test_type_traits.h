#pragma once

namespace ns1
{
	//1.integral_constant

	template<class _Ty, _Ty _Val>
	struct integral_constant
	{
		static constexpr _Ty value = _Val;  //核心!!  静态常量(static constexpr)

		using value_type = _Ty;
		using type = integral_constant;

		constexpr operator value_type() const noexcept
		{
			return (value);
		}

		_NODISCARD constexpr value_type operator()() const noexcept
		{
			return (value);
		}
	};

	//integral_constant存在的意义是：为了C++编译的时候能够使用模板初编译来确定其中的值

	//同理true_type 和 false_type

	template<bool _Val>
	using bool_constant = integral_constant<bool, _Val>;

	using true_type = bool_constant<true>;
	using false_type = bool_constant<false>;

	//(注意上面不是通过继承实现的)

	namespace primary_type_categories
	{
		//is_array
		//is_class
		//is_enum
		//is_floating_point
		//is_function
		//is_integral
		//is_lvalue_reference
		//is_member_function_pointer
		//is_member_object_pointer
		//is_pointer
		//is_rvalue_reference
		//is_union
		//is_void
	}

	namespace composite_type_categories
	{
		//is_arithmetic
		//is_compound
		//is_fundamental
		//is_member_pointer
		//is_object
		//is_reference
		//is_scalar
	}

	namespace type_properties
	{
		//is_abstract
		//is_const
		//is_empty
		//is_literal_type
		//is_pod
		//is_polymorphic
		//is_signed
		//is_standard_layout
		//is_trivial
		//is_trivially_copyable
		//is_unsigned
		//is_volatile
	}

	namespace type_features
	{
		//has_virtual_destructor
		//is_assignable
		//is_constructible
		//is_copy_assignable
		//is_copy_constructible
		//is_destructible
		//is_default_constructible
		//is_move_assignable
		//is_move_constructible
		//is_trivially_assignable
		//is_trivially_constructible
		//is_trivially_copy_assignable
		//is_trivially_copy_constructible
		//is_trivially_destructible
		//is_trivially_default_constructible
		//is_trivially_move_assignable
		//is_trivially_move_constructible
		//is_nothrow_assignable
		//is_nothrow_constructible
		//is_nothrow_copy_assignable
		//is_nothrow_copy_contructible
		//is_nothrow_desgtructible
		//is_nothrow_default_constructible
		//is_nothrow_move_assignable
		//is_nothrow_move_constructible
	}

	namespace type_relationships
	{
		//is_base_of
		//is_convertible
		//is_same
	}

	namespace property_queries
	{
		//alignment_of
		//extent
		//rank
	}

	namespace type_transformations
	{
		//const-volatile qualifications:

		//add_const
		//add_cv
		//add_volatile
		//remove_const
		//remove_cv
		//remove_volatile



		//compound type alterations:

		//add_pointer
		//add_lvalue_reference
		//add_rvalue_reference
		//decay
		//make_signed
		//make_unsigned
		//remove_all_extents
		//remove_extent
		//remove_pointer
		//remove_reference
		//underlying_type



		//other type generators:

		//aligned_storage
		//aligned_union
		//common_type
		//conditional
		//enable_if
		//result_of
	}

}
