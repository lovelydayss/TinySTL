#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "alloc.h"
#include "basic.h"
#include "type_traits.h"

#include <new>
#include <utility>

MSTL_NAMESPACE_BEGIN

// 空间配置
template <class T>
class allocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

public:
	// 调用 alloc 类函数执行空间分配
	static pointer allocate();
	static pointer allocate(size_type n);

	// 调用 alloc 类函数执行空间释放
	static void deallocate(pointer ptr);
	static void deallocate(pointer ptr, size_type n);

	// 调用类型的构造函数进行对象构造
	// 支持右值对象及可变参数构造
	static void construct(pointer ptr);
	static void construct(pointer ptr, const_reference value);
	static void construct(pointer ptr, value_type&& value) noexcept;

	template <class... Args>
	static void construct(pointer ptr, Args&&... args) noexcept;

	// 调用析构函数执行对象析构
	// 支持处理某一范围对象
	static void destroy(pointer ptr) noexcept;
	static void destroy(pointer first, pointer last) noexcept;

private:
	static void _destory(pointer ptr, _true_type) noexcept;
	static void _destory(pointer ptr, _false_type) noexcept;

	static void _destory(pointer first, pointer last, _true_type) noexcept;
	static void _destory(pointer first, pointer last, _false_type) noexcept;
};

template <class T>
typename allocator<T>::pointer allocator<T>::allocate() {
	return static_cast<pointer>(alloc::allocate(sizeof(T)));
}
template <class T>
typename allocator<T>::pointer allocator<T>::allocate(size_type n) {
	return n == 0 ? nullptr
	              : static_cast<pointer>(alloc::allocate(sizeof(T) * n));
}
template <class T>
void allocator<T>::deallocate(pointer ptr) {
	alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
}
template <class T>
void allocator<T>::deallocate(pointer ptr, size_type n) {
	if (n == 0)
		return;
	alloc::deallocate(static_cast<void*>(ptr), sizeof(T) * n);
}

template <class T>
void allocator<T>::construct(pointer ptr) {
	new (ptr) T();
}

template <class T>
void allocator<T>::construct(pointer ptr, const_reference value) {
	new (ptr) T(value);
}

template <class T>
void allocator<T>::construct(pointer ptr, value_type&& value) noexcept {
	new (ptr) T(std::forward<T>(value));
}

template <class T>
template <class... Args>
void allocator<T>::construct(pointer ptr, Args&&... args) noexcept {
	new (ptr) T(std::forward<Args>(args)...);
}

template <class T>
void allocator<T>::_destory(pointer ptr, _true_type) noexcept {}

template <class T>
void allocator<T>::_destory(pointer ptr, _false_type) noexcept {
	ptr->~T();
}
template <class T>
void allocator<T>::_destory(pointer first, pointer last, _true_type) noexcept {}

template <class T>
void allocator<T>::_destory(pointer first, pointer last, _false_type) noexcept {
	while (first != last) {
		destroy(first);
		++first;
	}
}

template <class T>
void allocator<T>::destroy(pointer ptr) noexcept {
	typedef typename _type_traits<pointer>::is_POD_type is_POD_type;
	allocator<T>::_destory(ptr, is_POD_type());
}
template <class T>
void allocator<T>::destroy(pointer first, pointer last) noexcept {
	typedef typename _type_traits<pointer>::is_POD_type is_POD_type;
	allocator<T>::_destory(first, last, is_POD_type());
}

MSTL_NAMESPACE_END

#endif