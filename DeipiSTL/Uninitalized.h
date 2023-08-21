//
//  Uninitalized.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/5.
//
#pragma once
#ifndef deipi_UNINITALIZED_h
#define deipi_UNINITALIZED_h

#include "TypeTraits.h"
#include "Iterator.h"
#include "Construct.h"
#include <vcruntime_string.h>

namespace DeipiSTL {
    //if initalize failed, object must rollback
    //copy another object's data between first and last to ForwardIterator
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator Uninitialized_Copy(InputIterator first, InputIterator last, ForwardIterator output) {
        //get the type iterator pointing to, and if it is POD or NOT
        typedef typename __type_traits<iterator_traits<output>::value_type>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, output, is_POD());
    }
    
    namespace {
        template <typename InputIterator, typename ForwardIterator, typename value_type>
        inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator output, __true_type) {
            //return copy(first, last, output);
            return 0;
        }
        template <typename InputIterator, typename ForwardIterator, typename value_type>
        inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator output, __false_type) {
            ForwardIterator cur = output;
            try {
                for (; first != last; ++first, ++cur)
                    Construct(&*cur, *first);
            }
            catch (...) {
                Destory(output, cur);
            }
            return cur;
        }
    }
    //specialization version for char and wchar_t
    inline char* Uninitialized_Copy(const char* first, const char* last, char* output) {
        memmove(output, first, last - first);
        //memcpy safe version
        return output + (last - first);
    }

    //specialization version for char and wchar_t
    inline wchar_t* Uninitialized_Copy(const wchar_t* first, const wchar_t* last, wchar_t* output) {
        memmove(output, first, last - first);
        //memcpy safe version
        return output + (last - first);
    }


    /****************************************************************************/
    //copy and fill object between first and last with value of T
    template <typename ForwardIterator, typename T>
    inline void Uninitialized_Fill(ForwardIterator first, ForwardIterator last, const T& val) {
        //get the type iterator pointing to, and if it is POD or NOT
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_fill_aux(first, last, val, is_POD())
    }
    namespace {
        template <typename ForwardIterator, typename T, typename value_type>
        inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __true_type) {
            //fill(first, last, val);
        }
        template <typename ForwardIterator, typename T, typename value_type>
        inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __false_type) {
            ForwardIterator cur = first;
            try {
                for (; cur != last; ++cur)
                    Construct(&*cur, val);
            }
            catch (...) {
                Destory(first, cur);
            }
        }
    }


    /****************************************************************************/
    //copy and fill object from first to first + n with value of T
    template <typename ForwardIterator, typename T>
    inline ForwardIterator Uninitialized_Fill_N(ForwardIterator first, size_t n, const T& val) {
        //get the type iterator pointing to, and if it is POD or NOT
        typedef typename __type_traits<T>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, val, is_POD());
    }

    namespace {
        template <typename ForwardIterator, typename T>
        inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T& val, __true_type) {
            //return fill_n(first, n, val);
            return 0;   //waiting for update
        }
        template <typename ForwardIterator, typename T>
        inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T& val, __false_type) {
            ForwardIterator cur = first;
            //restore STL error handle
            try {
                for (; n > 0; --n, ++cur)
                    Construct(&*cur, x);
            }
            catch (...) {
                Destory(first, cur);
            }
            return cur;
        }
    }
}

#endif /* deipi_UNINITALIZED_h */