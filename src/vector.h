#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <type_traits>

#include "allocator.h"
#include "algorithm.h"
#include "iterator.h"
#include "reverseIterator.h"
#include "uninitialized_functions.h"

namespace mSTL {

template <class T, class Alloc = allocator<T>>
class vector {
private:
	T* start_;
	T* finish_;
	T* endOfStorage_;

	typedef Alloc dataAllocator;
public:

	
};

}

#include "detail/vector.impl.h"
#endif