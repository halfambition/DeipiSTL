//
//  Memory.h
//  DeipiSTL
//
//  Created by deipi on 2023/8/29.
//
#pragma once
#ifndef deipi_MEMORY_h
#define deipi_MEMORY_h

#include "Allocator.h"
#include "Construct.h"

namespace DeipiSTL {
	template<typename T>
	struct default_delete {
		void operator()(T* ptr) {
			delete ptr;
		}
	};
	template<typename T>
	struct default_delete <T[]> {
		void operator()(T* ptr) {
			delete[] ptr;
		}
	};
}

namespace DeipiSTL {
	template<typename T, typename D = default_delete<T>>
	class unique_ptr {
	public:
		//type_traits
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t		    size_type;
		typedef D		delete_type;
		//about iterator
	private:
		pointer _ptr;
		delete_type _deleter;
	public:
		//abort
		unique_ptr() = delete;
		unique_ptr(unique_ptr&) = delete;
		unique_ptr<value_type, delete_type>& operator=(const unique_ptr&) = delete;

		//only init through passing param ptr
		explicit unique_ptr(pointer ptr = nullptr) : _ptr(ptr){}
		unique_ptr(pointer ptr, delete_type deleter) : _ptr(ptr), _del(deleter) { }
		~unique_ptr() {
			deleter(_ptr);
		}
		unique_ptr(unique_ptr&& new_u_ptr) : _ptr(nullptr){
			deleter(_ptr);
			~_deleter();
			_ptr = new_u_ptr._ptr;
			_deleter = new_u_ptr._deleter;
		}
		unique_ptr<value_type, delete_type>& operator=(unique_ptr&& new_u_ptr) {
			if(&new_u_ptr != this){
				deleter(_ptr);
				~_deleter();
				_ptr = new_u_ptr._ptr;
				_deleter = new_u_ptr._deleter;
			}
			return *this;
		}
	public:
		//interface operator overload
		value_type operator*() {
			return *_ptr;
		}
		const_reference operator*() const {
			return *_ptr;
		}

		pointer operator->() {
			return _ptr;
		}
		const_pointer operator->() const {
			return _ptr;
		}
		//this object can be judgement statement directly
		bool operator bool()const {
			return _ptr != nullptr;
		}

		reference operator[](size_type i) {
			return _ptr[i];
		}
		const_reference operator[](size_type i)const {
			return _ptr[i];
		}

		unique_ptr<value_type, delete_type>& operator++() {
			++_ptr;
			return this;
		}

		//normal interface
		pointer get() {
			return _ptr;
		}
		const_pointer get()const {
			return _ptr;
		}

		delete_type& get_deleter() {
			return _deleter;
		}
		const delete_type& get_deleter()const {
			return _deleter;
		}

		void release() {
			_deleter(_ptr);
			_ptr = nullptr;
		}

		void reset(pointer new_ptr) {
			_deleter(_ptr);
			_ptr = new_ptr;
		}

		void swap(unique_ptr& new_obj) {
			pointer temp_ptr = _ptr;
			delete_type& temp_deleter = _deleter;
			_ptr = new_obj.get();
			_deleter = new_obj.get_deleter();
			new_obj._ptr = temp_ptr;
			new_obj._deleter = temp_deleter;
		}

	}; 
	template <typename T, typename D>
	void swap(unique_ptr<T, D>& a, unique_ptr<T, D>& b) {
		a.swap(b);
	}
	
	template <typename T1, typename D1, typename T2, typename D2>
	bool operator==(unique_ptr<T1, D1>& a, unique_ptr<T2, D2>& b) {
		return a.get() == b.get();
	}

	template <typename T, typename D>
	bool operator==(unique_ptr<T, D>& a, nullptr_t b) {
		return a.get() == b;
	}

	template <typename T, typename D>
	bool operator==(nullptr_t b, unique_ptr<T, D>& a) {
		return a.get() == b;
	}

	template <typename T1, typename D1, typename T2, typename D2>
	bool operator!=(unique_ptr<T1, D1>& a, unique_ptr<T2, D2>& b) {
		return a.get() != b.get();
	}

	template <typename T, typename D>
	bool operator!=(unique_ptr<T, D>& a, nullptr_t b) {
		return a.get() != b;
	}

	template <typename T, typename D>
	bool operator!=(nullptr_t b, unique_ptr<T, D>& a) {
		return a.get() != b;
	}
}



namespace DeipiSTL {
	template<typename T, typename Alloc = allocator<T, deipi_Alloc>>
	class shared_ptr {

	};

	template<typename T, typename Alloc = allocator<T, deipi_Alloc>>
	class weak_ptr {

	};
}

#endif	/*deipi_VECTOR_h*/