#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include "basic.h"

#include "allocator.h"

MSTL_NAMESPACE_BEGIN

// initializer_list
template <class T, class Allocator = mSTL::allocator<T>>
class initializer_list {
public:
	using value_type = T;
	using size_type = size_t;
	using allocator_type = Allocator;

	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;

private:
	pointer first_;
	pointer last_;

public:
	initializer_list() noexcept = default;
	initializer_list(const_pointer first, const_pointer last) noexcept
	    : first_(first)
	    , last_(last) {}

	template <class... Args>
	initializer_list(Args... args) {
		size_type size = static_cast<size_type>(sizeof...(args));
		size_type count = 0;

		first_ = allocator_type::allocate(size);
		last_ = first_ + size;

		int arr[] = {(set_arg((first_ + count++), args), 0)...};
	}

	initializer_list(const initializer_list<T>& other) = delete;
	initializer_list<T>& operator=(const initializer_list<T>& other) = delete;

	size_t size() const noexcept {
		return static_cast<size_type>(last_ - first_);
	}
	const_pointer begin() const noexcept { return first_; }
	const_pointer end() const noexcept { return last_; }

	~initializer_list() {
		allocator_type::destroy(first_, last_);
		allocator_type::deallocate(first_, size());
	}

private:
	void set_arg(pointer ptr, const_reference value) {
		allocator_type::construct(ptr, value);
	}
};

// initializer list range access
template <class T>
typename initializer_list<T>::const_pointer
begin(initializer_list<T> il) noexcept {
	return il.begin();
}
template <class T>
typename initializer_list<T>::const_pointer
end(initializer_list<T> il) noexcept {
	return il.end();
}

MSTL_NAMESPACE_END

#endif