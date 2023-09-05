//
//  Iterator.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/8.
//
#pragma once
#ifndef deipi_ITERATOR_h
#define deipi_ITERATOR_h
#include <stddef.h>      //ptrdiff_t's head file


//traits of iterator, they are used for label of overload
namespace DeipiSTL {
    //iterator classification label
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag, output_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
}

//Really Iterator definition
namespace DeipiSTL {
    //Base Iterator, any iterator-user-defined should inherit this class
    //user-defined only need Category and value_type, Distance Pointer and Reference is default
    template<typename Category,
             typename T,
             typename Distance = ptrdiff_t,
             typename Pointer = T*,
             typename Reference = T&>
    struct iterator {
        typedef Category       iterator_category;
        typedef T              value_type;         //object Iterator point to
        typedef Distance       difference_type;    //the distance between two Iterator
        typedef Pointer        pointer;            //A pointer of value_type
        typedef Reference      reference;          //A reference of value_type
    };
    //sample of define iterator
    /*template <class Item>
    struct ListIter: public std::iterator<std::forward_iterator_tag, Item> {
        ...
    };*/

    //five Base-Iterator definitions
    template <typename T,
              typename Distance = ptrdiff_t>
    struct input_iterator {
        typedef input_iterator_tag      iterator_category;
        typedef T                       value_type;         //object Iterator point to
        typedef Distance                difference_type;    //the distance between two Iterator
        typedef T*                      pointer;            //A pointer of value_type
        typedef T&                      reference;          //A reference of value_type
    };

    struct output_iterator {        //these traits are no need in output iterator
        typedef output_iterator_tag     iterator_category;
        typedef void                    value_type;         //object Iterator point to
        typedef void                    difference_type;    //the distance between two Iterator
        typedef void                    pointer;            //A pointer of value_type
        typedef void                    reference;          //A reference of value_type
    };

    template <typename T,
              typename Distance = ptrdiff_t>
    struct forward_iterator {
        typedef forward_iterator_tag    iterator_category;
        typedef T                       value_type;         //object Iterator point to
        typedef Distance                difference_type;    //the distance between two Iterator
        typedef T*                      pointer;            //A pointer of value_type
        typedef T&                      reference;          //A reference of value_type
    };

    template <typename T,
              typename Distance = ptrdiff_t>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag      iterator_category;
        typedef T                               value_type;         //object Iterator point to
        typedef Distance                        difference_type;    //the distance between two Iterator
        typedef T*                              pointer;            //A pointer of value_type
        typedef T&                              reference;          //A reference of value_type
    };

    template <typename T,
              typename Distance = ptrdiff_t>
    struct random_access_iterator {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;         //object Iterator point to
        typedef Distance                        difference_type;    //the distance between two Iterator
        typedef T*                              pointer;            //A pointer of value_type
        typedef T&                              reference;          //A reference of value_type
    };
}

//iterator trait class
namespace DeipiSTL {
    //iterator for objects
    template <typename Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;         //object Iterator point to
        typedef typename Iterator::difference_type      difference_type;    //the distance between two Iterator
        typedef typename Iterator::pointer              pointer;            //A pointer of value_type
        typedef typename Iterator::reference            reference;          //A reference of value_type
    };

    //specialization of original pointer
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
    };

    //specialization of const original pointers
    template <typename T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
    };
}

//some trait extractor function
namespace DeipiSTL {
    //category extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    Iterator_Category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();      //return the object of target type 
    }

    //difference_type extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type
    Difference_Type(const Iterator&) {
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        return difference_type();      //return the object of target type
    }

    //value_type extraction function
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type
    Value_Type(const Iterator&) {
        typedef typename iterator_traits<Iterator>::value_type value_type;
        return value_type();      //return the object of target type
    }
    //no need for pointer_type and reference_type
}

namespace DeipiSTL {
    //Iterator advance
    namespace {
        //anonymous
        template <typename Iterator,
                  typename Distance>
        inline void __advance(Iterator& i, Distance n, input_iterator_tag)
        //this parameter only uses to distinguish the override
        {
            while (n--) ++i;
        }

        template <typename Iterator,
                  typename Distance>
        inline void __advance(Iterator& i, Distance n, bidirectional_iterator_tag)
        //this parameter only uses to distinguish the override
        {
            if (n >= 0)
                while (n--) ++i;
            else
                while (n++) --i;
        }

        template <typename Iterator,
                  typename Distance>
        inline void __advance(Iterator& i, Distance n, random_access_iterator_tag)
        //this parameter only uses to distinguish the override
        {
            i += n;
        }
    }

    template <typename Iterator,
              typename Distance>
    inline void Advance(Iterator& i, Distance n){
        __advance(i, n, Iterator_Category(i));
    }
}

namespace DeipiSTL {
    //Iterator distance
    namespace {

        template <typename Iterator>
        inline typename iterator_traits<Iterator>::difference_type
        __distance(Iterator first, Iterator last, input_iterator_tag) {
            typename iterator_traits<Iterator>::difference_type n = 0;
            while (first != last) {
                ++first;
                ++n;
            }
            return n;
        }

        template <typename Iterator>
        inline typename iterator_traits<Iterator>::difference_type
        __distance(Iterator first, Iterator last, random_access_iterator_tag) {
            return last - first;
        }
    }

    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type
    Distance(Iterator first, Iterator last) {
        //face to outside, iterator parameter must be in order
        return __distance(first, last, Iterator_Category(first));
    }
}
#endif /* deipi_ITERATOR_h */
