#ifndef VECTOR_H
#define VECTOR_H

#include "basic.h"

#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"

#include <initializer_list>
#include <limits>
#include <utility>

MSTL_NAMESPACE_BEGIN

template <class T, class Allocator = allocator<T>>
class vector {

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = value_type*;
	using const_iterator = const value_type*;

	using reverse_iterator = _reverse_iterator<iterator>;
	using const_reverse_iterator = _reverse_iterator<const_iterator>;

	using uninitialized_mem_func_type =
	    uninitialized_mem_func<value_type, Allocator>;

private:
	pointer start_;
	pointer finish_;
	pointer end_of_storage_;

public:
	// (construct)(destruct)(copy)(operator=)(assign) ......
	// 不支持构造函数中自定义 alloc 进行分配，均采用类型默认分配器

	vector()
	    : start_(nullptr)
	    , finish_(nullptr)
	    , end_of_storage_(nullptr) {}
	vector(const size_type count, const_reference value = value_type()) {
		start_ = allocator_type::allocate(count);
		uninitialized_mem_func_type::fill_n(start_, count, value);

		finish_ = start_ + count;
		end_of_storage_ = finish_;
	}
	template <class InputIterator>
	vector(InputIterator first, InputIterator last) {
		alloc_n_and_copy(&*first, &*last, static_cast<size_type>(last - first));
	}

	vector(const vector& other) {
		alloc_n_and_copy(other.begin(), other.end(), other.capacity());
	}
	vector(vector&& other) {

		start_ = other.start_;
		finish_ = other.finish_;
		end_of_storage_ = other.end_of_storage_;

		other.start_ = nullptr;
		other.finish_ = nullptr;
		other.end_of_storage_ = nullptr;
	}
	vector& operator=(const vector& other) {
		if (this == &other)
			return *this;

		clear();
		realloc_and_move(other.capacity());
		uninitialized_mem_func_type::copy(other.begin(), other.end(), begin());
		finish_ = start_ + other.size();

		return *this;
	}
	vector& operator=(vector&& other) {
		if (this == &other)
			return *this;

		clear();
		allocator_type::deallocate(begin(), capacity());

		start_ = other.start_;
		finish_ = other.finish_;
		end_of_storage_ = other.end_of_storage_;

		other.start_ = nullptr;
		other.finish_ = nullptr;
		other.end_of_storage_ = nullptr;

		return *this;
	}

	vector(const std::initializer_list<T>& il) {
		alloc_n_and_copy(il.begin(), il.end(), il.size());
	}
	vector& operator=(const std::initializer_list<T>& il) {
		clear();
		realloc_and_move(il.size());
		uninitialized_mem_func_type::copy(il.begin(), il.end(), begin());
		finish_ = start_ + il.size();

		return *this;
	}

