//
//  TypeTraits.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/19.
//
#pragma once
#ifndef deipi_TYPETRAITS_h
#define deipi_TYPETRAITS_h


namespace DeipiSTL {
	//same with iterator tag
	struct __true_type{
		static constexpr bool value = true;
	};
	struct __false_type {
		static constexpr bool value = false;
	};

	template <typename type>
	struct __type_traits {
		typedef __true_type		this_dummy_member_must_be_first;
			//this is a special member, it notices the compiler.

		typedef __false_type	has_trivial_default_constructor;
		typedef __false_type	has_trivial_copy_constructor;
		typedef __false_type	has_trivial_assignment_operator;
		typedef __false_type	has_trivial_destructor;
		typedef __false_type	is_POD_type;
		/*
		What's trivial ?
		trivial means generated by compiler.
		 
		What's trivial classes ?
		all copy, move, assignment constructor, default constructor and destructor are trivial.

		What's POD?
		POD is Plain Old Data, it interprets property of class/struct which is non-OOP.
		Plain means class without a virtual feature.
		Old means C++ class is compatible with C.
		POD has two features, trivial and Standard-layout.

		Constructor and Destructor in POD class are insignificant.
		POD class can initialize by memset() and memcpy().
		*/
	};

	template <>
	struct __type_traits<bool> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<wchar_t> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<signed char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<unsigned char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<unsigned short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<unsigned int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<long long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<unsigned long long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<float> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <>
	struct __type_traits<long double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <typename T>
	struct __type_traits<T*> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	template <typename T>
	struct __type_traits<const T*> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	/*
	if T& and T&& exception not exist, when call remove_reference<int&> or remove_reference<int&&>,
	T = int&, remove_reference::type is int&, removing a ref does not work.
	*/
	template <typename T>
	struct remove_reference {
		typedef T type;
	};
	template <typename T>
	struct remove_reference<T&> {
		typedef T type;
	};
	template <typename T>
	struct remove_reference<T&&> {
		typedef T type;
	};

	template<class T> struct remove_cv { 
		typedef T type; 
	};
	template<class T> struct remove_cv<const T> { 
		typedef T type; 
	};
	template<class T> struct remove_cv<volatile T> { 
		typedef T type;
	};
	template<class T> struct remove_cv<const volatile T> {
		typedef T type;
	};

	template<class T> struct remove_const {
		typedef T type;
	};
	template<class T> struct remove_const<const T> {
		typedef T type;
	};

	template<class T> struct remove_volatile {
		typedef T type;
	};
	template<class T> struct remove_volatile<volatile T> {
		typedef T type;
	};

	//use the GCC code
	namespace {
		template <typename T>
		struct is_pointer_helper : public __false_type { };

		template <typename T>
		struct is_pointer_helper<T*> : public __true_type { };
	}

	template <typename T>
	struct is_pointer : public is_pointer_helper<typename remove_cv<T>::type> { };

	template <typename T>
	struct is_lvalue_reference : public __false_type { };

	template <typename T>
	struct is_lvalue_reference<T&> : public __true_type { };

	template <typename T>
	struct is_lvalue_reference<T&&> : public __false_type { };

	template <typename T>
	struct is_rvalue_reference : public __false_type { };

	template <typename T>
	struct is_rvalue_reference<T&> : public __false_type { };

	template <typename T>
	struct is_rvalue_reference<T&&> : public __true_type { };
}

#endif /* deipi_TYPETRAITS_h */
