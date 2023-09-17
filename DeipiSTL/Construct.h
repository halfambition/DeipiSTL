//
//  Construct.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/5.
//
#pragma once
#ifndef deipi_CONSTRUCT_h
#define deipi_CONSTRUCT_h

#include <new>    //placement new
#include "TypeTraits.h"
#include "Move.h"

namespace DeipiSTL {
    //construct()
    //change use universal reference
    template <typename T1, typename T2>
    inline void Construct(T1* p, T2&& t2) {
        if (p == nullptr)
            throw "nullptr exception";
        new(p) T1(forward<remove_cv<T2>>(t2));       //transfer T2 to T1, call T1's constructor
    }

    //destroy()
    template <typename T>
    inline void Destroy(T* p) {     //destroy one obj
        p->~T();
    }

    namespace {
        //if value_type contains trivial non-destructor
        template <typename ForwardIterator>
        inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {     //destroy one obj
            for (; first < last; ++first)
                DeipiSTL::Destroy(&*first);
            //'&' is get address symbol, not reference
            //first is iterator not obj
            //so '&*first' is get the address of object which iterator is pointing
        }

        //else if value_type contains trivial destructor
        template <typename ForwardIterator>
        inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
        { }

        //judge if value_type contains trivial destructor
        /*what's trivial destructor, it's a type traitor
            judge if call destructor based on an object type*/
        template <typename ForwardIterator, typename T>
        inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {     //destroy one obj
            typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
            __destroy_aux(first, last, trivial_destructor());
        }
    //end anonymous namespace
    }
    //destroy()
    template <typename ForwardIterator>
    inline void Destroy(ForwardIterator first, ForwardIterator last) {     //destroy one obj
        __destroy(first, last, &*first);
    }

    //specialization destroys for char and wchar_t
    inline void Destroy(char*, char*){ }
    inline void Destroy(wchar_t*, wchar_t*) { }
}

#endif /* deipi_CONSTRUCT_h */
