#pragma once

#include <boost/mp11.hpp>

namespace ns1
{//boost.mpl�����⣬����boost.mp11�Ĳ�ͬ

	//boost.mpl���㷺ʹ�ã���Ȼ��������������

	//����C++11�ķ����������������˺ܴ�仯������û��������ʹ��boost.mpl�е�ʵ�ַ�ʽ��boost.mp11������

	//C++11֧�ִ��в�����(parameter packs)�ı䳤ģ��(variadic template)�����԰����·�ʽ���������б�(type list)
	template<class... T>
	struct type_list
	{};

	//C++11֧�ֱ���ģ�壬ԭ��Ԫ����(�ṩ�����������;ͷ��ؽ������)
	//���壺
	//template<class T>
	//struct add_pointer
	//{
	//	typedef T* type;
	//};
	//ʹ�ã�
	//typedef typename add_pointer<int>::type Intp;
	//���Էֱ��дΪ
	//���壺
	template<class T>
	using add_pointer = T*;
	//ʹ�ã�
	using Intp = add_pointer<int>;

	//C++11֧��ģ���>���ſ�������д������Ҫ�ո����

	//C++11֧�ֱ���ģ�崫�ݸ�ģ��ģ�����
	template<template<class... T> class F> struct X
	{
	};
	X<add_pointer>; //��ȷ

	//�ܽ᣺boost.mpl�Ѳ��������ѡ��
}

namespace ns2
{//�����б�(type list)�ı�ʾ �� mp_renameԪ������ʵ��

	//���������б�  (ע�������б���ģ��)
	template<class... T>
	struct mp_list
	{};

	//boost.mpl��ʹ�������ռ��ֹ���Ƴ�ͻ����boost.mp11�г���ʹ�������ռ��⻹������mp_ǰ׺��������ԭ��(��û������)
	/*
	* Why the mp_ prefix? mp obviously stands for metaprogramming, but could we not have used a namespace?
	* Indeed we could have. Past experience with Boost.MPL however indicates that name conflicts between our metaprogramming primitives and standard identifiers (such as list) and keywords (such as if, int or true) will be common and will be a source of problems. With a prefix, we avoid all that trouble.
	*/

	//�������б���ʢ������   (ע��ʢ�ź�������)
	using list = mp_list<int, char, float, double, void>;  //ע�������void

	//�����б�����в�ͬ�ı�ʾ��������ʹ�ö����������ʱ����������ͬ�������б�����
	template<class... T>
	struct tuple
	{};
	//����
	template<class... T>
	struct packer
	{};
	//����Ҫ�������б���в���ʱ������
	//packer<int, float, void> תΪ packer<int*, float*, void*>
	//��ô��Ҫ
	//packer<int, float, void> תΪ mp_list<int, float, void> תΪ mp_list<int*, float*, void*> תΪ packer<int*, float*, void*>

	//Ԫ���� mp_rename �Ķ���
	template<class A, template<class...> class B>      //��������������ĸ�ʽ
	struct mp_rename_impl;

	template<template<class...> class A, class... T, template<class...> class B>
	struct mp_rename_impl<A<T...>, B>                  //��ģ��ƫ�ػ�
	{
		using type = B<T...>;
	};

	template<class A, template<class...> class B>      //�����涨������������ʽ����һ��
	using mp_rename = typename mp_rename_impl<A, B>::type;    //���ֺ��һ������ģ�����ʽ���ձ�д��������ģ�岻֧��ƫ�ػ����ػ�

	//ʹ�ã�
	//mp_rename<packer<int, float, void*>, mp_list> ���� mp_list<int, float, void*>

	//ע������mp_rename��ʵ�ֲ�û������mp_list���������������úܶ������
	//mp_rename<std::pair<int, float>, std::tuple> ���� std::tuple<int, float>
	//mp_rename<mp_list<int, float>, std::pair> ���� std::pair<int, float>
	//mp_rename<std::shared_ptr<int>, std::unique_ptr> ���� std::unique_ptr<int>

	//ע�� mp_rename<std::unique_ptr<int>, std::shared_ptr> �Ǵ���ģ���Ϊ std::unique_ptr<int> ʵ���� std::unique_ptr<int, std::default_delete<int>>
	//���ص� std::shared_ptr<int, std::default_delete<int>> �޷�����ͨ��






	//����һ�������� mp_list ��Ԫ���� mp_size (����)
	namespace mp_size_by_mp_list
	{
		template<class L>
		struct mp_size_impl;

		template<class... T>
		struct mp_size_impl<mp_list<T...>>
		{
			using type = std::integral_constant<std::size_t, sizeof...(T)>;
		};

		template<class L>
		using mp_size = typename mp_size_impl<L>::type;
	}
	//integral_constant�������ǽ����������ͳ�����װΪ���ͣ���Ϊ�����б���ʢ�ŵĶ������ͣ����������Ҫ����תΪ�����ڳ���ֻ��Ҫ::value�Ϳ�����

	//����һ���������� mp_list ��Ԫ���� mp_size������mp_rename   (�ã�����Ҫ��תΪ mp_list ��)
	namespace mp_size_
	{
		template<class L>
		struct mp_size_impl;

		template<template<class...> class L, class... T>
		struct mp_size_impl<L<T...>>
		{
			using type = std::integral_constant<std::size_t, sizeof...(T)>;
		};

		template<class T>
		using mp_size = typename mp_size_impl<L>::type;
	}
	//���� mp_size Ҳ�� mp_rename һ��������Ӧ�õ����������б���

	//����ת���Ǻõģ���������˵ mp_rename ��û�����ˡ�������Щ����µ����� mp_list<int, void, float> ���� X<int, void, float> ʱ��
	//������ mp_rename ��:
	//template<class L>
	//using B = mp_rename<L, X>;
	
	//ͬ��mp_apply��������ʵ��
	template<template<class...> class F, class L>
	using mp_apply = mp_rename<L, F>;
	//calls the metafunction F with the contents of the list L
}