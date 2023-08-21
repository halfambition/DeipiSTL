//in case of conflict with other libs, the DeipiSTL's macro definition is rear underline
#pragma once
#ifndef deipi_ALLOC_h
#define deipi_ALLOC_h

#include <malloc.h>

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
            if (ptr == nullptr) {
                throw "null pointer exception";
            }
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
    template <bool threads>
    class second_level_allocator {
    private:
        constexpr static short MAX_BYTE = 128;                          //max size of each block
        constexpr static short MIN_BYTE = 8;                            //min size of each block
        constexpr static short N_MAX_BLOCKS = MAX_BYTE / MIN_BYTE;      //block nums
        
        static char* start_mem_pool;
        static char* end_mem_pool;
        
        static size_t alloc_weight;                                 //weight increasing along with alloc times
        
    private:
        static inline size_t Round_UP(size_t bytes) {
            return ((bytes + MIN_BYTE - 1) & ~(MIN_BYTE - 1));      //memory every allocted will round into 8
        }
        
        //unit block in free-list, sizeof mem_block is equal to a pointer <= 8. client_data is VLA
        union mem_block{
            union mem_block* next_block;
            char client_data[1];
        };
        
        //memory chain combined by 16 blocks
        static volatile mem_block* mem_block_chain[N_MAX_BLOCKS];
        
        //calculate the index of chain
        static inline size_t Chain_Index(size_t size){
            // x / 8 is incorrect, because when x = 8, index = 1.
            return (((size + MIN_BYTE - 1) / MIN_BYTE) - 1);
        }
        
        //return obj which size is n.
        //might join other blocks into chain
        static void* Refill(size_t);
        //alloc a large memory block, which size is num_objs * n
        //num_objs is not a fixed value
        static char* Chunk_Alloc(size_t, int&);         //reference is necessary when num of blocks less than 20
        
    public:
        static void* Allocate(size_t);
        static void* Reallocate(void*, size_t, size_t);
        static void Deallocate(void*, size_t);
    };

    typedef second_level_allocator<0> Deipi_Alloc;
}


#endif
