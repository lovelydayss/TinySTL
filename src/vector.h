#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <type_traits>

#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"
#include "uninitialized_functions.h"

MSTL_NAMESPACE_BEGIN

template <class T, class Alloc = allocator<T>>
class vector {
private:
	T* start_;
	T* finish_;
	T* endOfStorage_;

	typedef Alloc dataAllocator;

public:
};

MSTL_NAMESPACE_END

#endif