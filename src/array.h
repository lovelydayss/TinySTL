#ifndef ARRAY_H
#define ARRAY_H

#include "basic.h"

#include "algorithm.h"
#include "iterator.h"
#include "uninitialized.h"

MSTL_NAMESPACE_BEGIN

// 数组 T[N]
template <class T, size_t N>
struct _array_traits {
	typedef T Type[N];

	static constexpr T& static_ref(const Type& t, size_t pos) noexcept {
		return const_cast<T&>(t[pos]);
	}
	static constexpr T* static_ptr(const Type& t) noexcept {
		return const_cast<T*>(t);
	}
};

// 类模板部分特例化，处理空数组
template <class T>
struct _array_traits<T, 0> {
	struct Type {};

	static constexpr T& static_ref(const Type&, size_t) noexcept {
		return *static_cast<T*>(nullptr);
	}
	static constexpr T* static_ptr(const Type&) noexcept { return nullptr; }
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

	using iterator = value_type*;
	using const_iterator = const value_type*;

	using reverse_iterator = _reverse_iterator<iterator>;
	using const_reverse_iterator = _reverse_iterator<const_iterator>;

	// mem data
	typedef _array_traits<T, N> AT_Type;
	typename AT_Type::Type      M_elems;

	///<- (construct)(destruct)(copy)(operator=) ......
	/// 此处无需此部分

	// Element access
	reference at(size_type pos) {
		return pos < N ? AT_Type::static_ref(M_elems, pos)
		               : throw "out of range";
	}
	const_reference at(size_type pos) const {
		return pos < N ? AT_Type::static_ref(M_elems, pos)
		               : throw "out of range";
	}
	reference operator[](size_type pos) {
		return AT_Type::static_ref(M_elems, pos);
	}
	const_reference operator[](size_type pos) const {
		return AT_Type::static_ref(M_elems, pos);
	}
	reference       front() { return *(begin()); }
	const_reference front() const { return AT_Type::static_ref(M_elems, 0); }
	reference       back() { return N ? *(end() - 1) : *end(); }
	const_reference back() const {
		return N ? AT_Type::static_ref(M_elems, N - 1)
		         : AT_Type::static_ref(M_elems, 0);
	}

	pointer       data() { return AT_Type::static_ptr(M_elems); }
	const_pointer data() const { return AT_Type::static_ptr(M_elems); }

	// Iterators
	iterator       begin() { return static_cast<iterator>(data()); }
	const_iterator cbegin() const {
		return static_cast<const_iterator>(data());
	}
	iterator       end() { return static_cast<iterator>(data() + N); }
	const_iterator cend() const {
		return static_cast<const_iterator>(data() + N);
	}

	reverse_iterator rbegin() {
		return static_cast<reverse_iterator>(data() + N - 1);
	}
	const_reverse_iterator crbegin() const {
		return static_cast<const_reverse_iterator>(data() + N - 1);
	}
	reverse_iterator rend() {
		return static_cast<reverse_iterator>(data() - 1);
	}
	const_reverse_iterator crend() const {
		return static_cast<const_reverse_iterator>(data() - 1);
	}

	// Capacity
	constexpr bool      empty() const { return size() == 0; }
	constexpr size_type size() const { return N; }
	constexpr size_type max_size() const { return N; }

	// Operations
	void fill(const_reference value) { mSTL::fill_n(begin(), N, value); }
	void swap(array& other) noexcept {
		mSTL::swap_ranges(begin(), end(), other.begin());
	}
};

template <class T, size_t N>
inline bool operator==(const array<T, N>& lhs, const array<T, N>& rhs) {
	for (size_t i = 0; i < N; ++i) {
		if (lhs[i] != rhs[i])
			return false;
	}

	return true;
}

template <class T, size_t N>
inline bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs == rhs);
}

template <class T, size_t N>
inline bool operator<(const array<T, N>& lhs, const array<T, N>& rhs) {
	for (size_t i = 0; i < N; ++i) {
		if (lhs[i] > rhs[i])
			return false;
		else if (lhs[i] < rhs[i])
			return true;
	}

	return false;
}

template <class T, size_t N>
inline bool operator>(const array<T, N>& lhs, const array<T, N>& rhs) {
	return rhs < lhs;
}

template <class T, size_t N>
inline bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs > rhs);
}

template <class T, size_t N>
inline bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs) {
	return !(lhs < rhs);
}

template <class T, size_t N>
inline void swap(array<T, N>& lhs, array<T, N>& rhs) {
	lhs.swap(rhs);
}

template <size_t Index, class T, size_t N>
T& get(mSTL::array<T, N>& arr) noexcept {
	static_assert(Index < N, "out of range");
	return mSTL::_array_traits<T, N>::static_ref(arr.M_elems, Index);
}

template <size_t Index, typename T, size_t N>
const T& get(const mSTL::array<T, N>& arr) noexcept {
	static_assert(Index < N, "out of range");
	return mSTL::_array_traits<T, N>::static_ref(arr.M_elems, Index);
}

template <size_t Index, class T, size_t N>
const T&& get(const mSTL::array<T, N>&& arr) noexcept {
	static_assert(Index < N, "out of range");
	return std::move(get<Index>(arr));
}

MSTL_NAMESPACE_END

#endif