#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "algorithm.h"
#include "allocator.h"
#include "basic.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include <exception>

MSTL_NAMESPACE_BEGIN

// is POD type
// 执行内存拷贝操作即可
template <class InputIterator, class ForwardIterator>
ForwardIterator _uninitialized_copy_aux(InputIterator first,
                                        InputIterator last,
                                        ForwardIterator result,
                                        _true_type) {
	// 此处未采用直接调用 copy 函数写法，直接拷贝内存实现优化
	memcpy(result, first, (last - first) * sizeof(*first));
	return result + (last - first);
}

// is not POD type 情况下的 uninitialized_copy()
// 调用 construct 执行构造，需要考虑异常问题
template <class InputIterator, class ForwardIterator>
ForwardIterator _uninitialized_copy_aux(InputIterator first,
                                        InputIterator last,
                                        ForwardIterator result,
                                        _false_type) {
	ForwardIterator current = result;
	try {
		while (first != last) {
			construct(&*current, *first);
			++first;
			++current;
		}
		return current;
	} catch (std::exception error) {
		destroy(result, current);
		throw error;
	}
}

// uninitialized_copy() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
template <class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first,
                                   InputIterator last,
                                   ForwardIterator result) {
	typedef typename _type_traits<
	    typename iterator_traits<InputIterator>::value_type>::is_POD_type
	    isPODType;
	return _uninitialized_copy_aux(first, last, result, isPODType());
}

// is POD type
// 直接执行内存填充操作即可
template <class ForwardIterator, class T>
void _uninitialized_fill_aux(ForwardIterator first,
                             ForwardIterator last,
                             const T& value,
                             _true_type) {
	// 此处未采用直接调用 fill 函数写法，直接设置内存实现优化
	memset(first, value, last - first);
}

// is not POD type
// 调用 construct 执行构造，需要考虑异常问题
template <class ForwardIterator, class T>
void _uninitialized_fill_aux(ForwardIterator first,
                             ForwardIterator last,
                             const T& value,
                             _false_type) {
	ForwardIterator current = first;
	try {
		while (current != last) {
			construct(static_cast<T*>(current), value);
			++current;
		}
	} catch (std::exception error) {
		destroy(first, current);
		throw error;
	}
}

// uninitialized_fill() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
template <class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first,
                        ForwardIterator last,
                        const T& value) {
	typedef typename _type_traits<T>::is_POD_type isPODType;
	_uninitialized_fill_aux(first, last, value, isPODType());
}

// is POD type
// 直接执行内存填充操作即可
template <class ForwardIterator, class Size, class T>
ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                          Size n,
                                          const T& value,
                                          _true_type) {
	memset(first, value, n);
	return (first + n);
}

// is not POD type
// 调用 construct 执行构造，需要考虑异常问题
template <class ForwardIterator, class Size, class T>
ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
                                          Size n,
                                          const T& value,
                                          _false_type) {
	ForwardIterator current = first;
	ForwardIterator last = first + n;
	try {
		while (current != last) {
			construct(static_cast<T*>(current), value);
			++current;
		}
		return current;
	} catch (std::exception error) {
		destroy(first, current);
		throw error;
	}
}

// uninitialized_fill_n() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
template <class ForwardIterator, class Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	typedef typename _type_traits<T>::is_POD_type isPODType;
	return _uninitialized_fill_n_aux(first, n, x, isPODType());
}

MSTL_NAMESPACE_END

#endif