#ifndef ALLOC_H
#define ALLOC_H

#include "basic.h"
#include <cstdlib>

MSTL_NAMESPACE_BEGIN

class alloc {
private:
	enum { ALIGN = 8 }; // 每个链表的间隔，也是内存池块对齐边界
	enum {
		MAX_BYTES = 128
	}; // 内存池块大小上限，超过该大小的区块由 malloc 分配 (16 * 8)
	enum {
		NFREELISTS = MAX_BYTES / ALIGN
	}; // free-lists 结点的个数，每个结点为一个内存池，该内存池块大小逐个增加一个对齐单位
	enum { NOBJS = 20 }; // 默认每次分配增加的节点数
private:
	// bytes 上调至 8 的倍数
	static inline size_t RoundUp(size_t bytes) {
		return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
	}

private:
	// free-list 结点构造
	union obj {
		union obj* next;
	};

private:
	static char* start_free; // 预留内存空间起始位置
	static char* end_free;   // 预留内存空间结束位置
	static size_t heap_size; // 总计使用内存空间大小

	static obj*
	    free_list[NFREELISTS]; // 内存池数组，共组织 NFREELISTS 个不同大小内存池
private:
	// 根据区块大小，计算 freelists 编号
	static inline size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes) + ALIGN - 1) / ALIGN - 1);
	}

	static void* refill(size_t size); // 为链表分配内存并分割挂载
	static char* chunk_alloc(size_t size,
	                         size_t& nobjs); // 为内存池分配一大块内存

public:
	static void* allocate(size_t n);
	static void deallocate(void* p, size_t n);
	static void* reallocate(void* p, size_t old_n, size_t new_n);
};

MSTL_NAMESPACE_END

#endif
