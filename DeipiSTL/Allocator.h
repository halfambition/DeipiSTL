#pragma once
//this head file is simple encapsulation of Alloc
//all external user use alloc through this interface
#ifndef ALLOCATOR_h
#define ALLOCATOR_h

#include "Alloc.h"
#include "Construct.h"

namespace DeipiSTL {
    //define default allocator
    typedef second_level_allocator<0> deipi_Alloc;

    //briefly, this interface is allocate space through number of unit
    template <typename T, typename Alloc>
    class allocator {
    public:
        typedef T			    value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t		    size_type;
        typedef ptrdiff_t	    difference_type;

    public:
        static T* Allocate() {
            return static_cast<T*>(Alloc::Allocate(sizeof(T)));
        }
        static T* Allocate(size_t n) {
            return n == 0 ? 0 : static_cast<T*>(Alloc::Allocate(n * sizeof(T)));
        }
        static void Deallocate(T* p) {
            Alloc::Deallocate(p, sizeof(T));
        }
        static void Deallocate(T* p, size_t n) {
            if (n != 0)
                Alloc::Deallocate(p, n * sizeof(T));
        }
    };
}

#endif
