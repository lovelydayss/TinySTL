#include "../vector.h"

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
vector<T, Alloc>::vector(const size_type count, const value_type& value,
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
	finish_ = uninitialized_mem_func_type::copy(first, last);
	end_of_storage_ = finish_;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(std::initializer_list<T> il, const allocator_type& alloc) {
	start_ = allocator_type::allocate(il.size());
	finish_ = uninitialized_mem_func_type::copy(il.begin(), il.end());
	end_of_storage_ = finish_;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& other) {
	*this = other;
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& other) {
	*this = other;
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
	reserve(other.capacity());
	finish_ = uninitialized_mem_func_type::move(other.begin(), other.end(), begin()); // 执行移动

	return *this;
}


template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<T> il) {

}


template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type count, const T& value) {
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
void vector<T, Alloc>::assign(std::initializer_list<T> il) {

}


template <class T, class Alloc>
vector<T, Alloc>::~vector() noexcept {
	allocator_type::destroy(start_, finish_);
	allocator_type::deallocate(start_, capacity());
}

///<- Capacity ......

template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {

	size_type old_capacity_ = capacity();
	
	if (n <= old_capacity_)
		return;

	pointer old_start_ = start_;
	pointer old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(n);
	finish_ =  uninitialized_mem_func_type::move(old_start_, old_finish_, start_);
	end_of_storage_ = start_ + n;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity_);
}

template <class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {

	size_type old_size = size();
	size_type old_capacity_ = capacity();

	pointer old_start_ = start_;
	pointer old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(old_size);
	finish_ = uninitialized_mem_func_type::move(old_start_, old_finish_, start_);
	end_of_storage_ = finish_;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity_);
}

//<- Modifiers
template <class T, class Alloc>
void vector<T, Alloc>::clear() noexcept {
	allocator_type::destroy(begin(), end());
	finish_ = start_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, const_reference value) {
	const auto index = pos - start_;
	insert(pos, 1, value);
	return (start_ + index);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, value_type&& value) {
	const auto index = pos - start_;
	insert(pos, 1, std::move(value));
	return (start_ + index);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, size_type count,
                         const_reference value) {
	
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, size_type count, value_type&& value){}

template <class T, class Alloc>
template <class InputIterator>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, InputIterator first, InputIterator last){}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> il) {}

template <class T, class Alloc>
template <class... Args>
typename vector<T, Alloc>::iterator vector<T, Alloc>::emplace(const_iterator pos, Args&&... args) {
	
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const_iterator position) {
	earse(position, (position + 1));
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(const_iterator first, const_iterator last){}

template <class T, class Alloc>
void vector<T, Alloc>::push_back(const_reference value) {
	insert(end(), 1, value);
}

template <class T, class Alloc>
void vector<T, Alloc>::push_back(value_type&& value) {
	insert(end(), 1, std::move(value));
}

template <class T, class Alloc>
template <class... Args>
typename vector<T, Alloc>::reference vector<T, Alloc>::emplace_back(Args&&... args) {
	emplace(end(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
void vector<T, Alloc>::pop_back() {
	--finish_;
	allocator_type::destroy(finish_);
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type count) {
	resize(count, value_type());
}

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
		reserve(static_cast<size_type>(capacity_ * EXPANSION_FACTOR));
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
	
}

MSTL_NAMESPACE_END