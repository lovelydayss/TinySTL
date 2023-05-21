#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "basic.h"

#include "allocator.h"
#include "functional.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
#include <cstring>
#include <utility>

#include <algorithm>

MSTL_NAMESPACE_BEGIN


//<- uninitialized mem operator function

// Allocator 相关 uninitialized_mem_func
template <class T, class Allocator = mSTL::allocator<T>>
class uninitialized_mem_func {
public:
	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	using allocator_type = Allocator;
private:
	// is POD type
	// 执行内存拷贝操作即可
	template <class InputIterator, class ForwardIterator>
	static ForwardIterator _copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
		// 此处未采用直接调用 copy 函数写法，直接拷贝内存实现优化
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	// is not POD type 情况下的 uninitialized_copy()
	// 调用 construct 执行构造，需要考虑异常问题
	template <class InputIterator, class ForwardIterator>
	static ForwardIterator _copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
		ForwardIterator current = result;
		try {
			while (first != last) {
				// 采用 allocator 写法
				// allocator<typename iterator_traits<InputIterator>::value_type>::construct(&*current, *first);
				allocator_type::construct(&*current, *first);
				++first;
				++current;
			}
			return current;
		} catch (std::exception error) {
			allocator_type::destroy(result, current);
			throw error;
		}
	}

	// is POD type
	// 执行内存拷贝操作即可
	template <class InputIterator, class ForwardIterator>
	static ForwardIterator _move_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
		// 此处未采用直接调用 move 函数写法，直接拷贝内存实现优化
		memmove(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	// is not POD type 情况下的 uninitialized_copy()
	// 调用 construct 执行构造，需要考虑异常问题
	template <class InputIterator, class ForwardIterator>
	static ForwardIterator _move_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
		ForwardIterator current = result;
		try {
			while (first != last) {
				// 采用 allocator 写法
				// allocator<typename iterator_traits<InputIterator>::value_type>::construct(&*current, *first);
				allocator_type::construct(&*current, std::move(*first));
				++first;
				++current;
			}
			return current;
		} catch (std::exception error) {
			allocator_type::destroy(result, current);
			throw error;
		}
	}

	// is POD type
	// 直接执行内存填充操作即可
	template <class ForwardIterator>
	static void _fill_aux(ForwardIterator first, ForwardIterator last, const_reference value, _true_type) {
		// 此处未采用直接调用 fill 函数写法，直接设置内存实现优化
		memset(first, value, last - first);
	}

	// is not POD type
	// 调用 construct 执行构造，需要考虑异常问题
	template <class ForwardIterator>
	static void _fill_aux(ForwardIterator first, ForwardIterator last, const_reference value, _false_type) {
		ForwardIterator current = first;
		try {
			while (current != last) {
				allocator_type::construct(static_cast<pointer>(current), value);
				++current;
			}
		} catch (std::exception error) {
			allocator_type::destroy(first, current);
			throw error;
		}
	}

	// is POD type
	// 直接执行内存填充操作即可
	template <class ForwardIterator, class Size>
	static ForwardIterator _fill_n_aux(ForwardIterator first, Size count, const_reference value, _true_type) {
		memset(first, value, count);
		return (first + count);
	}

	// is not POD type
	// 调用 construct 执行构造，需要考虑异常问题
	template <class ForwardIterator, class Size>
	static ForwardIterator _fill_n_aux(ForwardIterator first, Size count, const_reference value, _false_type) {
		ForwardIterator current = first;
		ForwardIterator last = first + count;
		try {
			while (current != last) {
				allocator_type::construct(static_cast<pointer>(current), value);
				++current;
			}
			return current;
		} catch (std::exception error) {
			allocator_type::destroy(first, current);
			throw error;
		}
	}

