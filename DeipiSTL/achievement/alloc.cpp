//
//  Alloc.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/2.
//
#include "../Alloc.h"
#include <memory>
//first level allocator
namespace DeipiSTL {

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
                throw "bad_alloc";
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
                throw "bad_alloc";
            }
            (*Bad_Alloc_Handler)();
            mem = realloc(ptr, size);             //try again
        } while (mem == nullptr);             //loop when mem is nullptr

        return(mem);
    }
}

//second level allocator
namespace DeipiSTL{
    
    //class initialization
    template<bool threads>
    char* second_level_allocator<threads>::start_mem_pool = 0;
    template<bool threads>
    char* second_level_allocator<threads>::end_mem_pool = 0;

    template<bool threads>
    size_t second_level_allocator<threads>::alloc_weight = 0;

    template<bool threads>
    volatile union second_level_allocator<threads>::mem_block*
    second_level_allocator<threads>::mem_block_chain[N_MAX_BLOCKS] =
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    //Allocate
    template<bool threads>
    void* second_level_allocator<threads>::Allocate(size_t n){
        if (n > MAX_BYTE)                          //more than 128, use first level alloctor
            return first_level_allocator::Allocate(n);
        
        mem_block* res = mem_block_chain[Chain_Index(n)];
        
        if (res == nullptr) {               //if block chain doesn't exist mem block, call refill
            res = Refill(Round_UP(n));
            return (void*)res;
        }
        //listnode operation
        mem_block_chain[Chain_Index(n)] = res->next_block;
        return (void*)res;
    }
    
    //Free
    template<bool threads>
    void second_level_allocator<threads>::Deallocate(void* ptr, size_t n){
        //if memory ptr pointint to is not multiples of 8, it will pollute mem pool
        return first_level_allocator::Deallocate(ptr);
    }
    
    //Reallocate
    template<bool threads>
    void* second_level_allocator<threads>::Reallocate(void* ptr, size_t old_size, size_t new_size){
        //I assume is use allocate a size of a new memory, then memcpy() the data, return the new address, and deallocate old pointer
        if (old_size > MAX_BYTE && new_size > MAX_BYTE) {
            return first_level_allocator::Reallocate(ptr, new_size);
        }

        if (old_size == new_size) {
            return ptr;
        }
        
        void* new_ptr = Allocate(new_size);
        size_t cpy_num = old_size < new_size ? old_size : new_size;
        memcpy(new_ptr, ptr, cpy_num);
        Deallocate(ptr, old_size);
        
        return new_ptr;
    }

    //chain maintenance
    //Refill will call Chunk_Alloc to get a lot of memory space blocks, and load them into mem_block_chain
    template<bool threads>
    void* second_level_allocator<threads>::Refill(size_t roundup_size){
        //default amount of block asked for pool is 20, return 1, and put others in chain
        int n_blocks = 20;
        
        //asking for pool
        char* chunk = Chunk_Alloc(roundup_size, n_blocks);
        
        if (n_blocks == 1) {
            return chunk;
        }
        
        //clip the chunk into chain table
        void* res = (void*)chunk;
        mem_block* next = (mem_block*)(chunk + roundup_size);
        mem_block_chain[Chain_Index(roundup_size)] = next;
        
        for(int i = 1; i < n_blocks - 1; ++i){
            next->next_block = (mem_block*)((char*)next + roundup_size);
            next = next->next_block;
        }
        next->next_block = nullptr;
        
        return res;
    }
    
    //Chunk_Alloc try to get memory space from memory pool (memory pool is allocated space between start_mem_pool and end_mem_pool)
    //if there is enough space, or space more than 1 at least, return directly
    //otherwise allocate from heap
    template<bool threads>
    char* second_level_allocator<threads>::Chunk_Alloc(size_t size, int& num_blocks){
        char* res;
        size_t remain = end_mem_pool - start_mem_pool;
        size_t requirement = num_blocks * size;
        size_t get_size = (requirement << 1) + Round_UP(alloc_weight >> 4);
        
        //insufficient pool space but more than one block, then return all rest space
        if (remain >= size && remain < requirement) {
            num_blocks = (int)remain / MIN_BYTE;
        }
        else if(remain < size){
            //simplify SGI STL, delete the step which join the rest pool into chain.
            //usage realloc in first_level directly
            start_mem_pool = (char*)first_level_allocator::Reallocate(start_mem_pool, get_size);
            end_mem_pool = start_mem_pool + get_size;
        }
        
        //enough pool space, then return directly
        res = start_mem_pool;
        
        requirement = num_blocks * size;
        start_mem_pool = start_mem_pool + (requirement);
        
        alloc_weight += get_size;
        return res;
    }
}
