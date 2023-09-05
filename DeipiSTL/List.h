//
// Created by deipi on 2023/9/5.
//

#ifndef deipi_LIST_h
#define deipi_LIST_h

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"

namespace DeipiSTL{
    namespace {
        template <typename T>
        struct _list_node{
            //type_traits
            typedef T				value_type;
            typedef _list_node*     node_ptr;

            node_ptr _next;
            node_ptr _prev;
            value_type _data;

            _list_node():_next(nullptr), _prev(nullptr), _data(){}
            explicit _list_node(value_type data):_next(nullptr), _prev(nullptr), _data(data){}
            _list_node(value_type data, node_ptr next):_next(next), _prev(nullptr), _data(data){}
            _list_node(node_ptr prev, value_type data):_next(nullptr), _prev(prev), _data(data){}
            _list_node(node_ptr prev, value_type data, node_ptr next):_next(next), _prev(prev), _data(data){}
        };

        //list iterator
        template <typename T>
        struct _list_iterator{
            //typedef can be inherited and overwrite
            typedef size_t size_type;
            typedef _list_node<T>* link_ptr_type;

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
    }



}

#endif //deipi_LIST_h
