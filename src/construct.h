#ifndef CONSTRUCT_H
#define CONSTRUCT_H


#include "basic.h"
#include "type_traits.h"
#include <new>

MSTL_NAMESPACE_BEGIN

// 未初始化空间的处理
// 不依赖于 allocator
// 在修改 uninitialized 函数后失去作用 

// 对象构造
template<class T1, class T2>
inline void construct(T1 *ptr1, const T2& value){
	new(ptr1) T1(value);
}

// 对象的析构
template<class T>
inline void destroy(T *ptr) noexcept{
	ptr->~T();
}

// 基于迭代器区间的析构 均为 noexcept

// is POD type 不需要操作
template <class ForwardIterator>
inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type) noexcept {}

// is not POD type
// 调用对应析构函数执行对象析构
template <class ForwardIterator>
inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type) noexcept {
	while (first != last) {
		destroy(&*first);
		++first;
	}
}

// destory() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) noexcept {
	typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
	_destroy(first, last, is_POD_type());
}

MSTL_NAMESPACE_END


#endif