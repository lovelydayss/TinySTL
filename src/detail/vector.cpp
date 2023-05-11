#include "../vector.h"
#include <cstddef>

MSTL_NAMESPACE_BEGIN

///<- (construct)(destruct)(copy)(operator=)(assign) ......
// copy(): [first,last) -> [result,result + (last - first))  O(N)
// copy_n(): [first,last) -> [result,result + n)  O(N)
// copy_if(): [first,last) -> [result,result + (last - first)) (just condition is true)  O(N)
// copy_backward(): [first,last) -> [result - (last - first), result)  O(N)

//----------------- construct -------------------
template <class T, class Alloc>
vector<T, Alloc>::vector(const size_type count) {
	vector(count, T(), allocator_type());
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const size_type count, const value_type& value,
                         const allocator_type& alloc) {
	start_ = alloc::allocate(count);
	finish_ = uninitialized_fill_n(begin(), count, value);
	end_of_storage_ = finish_;
	
}

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last,
                         const allocator_type& alloc) {
	start_ = alloc::allocate((last - first));
	finish_ = uninitialized_copy(first, last);
	end_of_storage_ = finish_;
}

/*
template <class T, class Alloc>
vector<T, Alloc>::vector(initializer_list<T> init,
                        const allocator_type& alloc) {
	
}
*/

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

	if (*this == other)
		return *this;

	destroy(begin(), end());
	reserve(other.capacity());
	finish_ = uninitialized_copy(other.begin(), other.end(), begin());

	return *this; 
}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& other) {
	if (*this == other)
		return *this;

	destroy(begin(), end());
	reserve(other.capacity());
	finish_ = uninitialized_move(other.begin(), other.end(), begin()); // 执行移动

	return *this;
}

/*
template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(initializer_list<T> ilist) {}
*/

template <class T, class Alloc>
void vector<T, Alloc>::assign(size_type count, const T& value) {
	reserve(count);
	destroy(begin(), end());
	finish_ = uninitialized_fill_n(begin(), count, value);
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	reserve((last - first));
	destroy(begin(), end());
	finish_ = uninitialized_copy(first, last, begin());
}

/*
template <class T, class Alloc>
void vector<T, Alloc>::assign(initializer_list<T> ilist) {}
*/

template <class T, class Alloc>
vector<T, Alloc>::~vector() noexcept {
	destroy(begin(), end());
	allocator_type::deallocate(start_, capacity());
}

///<- Capacity ......

template <class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {

	size_type old_capacity_ = capacity();
	
	if (n <= old_capacity_)
		return;

	auto old_start_ = start_;
	auto old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(n);
	finish_ = uninitialized_move(old_start_, old_finish_, start_);
	end_of_storage_ = start_ + n;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity_);
}

template <class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {

	size_type old_size = size();
	size_type old_capacity_ = capacity();

	auto old_start_ = start_;
	auto old_finish_ = finish_;

	// 新内存块制作及数据移动
	start_ = allocator_type::allocate(old_size);
	finish_ = uninitialized_move(old_start_, old_finish_, start_);
	end_of_storage_ = finish_;

	// 释放此前的内存块
	allocator_type::deallocate(old_start_, old_capacity_);
}

//<- Modifiers

void     clear() noexcept;
iterator insert(const_iterator pos, const value_type& value);
iterator insert(const_iterator pos, T&& value);
iterator insert(const_iterator pos, size_type count, const_reference value);
template <class InputIterator>
iterator insert(const_iterator pos, InputIterator first, InputIterator last);
// iterator insert(const_iterator pos, initializer_list<T> ilist);

template <class... Args>
iterator emplace(const_iterator pos, Args&&... args);

iterator erase(const_iterator position);
iterator erase(const_iterator first, const_iterator last);

void push_back(const_reference value);
void push_back(T&& value);

template <class... Args>
reference emplace_back(Args&&... args);

void pop_back();

void resize(size_type count);
void resize(size_type count, const value_type& value);

void swap(vector& other);

MSTL_NAMESPACE_END