	void assign(size_type count, const_reference value) {
		clear();
		reserve(count);
		uninitialized_mem_func_type::fill_n(start_, count, value);
		finish_ = start_ + count;
	}
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		clear();
		reserve(static_cast<size_type>(last - first));
		uninitialized_mem_func_type::copy(&*first, &*last, start_);
		finish_ = start_ + static_cast<size_type>(last - first);
	}

	void assign(const std::initializer_list<T>& il) {
		assign(il.begin(), il.end());
	}

	~vector() noexcept {
		clear();
		allocator_type::deallocate(begin(), capacity());
		start_ = finish_ = end_of_storage_ = nullptr;
	}

	allocator_type get_allocator() { return allocator_type(); }

	// Element access

	reference       at(size_type pos) { return *(begin() + pos); }
	const_reference at(size_type pos) const { return *(begin() + pos); }

	reference operator[](const difference_type i) { return *(begin() + i); }
	const_reference operator[](const difference_type i) const {
		return *(cbegin() + i);
	}

	reference       front() { return *(begin()); }
	const_reference front() const { return *(begin()); }

	reference       back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	pointer       data() noexcept { return (start_); }
	const_pointer data() const noexcept { return (start_); }

	// Iterators

	iterator       begin() noexcept { return static_cast<iterator>(start_); }
	const_iterator begin() const noexcept {
		return static_cast<const_iterator>(start_);
	}
	const_iterator cbegin() const noexcept {
		return static_cast<const_iterator>(start_);
	}

	iterator       end() noexcept { return static_cast<iterator>(finish_); }
	const_iterator end() const noexcept {
		return static_cast<const_iterator>(finish_);
	}
	const_iterator cend() const noexcept {
		return static_cast<const_iterator>(finish_);
	}

	reverse_iterator rbegin() noexcept {
		return static_cast<reverse_iterator>(finish_ - 1);
	}
	const_reverse_iterator rbegin() const noexcept {
		return static_cast<const_reverse_iterator>(finish_ - 1);
	}
	const_reverse_iterator crbegin() const noexcept {
		return static_cast<const_reverse_iterator>(finish_ - 1);
	}

	reverse_iterator rend() noexcept {
		return static_cast<reverse_iterator>(start_ - 1);
	}
	const_reverse_iterator rend() const noexcept {
		return static_cast<const_reverse_iterator>(start_ - 1);
	}
	const_reverse_iterator crend() const noexcept {
		return static_cast<const_reverse_iterator>(start_ - 1);
	}

	// Capacity

	bool empty() const noexcept { return start_ == finish_; }

	size_type size() const noexcept {
		return static_cast<size_type>(finish_ - start_);
	}
	size_type max_size() const noexcept {
		return std::numeric_limits<difference_type>::max();
	}
	size_type capacity() const noexcept {
		return static_cast<size_type>(end_of_storage_ - start_);
	}

	void reserve(size_type n) {
		if (n <= capacity())
			return;

		realloc_and_move(n);
	}
	void shrink_to_fit() { realloc_and_move(size()); }

	// Modifiers

	void clear() noexcept {
		allocator_type::destroy(start_, finish_);
		finish_ = start_;
	}
	iterator insert(const_iterator pos, const_reference value) {
		return insert(pos, static_cast<size_type>(1), value);
	}
	iterator insert(const_iterator pos, value_type&& value) {
		const size_type index = pos - begin();
		make_empty_before_pos(const_cast<pointer>(pos),
		                      static_cast<size_type>(1));

		pointer new_pos = begin() + index;
		allocator_type::construct(new_pos, std::move(value));
		return new_pos;
	}
	iterator insert(const_iterator pos, size_type count,
	                const_reference value) {
		const size_type index = pos - begin();
		make_empty_before_pos(const_cast<pointer>(pos), count);

		pointer new_pos = begin() + index;
		uninitialized_mem_func_type::fill_n(new_pos, count, value);
		return new_pos;
	}
	template <class InputIterator>
	iterator insert(const_iterator pos, InputIterator first,
	                InputIterator last) {

		const size_type index = pos - begin();
		make_empty_before_pos(const_cast<pointer>(pos),
		                      static_cast<size_type>(last - first));

		pointer new_pos = begin() + index;
		uninitialized_mem_func_type::copy(first, last, new_pos);
		return new_pos;
	}
	iterator insert(const_iterator pos, const std::initializer_list<T>& il) {
		return insert(pos, il.begin(), il.end());
	}

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		const size_type index = pos - begin();
		make_empty_before_pos(const_cast<pointer>(pos),
		                      static_cast<size_type>(1));

		pointer new_pos = begin() + index;
		allocator_type::construct(new_pos, std::forward<Args>(args)...);
		return (begin() + index);
	}

	iterator erase(const_iterator position) {
		return erase(position, (position + 1));
	}
	iterator erase(const_iterator first, const_iterator last) {
		const size_type index = first - begin();

		allocator_type::destroy(const_cast<pointer>(first),
		                        const_cast<pointer>(last));
		erase_empty_in_pos(const_cast<pointer>(first),
		                   static_cast<size_type>(last - first));

		return (begin() + index);
	}

	void push_back(const_reference value) {
		insert(end(), static_cast<size_type>(1), value);
	}
	void push_back(value_type&& value) {
		insert(end(), static_cast<size_type>(1), std::move(value));
	}

	template <class... Args>
	reference emplace_back(Args&&... args) {
		return *(emplace(end(), std::forward<Args>(args)...));
	}

	void pop_back() {
		--finish_;
		allocator_type::destroy(finish_);
	}

	void resize(size_type count, const_reference value = value_type()) {
		size_type old_size = size();
		size_type old_capacity = capacity();

		size_type count_of_insert = count - old_size;
		if (count < old_size) {
			allocator_type::destroy((start_ + count), finish_);
		} else if (count > old_size && count <= old_capacity) {
			uninitialized_mem_func_type::fill_n(finish_, count_of_insert,
			                                    value);
		} else if (count > old_capacity) {
			realloc_and_move(get_new_capacity(count_of_insert));
			uninitialized_mem_func_type::fill_n(finish_, count_of_insert,
			                                    value);
		}

		finish_ = start_ + count;
	}

	void swap(vector& other) {
		if (this == &other)
			return;

		mSTL::swap(start_, other.start_);
		mSTL::swap(finish_, other.finish_);
		mSTL::swap(end_of_storage_, other.end_of_storage_);
	}

