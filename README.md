
# mSTL

在 C++ STL 基础上使用 C++11 进行合理增减实现的 C++ 标准模板库

## 支持功能

### 基础组件

|    功能/组件    |                   路径                   | 完成情况 | 单元测试 |    来源参考    |
| :--------------: | :---------------------------------------: | :-------: | :------: | :-------------: |
|    allocator    |        [allocator.h](src/allocator.h)        | completed |  ......  | STL<br />C++11 |
|   type_traits   |      [type_traits.h](src/type_traits.h)      | completed |  ......  | STL<br />C++11 |
|     iterator     |         [iterator.h](src/iterator.h)         | completed |  ......  | STL<br />C++11 |
|    algorithm    |        [algorithm.h](src/algorithm.h)        | completed |  to do  | STL<br />C++11 |
| initializer_list | [initializer_list.h](src/initializer_list.h) | completed |  ......  | STL<br /> C++11 |

### 容器及相关组件

|          功能/组件          |               路径               | 完成情况 |            单元测试            | 性能对比 | 组件来源参考<br />开发版本 |
| :--------------------------: | :-------------------------------: | :-------: | :----------------------------: | :-------: | :------------------------: |
|            array            |        [array.h](src/array.h)        | completed | [completed](test/test_array.cpp) | completed |       STL<br />C++11       |
|            vector            |       [vector.h](src/vector.h)       | completed | [completed](test/test_vector.cpp) | completed |       STL<br />C++11       |
|             list             |         [list.h](src/list.h)         | completed |           completed           |   to do   |       STL<br />C++11       |
|         forward_list         | [forward_list.h](src/forward_list.h) | completed |           completed           |   to do   |       STL<br />C++11       |
|       circular buffer       |         circular_buffer.h         | completed |             to do             |   to do   |           ......           |
|         suffix array         |          suffix_array.h          |   to do   |             ......             |  ......  |           ......           |
|            deque            |        [deque.h](src/deque.h)        | completed |           completed           | completed |       STL<br />C++11       |
|            stack            |        [stack.h](src/stack.h)        | completed |           completed           | completed |       STL<br />C++11       |
|            queue            |        [queue.h](src/queue.h)        | completed |           completed           | completed |       STL<br />C++11       |
|        priority_queue        |         priority_queue.h         | completed |             to do             |   to do   |       STL<br />C++11       |
|         disjoint set         |          disjoint_set.h          | completed |             to do             |   to do   |           ......           |
|         map/multimap         |               map.h               |   to do   |             ......             |  ......  |       STL<br />C++11       |
| unordered_<br />map/multimap |          unordered_map.h          |   to do   |             ......             |  ......  |       STL<br />C++11       |
|         set/multiset         |               set.h               |   to do   |             ......             |  ......  |       STL<br />C++11       |
| unordered_<br />set/multiset |          unordered_set.h          |   to do   |             ......             |  ......  |       STL<br />C++11       |
|    string/<br />fbstring    |       [string.h](src/string.h)       | completed |           completed           |  ......  |    STL/folly<br />C++11    |
|            bitmap            |             bitmap.h             |   to do   |             ......             |  ......  |           ......           |
|          skip_list          |            skip_list.h            | completed |             to do             |  ......  |      redis<br />C++11      |
|           avl_tree           |            avl_tree.h            |   to do   |             ......             |  ......  |           ......           |
|            b_tree            |             b_tree.h             |   to do   |             ......             |  ......  |           ......           |
|           b+_tree           |           bplus_tree.h           |   to do   |             ......             |  ......  |           ......           |
|           rb_tree           |             rb_tree.h             |   to do   |             ......             |  ......  |       STL<br />C++11       |
|            graph            |              graph.h              |   to do   |             ......             |  ......  |           ......           |

### 线程安全相关组件

|    功能/组件    |         路径         | 完成情况 | 单元测试 | 性能对比 | 功能/组件来源参考 |
| :-------------: | :------------------: | :-------: | :------: | :------: | :---------------: |
|    spin_lock    |    src/spinlock.h    | completed |  ......  |  ......  |       folly       |
| lock-free queue | src/lockfree_queue.h | completed |  ......  |  ......  |       folly       |

## 项目开发支持

### 各组件文档

| 名称        | 路径                                           | 组件功能简介                       |
| ----------- | ---------------------------------------------- | ---------------------------------- |
| memory      | [doc/memory](doc/memory/memory.md)                | C++ 内存管理及 std::alloc 源码解析 |
| iterator    | [doc/iterator](doc/iterator/iterator.md)          | C++ 迭代器原理及源码解析           |
| type_traits | [doc/type_traits](doc/type_traits/type_traits.md) | C++ 类型萃取原理及应用             |
| vector      | [doc/vector](doc/vector/vector.md)                | vector/array/initializer_list 文档 |

### 项目工具列表

| 工具名称 | 路径                                               | 工具简介                |
| -------- | -------------------------------------------------- | ----------------------- |
| doctest  | [doctest/doctest](https://github.com/doctest/doctest) | a test framework      |
| xmake    | [xmake-io/xmake](https://github.com/xmake-io/xmake)   | c++ compiler framework |

### 参考资料

|   参考资料名称   |                                路径                                | 参考资料简介                     |
| :--------------: | :----------------------------------------------------------------: | -------------------------------- |
|   cppreference   |            [cppreference](https://en.cppreference.com/w/)            | doc of cpp language              |
|     TinySTL     | [TinySTL](https://github.com/zouxiaohang/TinySTL/tree/master/TinySTL) | a tiny standard template library |
|  Facebook folly  |          [facebook/folly](https://github.com/facebook/folly)          | Facebook folly standard library  |
| 《STL 源码剖析》 |                               ......                               | 侯捷《STL 源码剖析》             |
|  《C++ Primer》  |                               ......                               | 《C++ Primer》                   |

## 更新记录

**2023-5-7** 首次更新，完成 iterator 功能模块，重新整理了项目文档   [git-commit](https://github.com/lovelydayss/mSTL/commit/5e332f85ee5d1d945539f4b8573431a74a81e10e)

**2023-5-24** 完成 vector 功能模块测试   [git-commit](https://github.com/lovelydayss/mSTL/commit/52ea757ae88647d35bb71455a3c6ca7e86b0cf99)

**2023-5-25** 完成 array 功能模块测试   [git-commit](https://github.com/lovelydayss/mSTL/commit/447db84fde564bf13660a8ea49612b8cb1a84728)
