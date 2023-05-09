#include <memory>
#include <vector>
#include <ext/pool_allocator.h>



int main() {

	std::allocator<int> basic_allocator;
	__gnu_cxx::__pool_alloc<int> pool_allocator;

	for (int i = 0; i < 10; i++) {
		std::vector<int, std::allocator<int>> basic_vector(1);
    }

	for (int i = 0; i < 10; i++) {
		std::vector<int, __gnu_cxx::__pool_alloc<int>> pool_vector(1);
	}

	return 0;
}