public:
	inline size_type get_new_capacity(size_type count) const;

private:
	inline void alloc_n_and_copy(const_iterator first, const_iterator last,
	                             size_type count);
	inline void realloc_and_move(size_type count);

	inline void make_empty_before_pos(pointer pos, size_type count);
	inline void erase_empty_in_pos(pointer pos, size_type count);

	inline void memmove_aux(pointer dest, pointer src, size_type count,
	                        _true_type, _direction);

	inline void memmove_aux(pointer dest, pointer src, size_type count,
	                        _false_type, _common_direction);
	inline void memmove_aux(pointer dest, pointer src, size_type count,
	                        _false_type, _reverse_direction);
};

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;

	for (size_t i = 0; i < lhs.size(); ++i) {
		if (lhs[i] != rhs[i])
			return false;
	}

	return true;
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {

	size_t min_size = mSTL::min(lhs.size(), rhs.size());

	for (size_t i = 0; i < min_size; ++i) {
		if (lhs[i] > rhs[i])
			return false;
		else if (lhs[i] < rhs[i])
			return true;
	}

	return lhs.size() < rhs.size() ? true : false;
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(rhs < lhs);
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return rhs < lhs;
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

template <class T, class Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
	lhs.swap(rhs);
}

template <class T, class Alloc, class U>
size_t erase(vector<T, Alloc>& c, const U& value) {
	size_t count = 0;
	for (auto item = c.begin(); item != c.end(); ++item) {
		if (*item == value) {
			c.erase(item);
			++count;
		}
	}

	return count;
}

/*
template <class T, class Alloc, class Pred>
size_t erase_if(vector<T, Alloc>& c, Pred pred) {
	size_t count = 0;
	for (auto item = c.begin(); item != c.end(); ++item) {
		if (pred(*item)) {
			c.erase(item);
			++count;
		}
	}

	return count;
}
*/

// implement

///<- (construct)(destruct)(copy)(operator=)(assign) ......

//----------------- construct -------------------
//----------------- copy -------------------
//----------------- operator= -------------------
//----------------- assign -------------------
//----------------- destruct -------------------

///<- Capacity
///<- Modifiers

///<- private function

