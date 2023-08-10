//
//  Iterator.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/8.
//

#ifndef ITERATOR_H
#define ITERATOR_H
#include <cstddef>


//traits of iterator
namespace DeipiSTL {

    //iterator classification label
    

    //iterator for object
    template <typename Iterator>
    struct iterator_traits{
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;         //object Iterator point to
        typedef typename Iterator::difference_type      difference_type;    //the distence between two Iterator
        typedef typename Iterator::pointer              pointer;            //A pointer of value_type
        typedef typename Iterator::reference            reference;          //A reference of value_type
    };

    //specialization of original pointer
    template <class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    //specialization of const original pointer
    template <typename T>
    struct iterator_traits<const T*>{
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    //category extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();      //extract category of iterator
    }

    //difference_type extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type
    difference_type(const Iterator&){
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        return difference_type();      //extract difference_type of iterator
    }

    //value_type extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type
    value_type(const Iterator&){
        typedef typename iterator_traits<Iterator>::value_type value_type;
        return value_type();      //extract value_type of iterator
    }
    //no need for pointer_type and reference_type
}

#endif /* Iterator_h */
