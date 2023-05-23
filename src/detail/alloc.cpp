#include "../alloc.h"

MSTL_NAMESPACE_BEGIN

char* alloc::start_free = nullptr;
char* alloc::end_free = nullptr;
size_t alloc::heap_size = 0;

alloc::obj* alloc::free_list[alloc::NFREELISTS] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

// private

void* alloc::refill(size_t size) {

	// 从此前预留空间里取
	size_t nobjs = NOBJS;
	char* chunk = chunk_alloc(size, nobjs);

	// 取出的空间刚好够一个对象使用
	if (nobjs == 1) {
		return chunk;
	}

	obj** my_free_list = free_list + FREELIST_INDEX(size);
	obj* result = (obj*)(chunk);
	obj* next_obj = *my_free_list = (obj*)(chunk + size);
	obj* current_obj = nullptr;
	int i;

	// 将取出的多余的空间加入到相应的 free list 里面去
	for (i = 1; i < nobjs - 1; i++) {
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + size);

		current_obj->next = next_obj;
	}

	// 最后一个内存块
	current_obj->next = nullptr;

	return result;
	
}

char* alloc::chunk_alloc(size_t size, size_t& nobjs) {
	char* result = nullptr;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;

	// 此前预留空间完全满足需要
	// 分配默认数量个内存块至指定链表
	if (bytes_left >= total_bytes) {
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	// 此前预留空间不能完全满足需要
	// 尽可能多的分配内存块
	else if (bytes_left >= size) {
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	// 此前预留空间小于单个块空间
	// 使用 malloc 重新分配内存
	else {
		// 将此前预留空间作为内存碎片挂载到对应链表中
		if (bytes_left > 0) {
			obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj*)start_free)->next = *my_free_list;
			*my_free_list = (obj*)start_free;
		}

		// 执行空间分配
		// 2 * 当前需要空间 + ((总使用空间)/16,上调至 8 的倍数)
		const size_t bytes_to_get = 2 * total_bytes + RoundUp(heap_size >> 4);
		start_free = (char*)malloc(bytes_to_get);

		// 空间分配失败情况
		if (start_free == nullptr) {
			int i;
			obj** my_free_list = nullptr;
			obj* p = nullptr;

			// 尝试从当前链表位置向后寻找最近的内存块并取出一个作为扩展空间
			// 类似于在内存分块调度中的最优调度算法
			for (i = size; i <= MAX_BYTES; i += ALIGN) {
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;

				// 如果存在可用内存块，递归处理
				if (p != nullptr) {
					*my_free_list = p->next;
					start_free = (char*)p;
					end_free = start_free + i;
					return chunk_alloc(size, nobjs);
				}
			}

			// 进行至此说明没有可分配的内存了
			// 源码中尝试使用其他方法分配，此处直接抛出异常
			// start_free = (char*)malloc_allocate(bytes_to_get);
			throw "out of memory";
			
			// 此时 start_free 和 end_free 均应为 nullptr
			end_free = nullptr;
		}

		// 执行至此说明保留内存空间已经完成扩充
		// 递归调用处理
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc(size, nobjs);
	}
}

// public

void* alloc::allocate(size_t n) {

	// 较大块直接使用 malloc 函数分配
	if (n > static_cast<size_t>(MAX_BYTES))
		return malloc(n);

	// 计算获取分配块位置
	obj** my_free_list = free_list + FREELIST_INDEX(n);
	obj* result = *my_free_list;

	if (result != nullptr) {
		// 此时仍有空间，摘取当前头节点即可
		*my_free_list = (*my_free_list)->next;
		return result;
	} else {
		// 没有足够空间，内存池重新构造
		return refill(RoundUp(n));
	}
}

void alloc::deallocate(void* p, size_t n) {
	// 大的块由于 malloc 直接分配，使用 free 释放即可
	if (n > static_cast<size_t>(MAX_BYTES)) {
		free(p);
		return;
	}

	// 查找对应块链表
	obj** my_free_list = free_list + FREELIST_INDEX(n);
	obj* node = static_cast<obj*>(p);

	// 将空块挂在链表头
	node->next = *my_free_list;
	*my_free_list = node;
	
}

void* alloc::reallocate(void* p, size_t old_n, size_t new_n) {
	deallocate(p, old_n);
	p = allocate(new_n);

	return p;
}

MSTL_NAMESPACE_END