public:
	
	// uninitialized_copy() 函数调用
	// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
	template <class InputIterator, class ForwardIterator>
	static inline ForwardIterator copy(InputIterator first, InputIterator last, ForwardIterator result) {
		typedef typename _type_traits<
		    typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _copy_aux(first, last, result, isPODType());
	}

	// uninitialized_move() 函数调用
	// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
	template <class InputIterator, class ForwardIterator>
	static inline ForwardIterator move(InputIterator first, InputIterator last, ForwardIterator result) {
		typedef typename _type_traits<
		    typename iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _move_aux(first, last, result, isPODType());
	}

	// uninitialized_fill() 函数调用
	// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
	template <class ForwardIterator>
	static inline void fill(ForwardIterator first, ForwardIterator last, const_reference value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_fill_aux(first, last, value, isPODType());
	}

	// uninitialized_fill_n() 函数调用
	// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载
	template <class ForwardIterator, class Size>
	static inline ForwardIterator fill_n(ForwardIterator first, Size count, const_reference value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _fill_n_aux(first, count, value, isPODType());
	}
};

//<- Non-modifying sequence operations
// ....

//<- Modifying sequence operations

///<- copy series
// copy(): [first, last) -> [result, result + (last - first))  O(N)
// copy_n(): [first, last) -> [result, result + n)  O(N)
// copy_if(): [first, last) -> [result, result + (last - first)) (just condition is true)  O(N)
// copy_backward(): [first, last) -> [result - (last - first), result)  O(N)

//----------------- copy -------------------
template <class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	while (first != last) {
		*result = *first;
		++result;
		++first;
	}
	return result;
}

//----------------- copy_n -------------------
template <class InputIterator, class Size, class OutputIterator>
OutputIterator copy_n(InputIterator first, Size n, OutputIterator result) {
	while (n > 0) {
		*result = *first;
		++result;
		++first;
		--n;
	}
	return result;
}

//----------------- copy_if -------------------
template <class InputIterator, class OutputIterator, class UnaryPredicate>
OutputIterator copy_if(InputIterator first,
                       InputIterator last,
                       OutputIterator result,
                       UnaryPredicate pred) {
	while (first != last) {
		if (pred(*first)) {
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}

//--------------- copy_backward ---------------
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result) {
	while (last != first)
		*(--result) = *(--last);
	return result;
}

///<- move series
// copy(): [first, last) -> [result, result + (last - first))  O(N)
// copy_backward(): [first, last) -> [result - (last - first), result)  O(N)
//-------------------- move ----------------------
template< class InputIterator, class OutputIterator >
OutputIterator move(InputIterator first, InputIterator last, OutputIterator result) {
	while (first != last) {
		*result = std::move(*first);
		++result;
		++first;
	}
	return result;
}

//-------------- move_backward -------------------
template <class BidirIterator1, class BidirIterator2>
BidirIterator2 move_backward(BidirIterator1 first, BidirIterator1 last, BidirIterator2 result) {

	while (first != last)
		*(result) = std::move(*(--last));

	return result;
}

///<- fill series
// fill(): value -> [first, end)  O(N)
// fill_n(): value -> [first, first + n)  O(N)
//-------------------- fill ----------------------
template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	while (first != last) {
		*first = value;
		++first;
	}
}

inline void fill(char* first, char* last, const char& value) {
	memset(first, static_cast<unsigned char>(value), last - first);
}
inline void fill(wchar_t* first, wchar_t* last, const wchar_t& value) {
	memset(first,
	       static_cast<unsigned char>(value),
	       (last - first) * sizeof(wchar_t));
}

//------------------- fill_n ---------------------
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	while (n > 0) {
		*first = value;
		--n;
		++first;
	}
	return first;
}

template <class Size>
char* fill_n(char* first, Size n, const char& value) {
	memset(first, static_cast<unsigned char>(value), n);
	return first + n;
}
template <class Size>
wchar_t* fill_n(wchar_t* first, Size n, const wchar_t& value) {
	memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
	return first + n;
}

///<- transform series
///<- generate series
///<- remove series
///<- replace series

