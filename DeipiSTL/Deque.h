//
// Created by deipi on 2023/9/7.
//

#ifndef deipi_DEQUE_h
#define deipi_DEQUE_h
#include "Allocator.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Move.h"
#include "ReverseIterator.h"
#include "Algorithms.h"
namespace DeipiSTL{
    namespace {
        inline size_t __deque_buf_size(size_t n, size_t sz){
            return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
        }
    }

    template <class T, class Ref, class Ptr, size_t BufSiz> //搞这么多主要是为了const迭代器
    struct deque_iterator {
        typedef deque_iterator<T, T&, T*, BufSiz>             iterator;
        typedef deque_iterator<T, const T&, const T*, BufSiz> const_iterator;

        typedef DeipiSTL::random_access_iterator_tag    iterator_category; // (1)
        typedef T                                       value_type;        // (2)
        typedef Ptr                                     pointer;           // (3)
        typedef Ref                                     reference;       // (4)
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;     // (5)
        typedef T**                                     map_pointer;

    protected:
        //保持与容器的联结
        pointer cur;
        pointer first;         //buf head
        pointer last;          //buf tail
        map_pointer node; //pointing to map
        void set_node(map_pointer _node){
            this->node = _node;
            first = *_node;
            last = first + buffer_size();
            //do not set cur
        }
    public:
        deque_iterator(T* x, map_pointer y):cur(x), first(*y), last(*y + buffer_size()), node(y) { }
        deque_iterator() : cur(0), first(0), last(0), node(0) { }
        explicit deque_iterator(const iterator& x):cur(x.cur), first(x.first), last(x.last), node(x.node) { }
        iterator operator=(const iterator& x){
            this->cur = x.cur;
            this->node = x.node;
            this->first = x.first;
            this->last = x.last;
        }
        static size_t buffer_size(){
            return __deque_buf_size(BufSiz, sizeof(T));
        }
        reference operator*()const{
            return *cur;
        }
        pointer operator->()const{
            return cur;
        }
        iterator operator++(){
            ++cur;
            if(cur == last){
                set_node(node+1);
                cur = first;
            }
            return *this;
        }
        iterator operator++(int){
            iterator temp(*this);
            ++cur;
            operator++();
            return cur;
        }
        iterator operator--(){
            if(cur == first) {
                set_node(node - 1);
                cur = last - 1;
            }
            else
                --cur;
            return *this;
        }
        iterator operator--(int){
            iterator temp(*this);
            operator--();
            return temp;
        }
        bool operator<(iterator it) const{
            return (node == it.node) ? (cur < it.cur) : (node < it.node);
        }
        bool operator<=(iterator it) const{
            return (node == it.node) ? (cur <= it.cur) : (node < it.node);
        }
        bool operator>(iterator it){
            return (node == it.node) ? (cur > it.cur) : (node > it.node);
        }
        bool operator>=(iterator it){
            return (node == it.node) ? (cur >= it.cur) : (node > it.node);
        }
        bool operator==(iterator it){
            return cur == it.cur;
        }
        bool operator!=(iterator it){
            return cur != it.cur;
        }
        difference_type operator-(iterator it){
            difference_type diff_node = DeipiSTL::ABS(this->node - it.node) - 1;
            diff_node * buffer_size();
        }
    };

    template <class T, class Alloc = DeipiSTL::deipi_Alloc, size_t BufSiz = 0>
    class deque{
    public:
        //type_traits
        typedef T				value_type;
        typedef T*				pointer;
        typedef const T*		const_pointer;
        typedef T&				reference;
        typedef const T&		const_reference;
        typedef size_t		    size_type;
        typedef ptrdiff_t	    difference_type;
        //about iterator
        typedef deque_iterator<T, T&, T*,BufSiz>                iterator;
        typedef deque_iterator<T, const T&, const T*,BufSiz>    const_iterator;
    protected:  //Internal typedefs
        //controller
        typedef pointer*        map_pointer;
        //专属的空间分配器，每次分配一个元素大小
        typedef allocator<value_type,Alloc>     data_allocator;
        //专属的空间分配器，每次分配一个指针大小
        typedef allocator<pointer,Alloc>        map_allocator;
    protected:  //Data members
        iterator start;
        iterator finish;

        map_pointer map;
        size_type map_size;

    };

}


#endif //deipi_DEQUE_h
