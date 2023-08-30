//
//  Memory.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/29.
//
#pragma once
#ifndef deipi_MOVE_h
#define deipi_MOVE_h

#include "TypeTraits.h"

namespace DeipiSTL {
	template <typename T>
	constexpr typename remove_reference<T>::type&& move(T&& t) {
		return static_cast<typename remove_reference<T>::type&&>(t);
	}

	template <typename T>
	constexpr T&& forward(typename remove_reference<T>::type& t) {
		return static_cast<T&&>(t);
	}
	template <typename T>
	constexpr T&& forward(typename remove_reference<T>::type&& t) {
		static_assert(!is_lvalue_referenece<T>::value, "template argument substituting T is an lvaule reference type")
		return static_cast<T&&>(t);
	}
}

#endif	/*deipi_MOVE_h*/