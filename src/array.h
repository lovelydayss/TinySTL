#ifndef ARRAY_H
#define ARRAY_H

#include "basic.h"

#include "algorithm.h"
#include "iterator.h"
#include "uninitialized.h"

MSTL_NAMESPACE_BEGIN

#if 0
// 非静态版本
template <class T, size_t N, class Allocator = mSTL::allocator<T>>
class array {
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using allocator_type = Allocator;

	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = _iterator<random_access_iterator_tag, value_type>;
	using const_iterator = _iterator<random_access_iterator_tag, const value_type>;

	using reverse_iterator = _reverse_iterator<iterator>;
	using const_reverse_iterator = _reverse_iterator<const_iterator>;

	using uninitialized_mem_func_type = uninitialized_mem_func<T, Allocator>;

private:
	pointer start_;
	pointer finish_;

public:
	///<- (construct)(destruct)(copy)(operator=) ......
	array() {
		start_ = allocator_type::allocator(N);
		finish_ = start_ + N;
	}
	array(std::initializer_list<T> li) {
		// assert(N >= li.size());   		// 此处似乎 initializer_list 会处理

		start_ = allocator_type::allocator(N);
		finish_ = start_ + N;

		for (auto item : li) {
			allocator_type::construct(start_, item);
			start_++;
		}
	}

	array(const array<T, N>& other) {
		*this = other;
	}

	array(array<T, N>&& other) {
		if (this == &other)
			return;

		allocator_type::destory(start_, finish_);
		uninitialized_mem_func_type::move(other.start_, other.finish_, start_);
	}

	array& operator=(const array<T, N>& other) {
		if (this == &other)
			return *this;
		
		allocator_type::destory(start_, finish_);
		uninitialized_mem_func_type::copy(other.start_, other.finish_, start_);

		return *this;
	}

	~array() {
		allocator_type::destroy(start_, finish_);
		allocator_type::deallocate(start_, N);
	}

	// Element access
	constexpr reference at(size_type pos) {
		return pos < N ? *(start_ + pos) : throw "out of range";
	}
	constexpr const_reference at(size_type pos) const {
		return pos < N ? *(start_ + pos) : throw "out of range";
	}
	constexpr reference operator[](size_type pos) {
		return *(start_ + pos);
	}
	constexpr const_reference operator[](size_type pos) const {
		return *(start_ + pos);
	}
	constexpr reference front() { return *(begin()); }
	constexpr const_reference front() const { return *(begin()); }

	constexpr reference back() { return *(end() - 1); }
	constexpr const_reference back() const { return  *(end() - 1); }

	constexpr pointer data() { return (start_); }
	constexpr const_pointer data() const { return (start_); }

	// Iterators
	constexpr iterator begin() { return iterator(start_); }
	constexpr const_iterator cbegin() const { return const_iterator(start_); }

	constexpr iterator end() { return iterator(finish_); }
	constexpr const_iterator cend() const { return const_iterator(finish_); }

	constexpr reverse_iterator rbegin() { return reverse_iterator(start_); }
	constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator(start_); }

	constexpr reverse_iterator rend() { return reverse_iterator(finish_); }
	constexpr const_reverse_iterator crend() const { return const_reverse_iterator(finish_); }


	// Capacity
	constexpr bool empty() const { return start_ == finish_; }

	constexpr size_type size() const { return N; }
	constexpr size_type max_size() const { return N; }

	// Operations
	void fill(reference value) {
		allocator_type::destor(start_, finish_);
		uninitialized_mem_func_type::fill_n(start_, finish_, value);
	}
	void swap(array& other) noexcept {
		pointer tmp_start_ = start_;
		pointer tmp_finish_ = finish_;

		start_ = other.start_;
		finish_ = other.finish_;
		other.start_ = tmp_start_;
		other.finish_ = tmp_finish_;

	}

};

template <typename T, size_t N>
inline bool operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, size_t N>
inline bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs == rhs);
}

template <typename T, size_t N>
inline bool operator<(const array<T, N>& lhs, const array<T, N>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
	                                    rhs.end());
}

template <typename T, size_t N>
inline bool operator>(const array<T, N>& lhs, const array<T, N>& rhs) {
	return rhs < lhs;
}

template <typename T, size_t N>
inline bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs > rhs);
}

template <typename T, size_t N>
inline bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs < rhs);
}

template <typename T, size_t N>
inline void swap(array<T, N>& lhs, array<T, N>& rhs) {
	lhs.swap(rhs);
}