///<- swap series
/// swap()  &a <-> &b  O(1)
/// iter_swap() *a <-> *b  O(1)
/// swap_ranges() [first1, last1) <-> [first2, first2 + (last1 - first1))  O(N)
/// swap()(a[], b[])  a[] <-> b[]  O(N)
//------------------ swap() ---------------------
template <class T>
void swap(T& a, T& b) {
	T temp = std::move(a);
	a = std::move(b);
	b = std::move(temp);
}

//--------------- iter_swap() -----------------
template <class ForwardIt1, class ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b) {
	swap(*a, *b);
}

//--------------- swap_ranges() -----------------
template <class ForwardIt1, class ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) {
	for (; first1 != last1; ++first1, (void)++first2)
		std::iter_swap(first1, first2);
	return first2;
}

//------------- swap()(a[], b[])  -----------------
template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) {
	swap_ranges(a, (a + N), b);
}

///<- reverse series
///<- rotate series
///<- unique series

//<- Partitioning operations
//<- Sorting operations
//<- Binary search operations(on sorted ranges)
//<- Other operations on sorted ranges
//<- Set operations (on sorted ranges)
//<- Heap operations

//<- Minimum / maximum operations
// max
template <class T>
const T& max(const T& a, const T& b) {
	return std::max(a, b);
}

template <class T, class Compare>
const T& max(const T& a, const T& b, Compare comp) {
	return std::max(a, b, comp);
}

template <class T>
T max(std::initializer_list<T> ilist) {
	return std::max(ilist);
}

template <class T, class Compare>
T max(std::initializer_list<T> ilist, Compare comp) {
	return std::max(ilist, comp);
}

// max_element
template <class ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last) {
	return std::max_element(first, last);
}

template <class ForwardIt, class Compare>
ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp) {
	return std::max_element(first, last, comp);
}

// min
template <class T>
const T& min(const T& a, const T& b) {
	return std::min(a, b);
}

template <class T, class Compare>
const T& min(const T& a, const T& b, Compare comp) {
	return std::min(a, b, comp);
}

template <class T>
T min(std::initializer_list<T> ilist) {
	return std::min(ilist);
}

template <class T, class Compare>
T min(std::initializer_list<T> ilist, Compare comp) {
	return std::min(ilist, comp);
}

// min_element
template <class ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last) {
	return std::min_element(first, last);
}

template <class ForwardIt, class Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp) {
	return std::min_element(first, last, comp);
}

// minmax
template <class T>
std::pair<const T&, const T&> minmax(const T& a, const T& b) {
	return std::minmax(a, b);
}

template <class T, class Compare>
std::pair<const T&, const T&> minmax(const T& a, const T& b, Compare comp) {
	return std::minmax(a, b, comp);
}

template <class T>
std::pair<T, T> minmax(std::initializer_list<T> ilist) {
	return std::minmax(ilist);
}

template <class T, class Compare>
std::pair<T, T> minmax(std::initializer_list<T> ilist, Compare comp) {
	return std::minmax(ilist, comp);
}

// minmax_element
template <class ForwardIt>
std::pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first,
                                               ForwardIt last) {
	return std::minmax_element(first, last);
}

template <class ForwardIt, class Compare>
std::pair<ForwardIt, ForwardIt> minmax_element(ForwardIt first, ForwardIt last,
                                               Compare comp) {
	return std::minmax_element(first, last, comp);
}

// clamp
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
}

template <class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
	
}

//<- Comparison operations
///<- equal series
template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
	return std::equal(first1, last1, first2);
}

template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2,
           BinaryPredicate p) {
	return std::equal(first1, last1, first2, p);
}

template <class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
		return std::equal(first1, last1, first2, last2);
}

template <class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
           BinaryPredicate p) {
	    return std::equal(first1, last1, first2, last2, p);
}

///<- lexicographical_compare series
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2) {
	return std::lexicographical_compare(first1, last1, first2, last2);
}

template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2,
                             InputIt2 last2, Compare comp) {
	return std::lexicographical_compare(first1, last1, first2, last2, comp);
}

//<- Permutation operations

MSTL_NAMESPACE_END

#endif