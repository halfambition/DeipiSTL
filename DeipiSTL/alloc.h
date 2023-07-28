//in case of conflict with other libs, the DeipiSTL's macro definition is rear underline
#ifndef ALLOC_
#define ALLOC_

#include <cstdlib>

namespace deipiSTL {

    //STL SGI stand memory space allocator, devide malloc allocator to two levels
    //First level, is simple encapsulation of malloc, and join the set-new-handler to handling the fault of allocate
    class first_level_allocator {
    private:
        //out of memory handler
        static void* Bad_Allocate(const size_t);

        static void* Bad_Reallocate(void*, const size_t);

        //function pointer, users can assign themselves
        static void (*Bad_Alloc_Handler)();
    public:

        static inline void* Allocate(const size_t n) {
            void* mem = malloc(n);      //use malloc direct
            if (mem == nullptr)         //alloc fault
                mem = Bad_Allocate(n);
            return mem;
        }

        static inline void Deallocate(void* ptr) {
            free(ptr);
        }

        static inline void* Reallocate(void* ptr, const size_t n) {
            void* mem = realloc(ptr, n);        //use relloc direct
            if (mem == nullptr)
                mem = Bad_Reallocate(ptr, n);   //alloc fault
            return mem;
        }

        //use func ptr for return old handler-function
        static void (*Set_alloc_handler(void (*func)()))(){
            void (*old_func)() = Bad_Alloc_Handler;
            Bad_Alloc_Handler = func;
            return old_func;
        }
    };



    //Second level, maintainung a chain list named memory_pool which is the solution of memory request less than 128B
    //If memory request more than 128B, then use first level
    template <class threads>
    class second_level_allocator {
    private:
        const static short N_MAX_BLOCKS = 16;      //block nums
        const static short MAX_BYTE = 128;         //max size of each block
        const static short MIN_BYTE = 8;           //min size of each block
    private:
        static inline size_t round_off(size_t bytes) {
            return ((bytes + MIN_BYTE - 1) & ~(MIN_BYTE - 1));      //memory every allocted will round into 8
        }
    private:

    };
}


#endif
