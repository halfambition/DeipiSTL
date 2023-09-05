//
//  String.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/26.
//
#pragma once
#ifndef deipi_STRING_h
#define deipi_STRING_h

#include "Allocator.h"
#include "Uninitalized.h"
#include <string.h>

namespace DeipiSTL {
	template <typename Char, typename Alloc = allocator<Char, deipi_Alloc>>
	class basic_string {
	public:
		//traits
		//type_traits
		typedef Char				value_type;
		typedef Char*				pointer;
		typedef const Char*			const_pointer;
		typedef Char&				reference;
		typedef const Char&			const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		//about iterator
		typedef Char*				iterator;
		typedef const Char*			const_iterator;

		constexpr static size_type npos = -1;
	protected:
		typedef Alloc data_allocator;	//default space allocator
		iterator start;				//head of usage
		iterator finish;			//end of usage
		iterator end_of_storage;	//end of all space

	protected:
		//inner aux
		void allocate_and_fill(Char c, size_type n);
		template <typename InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		void allocate_and_move(basic_string& str);
		void deallocate();
		iterator realloc_aux(size_type new_capacity);
		void grow_capacity(iterator new_start, size_type& space_need);
		iterator insert_aux(iterator position, size_type insert_size);
		iterator insert_aux(iterator position, size_type insert_size, iterator tar);
		iterator __insert(iterator pos, size_type n);
		bool is_full();

	public:
		//constructor and destructor
		basic_string() : start(nullptr), finish(nullptr), end_of_storage(nullptr) { };
		basic_string(const char* str) : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_copy(str, str + strlen(str));
		}
		basic_string(const char* str, size_type n) : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_copy(str, str + n);
		}
		basic_string(const char c, size_type n) {
			allocate_and_fill(c, n);
		}
		basic_string(const basic_string& str, size_type pos, size_type len = npos) : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			len = len == -1 ? str.size() - pos : len;
			allocate_and_copy(str.begin() + pos, len);
		}
		template <typename InputIterator>
		basic_string(InputIterator first, InputIterator last) : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_copy(first, last);
		}
		//copy constructor
		basic_string(const basic_string& str) : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_copy(str.start, str.finish);
		}
		//move constructor
		basic_string(basic_string&& str)  noexcept : start(nullptr), finish(nullptr), end_of_storage(nullptr) {
			allocate_and_move(str.start, str.finish);
		}
		//destructor
		virtual ~basic_string();

		//operator overload
		basic_string<Char, data_allocator>& operator=(const basic_string& str) {
			allocate_and_copy(str.start, str.finish);
			return *this;
		}
		basic_string<Char, data_allocator>& operator=(basic_string&& str)  noexcept {
			allocate_and_move(str.start, str.finish);
			return *this;
		}
		basic_string<Char, data_allocator>& operator=(const char* str) {
			allocate_and_copy(str, str + strlen(str));
			return *this;
		}

	public:
		//all bellow function bodies are in Vector.cpp
		//return element or iterator
		iterator begin() {
			return start;
		}
		iterator end() {
			return finish;
		}

//		iterator rbegin();
//		iterator rend();

		const_iterator cbegin() const {
			return start;
		}
		const_iterator cend() const {
			return finish;
		}
		reference at(const size_type pos) {
			if (start + pos >= finish)
				throw "out of range";
			return *(start + pos);
		}
		reference operator[](const size_type pos) {
			return *(start + pos);
		}
		const_reference operator[](const size_type pos) const {
			return *(start + pos);
		}
		reference front() {
			return *start;
		}
		const_reference cfront() const {
			return *start;
		}
		reference back() {
			return *(finish - 1);
		}
		const_reference cback() const {
			return *(finish - 1);
		}
		pointer data() {
			return start;
		}
		//about space
		size_type size() const {
			return finish - start;
		}
		size_type length() const {
			return size();
		}
		size_type capacity() const {
			return end_of_storage - start;
		}
		bool empty() const {
			return start == finish;
		}
		void reserve(size_type new_size);
		void resize(size_type new_size);
		void resize(size_type new_size, const_reference c);
		void shrink_to_fit();

		//insert or delete element
//		basic_string<Char, data_allocator>& insert(iterator pos, const_reference c);
//		basic_string<Char, data_allocator>& insert(iterator pos, const_reference c, size_type n);
//		template <typename InputIterator>
//		basic_string<Char, data_allocator>& insert(iterator pos, InputIterator first, InputIterator last);
//		basic_string<Char, data_allocator>& erase(iterator pos);
//		basic_string<Char, data_allocator>& erase(iterator first, iterator second);
		void clear() {
			DeipiSTL::Destroy(start, finish);
			finish = start;
		}
	};
	using string = basic_string<char>;
}

#endif