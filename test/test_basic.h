#ifndef TEST_BASIC_H
#define TEST_BASIC_H

#include <cstddef>

#define MSTL_TEST_NAMESPACE_BEGIN \
	namespace mSTL {              \
	namespace mSTL_test {

#define MSTL_TEST_NAMESPACE_END \
	}                           \
	}

MSTL_TEST_NAMESPACE_BEGIN

class test_base {
public:
	virtual void test_construct();
	virtual void test_element_access();
	virtual void test_iterator();
	virtual void test_capacity();
	virtual void test_modifiers();
	virtual void test_compare();
	virtual void test_non_member_function();

	virtual void performance_comparison();

	virtual void test();
};

MSTL_TEST_NAMESPACE_END

#endif