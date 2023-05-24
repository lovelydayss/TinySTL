# mSTL

在 C++ STL 基础上使用 C++11 参考 folly 等进行合理增减实现的 C++ 标准模板库，部分实现线程安全支持

## 支持功能

### 基本组件

|               功能/组件               |                   路径                   | 完成情况 |   线程安全支持   |               单元测试               | 性能对比 | 功能/组件来源参考 |
| :------------------------------------: | :---------------------------------------: | :-------: | :---------------: | :----------------------------------: | :-------: | :---------------: |
|               allocator               |        [allocator.h](src/allocator.h)        | completed |      ......      |                ......                |  ......  |     STL C++11     |
|              type_traits              |      [type_traits.h](src/type_traits.h)      | completed |      ......      |                ......                |  ......  |     STL C++11     |
|                iterator                |         [iterator.h](src/iterator.h)         | completed |      ......      |                ......                |  ......  |     STL C++11     |
|               algorithm               |        [algorithm.h](src/algorithm.h)        | completed |      ......      |                to do                |  ......  |     STL C++11     |
|            initializer_list            | [initializer_list.h](src/initializer_list.h) | completed |      ......      |                ......                |  ......  |     STL C++11     |
|                  pair                  |                  pair.h                  | completed |      ......      |                to do                |   to do   |     STL C++11     |
|                 tuple                 |                  tuple.h                  | completed |      ......      |                to do                |   to do   |     STL C++11     |
|                 array                 |            [array.h](src/array.h)            | completed |    mt_array.h    |  [test_array.cpp](test/test_array.cpp)  | completed |     STL C++11     |
|                 vector                 |           [vector.h](src/vector.h)           | completed |      ......      | [test_vector.cpp](test/test_vector.cpp) | completed |     STL C++11     |
|                fbvector                |                fbvector.h                |   to do   |      ......      |                ......                |  ......  |      ......      |
|                  span                  |                  span.h                  |   to do   |      ......      |                ......                |  ......  |     STL C++20     |
|                  list                  |                  list.h                  | completed |     mt_list.h     |                to do                |   to do   |     STL C++11     |
|              forward_list              |              forward_list.h              | completed | mt_forward_list.h |                to do                |   to do   |     STL C++11     |
|            circular buffer            |             circular_buffer.h             | completed |      ......      |                to do                |   to do   |      ......      |
|              suffix array              |              suffix_array.h              |   to do   |      ......      |                ......                |  ......  |      ......      |
|                 deque                 |                  deque.h                  | completed |      ......      |                to do                |   to do   |     STL C++11     |
|                 stack                 |                  stack.h                  | completed |      ......      |                to do                |   to do   |     STL C++11     |
|                 queue                 |                  queue.h                  | completed |      ......      |                to do                |   to do   |     STL C++11     |
|             priority_queue             |             priority_queue.h             | completed |      ......      |                to do                |   to do   |     STL C++1     |
|              disjoint set              |              disjoint_set.h              | completed |      ......      |                to do                |   to do   |      ......      |
|              map/multimap              |                   map.h                   |   to do   |      ......      |                ......                |  ......  |     STL C++11     |
| unordered_map/<br />unordered_multimap |              unordered_map.h              |   to do   |      ......      |                ......                |  ......  |     STL C++11     |
|              set/multiset              |                   set.h                   |   to do   |      ......      |                ......                |  ......  |     STL C++11     |
| unordered_set/<br />unordered_multiset |              unordered_set.h              |   to do   |      ......      |                ......                |  ......  |     STL C++11     |
|                 string                 |                 string.h                 | completed |      ......      |                to do                |  ......  |     STL C++11     |
|                fbstring                |                fbstring.h                | completed |      ......      |                to do                |  ......  |       folly       |
|              string_view              |               string_view.h               | completed |      ......      |                to do                |  ......  |     STL C++17     |
|                 bitmap                 |                 bitmap.h                 |   to do   |      ......      |                ......                |  ......  |      ......      |
|               skip_list               |                skip_list.h                | completed |  mt_skip_list.h  |                to do                |  ......  |       redis       |
|                avl_tree                |                avl_tree.h                |   to do   |      ......      |                ......                |  ......  |      ......      |
|                 b_tree                 |                 b_tree.h                 |   to do   |      ......      |                ......                |  ......  |      ......      |
|                b+_tree                |               bplus_tree.h               |   to do   |      ......      |                ......                |  ......  |      ......      |
|                rb_tree                |                 rb_tree.h                 |   to do   |      ......      |                ......                |  ......  |      ......      |
|               trie tree               |                trie_tree.h                |   to do   |      ......      |                ......                |  ......  |      ......      |
|                 graph                 |                  graph.h                  |   to do   |      ......      |                ......                |  ......  |      ......      |

### 线程安全相关

|    功能/组件    |         路径         | 完成情况 | 单元测试 | 性能对比 | 功能/组件来源参考 |
| :-------------: | :------------------: | :-------: | :------: | :------: | :---------------: |
|    spin_lock    |    src/spinlock.h    | completed |  ......  |  ......  |       folly       |
| lock-free queue | src/lockfree_queue.h | completed |  ......  |  ......  |       folly       |

### 工具列表

| 工具名称 | 工具简介           | 项目目标位置                                       |
| -------- | ------------------ | -------------------------------------------------- |
| doctest  | a test framework | [doctest/doctest](https://github.com/doctest/doctest) |

## 项目支持

### 组件文档及知识总结

| 名称        | 简介                               | 路径                                           |
| ----------- | ---------------------------------- | ---------------------------------------------- |
| memory      | C++ 内存管理及 std::alloc 源码解析 | [doc/memory](doc/memory/memory.md)                |
| iterator    | C++ 迭代器原理及源码解析           | [doc/iterator](doc/iterator/iterator.md)          |
| type_traits | C++ 类型萃取原理及应用             | [doc/type_traits](doc/type_traits/type_traits.md) |
| vector      | vector/array/initializer_list 文档 | [doc/vector](doc/vector/vector.md)                |

### 项目代码结构

| 代码文件         | 路径                                          | 完成情况 | 文件功能 |
| ---------------- | --------------------------------------------- | :-------: | :------: |
| allocator        | [src/allocator.h](src/allocator.h)               | completed |  ......  |
| iterator         | [src/iterator.h](src/iterator.h)                 | completed |  ......  |
| vector           | [src/vector.h](src/vector.h)                     |   to do   |  ......  |
| construct        | [src/construct.h](src/construct.h)               | completed |  ......  |
| uninitialized    | [src/uninitialized.h](src/uninitialized.h)       | completed |  ......  |
| type_traits      | [src/type_traits.h](src/type_traits.h)           | completed |  ......  |
| algorithm        | [src/algorithm.h](src/algorithm.h)               |   to do   |  ......  |
| initializer_list | [src/initializer_list.h](src/initializer_list.h) | completed |  ......  |
| array            | [src/array.h](src/array.h)                       |   to do   |  ......  |

### 参考内容

[cppreference](https://en.cppreference.com/w/)

[TinySTL](https://github.com/zouxiaohang/TinySTL/tree/master/TinySTL)

侯捷《STL 源码剖析》，《C++ Primer》...

## 更新记录

**2023-5-7** 首次更新，完成 iterator 功能模块，重新整理了项目文档   [git-commit](https://github.com/lovelydayss/mSTL/commit/5e332f85ee5d1d945539f4b8573431a74a81e10e)

**2023-5-24** 完成 vector 功能模块测试   [git-commit](https://github.com/lovelydayss/mSTL/commit/52ea757ae88647d35bb71455a3c6ca7e86b0cf99)
