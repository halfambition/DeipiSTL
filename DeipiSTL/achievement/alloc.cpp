//
//  alloc.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/2.
//
#include "../alloc.h"


namespace deipiSTL {

    //default bad_alloc_handler
    void (*first_level_allocator::Bad_Alloc_Handler)() = 0;


    //out of memory allocator function body
    void* first_level_allocator::Bad_Allocate(const size_t size) {
        void* mem;
        //call bug handler
        do{
            //if never assign bad_alloc_handler, then throw exception
            if (Bad_Alloc_Handler == 0) {
                //exception
                throw "never assign bad_alloc_handler";
            }
            (*Bad_Alloc_Handler)();
            mem = malloc(size);             //try again
        } while (mem==nullptr);             //loop when mem is nullptr

        return(mem);
    }

    void* first_level_allocator::Bad_Reallocate(void* ptr, const size_t size) {
        void* mem;
        //call bug handler
        do {
            //if never assign bad_alloc_handler, then throw exception
            if (Bad_Alloc_Handler == 0) {
                //exception
                throw "never assign bad_alloc_handler";
            }
            (*Bad_Alloc_Handler)();
            mem = realloc(ptr, size);             //try again
        } while (mem == nullptr);             //loop when mem is nullptr

        return(mem);
    }


}
