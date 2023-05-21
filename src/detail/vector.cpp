#include "../vector.h"

#include <cassert>
#include <cstring>

MSTL_NAMESPACE_BEGIN

///<- (construct)(destruct)(copy)(operator=)(assign) ......
// copy(): [first,last) -> [result,result + (last - first))  O(N)
// copy_n(): [first,last) -> [result,result + n)  O(N)
// copy_if(): [first,last) -> [result,result + (last - first)) (just condition is true)  O(N)
// copy_backward(): [first,last) -> [result - (last - first), result)  O(N)

//----------------- construct -------------------
template <class T, class Alloc>
vector<T, Alloc>::vector(const size_type count) {
	vector(count, value_type(), allocator_type());
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const size_type count, const_reference value,
                         const allocator_type& alloc) {
	start_ = allocator_type::allocate(count);
	finish_ = uninitialized_mem_func_type::fill_n(begin(), count, value);
	end_of_storage_ = finish_;
}

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last,
                         const allocator_type& alloc) {
	start_ = allocator_type::allocate((last - first));
	finish_ = uninitialized_mem_func_type::copy(first, last, begin());
	end_of_storage_ = finish_;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const std::initializer_list<T>& il, const allocator_type& alloc) {
	*this = il;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& other) {
	*this = other;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& other) {
	*this = std::move(other);
}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& other) {

	if (this == &other)
		return *this;

	allocator_type::destroy(begin(), end());
	reserve(other.capacity());
	finish_ = uninitialized_mem_func_type::copy(other.begin(), other.end(), begin());

	return *this; 
}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& other) {
	if (this == &other)
		return *this;

	allocator_type::destroy(begin(), end());
	allocator_type::deallocator(begin(), capacity());

	start_ = other.start_;
	finish_ = other.finish_;
	end_of_storage_ = other.end_of_storage_;

	other.start_ = nullptr;
	other.finish_ = nullptr;
	other.end_of_storage_ = nullptr;

	return *this;
}


template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const std::initializer_list<T>& il) {
	start_ = allocator_type::allocate(il.size());
	finish_ = uninitialized_mem_func_type::copy(il.begin(), il.end(), begin());
	end_of_storage_ = finish_;
}


template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type count, const_reference value) {
	reserve(count);
	allocator_type::destroy(begin(), end());
	finish_ = uninitialized_mem_func_type::fill_n(begin(), count, value);
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	reserve((last - first));
	allocator_type::destroy(begin(), end());
	finish_ = uninitialized_mem_func_type::copy(first, last, begin());
}

template <class T, class Alloc>
void vector<T, Alloc>::assign(const std::initializer_list<T>& il) {
	assign(il.begin(), il.end());
}


template <class T, class Alloc>
vector<T, Alloc>::~vector() noexcept {
	allocator_type::destroy(begin(), end());
	allocator_type::deallocate(begin(), capacity());
}

///<- Capacity ......

template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {

	assert(n < max_size());

	size_type old_capacity = capacity();
	
	if (n <= old_capacity)
		return;

	pointer old_start_ = start_;
	pointer old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(n);
	finish_ =  uninitialized_mem_func_type::move(old_start_, old_finish_, start_);
	end_of_storage_ = start_ + n;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity);
}

template <class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {

	size_type old_size = size();
	size_type old_capacity = capacity();

	pointer old_start_ = start_;
	pointer old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(old_size);
	finish_ = uninitialized_mem_func_type::move(old_start_, old_finish_, start_);
	end_of_storage_ = finish_;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity);
}

//<- Modifiers

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type count, const_reference value) {
	size_type size_ = size();
	size_type capacity_ = capacity();

	pointer   new_finish_ = start_ + count;

	if (count <= size_) {
		allocator_type::destroy(new_finish_, finish_);
		finish_ = new_finish_;
	}

	else if (count > size_ && count <= capacity_) {
		uninitialized_mem_func_type::fill_n(finish_, (count - size_), value);
	}

	else if (count > capacity_) {
		reserve(static_cast<size_type>(get_new_capacity(count)));
		uninitialized_mem_func_type::fill_n(finish_, new_finish_, value);
	}
}

template <class T, class Alloc>
void vector<T, Alloc>::swap(vector& other) {
	if (this == &other)
		return;

	mSTL::swap(start_, other.start_);
	mSTL::swap(finish_, other.finish_);
	mSTL::swap(end_of_storage_, end_of_storage_);

}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::get_new_capacity(size_type len) const {
	size_type old_capacity = capacity();
	return (old_capacity == 0 ? len : old_capacity + mSTL::max(old_capacity, len));
}

template <class T, class Alloc>
void vector<T, Alloc>::make_empty_before_pos(const_iterator pos, size_type count) {
	size_type new_size = size() + count;

	size_type size_before_pos = static_cast<size_type>(pos - begin());
	size_type size_after_pos = static_cast<size_type>(end() - pos);

	typedef typename _type_traits<value_type>::is_POD_type isPODType;

	if (new_size < capacity()) {
		memmove_aux((pos + count), pos, size_after_pos, isPODType(), _reverse_direction());
		finish_ = begin() + new_size;
	}
	else {
		size_type old_capacity = capacity();
		size_type new_capacity = get_new_capacity(count);

		pointer old_start_ = start_;
		pointer old_finish_ = finish_;

		// 新内存块制作及数据移动
		start_ = allocator_type::allocate(new_capacity);
		finish_ = start_ + new_size;
		end_of_storage_ = start_ + new_capacity;

		memmove_aux(start_, old_start_, size_before_pos, isPODType(), _common_direction());
		memmove_aux((start_ + size_before_pos + count), pos, size_after_pos, isPODType(), _reverse_direction());

		// 释放此前的内存块
		allocator_type::deallocate(old_start_, old_capacity);
	}
}

template <class T, class Alloc>
void vector<T, Alloc>::earse_empty_in_pos(const_iterator pos, size_type count) {
	size_type size_move = end() - (pos + count);

	typedef typename _type_traits<value_type>::is_POD_type isPODType;
	memmove_aux(pos, (pos + count), size_move, isPODType(), _common_direction());
	finish_ = pos + size_move;
}

MSTL_NAMESPACE_END