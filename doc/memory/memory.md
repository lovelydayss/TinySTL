# C++ 内存模型及相关操作源码分析

## C++ 内存模型

## 三种基本内存分配

### malloc & free

### new & delete

### operator new & operator delete

## STL 中的内存分配

### std::allocator<>

> **针对不同的应用场景，GNU 中定义了一系列 Allocator 扩展**

### __gnu_cxx::new_allocator<>

### __gnu_cxx::malloc_allocator<>

### __gnu_cxx::bitmap_allocator<>

### __gnu_cxx::__pool_alloc<>

#### alloc 内存池

[C++内存分配详解四：std::alloc行为剖析](https://blog.csdn.net/qq_34269632/article/details/115636008)

```c++
#include <vector>
#include <ext/pool_allocator.h>

std::vector<int, __gnu_cxx::__pool_alloc<int>> result;
```

### __gnu_cxx::__mt_alloc<>

### __gnu_cxx::debug_allocator<>

内部为一个分配器和一个unsign_int（相当于封装了别的分配器，并多用一块空间来记录整块的大小，没什么屌用）

## STL 基本内存处理

### uninitialized_copy()

 `uninitialized_copy()` 将 `[first,last)` 表示的源迭代器区间的所有对象拷贝到以 `result` 为起始地址的目的区间。如果源迭代器 `is POD type` ，直接执行内存复制，而若是 `is not POD type`，则 `uninitialized_copy()` 会调用 `construct()` 为每个输入对象进行构造

容器的全区间构造函数（range constructor）通常以两个步骤完成：

1. 配置内存区块，足以包含范围内所有元素
2. 使用 `uninitialized_copy()`，在该内存区块上构造元素

使用 `uninitialized_copy()` 可以很方便的对容器的全区间构造函数去实现

`uninitialized_copy()` 必须具有 commit or rollback 语义，即要么“构造出所有必要元素”，要么 “不构造任何东西”（当有任何一个对象构造失败时必须执行回滚）

```c++

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
```

```c++

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
```

```c++

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
```

### uninitialized_fill()

`uninitialized_fill()` 将 `value`填充到 `[first,last)` 表示的目的区间。如果源迭代器 `is POD type` ，直接执行内存复制函数，而若是 `is not POD type`，则 `uninitialized_fill()` 会调用 `construct()` 为每个输入对象进行构造

与 `uninitialized_copy()` 一样，`unininitialized_fill()` 必须具有 commit or rollback 语义，即要么“构造出所有必要元素”，要么 “不构造任何东西”（当有任何一个对象构造失败时必须执行回滚）

```c++

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
```

```c++

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
```

```c++

// uninitialized_fill() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载

template <class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first,
                        ForwardIterator last,
                        const T& value) {
	typedef typename _type_traits<T>::is_POD_type isPODType;
	_uninitialized_fill_aux(first, last, value, isPODType());
}
```

### uninitialized_fill_n()

`uninitialized_fill_n()` 函数是 `uninitialized_fill()` 函数的区间长度参数版本，其采用 `(first, n)`（迭代器起始位置，长度）的形式替代迭代器区间。（此处仍进行了实现，不过似乎可以重构为转换为迭代器后对 `uninitialized_fill()` 函数进行调用）

与上述两个函数一样，`uninitialized_fill_n()` 必须具有 commit or rollback 语义，即要么“构造出所有必要元素”，要么 “不构造任何东西”（当有任何一个对象构造失败时必须执行回滚）。

```c++

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
```

```c++

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
```

```c++

// uninitialized_fill_n() 函数调用
// 在此萃取出输入迭代器的 value type 特性, 即迭代器所指对象原生类型以实现重载

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	typedef typename _type_traits<T>::is_POD_type isPODType;
	return _uninitialized_fill_n_aux(first, n, x, isPODType());
}
```

## 参考

侯捷 《STL源码剖析》

[SGI STL内存基本处理工具：uninitialized_copy/uninitialized_fill/uninitialized_fill_n ](https://www.cnblogs.com/fortunely/p/16223417.html)

[SGI STL空间配置器 allocator/alloc](https://www.cnblogs.com/fortunely/p/16219743.html)

&nbsp;
