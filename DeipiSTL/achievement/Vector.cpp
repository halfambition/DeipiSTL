//
//  Vector.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/23.
//
#include "../Vector.h"

namespace DeipiSTL {
	//here's some interface of vector
	
	//inner aux
	template <typename T, typename Alloc>
	void vector<T, Alloc>::allocate_and_fill(size_type n, const T& value) {
		start = data_allocator::Allocate(n);
		finish = DeipiSTL::Uninitialized_Fill_N(start, n, value);
		end_of_storage = finish;
	}

	template <typename T, typename Alloc>
	void vector<T, Alloc>::allocate_and_copy(const vector& vec) {
		Destory(start, finish);
		deallocate();

		start = data_allocator::Allocate(vec.size());
		finish = DeipiSTL::Uninitialized_Copy(vec.cbegin(), vec.cend(), start);
		end_of_storage = start + vec.size();
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::allocate_and_move(vector&& vec) {
		Destory(start, finish);
		deallocate();

		start = vec.start;
		finish = vec.finish;
		end_of_storage = vec.end_of_storage;
		vec.start = nullptr;
		vec.finish = nullptr;
		vec.end_of_storage = nullptr;
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::deallocate() {
		if (start != nullptr)
			data_allocator::Deallocate(start, end_of_storage - start);
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::realloc_aux(size_type new_capacity) {
		//expand or reduce capacity, then copy all orderly
		size_type len = size();
		if (new_capacity < size())
			len = new_capacity;
		iterator new_start = data_allocator::Allocate(new_capacity);
		iterator new_finish = nullptr;
		try {
			new_finish = DeipiSTL::Uninitialized_Copy(start, start + len, new_start);

			DeipiSTL::Destroy(start, finish);
			deallocate();

			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_capacity;
		}
		catch (...){
			Destory(new_start, new_finish);
			data_allocator::Deallocate(new_start, len);
		}
		return start;
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::grow_capacity(iterator& new_start, size_type& space_need) {
		//expand capacity
		size_type new_size = 2 * capacity();
		if (size() + space_need > new_size)
			new_size = size() + space_need + (0.5 * capacity());
		space_need = new_size;
		new_start = data_allocator::Allocate(new_size);
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, size_type insert_size, iterator tar) {
		iterator new_position = nullptr;
		new_position = DeipiSTL::Uninitialized_Copy(start, position, tar);
		DeipiSTL::Uninitialized_Fill_N(new_position, new_position + insert_size, T());
		if (position < finish) 
			DeipiSTL::Uninitialized_Copy(position, finish, new_position + insert_size);

		DeipiSTL::Destroy(start, finish);
		deallocate();

		start = tar;
		finish = tar + size() + insert_size;
		
		return new_position;
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert_aux(iterator position, size_type insert_size) {
		DeipiSTL::Uninitialized_Fill_N(finish, finish + insert_size, T());
		if (position < finish)
			DeipiSTL::Copy_backward(position, finish, finish + insert_size);
		finish = finish + insert_size;
		return position;
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::__insert(iterator pos, size_type n) {
		if (!In_Range(start, finish, pos))
			throw "out of range";
		size_type new_space = n;
		if (capacity() == 0 || is_full()) {
			//capacity expansion
			iterator new_start = nullptr;

			//new_space will change into new capacity
			grow_capacity(new_start, new_space);
			pos = insert_aux(pos, n, new_start);
			end_of_storage = start + new_space;
		}
		else
			pos = insert_aux(pos, n);
		return pos;
	}
	template <typename T, typename Alloc>
	bool vector<T, Alloc>::is_full() {
		return end_of_storage == finish;
	}

	//constructor and deconstruct
	template <typename T, typename Alloc>
	template <typename InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		typename iterator_traits<InputIterator>::difference_type n = Distance(first, last);
		start = data_allocator::Allocate(n);
		end_of_storage = finish = Uninitialized_Copy(first, last, start);
	}
	template <typename T, typename Alloc>
	vector<T, Alloc>::~vector() {
		if (capacity() != 0) {
			DeipiSTL::Destroy(start, finish);
			deallocate();
		}
	}
	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& vec) {
		if (this != &vec)
			allocate_and_copy(vec);
		return *this;
	}
	template <typename T, typename Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& vec) noexcept {
		if (this != &vec)
			allocate_and_move(DeipiSTL::move(vec));
		return *this;
	}

	//insert or delete element
	template <typename T, typename Alloc>
	void vector<T, Alloc>::push_back(const T& val) {
		insert(finish, val);
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::pop_back() {
		if (finish < start)
			throw "error: index out of range";
		--finish;
        DeipiSTL::Destroy(finish);
	}
	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const T& val) {
		pos = __insert(pos, 1);
		DeipiSTL::Copy(pos, pos +1, val);
		return pos - 1;
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const T& val, size_type& n) {
		pos = __insert(pos, n);
		DeipiSTL::Fill_N(pos, n, val);
		return pos - 1;
	}
	
	template <typename T, typename Alloc>
	template <typename InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) {
		typename iterator_traits<InputIterator>::difference_type n = Distance(first, last);
		pos = __insert(pos, n);
		DeipiSTL::Copy(first, last, pos);
		return pos - 1;
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator pos) {
		if (pos < finish || pos > finish)
			throw "out of range";
		DeipiSTL::Copy(pos + 1, finish, pos);
		DeipiSTL::Destroy(finish);
		--finish;
		return pos - 1;
	}

	template <typename T, typename Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator second) {
		if (!In_Range(start, finish, first) || !In_Range(start, finish, second)) 
			throw "out of range";
		iterator last = DeipiSTL::Copy(second, finish, first);
        DeipiSTL::Destroy(last, finish);
		finish -= (second - first);
		return first - 1;
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::swap(vector& v) {
		if (this == &v)
			return;
		DeipiSTL::Swap(start, v.start);
		DeipiSTL::Swap(finish, v.finish);
		DeipiSTL::Swap(end_of_storage, v.end_of_storage);
	}
	//about space
	template <typename T, typename Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		//expand capacity
		if (n <= capacity())
			return;
		realloc_aux(n);
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type new_size) {
		resize(new_size, T(0));
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::resize(size_type new_size, const T& val) {
		if (new_size < size()) {
			DeipiSTL::Destroy(start + new_size, finish);
			finish = start + new_size;
		}
		else if (new_size > size() && new_size <= capacity()) {
			finish = DeipiSTL::Uninitialized_Fill_N(finish, new_size - size(), val);
		}
		else if (new_size > capacity()) {
			reserve(new_size);
			finish = DeipiSTL::Uninitialized_Fill_N(finish, new_size - size(), val);
		}
	}
	template <typename T, typename Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		//try not to give tiny space back to memory
		//so decide to reallocate
		//O(n)
		realloc_aux(size());

		//old
		//DeipiSTL::Destory(finish + 1, end_of_storage);
		//end_of_storage = finish;
	}

	template <typename T, typename Alloc>
	std::ostream operator<<(std::ostream& os, vector<T, Alloc>& vec) {
		for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
			os << *iter << " ";
		}
		os << std::endl;
	}
}