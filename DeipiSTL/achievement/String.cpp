//
//  String.cpp
//  DeipiSTL
//
//  Created by deipi on 2023/8/28.
//
#include "../String.h"
namespace DeipiSTL {
	//here's some interface of vector
	//inner aux
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::allocate_and_fill(const Char c, size_type n) {
		start = data_allocator::Allocate(n);
		finish = DeipiSTL::Uninitialized_Fill_N(start, n, c);
		end_of_storage = finish;
	}

	template <typename Char, typename Alloc>
	template <typename InputIterator>
	void basic_string<Char, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
		Destory(start, finish);
		deallocate();

		size_type n = last - first;
		start = data_allocator::Allocate(n);
		finish = DeipiSTL::Uninitialized_Copy(first, last, start);
		end_of_storage = start + n;
	}

	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::allocate_and_move(basic_string& str) {
		Destory(start, finish);
		deallocate();

		start = str.start;
		finish = str.finish;
		end_of_storage = str.end_of_storage;
		str.start = nullptr;
		str.finish = nullptr;
		str.end_of_storage = nullptr;
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::deallocate() {
		data_allocator::Deallocate(start, end_of_storage - start);
	}
	template <typename Char, typename Alloc>
	typename basic_string<Char, Alloc>::iterator basic_string<Char, Alloc>::realloc_aux(size_type new_capacity) {
		//expand or reduce capacity, then copy all orderly
		size_type len = size();
		if (new_capacity < size())
			len = new_capacity;
		iterator new_start = data_allocator::Allocate(new_capacity);
		iterator new_finish = DeipiSTL::Uninitialized_Copy(start, start + len, new_start);

		DeipiSTL::Destroy(start, finish);
		deallocate();

		start = new_start;
		finish = new_finish;
		end_of_storage = start + new_capacity;
		
		return start;
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::grow_capacity(iterator new_start, size_type& space_need) {
		//expand capacity
		size_type new_size = 2 * capacity();
		if (size() + space_need > new_size)
			new_size = size() + space_need + (0.5 * capacity());
		space_need = new_size;
		new_start = data_allocator::Allocate(new_size);
	}
	template <typename Char, typename Alloc>
	typename basic_string<Char, Alloc>::iterator basic_string<Char, Alloc>::insert_aux(iterator position, size_type insert_size, iterator tar) {
		iterator new_position = nullptr;
		new_position = DeipiSTL::Uninitialized_Copy(start, position, tar);
		DeipiSTL::Uninitialized_Fill_N(new_position, new_position + insert_size, Char());
		if (position < finish)
			DeipiSTL::Uninitialized_Copy(position, finish, new_position + insert_size);

		DeipiSTL::Destroy(start, finish);
		deallocate();

		start = tar;
		finish = tar + size() + insert_size;

		return new_position;
	}
	template <typename Char, typename Alloc>
	typename basic_string<Char, Alloc>::iterator basic_string<Char, Alloc>::insert_aux(iterator position, size_type insert_size) {
		DeipiSTL::Uninitialized_Fill_N(finish, finish + insert_size, Char());
		size_type new_finish = finish + insert_size;
		if (position < finish)
			DeipiSTL::Copy_backward(position, finish, new_finish);
		finish = new_finish;

		return position;
	}
	template <typename Char, typename Alloc>
	typename basic_string<Char, Alloc>::iterator basic_string<Char, Alloc>::__insert(iterator pos, size_type n) {
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
	template <typename Char, typename Alloc>
	bool basic_string<Char, Alloc>::is_full() {
		return end_of_storage == finish;
	}
	template <typename Char, typename Alloc>
	basic_string<Char, Alloc>::~basic_string() {
		DeipiSTL::Destroy(start, finish);
		deallocate();
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::reserve(size_type new_size) {
		//expand capacity
		if (new_size <= capacity())
			return;
		realloc_aux(new_size);
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::resize(size_type new_size) {
		resize(new_size, '/0');
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::resize(size_type new_size, const_reference c) {
		if (new_size < size()) {
			DeipiSTL::Destroy(start + new_size, finish);
			finish = start + new_size;
		}
		else if (new_size > size() && new_size <= capacity()) {
			finish = DeipiSTL::Uninitialized_Fill_N(finish, new_size - size(), c);
		}
		else if (new_size > capacity()) {
			reserve(new_size);
			finish = DeipiSTL::Uninitialized_Fill_N(finish, new_size - size(), c);
		}
	}
	template <typename Char, typename Alloc>
	void basic_string<Char, Alloc>::shrink_to_fit() {
		//try not to give tiny space back to memory
		//so decide to reallocate
		//O(n)
		realloc_aux(size());

		//old
		//DeipiSTL::Destory(finish + 1, end_of_storage);
		//end_of_storage = finish;
	}
}