#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "allocator.h"
#include "basic.h"
#include "functional.h"
#include "iterator.h"
#include "type_traits.h"
#include "utility.h"
#include <cstring>
#include <utility>

MSTL_NAMESPACE_BEGIN

///<- copy series
// copy(): [first,last) -> [result,result + (last - first))  O(N)
// copy_n(): [first,last) -> [result,result + n)  O(N)
// copy_if(): [first,last) -> [result,result + (last - first)) (just condition is true)  O(N)
// copy_backward(): [first,last) -> [result - (last - first), result)  O(N)

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

///<- min/max series


MSTL_NAMESPACE_END

#endif