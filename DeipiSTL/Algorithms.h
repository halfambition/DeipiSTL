//
//  Algorithms.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/25.
//
#pragma once
#ifndef deipi_ALGORITHMS_h
#define deipi_ALGORITHMS_h

#include <vcruntime_string.h>
#include "TypeTraits.h"

namespace DeipiSTL {
	template <typename ForwardIterator, typename T>
	void Fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first)
			*first = value;
	}
	template <typename OutputIterator, typename T>
	OutputIterator Fill_N(OutputIterator first, size_t n, const T& value) {
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	template <typename T>
	inline const T& Max(const T& a, const T& b) {
		return a < b ? b : a;
	}
	template <typename T, typename Compare>
	inline const T& Max(const T& a, const T& b, Compare comp) {
		return comp(a, b)? b : a;
	}
	template <typename T>
	inline const T& Min(const T& a, const T& b) {
		return a > b ? b : a;
	}
	template <typename T, typename Compare>
	inline const T& Min(const T& a, const T& b, Compare comp) {
		return comp(b, a) ? b : a;
	}
	template <typename T, typename Tar>
	inline bool In_Range(T& start, T& end, Tar& tar) {
		return (tar >= start && tar <= end);
	}
	template <typename T>
	inline void Swap(T& a, T& b) {
		T temp = a;
		a = b;
		b = temp;
	}

	//COPY
	namespace {
		template <typename InputIterator, typename OutputIterator>
		struct __copy_dispatch {
			OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator output) {
				return __copy(first, last, output, Iterator_Category(first));
			}
		};
		template <typename T>
		struct __copy_dispatch<T*, T*> {
			T* operator()(T* first, T* last, T* output) {
				typedef typename __type_traits<T>::has_trival_assignment_operator t;
				return __copy(first, last, output, t());
			}
		};
		template <typename T>
		struct __copy_dispatch<const T*, T*> {
			T* operator()(const T* first, const T* last, T* output) {
				typedef typename __type_traits<T>::has_trival_assignment_operator t;
				return __copy(first, last, output, t());
			}
		};
		//InputIterator
		template <typename InputIterator, typename OutputIterator>
		inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator output, input_iterator_tag) {
			for (; first != last; ++output, ++first)
				*output = *first;
			return output;
		}
		//RandomAccessIterator
		template <typename RandomAccessIterator, typename OutputIterator>
		inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator output, random_access_iterator_tag) {
			typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
			for (distance n = last - first; n > 0; --n, ++output, ++first)
				//I don't understand why
				*output = *first;
			return output;
		}

		//pointer type
		template <typename T>
		inline T* __copy_t(const T* first, const T* last, T* output, __true_type) {
			//do has trival assignment operator
			memmove(output, first, sizeof(T) * (last - first));
			return output + (last - first);
		}
		template <typename T>
		inline T* __copy_t(const T* first, const T* last, T* output, __false_type) {
			//not has trival assignment operator
			return __copy(first, last, output, random_access_iterator_tag());
		}
	}
	inline char* Copy(const char* first, const char* last, char* output) {
		memcpy(output, first, last - first);
		return output + (last - first);
	}
	inline wchar_t* Copy(const wchar_t* first, const wchar_t* last, wchar_t* output) {
		memcpy(output, first, sizeof(wchar_t) * (last - first));
		return output + (last - first);
	}
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator Copy(InputIterator first, InputIterator last, OutputIterator output) {
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, output);
		//return last of output
	}

	//backward copy
	//last is null iterator, so is output_rear
	//copy from last - 1 to output_rear - 1, and go on
	namespace {
		template <typename InputIterator, typename OutputIterator>
		struct __copy_dispatch_backward {
			OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator output_rear) {
				return __copy_backward(first, last, output_rear, Iterator_Category(first));
			}
		};
		template <typename T>
		struct __copy_dispatch_backward<T*, T*> {
			T* operator()(T* first, T* last, T* output_rear) {
				typedef typename __type_traits<T>::has_trival_assignment_operator t;
				return __copy_backward(first, last, output_rear, t());
			}
		};
		template <typename T>
		struct __copy_dispatch_backward<const T*, T*> {
			T* operator()(const T* first, const T* last, T* output_rear) {
				typedef typename __type_traits<T>::has_trival_assignment_operator t;
				return __copy_backward(first, last, output_rear, t());
			}
		};
		//InputIterator
		template <typename InputIterator, typename OutputIterator>
		inline OutputIterator __copy_backward(InputIterator first, InputIterator last, OutputIterator output_rear, input_iterator_tag) {
			--output_rear; --last;
			for (; last != first - 1; --output_rear, --last)
				*output_rear = *first;
			return output_rear;
		}
		//RandomAccessIterator
		template <typename RandomAccessIterator, typename OutputIterator>
		inline OutputIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, OutputIterator output_rear, random_access_iterator_tag) {
			typedef typename iterator_traits<RandomAccessIterator>::difference_type distance;
			distance n = last - first;
			--output_rear; --last;
			for (; n > 0; --n, --output_rear, --last)
				//I don't understand why
				*output_rear = *first;
			return output_rear;
		}

		//pointer type
		template <typename T>
		inline T* __copy_t_backward(const T* first, const T* last, T* output_rear, __true_type) {
			//do has trival assignment operator
			return __copy_t(first, last, output_rear, __true_type);
		}
		template <typename T>
		inline T* __copy_t_backward(const T* first, const T* last, T* output_rear, __false_type) {
			//not has trival assignment operator
			return __copy_backward(first, last, output_rear, random_access_iterator_tag());
		}
	}
	inline char* Copy_backward(const char* first, const char* last, char* output_rear) {
		return Copy(first, last, output_rear);
	}
	inline wchar_t* Copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* output_rear) {
		return Copy(first, last, output_rear);
	}
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator Copy_backward(InputIterator first, InputIterator last, OutputIterator output_rear) {
		return __copy_dispatch_backward<InputIterator, OutputIterator>()(first, last, output_rear);
	}
}

#endif /* deipi_ALGORITHMS_h */