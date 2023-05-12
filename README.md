# mSTL

在 C++ STL 基础上参考 folly 等进行合理增减实现的 C++ 标准模板库，部分保证线程安全

## 支持功能

| 功能/组件        | 路径                                          | 完成情况 | 线程安全支持 | 单元测试 | 性能对比 |
| ---------------- | --------------------------------------------- | :-------: | :----------: | :------: | :------: |
| allocator        | [src/allocator.h](src/allocator.h)               | completed |    ......    |  ......  |  ......  |
| iterator         | [src/iterator.h](src/iterator.h)                 | completed |    ......    |  ......  |  ......  |
| vector           | [src/vector.h](src/vector.h)                     |   to do   |    ......    |  ......  |  ......  |
| construct        | [src/construct.h](src/construct.h)               | completed |    ......    |  ......  |  ......  |
| uninitialized    | [src/uninitialized.h](src/uninitialized.h)       | completed |    ......    |  ......  |  ......  |
| type_traits      | [src/type_traits.h](src/type_traits.h)           | completed |    ......    |  ......  |  ......  |
| algorithm        | [src/algorithm.h](src/algorithm.h)               |   to do   |    ......    |  ......  |  ......  |
| initializer_list | [src/initializer_list.h](src/initializer_list.h) |   to do   |    ......    |  ......  |  ......  |

### 项目代码结构

## 相关文档资料

| 名称        | 简介                               | 路径                                           |
| ----------- | ---------------------------------- | ---------------------------------------------- |
| memory      | C++ 内存管理及 std::alloc 源码解析 | [doc/memory](doc/memory/memory.md)                |
| iterator    | C++ 迭代器原理及源码解析           | [doc/iterator](doc/iterator/iterator.md)          |
| type_traits | C++ 类型萃取原理及应用             | [doc/type_traits](doc/type_traits/type_traits.md) |

## 参考

[cppreference](https://en.cppreference.com/w/)

[TinySTL](https://github.com/zouxiaohang/TinySTL/tree/master/TinySTL)

侯捷《STL 源码剖析》，《C++ Primer》...

## 更新记录

**2023-5-7** 在写完 iterator 后重新整理了项目文档，姑且算作首次更新吧，接下来将完成 vector 功能模块   [git-commit](https://github.com/lovelydayss/mSTL/commit/5e332f85ee5d1d945539f4b8573431a74a81e10e)
