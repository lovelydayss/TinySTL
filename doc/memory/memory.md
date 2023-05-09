# C++ 内存管理及 std::alloc 源码剖析

## alloc 内存池

侯捷 《STL源码剖析》

[C++内存分配详解四：std::alloc行为剖析](https://blog.csdn.net/qq_34269632/article/details/115636008)

## std::allocator

```
#include <vector>
#include <ext/pool_allocator.h>

std::vector<int, __gnu_cxx::__pool_alloc<int>> result;
```
