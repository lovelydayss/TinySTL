#include "../vector.h"

#include <cassert>
#include <cstring>

MSTL_NAMESPACE_BEGIN

///<- (construct)(destruct)(copy)(operator=)(assign) ......

//----------------- construct -------------------

//----------------- copy -------------------

//----------------- operator= -------------------
template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& other) {

	if (this == &other)
		return *this;

	allocator_type::destroy(begin(), end());
	realloc_and_move(other.capacity());
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
	allocator_type::destory(begin(), end());
	realloc_and_move(il.size());
	finish_ = uninitialized_mem_func_type::copy(il.begin(), il.end(), begin());

	return *this;
}

//----------------- assign -------------------
template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type count, const_reference value) {
	reserve(count);
	allocator_type::destroy(begin(), end());
	finish_ = uninitialized_mem_func_type::fill_n(begin(), count, value);
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	reserve(static_cast<size_type>(last - first));
	allocator_type::destroy(begin(), end());
	finish_ = uninitialized_mem_func_type::copy(first, last, begin());
}

//----------------- destruct -------------------



///<- Capacity ......


///<- Modifiers

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type count, const_reference value) {
	size_type old_size = size();
	size_type old_capacity = capacity();

	pointer   new_finish_ = start_ + count;

	if (count <= old_size) {
		allocator_type::destroy(new_finish_, finish_);
		finish_ = new_finish_;
	}

	else if (count > old_size && count <= old_capacity) {
		uninitialized_mem_func_type::fill_n(finish_, (count - old_size), value);
	}

	else if (count > old_capacity) {
		realloc_and_move(get_new_capacity(count));
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

	size_type new_capacity = old_capacity + mSTL::max(old_capacity, len);
	assert(new_capacity < max_size());

	return (old_capacity == 0 ? len : new_capacity);
}

template <class T, class Alloc>
void vector<T, Alloc>::realloc_and_move(size_type count) {

	pointer old_start_ = start_;
	pointer old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(count);
	finish_ = uninitialized_mem_func_type::move(old_start_, old_finish_, start_);
	end_of_storage_ = start_ + count;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, capacity());
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
	} else {

		// 此处由于需要自定义移动，无法使用 realloc_and_move
		size_type old_capacity = capacity();
		size_type new_capacity = get_new_capacity(new_size);

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

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(const_iterator dest, const_iterator src, size_type count, _true_type, _common_direction){
	memmove(src, dest, count * sizeof(value_type));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(const_iterator dest, const_iterator src, size_type count, _true_type, _reverse_direction){
	memmove(src, dest, count * sizeof(value_type));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(const_iterator dest, const_iterator src, size_type count, _false_type, _common_direction){
	for (size_type i=0; i < count; ++i, ++src, ++dest) 
		allocator_type::construct(dest, std::move(static_cast<value_type>(*src)));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(const_iterator dest, const_iterator src, size_type count, _false_type, _reverse_direction){

	// 反方向即需要移动 [src, src + count) 元素至 [dest, dest + count)
	const_iterator src_reverse = src + count - 1, dest_reverse = dest + count - 1;
	for (size_type i=0; i < count; ++i, --src_reverse, --dest_reverse) 
		allocator_type::construct(dest_reverse, std::move(static_cast<value_type>(*src_reverse)));
}

MSTL_NAMESPACE_END