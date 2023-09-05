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
#include "Move.h"
#include "Algorithms.h"
#include "TypeTraits.h"

namespace DeipiSTL {
	//use tiny stl code
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
		typedef D				delete_type;
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
		unique_ptr(pointer ptr, delete_type deleter) : _ptr(ptr), _deleter(deleter) { }
		~unique_ptr() {
			deleter(_ptr);
		}
		unique_ptr(unique_ptr&& new_u_ptr)  noexcept : _ptr(nullptr){
			deleter(_ptr);
			~_deleter();
			_ptr = new_u_ptr._ptr;
			_deleter = new_u_ptr._deleter;
		}
		unique_ptr<value_type, delete_type>& operator=(unique_ptr&& new_u_ptr)  noexcept {
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
		//this object can be a judgment statement directly
		explicit operator bool()const {
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

		pointer release() {
			//release does not delete the pointing target
			pointer p = _ptr;
			_ptr = nullptr;
			return p;
		}

		void reset(pointer new_ptr) {
			_deleter(_ptr);
			_ptr = new_ptr;
		}

		void swap(unique_ptr& new_obj) {
			DeipiSTL::Swap(_ptr, new_obj._ptr);
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
	template<typename T, typename D = default_delete<T>>
	class shared_ptr {
	public:
		//type_traits
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t		    size_type;
		typedef D				delete_type;
        typedef size_t          ref_type;

	private:
        //usage for allocate_shared to alloc whole memory
        struct all_data_tag{
            value_type v;
            size_type s;
        };

    private:
		pointer _ptr;
		delete_type _deleter;
		size_type* ref_count;
	private:
		void reduce_ref_count() {
			if (--(*ref_count) == 0) {
				_deleter(_ptr);
				_deleter(ref_count);
				_ptr = nullptr;
				ref_count = nullptr;
			}
		}
		void copy_ref_count(size_type* new_ref_count) {
			if (ref_count == nullptr)
				ref_count = new size_type(0);
			ref_count = new_ref_count;
			++(*ref_count);
		}
	public:
		//using pointer which another shared_ptr is managing to create a new shared_ptr is undefined
		shared_ptr() : _ptr(nullptr), ref_count(nullptr){};
		explicit shared_ptr(pointer ptr) : _ptr(ptr), ref_count(new size_type(1)){};
		shared_ptr(pointer ptr, delete_type deleter) : _ptr(ptr), _deleter(deleter), ref_count(new size_type(1)) {};
		shared_ptr(const shared_ptr& s_ptr) : _ptr(s_ptr._ptr), ref_count(nullptr) {
			copy_ref_count(s_ptr.use_count);
		}
        shared_ptr(const shared_ptr& s_ptr, pointer ptr) : _ptr(ptr), ref_count(nullptr) {
            copy_ref_count(s_ptr.use_count);
        }
		~shared_ptr(){
			reduce_ref_count();
		}
		shared_ptr<value_type, delete_type>& operator=(const shared_ptr& s_ptr) {
			if (this != &s_ptr) {
				reduce_ref_count();
				_ptr = s_ptr.get();
				copy_ref_count(s_ptr.use_count);
			}
			return *this;
		}

		shared_ptr(shared_ptr&& s_ptr) noexcept : ref_count(nullptr) {
			this->_ptr = s_ptr._ptr;
			this->_deleter = s_ptr._deleter;
			this->ref_count = s_ptr.ref_count;
			s_ptr._ptr = nullptr;
			s_ptr.ref_count = nullptr;
		}
		shared_ptr<value_type, delete_type>& operator=(shared_ptr&& s_ptr)  noexcept {
			if (this == &s_ptr)
				return *this;
			if (this->_ptr != nullptr) {
				reduce_ref_count();
			}
			this->_ptr = s_ptr._ptr;
			this->_deleter = s_ptr._deleter;
			this->ref_count = s_ptr.ref_count;
			s_ptr._ptr = nullptr;
			s_ptr.ref_count = nullptr;
		}

		//modifier
		void reset(pointer new_ptr) {
			reduce_ref_count();
			_ptr = new_ptr;
			ref_count = new size_type(1);
		}
		void swap(shared_ptr& s_ptr) {
			DeipiSTL::Swap(_ptr, s_ptr._ptr);
            DeipiSTL::Swap(ref_count, s_ptr.ref_count);
            DeipiSTL::Swap(_deleter, s_ptr._deleter);
		}

		//observer
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

		reference operator*() {
			return *_ptr;
		}
		const_reference operator*()const {
			return *_ptr;
		}

		pointer operator->() {
			return _ptr;
		}
		const_pointer operator->()const {
			return _ptr;
		}

		/*reference operator[](const size_type n) {
			return *(_ptr + n);
		}
		const_reference operator[](const size_type n)const{
			return *(_ptr + n);
		}*/

		size_type use_count()const {
			return *ref_count;
		}

		bool unique()const {
			return *ref_count == 1;
		}
		explicit operator bool()const {
			return _ptr != nullptr;
		}

		bool onwer_befor(const shared_ptr& other)const {
			return _ptr < other._ptr;
		}
        template <typename X, typename Alloc, typename... Args>
        friend shared_ptr<X> allocate_shared(const Alloc& alloc, Args&&...args);
	};

    template <typename X, typename D, typename Alloc, typename... Args>
    shared_ptr<X, D> allocate_shared(const Alloc& alloc, Args&&... args) {
        //abandon TinySTL, use first_level to alloc adjacent address
        typename shared_ptr<X, D>::all_data_tag* ptr = alloc.template Allocate<shared_ptr<X>::all_data_tag>();
        X* t_ptr = (X)ptr;
        typename shared_ptr<X, D>::ref_type* ref_ptr = &ptr->s;
        Construct(t_ptr, DeipiSTL::forward<remove_cv<Args>>(args)...);
        Construct(ref_ptr, 1);
        shared_ptr<X, D> s;
        s._ptr = ptr;
        s.ref_count = ref_ptr;
        return s;
    }

	template <typename T, typename... Args, typename D = default_delete<T>>
	shared_ptr<T, D> make_shared(Args&&...args) {
		//make_shared<T>(new T(...))
        allocator<T, deipi_Alloc> alloc;
		return allocate_shared<T, D, allocator<T, deipi_Alloc>, Args...>(alloc, DeipiSTL::forward<Args>(args)...);
	}

	//use cpp reference edition
	template< typename T, typename D1, typename U, typename D2>
	shared_ptr<T, D1> static_pointer_cast(const shared_ptr<U, D2>& r) noexcept {
		auto p = static_cast<typename shared_ptr<T, D1>::value_type*>(r.get());
		return shared_ptr<T, D1>(r, p);
	}
    template< typename T, typename D1, typename U, typename D2>
    shared_ptr<T, D1> dynamic_pointer_cast(const shared_ptr<U, D2>& r) noexcept
	{
		if (auto p = dynamic_cast<typename shared_ptr<T, D1>::value_type*>(r.get())) {
			return shared_ptr<T, D1>(r, p);
		}
		else {
			return shared_ptr<T, D1>();
		}
	}
    template< typename T, typename D1, typename U, typename D2>
    shared_ptr<T, D1> const_pointer_cast(const shared_ptr<U, D2>& r) noexcept
	{
		auto p = const_cast<typename shared_ptr<T, D1>::value_type*>(r.get());
		return shared_ptr<T, D1>(r, p);
	}

    template <typename T, typename D>
    bool operator==(nullptr_t* p1, shared_ptr<T, D>& p2){
        return p1 == p2.get();
    }

    template <typename T, typename D>
    bool operator==(shared_ptr<T, D>& p1, nullptr_t* p2){
        return p1.get() == p2;
    }

    template <typename T1, typename D1, typename T2, typename D2>
    bool operator==(shared_ptr<T1, D1>& p1, shared_ptr<T2, D2>& p2){
        return p1.get() == p2.get();
    }

    template <typename T1, typename D1, typename T2, typename D2>
    bool operator!=(shared_ptr<T1, D1>& a, shared_ptr<T2, D2>& b) {
        return a.get() != b.get();
    }

    template <typename T, typename D>
    bool operator!=(shared_ptr<T, D>& a, nullptr_t b) {
        return a.get() != b;
    }

    template <typename T, typename D>
    bool operator!=(nullptr_t b, shared_ptr<T, D>& a) {
        return a.get() != b;
    }

    template <typename T, typename D>
    void swap(shared_ptr<T, D>& a, shared_ptr<T, D>& b) {
        a.swap(b);
    }
}

namespace DeipiSTL {
	template<typename T>
	class weak_ptr {

	};
}

#endif	/*deipi_MEMORY_h*/