/*

template <typename T, size_t N, size_t Index>
constexpr T& get(array<T, N>& arr) {
	static_assert(Index < N);
	return typename __array_traits<T, N>::S_ref(arr.M_elems, Index);
}

template <typename T, size_t N, size_t Index>
constexpr const T& get(const array<T, N>& arr) {
	static_assert(Index < N);
	return typename __array_traits<T, N>::S_ref(arr.M_elems, Index);
}

template <typename T, size_t N, size_t Index>
constexpr T&& get(array<T, N>&& arr) {
	static_assert(Index < N);
	return std::move(get<Index>(arr));
}

*/

#endif

// 静态版本
// 数组 T[N]
template <typename T, size_t N>
struct _array_traits {
	typedef T Type[N];

	static constexpr T& static_ref(const Type& t, size_t pos) { return const_cast<T&>(t[pos]); }
	static constexpr T* static_ptr(const Type& t) { return const_cast<T*>(t); }
};

// 类模板部分特例化，处理空数组
template <typename T>
struct _array_traits<T, 0> {
	struct Type {};

	static constexpr T& static_ref(const Type&, size_t) { return *static_cast<T*>(nullptr); }
	static constexpr T* static_ptr(const Type&) { return nullptr; }
};

template <class T, size_t N>
struct array {
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;

	using iterator = _iterator<random_access_iterator_tag, value_type>;
	using const_iterator = _iterator<random_access_iterator_tag, const value_type>;

	using reverse_iterator = _reverse_iterator<iterator>;
	using const_reverse_iterator = _reverse_iterator<const_iterator>;

	// mem data
	typedef _array_traits<T, N> AT_Type;
	typename AT_Type::Type M_elems;

	///<- (construct)(destruct)(copy)(operator=) ......
	/// 结构体无需此部分

	// Element access
	constexpr reference at(size_type pos) {
		return pos < N ? AT_Type::static_ref(M_elems, pos) : throw "out of range";
	}
	constexpr const_reference at(size_type pos) const {
		return pos < N ? AT_Type::static_ref(M_elems, pos) : throw "out of range";
	}
	constexpr reference operator[](size_type pos) {
		return AT_Type::static_ref(M_elems, pos);
	}
	constexpr const_reference operator[](size_type pos) const {
		return AT_Type::static_ref(M_elems, pos);
	}
	constexpr reference front() { return *(begin()); }
	constexpr const_reference front() const { return *(begin()); }
	constexpr reference back() { return N ? *(end() - 1) : *end(); }
	constexpr const_reference back() const { return N ? *(end() - 1) : *end(); }

	constexpr pointer data() { return AT_Type::static_ptr(M_elems); }
	constexpr const_pointer data() const { return AT_Type::static_ptr(M_elems);}

	// Iterators
	constexpr iterator begin() { return iterator(data()); }
	constexpr const_iterator cbegin() const { return const_iterator(data()); }
	constexpr iterator end() { return iterator(data() + N); }
	constexpr const_iterator cend() const { return const_iterator(data() + N); }

	constexpr reverse_iterator rbegin() { return reverse_iterator(data() + N); }
	constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator(data() + N); }
	constexpr reverse_iterator rend() { return reverse_iterator(data()); }
	constexpr const_reverse_iterator crend() const { return const_reverse_iterator(data()); }


	// Capacity
	constexpr bool empty() const { return size() == 0; }
	constexpr size_type size() const { return N; }
	constexpr size_type max_size() const { return N; }

	// Operations
	void fill(reference value) { mSTL::fill_n(begin(), N, value);}
	void swap(array& other) noexcept { mSTL::swap_ranges(begin(), end(), other.begin());}

};

template <typename T, size_t N>
inline bool operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
	return mSTL::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, size_t N>
inline bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs == rhs);
}

template <typename T, size_t N>
inline bool operator<(const array<T, N>& lhs, const array<T, N>& rhs) {
	return mSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, size_t N>
inline bool operator>(const array<T, N>& lhs, const array<T, N>& rhs) {
	return rhs < lhs;
}

template <typename T, size_t N>
inline bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs > rhs);
}

template <typename T, size_t N>
inline bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs < rhs);
}

template <typename T, size_t N>
inline void swap(array<T, N>& lhs, array<T, N>& rhs) {
	lhs.swap(rhs);
}

template <typename T, size_t N, size_t Index>
constexpr T& get(array<T, N>& arr) {
	static_assert(Index < N, "out of range");
	return typename _array_traits<T, N>::static_ref(arr.M_elems, Index);
}

template <typename T, size_t N, size_t Index>
constexpr const T& get(const array<T, N>& arr) {
	static_assert(Index < N, "out of range");
	return typename _array_traits<T, N>::static_ref(arr.M_elems, Index);
}

template <typename T, size_t N, size_t Index>
constexpr T&& get(array<T, N>&& arr) {
	static_assert(Index < N, "out of range");
	return std::move(get<Index>(arr));
}


MSTL_NAMESPACE_END

#endif