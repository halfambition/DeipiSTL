//
// Created by deipi on 2023/9/5.
//

#ifndef deipi_LIST_h
#define deipi_LIST_h

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "ReverseIterator.h"

namespace DeipiSTL{
    namespace {
        template<typename T>
        struct _list_node {
            //type_traits
            typedef T value_type;
            typedef _list_node *node_ptr;

            node_ptr _next;
            node_ptr _prev;
            value_type _data;

            _list_node() : _next(nullptr), _prev(nullptr), _data() {}

            explicit _list_node(value_type data) : _next(nullptr), _prev(nullptr), _data(data) {}

            _list_node(value_type data, node_ptr next) : _next(next), _prev(nullptr), _data(data) {}

            _list_node(node_ptr prev, value_type data) : _next(nullptr), _prev(prev), _data(data) {}

            _list_node(node_ptr prev, value_type data, node_ptr next) : _next(next), _prev(prev), _data(data) {}
        };
    }

    //list iterator
    template <typename T>
    struct _list_iterator{
        //typedef can be inherited and overwrite
        typedef size_t                      size_type;
        typedef _list_node<T>*              link_ptr_type;

        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;         //object Iterator point to
        typedef size_t                      difference_type;    //the distance between two Iterator
        typedef T*                          pointer;            //A pointer of value_type
        typedef T&                          reference;          //A reference of value_type

        link_ptr_type _node;

        _list_iterator() : _node(nullptr) {}
        explicit _list_iterator(link_ptr_type node) : _node(node){}
        _list_iterator(const _list_iterator& iter) : _node(iter._node) {}
        _list_iterator<T>& operator=(const _list_iterator& iter) {
            this->_node = iter._node;
            return *this;
        }

        bool operator==(const _list_iterator& iter) const {
            return iter._node == this->_node;
        }
        bool operator!=(const _list_iterator& iter) const {
            return iter._node != this->_node;
        }
        reference operator*() const {
            return _node->_data;
        }
        _list_iterator<T>& operator++(){    //i++
            _node = _node->_next;
            return *this;
        }
        _list_iterator<T> operator++(int){ //++i
            _list_iterator<T> temp(_node->_next);
            _node = _node->_next;
            return temp;
        }
        _list_iterator<T>& operator--(){    //i++
            _node = _node->_prev;
            return *this;
        }
        _list_iterator<T> operator--(int){ //++i
            _list_iterator<T> temp(_node->_prev);
            _node = _node->_prev;
            return temp;
        }
    };

    template <typename T, typename Alloc = allocator<T, deipi_Alloc>>
    class list{
    public:
        //type_traits
        typedef T				        value_type;
        typedef T*				        pointer;
        typedef const T*		        const_pointer;
        typedef T&				        reference;
        typedef const T&		        const_reference;
        typedef size_t		            size_type;
        typedef ptrdiff_t	            difference_type;
        //about iterator
        typedef _list_iterator<T>	    iterator;
        typedef const _list_iterator<T>	const_iterator;
        typedef _list_node<T>           node_type;
        typedef const _list_node<T>     const_node_type;
        typedef _list_node<T>*          node_ptr;
        typedef const _list_node<T>*    const_node_ptr;
    private:
        typedef allocator<node_type, deipi_Alloc>  data_allocator;	//default space allocator
        node_ptr _node;
        size_type _size;
        //list is a bidirectional list table，_node is head node, also is end node.
    public:
        list() : _size(0){
            _node->_prev = _node;
            _node->_next = _node;
        }
        list(const list& l) = delete;
        list(list&& l) = delete;
        list<T, Alloc>& operator=(const list& l) = delete;
        list<T, Alloc>& operator=(list&& l) noexcept {
            _node = l._node;
            _size = l._size;
            return *this;
        }
        ~list(){
            clear();
        }
    private:
        node_ptr alloc_node(){
            return data_allocator::Allocate();
        }
        void dealloc_node(node_ptr p){
            data_allocator::Deallocate(p);
        }
        node_ptr create_node(const T& val) {
            node_ptr ptr = alloc_node();
            Construct(ptr, val);
            return ptr;
        }
        void destroy_node(iterator& iter){
            --_size;
            Destroy(iter._node);
            dealloc_node(iter._node);
        }
        void erase_aux(node_ptr ptr){
            if (_node == nullptr){
                throw "null ptr";
                return;
            }
            ptr->_prev->_next = ptr->_next;
            ptr->_next->_prev = ptr->_prev;
            destroy_node(ptr);
        }
        void insert_back_aux(node_ptr p, const value_type& val){
            node_ptr temp = create_node(val);
            temp->_next = p->_next;
            temp->_prev = p;
            p->_next = temp;
            temp->_next->_prev = temp;
            ++_size;
        }
    public:
        iterator begin(){
            return (iterator)_node->_next;
        }
        iterator end(){
            return (iterator)_node;
        }
        const_iterator cbegin() const {
            return (iterator)_node->_next;
        }
        const_iterator cend() const {
            return (iterator)_node;
        }
        reverse_iterator<iterator> rbegin() const {
            return reverse_iterator<iterator>(end());
        }
        reverse_iterator<iterator> rend() const {
            return reverse_iterator<iterator>(begin());
        }
        size_type size() const {
            return _size;
        }
        void push_back(const value_type& val){
            insert_back_aux(_node->_prev, val);
        }
        void push_front(const value_type& val){
            insert_back_aux(_node, val);
        }
        void pop_back(){
            erase_aux(_node->_prev);
        }
        void pop_front(){
            erase_aux(_node->_next);
        }
        void insert(iterator iter, const T& val){
            iter._node != nullptr;
            insert_back_aux(iter._node, val);
        }
        void erase(iterator iter){
            erase_aux(iter);
        }
        void clear() {
            Destroy(begin(), end());
            data_allocator::Deallocate(_node->_next, _size);
            _size = 0;
        }
        template <typename X, typename A>
         friend list<X, A> shallow_copy(const list<X, A>& l);
    };
    template <typename T, typename Alloc>
    list<T, Alloc> shallow_copy(const list<T, Alloc>& l){
        list<T, Alloc> new_l;
        new_l._node = l._node;
        new_l._size = l._size;
        return new_l;
    }
    template <typename T, typename Alloc>
    list<T, Alloc> deep_copy(const list<T, Alloc>& l){
        list<T, Alloc> new_l;
        for(auto it = l.cbegin(); it!=l.cend(); ++it){
            new_l.push_back(it._node->_data);
        }
        return new_l;
    }
    template <typename T, typename Alloc>
    list<T, Alloc>& splice(const list<T, Alloc>& l1, const list<T, Alloc>& l2){
        //connect l1 and l2, this function would abort l2;
        typedef typename list<T, Alloc>::iterator iterator;
        iterator l1b = l1.cbegin();
        iterator l1e = l1.cend();
        iterator l2b = l2.cbegin();
        iterator l2e = l2.cend();

        l1e._node->_prev->_next = l2b._node;
        l2b._node->_prev = l1e._node->_prev;
        l1e._node->_prev = l2e._node->_prev;
        l2e._node->_prev->_next = l1e._node;

        l2e._node->_next = l2e._node;
        l2e._node->_prev = l2e._node;
        return l1;
    }
}

#endif //deipi_LIST_h
