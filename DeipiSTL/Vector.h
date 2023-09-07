//
//  Vector.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/21.
//
#pragma once
#ifndef deipi_VECTOR_h
#define deipi_VECTOR_h

#include "Allocator.h"
#include "Uninitalized.h"
#include <ostream>
#include "ReverseIterator.h"

namespace DeipiSTL {
	template <typename T, typename Alloc = allocator<T, deipi_Alloc>>
	class vector {
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
		typedef T*				iterator;
		typedef const T*		const_iterator;

	protected:
		typedef Alloc           data_allocator;	//default space allocator
		iterator start;				//head of usage
		iterator finish;			//end of usage
		iterator end_of_storage;	//end of all space

	protected:
		//inner aux
		void allocate_and_fill(size_type n, const T& value);
		void allocate_and_copy(const vector& vec);
		void allocate_and_move(vector& vec);
		void deallocate();
		iterator realloc_aux(size_type new_capacity);
		void grow_capacity(iterator& new_start, size_type& space_need);
		iterator insert_aux(iterator position, size_type insert_size);
		iterator insert_aux(iterator position, size_type insert_size, iterator tar);
		iterator __insert(iterator pos, size_type n);
		bool is_full();
	public:
		//constructor and destructor
		vector():start(nullptr), finish(nullptr), end_of_storage(nullptr){ };
		explicit vector(size_type n) {
			allocate_and_fill(n, T());
		}
		vector(size_type n, const T& value) {
			allocate_and_fill(n, value);
		}
		template <typename InputIterator>
		vector(InputIterator first, InputIterator last);
		//copy constructor
		vector(const vector& vec) :start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_copy(vec);
		}
		//move constructor
		vector(vector&& vec)  noexcept :start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_move(vec);
		}
		//destructor
		virtual ~vector();

		//operator overload
		vector<T, data_allocator>& operator=(const vector& vec);
		vector<T, data_allocator>& operator=(vector&& vec) noexcept ;

	public:
		//all bellow function bodies are in Vector.cpp
		//return element or iterator
		iterator begin() {
			return start;
		}
		iterator end() {
			return finish;
		}

		iterator rbegin(){
            return reverse_iterator<iterator>(end());
        }
		iterator rend(){
            return reverse_iterator<iterator>(begin());
        }

		const_iterator cbegin() const {
			return start;
		}
		const_iterator cend() const {
			return finish;
		}
		T& at(const size_type pos) {
			if (start + pos >= finish)
				throw "out of range";
			return *(start + pos);
		}
		T& operator[](const size_type pos) {
			return *(start + pos);
		}
		T& front() {
			return *start;
		}
		T& back() {
			return *(finish - 1);
		}
		T* data() {
			return start;
		}
	public:
		//insert or delete element
		void push_back(const T& val);
		void pop_back();
		iterator emplace(iterator pos, const T& val);
		void emplace_back(const T& val);
		iterator insert(iterator pos, const T& val);
		iterator insert(iterator pos, const T& val, size_type& n);
		template <typename InputIterator>
		iterator insert(iterator pos, InputIterator first, InputIterator last);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator second);
		void clear() {
			DeipiSTL::Destroy(start, finish);
			finish = start;
		}
		void swap(vector& vec);

	public:
		//about space
		size_type size() const {
			return finish - start;
		}
		size_type capacity() const {
			return end_of_storage - start;
		}
		bool empty() const {
			return start == finish;
		}
		void reserve(size_type n);
		void resize(size_type new_size);
		void resize(size_type new_size, const T& val);
		void shrink_to_fit();
	};
}

#endif	/*deipi_VECTOR_h*/