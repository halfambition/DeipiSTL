//
// Created by deipi on 2023/9/6.
//
#pragma once
#ifndef deipi_REVERSEITERATOR_h
#define deipi_REVERSEITERATOR_h

#include "Iterator.h"

namespace DeipiSTL{
    template <class Iterator>
    class reverse_iterator
    {
    protected:
        Iterator current; //forward iterator
    public:
        typedef typename iterator_traits<Iterator>::iterator_category
                iterator_category;
        typedef typename iterator_traits<Iterator>::value_type
                value_type;
        typedef typename iterator_traits<Iterator>::difference_type
                difference_type;
        typedef typename iterator_traits<Iterator>::pointer
                pointer;
        typedef typename iterator_traits<Iterator>::reference
                reference;

        typedef Iterator iterator_type;              //forward iterator
        typedef reverse_iterator<Iterator> self;     //reverse iterator

    public:
        reverse_iterator() = default;
        //connect forward iterator with reverse iterator
        explicit reverse_iterator(iterator_type x) : current(x) {}
        reverse_iterator(const self& x) : current(x.current) {}

        //return forward iterator
        iterator_type base() const { return current; }

        reference operator*() const {
            Iterator tmp = current;
            return *--tmp;
        }

        self& operator++() {
            --current;
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            --current;
            return tmp;
        }
        self& operator--() {
            ++current;
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            ++current;
            return tmp;
        }

        self operator+(difference_type n) const {
            return self(current - n);
        }
        self& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        self operator-(difference_type n) const {
            return self(current + n);
        }
        self& operator-=(difference_type n) {
            current += n;
            return *this;
        }
        reference operator[](difference_type n) const { return *(*this + n); }
    };
}


#endif //deipi_REVERSEITERATOR_h
