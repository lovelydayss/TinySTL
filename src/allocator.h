#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "basic.h"
#include "alloc.h"
#include <utility>
#include <new>

MSTL_NAMESPACE_BEGIN

// 空间配置
template <class T>
class allocator {
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const pointer const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	// 调用 alloc 类函数执行空间分配
	static pointer allocate();
	static pointer allocate(size_t n);

	// 调用 alloc 类函数执行空间释放
	static void deallocate(pointer ptr);
	static void deallocate(pointer ptr, size_t n);

	// 调用类型的构造函数进行对象构造
	// 支持右值对象及可变参数构造
	static void construct(pointer ptr);
	static void construct(pointer ptr, const_reference value);

	static void construct(pointer ptr, T&& value) noexcept;
	template<class... Args>
	void construct(pointer ptr, Args&&... args) noexcept;

	// 调用析构函数执行对象析构
	// 支持处理某一范围对象
	static void destroy(pointer ptr) noexcept;
	static void destroy(pointer first, pointer last) noexcept;
};

template <class T>
T* allocator<T>::allocate() {
	return static_cast<pointer>(alloc::allocate(sizeof(T)));
}
template <class T>
T* allocator<T>::allocate(size_t n) {
	return n == 0 ? nullptr : static_cast<pointer>(alloc::allocate(sizeof(T) * n));
}
template <class T>
void allocator<T>::deallocate(pointer ptr) {
	alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
}
template <class T>
void allocator<T>::deallocate(pointer ptr, size_t n) {
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
void allocator<T>::construct(pointer ptr, T&& value) noexcept {
	new (ptr) T(std::move(value));
}
template <class T>
template <class... Args>
void allocator<T>::construct(pointer ptr, Args&&... args) noexcept {
	new (ptr) T(std::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(pointer ptr) noexcept{
	ptr->~T();
}
template <class T>
void allocator<T>::destroy(pointer first, pointer last) noexcept{
	for (; first != last; first++) {
		first->~T();
	}
}

MSTL_NAMESPACE_END

#endif