// 此处可自定义增长策略
// 测试可发现 std::vector 采用的并不是严格双倍的增长策略
template <class T, class Alloc>
inline typename vector<T, Alloc>::size_type
vector<T, Alloc>::get_new_capacity(size_type count) const {

	size_type old_capacity = capacity();

	size_type new_capacity = old_capacity + mSTL::max(old_capacity, count);
	assert(new_capacity < max_size());

	return (old_capacity == 0 ? count : new_capacity);

	// gcc
	// return (capacity() == 0 ? len : capacity() * 2);

	// msvc
	// return (capacity() == 0 ? len : static_cast<size_type>(capacity() * 1.5));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::alloc_n_and_copy(const_iterator first,
                                               const_iterator last,
                                               size_type      count) {
	start_ = allocator_type::allocate(count);
	uninitialized_mem_func_type::copy(first, last, start_);

	finish_ = start_ + static_cast<size_type>(last - first);
	end_of_storage_ = start_ + count;
}

template <class T, class Alloc>
inline void vector<T, Alloc>::realloc_and_move(size_type count) {

	pointer new_start_ = allocator_type::allocate(count);

	size_type old_size = size();

	// 此处 count 默认会大于等于 old_size 即元素个数
	// 即不会产生截断情况
	assert(count >= old_size);

	if (start_ != nullptr) {
		// 数据移动
		uninitialized_mem_func_type::move(start_, finish_, new_start_);
		allocator_type::deallocate(start_, capacity());
	}

	start_ = new_start_;
	finish_ = start_ + old_size;
	end_of_storage_ = start_ + count;
}

template <class T, class Alloc>
inline void vector<T, Alloc>::make_empty_before_pos(pointer   pos,
                                                    size_type count) {
	size_type new_size = size() + count;

	size_type size_before_pos = static_cast<size_type>(pos - begin());
	size_type size_after_pos = static_cast<size_type>(end() - pos);

	typedef typename _type_traits<value_type>::is_POD_type isPODType;

	if (new_size <= capacity()) {
		memmove_aux((pos + count), pos, size_after_pos, isPODType(),
		            _reverse_direction());
		finish_ = start_ + new_size;
	} else {

		size_type new_capacity = get_new_capacity(count);
		// 构建新的内存块
		pointer new_start_ = allocator_type::allocate(new_capacity);

		if (start_ != nullptr) {

			memmove_aux(new_start_, start_, size_before_pos, isPODType(),
			            _common_direction());
			memmove_aux((new_start_ + size_before_pos + count), pos,
			            size_after_pos, isPODType(), _reverse_direction());

			// 释放此前的内存块
			allocator_type::deallocate(start_, capacity());
		}

		start_ = new_start_;
		finish_ = start_ + new_size;
		end_of_storage_ = start_ + new_capacity;
	}
}

template <class T, class Alloc>
inline void vector<T, Alloc>::erase_empty_in_pos(pointer pos, size_type count) {

	pointer   dest = pos + count;
	size_type size_move = finish_ - dest;

	typedef typename _type_traits<value_type>::is_POD_type isPODType;
	memmove_aux(pos, dest, size_move, isPODType(), _common_direction());
	finish_ = pos + size_move;
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(pointer dest, pointer src,
                                          size_type count, _true_type,
                                          _direction) {
	memmove(dest, src, count * sizeof(value_type));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(pointer dest, pointer src,
                                          size_type count, _false_type,
                                          _common_direction) {
	for (size_type i = 0; i < count; ++i, ++src, ++dest)
		allocator_type::construct(dest,
		                          std::move(static_cast<value_type>(*src)));
}

template <class T, class Alloc>
inline void vector<T, Alloc>::memmove_aux(pointer dest, pointer src,
                                          size_type count, _false_type,
                                          _reverse_direction) {

	// 反方向即需要移动 [src, src + count) 元素至 [dest, dest + count)
	pointer src_reverse = src + count - 1, dest_reverse = dest + count - 1;
	for (size_type i = 0; i < count; ++i, --src_reverse, --dest_reverse)
		allocator_type::construct(
		    dest_reverse, std::move(static_cast<value_type>(*src_reverse)));
}

MSTL_NAMESPACE_END

#endif