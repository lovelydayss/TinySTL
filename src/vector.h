#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <limits>

#include "basic.h"

#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"

#include <initializer_list>
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

	// using iterator = _iterator<random_access_iterator_tag, value_type>;
	// using const_iterator = _iterator<random_access_iterator_tag, const value_type>;

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
	// 不支持构造函数中自定义 alloc 进行分配，均采用类型默认 allocator

	vector()
	    : start_(nullptr)
	    , finish_(nullptr)
	    , end_of_storage_(nullptr) {}
	explicit vector(const size_type count,
	                const_reference value = value_type()) {
		start_ = allocator_type::allocate(count);
		finish_ = uninitialized_mem_func_type::fill_n(begin(), count, value);
		end_of_storage_ = start_ + count;
	}
	template <class InputIterator>
	vector(InputIterator first, InputIterator last) {
		start_ = allocator_type::allocate(static_cast<size_type>(last - first));
		finish_ = uninitialized_mem_func_type::copy(first, last, begin());
		end_of_storage_ = finish_;
	}

	vector(const vector& other) { *this = other; }
	vector(vector&& other) { *this = std::move(other); }
	vector& operator=(const vector& other);
	vector& operator=(vector&& other);

	vector(const std::initializer_list<T>& il) { *this = il; }
	vector& operator=(const std::initializer_list<T>& il);

	void assign(size_type count, const_reference value);
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);

	void assign(const std::initializer_list<T>& il) {
		assign(il.begin(), il.end());
	}

	~vector() noexcept {
		if (start_ == nullptr)
			return;

		if (start_ != finish_)
			allocator_type::destroy(begin(), end());

		allocator_type::deallocate(begin(), capacity());
	}

	// allocator_type get_allocator() { return allocator_type(); }

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
		return static_cast<reverse_iterator>(finish_);
	}
	const_reverse_iterator rbegin() const noexcept {
		return static_cast<const_reverse_iterator>(finish_);
	}
	const_reverse_iterator crbegin() const noexcept {
		return static_cast<const_reverse_iterator>(finish_);
	}

	reverse_iterator rend() noexcept {
		return static_cast<reverse_iterator>(start_);
	}
	const_reverse_iterator rend() const noexcept {
		return static_cast<const_reverse_iterator>(start_);
	}
	const_reverse_iterator crend() const noexcept {
		return static_cast<const_reverse_iterator>(start_);
	}

	// Capacity

	bool      empty() const noexcept { return start_ == finish_; }
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
		allocator_type::destroy(begin(), end());
		finish_ = start_;
	}
	iterator insert(const_iterator pos, const_reference value) {
		return insert(pos, 1, value);
	}
	iterator insert(const_iterator pos, value_type&& value) {
		const size_type index = pos - begin();

		make_empty_in_pos(pos, 1);
		allocator_type::construct(pos, std::move(value));

		return (begin() + index);
	}
	iterator insert(const_iterator pos, size_type count,
	                const_reference value) {
		const size_type index = pos - begin();

		make_empty_in_pos(pos, count);
		uninitialized_mem_func_type::fill_n(pos, count, value);

		return (begin() + index);
	}
	template <class InputIterator>
	iterator insert(const_iterator pos, InputIterator first,
	                InputIterator last) {
		const size_type index = pos - begin();

		make_empty_in_pos(pos, static_cast<size_type>(last - first));
		uninitialized_mem_func_type::copy(first, last, pos);

		return (begin() + index);
	}
	iterator insert(const_iterator pos, const std::initializer_list<T>& il) {
		return insert(pos, il.begin(), il.end());
	}

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		const size_type index = pos - begin();

		make_empty_in_pos(pos, 1);
		allocator_type::construct(pos, std::forward<Args>(args)...);

		return (begin() + index);
	}

	iterator erase(const_iterator position) {
		return earse(position, (position + 1));
	}
	iterator erase(const_iterator first, const_iterator last) {
		const size_type index = first - begin();

		allocator_type::destroy(first, last);
		earse_empty_in_pos(first, static_cast<size_type>(last - first));

		return (begin() + index);
	}

	void push_back(const_reference value) { insert(end(), 1, value); }
	void push_back(value_type&& value) { insert(end(), 1, std::move(value)); }

	template <class... Args>
	reference emplace_back(Args&&... args) {
		return *(emplace(end(), std::forward<Args>(args)...));
	}

	void pop_back() {
		--finish_;
		allocator_type::destroy(finish_);
	}

	void resize(size_type count) { resize(count, value_type()); }
	void resize(size_type count, const value_type& value);

	void swap(vector& other);

private:
	inline size_type get_new_capacity(size_type len) const;

	inline void realloc_and_move(size_type count);

	void make_empty_before_pos(const_iterator pos, size_type count);
	void earse_empty_in_pos(const_iterator pos, size_type count);

	inline void memmove_aux(const_iterator dest, const_iterator src,
	                        size_type count, _true_type, _common_direction);
	inline void memmove_aux(const_iterator dest, const_iterator src,
	                        size_type count, _true_type, _reverse_direction);
	inline void memmove_aux(const_iterator dest, const_iterator src,
	                        size_type count, _false_type, _common_direction);
	inline void memmove_aux(const_iterator dest, const_iterator src,
	                        size_type count, _false_type, _reverse_direction);
};

// 此处就简单用循环实现了下

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;

	return mSTL::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;

	for (auto item_lhs = lhs.begin(), item_rhs = rhs.begin();
	     item_lhs != lhs.end(); ++item_lhs, ++item_rhs)
		if (*item_lhs > *item_rhs)
			return false;

	return true;
}

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs > rhs);
}

template <class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	if (lhs.size() != rhs.size())
		return false;

	for (auto item_lhs = lhs.begin(), item_rhs = rhs.begin();
	     item_lhs != lhs.end(); ++item_lhs, ++item_rhs)
		if (*item_lhs < *item_rhs)
			return false;

	return true;
}

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs < rhs);
}

template <class T, class Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) {
	lhs.swap(rhs);
}

// 此处标准库采用的似乎并不是这种简单实现，具有更好的性能
template <class T, class Alloc, class U>
typename vector<T, Alloc>::size_type erase(vector<T, Alloc>& c,
                                           const U&          value) {
	size_t count = 0;
	for (auto item = c.begin(); item != c.end(); ++item) {
		if (*item == value) {
			item = c.erase(item);
			++count;
		}
	}
	return count;
}

template <class T, class Alloc, class Pred>
typename vector<T, Alloc>::size_type erase_if(vector<T, Alloc>& c, Pred pred) {
	size_t count = 0;
	for (auto item = c.begin(); item != c.end(); ++item) {
		if (pred) {
			item = c.erase(item);
			++count;
		}
	}
	return count;
}

MSTL_NAMESPACE_END

#endif