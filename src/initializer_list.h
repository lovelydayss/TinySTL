#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include "basic.h"

MSTL_NAMESPACE_BEGIN

// initializer_list
template <class T>
class initializer_list {
public:
	using value_type = T;
	using size_type = size_t;
	
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;

private:
	const_iterator first_;
	const_iterator last_;

public:
	initializer_list() noexcept = default;
	initializer_list(const_iterator first, const_iterator last) noexcept : first_(first), last_(last) {}

	size_t size() const noexcept {
		return static_cast<size_type>(last_ - first_);
	}
	const_iterator begin() const noexcept {
		return first_;
    }
	const_iterator end() const noexcept {
		return last_;
    }

	~initializer_list() = default;
};

// initializer list range access
template <class T>
typename initializer_list<T>::const_iterator begin(initializer_list<T> il) noexcept {
	return il.begin();
}
template <class T>
typename initializer_list<T>::const_iterator end(initializer_list<T> il) noexcept {
	return il.end();
}

MSTL_NAMESPACE_END



#endif