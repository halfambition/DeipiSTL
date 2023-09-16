//
//  Algorithms.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/25.
//
#pragma once
#ifndef deipi_ALGORITHMS_h
#define deipi_ALGORITHMS_h
#include <cstring>
#include "TypeTraits.h"
#include <stdio.h>
#include <vector>
namespace DeipiSTL {
    template <typename T>
    T ABS(T& val) {
        if(val < 0)
            return -val;
        return val;
    }
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
    //parody python in range
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
				typedef typename __type_traits<T>::has_trivial_assignment_operator t;
				return __copy_t(first, last, output, t());
			}
		};
		template <typename T>
		struct __copy_dispatch<const T*, T*> {
			T* operator()(const T* first, const T* last, T* output) {
				typedef typename __type_traits<T>::has_trivial_assignment_operator t;
				return __copy_t(first, last, output, t());
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
			//do has trivial assignment operator
			memmove(output, first, sizeof(T) * (last - first));
			return output + (last - first);
		}
		template <typename T>
		inline T* __copy_t(const T* first, const T* last, T* output, __false_type) {
			//not has trivial assignment operator
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
				typedef typename __type_traits<T>::has_trivial_assignment_operator t;
				return __copy_t_backward(first, last, output_rear, t());
			}
		};
		template <typename T>
		struct __copy_dispatch_backward<const T*, T*> {
			T* operator()(const T* first, const T* last, T* output_rear) {
				typedef typename __type_traits<T>::has_triial_assignment_operator t;
				return __copy_t_backward(first, last, output_rear, t());
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
			//do has trivial assignment operator
			return __copy_t(first, last, output_rear, __true_type());
		}
		template <typename T>
		inline T* __copy_t_backward(const T* first, const T* last, T* output_rear, __false_type) {
			//not has trivial assignment operator
			return __copy_backward(first, last, output_rear, random_access_iterator_tag());
		}
	}
	inline char* Copy_backward(const char* first, const char* last, char* output_rear) {
		size_t n = (last - first);
		memcpy(output_rear, first, last - first);
		return output_rear;
	}
	inline wchar_t* Copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* output_rear) {
		size_t n = (last - first);
		memcpy(output_rear, first, sizeof(wchar_t) * (last - first));
		return output_rear;
	}
	template <typename InputIterator, typename OutputIterator>
	inline OutputIterator Copy_backward(InputIterator first, InputIterator last, OutputIterator output_rear) {
		return __copy_dispatch_backward<InputIterator, OutputIterator>()(first, last, output_rear);
	}
}

namespace DeipiSTL{
    //sort
    namespace {
        //insert sort
        template <typename iterator, typename comparator>
        inline void insert_sort(iterator start, iterator end, const comparator& cmp){
            for(auto it1 = start; it1 <= end; ++it1)
                for(auto it2 = it1; it2 > start; --it2){
                    if(!cmp(*(it2-1), *it2))
                        Swap(*(it2-1), *it2);
                }
        }

        //bubble sort
        template <typename iterator, typename comparator>
        inline void bubble_sort(iterator start, iterator end, const comparator& cmp){
            for(auto it1 = start; it1 != end; ++it1)
                for(auto it2 = end; it2 > it1; --it2)
                    if(!cmp(*(it2-1), *it2))
                        Swap(*(it2-1), *it2);
        }

        //quick sort
        template <typename iterator, typename comparator>
        inline void quick_sort(iterator start, iterator end, const comparator& cmp){
            static int depth = 1;
            depth = depth == 1?1:depth+1;
            if(end - start <= 16)
                return insert_sort(start, end);
            if(depth > 16)
                //heap sort
                return insert_sort(start, end);
            if(start >= end)
                return;
            auto l = start;
            auto r = end;
            auto temp = *l;
            while(l < r){
                while(l < r && !cmp(*r, temp)){
                    --r;
                }
                if(l < r){
                    *l = *r;
                    ++l;
                }
                while(l < r && cmp(*l, temp)){
                    ++l;
                }
                if(l < r){
                    *r = *l;
                    --r;
                }
            }
            *l = temp;
            quick_sort(start, l-1, cmp);
            quick_sort(l+1, end, cmp);
        }
        //heap sort

        //bucket sort
    }
    namespace {
        template <typename RandomAccessIterator, typename comparator>
        inline void __sort(RandomAccessIterator start, RandomAccessIterator end, comparator cmp, random_access_iterator_tag){
            quick_sort(start, end, cmp);
        }

        //handle for other iterators
        template <typename InputIterator, typename comparator>
        inline void __sort(InputIterator start, InputIterator end, comparator cmp, input_iterator_tag){
            printf("only Random Access Iterator can be sorted\n");
        }
    }

    template <typename T>
    struct lessComparator{
        bool operator()(T& a, T& b) const{
            return a < b;
        }
    };
    template <typename T>
    struct greaterComparator{
        bool operator()(T& a, T& b) const{
            return a > b;
        }
    };
    template <typename RandomAccessIterator, typename comparator>
    inline void Sort(RandomAccessIterator start, RandomAccessIterator end, comparator cmp = lessComparator<RandomAccessIterator>()){
        typedef typename iterator_traits<RandomAccessIterator>::iterator_category    category;
        //if using 'end', error will generate
        __sort(start, end-1, cmp, category());
    }
}

namespace DeipiSTL {
    namespace {
        template <typename InputIterator, typename T>
        inline InputIterator linear_find(InputIterator start, InputIterator end, const T& val) {
            for (; start != end; ++start)
                if (*start == val)
                    return start;
            return end;
        }
    }
    template <typename RandomAccessIterator, typename T>
    inline RandomAccessIterator binary_search(RandomAccessIterator start, RandomAccessIterator end, const T& val){
        if(end - start <= 16)
            return linear_find(start, end, val);
        auto low = start, high = end;
        while(start <= end){
            RandomAccessIterator mid = low + ((high - low)>>2);
            if(*mid == val){
                return mid;
            }
            else if(*mid < val){
                high = mid - 1;
            }
            else if(*mid > val){
                low = mid + 1;
            }
        }
        return end;
    }
    template <typename InputIterator, typename T>
    inline InputIterator Find(InputIterator start, InputIterator end, const T& val){
        return linear_find(start, end, val);
    }
}
#endif /* deipi_ALGORITHMS_h */