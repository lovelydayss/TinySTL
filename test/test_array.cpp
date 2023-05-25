#include <cstddef>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test_basic.h"

#include "../include/doctest.h"
#include "../src/array.h"

#include <array>
#include <string>

MSTL_TEST_NAMESPACE_BEGIN

template <class T, size_t N>
using m_array = mSTL::array<T, N>;
template <class T, size_t N>
using std_array = std::array<T, N>;
using string = std::string;

TEST_CASE(" construct array<type, 0>") {

	///<- string

	m_array<string, 0>   marray_s{};
	std_array<string, 0> stdarray_s{};

	CHECK_EQ(marray_s.data(), nullptr);
	CHECK_EQ(marray_s.data(), stdarray_s.data());

	CHECK_EQ(marray_s.size(), 0);
	CHECK_EQ(marray_s.max_size(), 0);
	CHECK(marray_s.empty());

	CHECK_EQ(marray_s.size(), stdarray_s.size());
	CHECK_EQ(marray_s.max_size(), stdarray_s.max_size());
	CHECK_EQ(stdarray_s.empty(), marray_s.empty());

	///<- int

	m_array<string, 0>   marray_i{};
	std_array<string, 0> stdarray_i{};

	CHECK_EQ(marray_i.data(), nullptr);
	CHECK_EQ(marray_i.data(), stdarray_i.data());

	CHECK_EQ(marray_i.size(), 0);
	CHECK_EQ(marray_i.max_size(), 0);
	CHECK(marray_i.empty());

	CHECK_EQ(marray_i.size(), stdarray_i.size());
	CHECK_EQ(marray_i.max_size(), stdarray_i.max_size());
	CHECK_EQ(stdarray_i.empty(), marray_i.empty());
}

TEST_CASE(" array<type, N>{}  fill, size, max_size, empty... ") {

	///<- string
	m_array<string, 10>   marray_s{};
	std_array<string, 10> stdarray_s{};

	CHECK_EQ(marray_s.size(), 10);
	CHECK_EQ(marray_s.max_size(), 10);
	CHECK_FALSE(marray_s.empty());

	CHECK_EQ(marray_s.size(), stdarray_s.size());
	CHECK_EQ(marray_s.max_size(), stdarray_s.max_size());
	CHECK_EQ(stdarray_s.empty(), marray_s.empty());

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_s[i], string());

	stdarray_s.fill("dasdasfs");
	marray_s.fill("dasdasfs");

	for (int i = 0; i < 10; i++) {
		CHECK_EQ(marray_s[i], "dasdasfs");
		CHECK_EQ(marray_s[i], stdarray_s[i]);
	}

	CHECK_EQ(marray_s.size(), 10);
	CHECK_EQ(marray_s.max_size(), 10);
	CHECK_FALSE(marray_s.empty());

	CHECK_EQ(marray_s.size(), stdarray_s.size());
	CHECK_EQ(marray_s.max_size(), stdarray_s.max_size());
	CHECK_EQ(stdarray_s.empty(), marray_s.empty());

	///<- int
	m_array<int, 10>   marray_i{};
	std_array<int, 10> stdarray_i{};

	CHECK_EQ(marray_i.size(), 10);
	CHECK_EQ(marray_i.max_size(), 10);
	CHECK_FALSE(marray_i.empty());

	CHECK_EQ(marray_i.size(), stdarray_i.size());
	CHECK_EQ(marray_i.max_size(), stdarray_i.max_size());
	CHECK_EQ(stdarray_i.empty(), marray_i.empty());

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_i[i], 0);

	stdarray_i.fill(12);
	marray_i.fill(12);

	for (int i = 0; i < 10; i++) {
		CHECK_EQ(marray_i[i], 12);
		CHECK_EQ(marray_i[i], stdarray_i[i]);
	}

	CHECK_EQ(marray_i.size(), 10);
	CHECK_EQ(marray_i.max_size(), 10);
	CHECK_FALSE(marray_i.empty());

	CHECK_EQ(marray_i.size(), stdarray_i.size());
	CHECK_EQ(marray_i.max_size(), stdarray_i.max_size());
	CHECK_EQ(stdarray_i.empty(), marray_i.empty());
}

TEST_CASE(" copy construct and operator= ") {

	///<- string

	std_array<string, 10> stdarray_s{};
	m_array<string, 10>   marray_s{};

	stdarray_s.fill("dasdasfs");
	marray_s.fill("dasdasfs");

	///<- copy construct

	// copy
	m_array<string, 10> marray_s_1(marray_s);
	m_array<string, 10> marray_s_2 = marray_s;

	std_array<string, 10> stdarray_s_1(stdarray_s);
	std_array<string, 10> stdarray_s_2 = stdarray_s;

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_1[i], "dasdasfs");
		CHECK_EQ(marray_s_1[i], marray_s[i]);

		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);
	}

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_2[i], "dasdasfs");
		CHECK_EQ(marray_s_2[i], marray_s[i]);

		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);
	}

	// move
	m_array<string, 10> marray_s_3(std::move(marray_s_1));
	m_array<string, 10> marray_s_4 = std::move(marray_s_2);

	std_array<string, 10> stdarray_s_3(std::move(stdarray_s_1));
	std_array<string, 10> stdarray_s_4 = std::move(stdarray_s_2);

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_3[i], "dasdasfs");

		CHECK_EQ(marray_s_3[i], stdarray_s_3[i]);
		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);
	}

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_4[i], "dasdasfs");

		CHECK_EQ(marray_s_4[i], stdarray_s_4[i]);
		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);
	}

	///<- operator=

	// copy
	m_array<string, 10> marray_s_5;
	marray_s_5 = marray_s_3;

	std_array<string, 10> stdarray_s_5;
	stdarray_s_5 = stdarray_s_3;

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_5[i], "dasdasfs");

		CHECK_EQ(marray_s_5[i], stdarray_s_5[i]);
		CHECK_EQ(marray_s_3[i], stdarray_s_3[i]);
	}

	// move
	m_array<string, 10> marray_s_6;
	marray_s_6 = std::move(marray_s_4);

	std_array<string, 10> stdarray_s_6;
	stdarray_s_6 = std::move(stdarray_s_4);

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_5[i], "dasdasfs");

		CHECK_EQ(marray_s_6[i], stdarray_s_6[i]);
		CHECK_EQ(marray_s_4[i], stdarray_s_4[i]);
	}

	///<- int

	std_array<int, 10> stdarray_i{};
	m_array<int, 10>   marray_i{};

	stdarray_i.fill(12);
	marray_i.fill(12);

	///<- copy construct

	// copy
	m_array<int, 10> marray_i_1(marray_i);
	m_array<int, 10> marray_i_2 = marray_i;

	std_array<int, 10> stdarray_i_1(stdarray_i);
	std_array<int, 10> stdarray_i_2 = stdarray_i;

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_1[i], 12);
		CHECK_EQ(marray_i_1[i], marray_i[i]);

		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);
	}

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_2[i], 12);
		CHECK_EQ(marray_i_2[i], marray_i[i]);

		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);
	}

	// move
	m_array<int, 10> marray_i_3(std::move(marray_i_1));
	m_array<int, 10> marray_i_4 = std::move(marray_i_2);

	std_array<int, 10> stdarray_i_3(std::move(stdarray_i_1));
	std_array<int, 10> stdarray_i_4 = std::move(stdarray_i_2);

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_3[i], 12);

		CHECK_EQ(marray_i_3[i], stdarray_i_3[i]);
		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);
	}

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_4[i], 12);

		CHECK_EQ(marray_i_4[i], stdarray_i_4[i]);
		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);
	}

	///<- operator=

	// copy
	m_array<int, 10> marray_i_5;
	marray_i_5 = marray_i_3;

	std_array<int, 10> stdarray_i_5;
	stdarray_i_5 = stdarray_i_3;

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_5[i], 12);

		CHECK_EQ(marray_i_5[i], stdarray_i_5[i]);
		CHECK_EQ(marray_i_3[i], stdarray_i_3[i]);
	}

	// move
	m_array<int, 10> marray_i_6;
	marray_i_6 = std::move(marray_i_4);

	std_array<int, 10> stdarray_i_6;
	stdarray_i_6 = std::move(stdarray_i_4);

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_5[i], 12);

		CHECK_EQ(marray_i_6[i], stdarray_i_6[i]);
		CHECK_EQ(marray_i_4[i], stdarray_i_4[i]);
	}
}

TEST_CASE(" initializer_list ") {

	///<- string

	std_array<string, 10> stdarray_s_1({"a", "b", "c", "d", "e"});
	std_array<string, 10> stdarray_s_2 = {"a", "b", "c", "d", "e"};
	m_array<string, 10>   marray_s_1({"a", "b", "c", "d", "e"});
	m_array<string, 10>   marray_s_2 = {"a", "b", "c", "d", "e"};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);
		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);
	}

	// 此种写法发生二次赋值( 尾部仍然填充 0 )

	marray_s_1 = {"h", "j", "k"};
	stdarray_s_1 = {"h", "j", "k"};
	marray_s_2 = {"h", "j", "k"};
	stdarray_s_2 = {"h", "j", "k"};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);
		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);
	}

	CHECK_EQ(marray_s_1[0], "h");
	CHECK_EQ(marray_s_2[0], "h");

	CHECK_EQ(marray_s_1[4], string());
	CHECK_EQ(marray_s_2[4], string());

	CHECK_EQ(marray_s_1[6], string());
	CHECK_EQ(marray_s_2[6], string());

	///<- int

	std_array<int, 10> stdarray_i_1 = {1, 2, 3, 4, 5};
	std_array<int, 10> stdarray_i_2 = {1, 2, 3, 4, 5};
	m_array<int, 10>   marray_i_1({1, 2, 3, 4, 5});
	m_array<int, 10>   marray_i_2 = {1, 2, 3, 4, 5};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);
		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);
	}

	marray_i_1 = {9, 10, 11};
	stdarray_i_1 = {9, 10, 11};
	marray_i_2 = {9, 10, 11};
	stdarray_i_2 = {9, 10, 11};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);
		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);
	}

	CHECK_EQ(marray_i_1[0], 9);
	CHECK_EQ(marray_i_2[0], 9);

	CHECK_EQ(marray_i_1[4], 0);
	CHECK_EQ(marray_i_2[4], 0);

	CHECK_EQ(marray_i_1[6], 0);
	CHECK_EQ(marray_i_2[6], 0);
}

TEST_CASE(" element access ") {

	///<- string

	std_array<string, 10> stdarray_s = {"a", "b", "c", "d", "e", "f", "g", "h"};
	m_array<string, 10>   marray_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	const std_array<string, 10> c_stdarray_s = {"a", "b", "c", "d",
	                                            "e", "f", "g", "h"};
	const m_array<string, 10>   c_marray_s = {"a", "b", "c", "d",
	                                          "e", "f", "g", "h"};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_s.at(i), stdarray_s.at(i));
		CHECK_EQ(c_marray_s.at(i), c_stdarray_s.at(i));

		CHECK_EQ(marray_s[i], stdarray_s[i]);
		CHECK_EQ(c_marray_s[i], c_stdarray_s[i]);
	}

	CHECK_EQ(marray_s.front(), stdarray_s.front());
	CHECK_EQ(c_marray_s.front(), c_stdarray_s.front());

	CHECK_EQ(marray_s.back(), stdarray_s.back());
	CHECK_EQ(c_marray_s.back(), c_stdarray_s.back());

	CHECK_EQ(*(marray_s.data()), *(stdarray_s.data()));
	CHECK_EQ(*(c_marray_s.data()), *(c_stdarray_s.data()));

	///<- int

	std_array<int, 10> stdarray_i = {1, 2, 3, 4, 5, 6, 7, 8};
	m_array<int, 10>   marray_i = {1, 2, 3, 4, 5, 6, 7, 8};

	const std_array<int, 10> c_stdarray_i = {1, 2, 3, 4, 5, 6, 7, 8};
	const m_array<int, 10>   c_marray_i = {1, 2, 3, 4, 5, 6, 7, 8};

	for (int i = 0; i < 10; ++i) {
		CHECK_EQ(marray_i.at(i), stdarray_i.at(i));
		CHECK_EQ(c_marray_i.at(i), c_stdarray_i.at(i));

		CHECK_EQ(marray_i[i], stdarray_i[i]);
		CHECK_EQ(c_marray_i[i], c_stdarray_i[i]);
	}

	CHECK_EQ(marray_i.front(), stdarray_i.front());
	CHECK_EQ(c_marray_i.front(), c_stdarray_i.front());

	CHECK_EQ(marray_i.back(), stdarray_i.back());
	CHECK_EQ(c_marray_i.back(), c_stdarray_i.back());

	CHECK_EQ(*(marray_i.data()), *(stdarray_i.data()));
	CHECK_EQ(*(c_marray_i.data()), *(c_stdarray_i.data()));
}

TEST_CASE(" iterator ") {

	///<- string

	std_array<string, 10> stdarray_s = {"a", "b", "c", "d", "e", "f", "g", "h"};
	m_array<string, 10>   marray_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	const std_array<string, 10> c_stdarray_s = {"a", "b", "c", "d",
	                                            "e", "f", "g", "h"};
	const m_array<string, 10>   c_marray_s = {"a", "b", "c", "d",
	                                          "e", "f", "g", "h"};

	///<- begin()
	auto m_item_s = marray_s.begin();
	auto std_item_s = stdarray_s.begin();

	for (; m_item_s != marray_s.end(); ++m_item_s, ++std_item_s)
		CHECK_EQ(*m_item_s, *std_item_s);

	///<- cbegin()
	auto c_m_item_s_2 = c_marray_s.cbegin();
	auto c_std_item_s_2 = c_stdarray_s.cbegin();

	for (; c_m_item_s_2 != c_marray_s.cend(); ++c_m_item_s_2, ++c_std_item_s_2)
		CHECK_EQ(*c_m_item_s_2, *c_std_item_s_2);

	///<- rbegin()
	auto rm_item_s = marray_s.rbegin();
	auto rstd_item_s = stdarray_s.rbegin();

	for (; rm_item_s != marray_s.rend(); ++rm_item_s, ++rstd_item_s)
		CHECK_EQ((*rm_item_s), (*rstd_item_s));

	///<- crbegin()
	auto cr_m_item_s_2 = c_marray_s.crbegin();
	auto cr_std_item_s_2 = c_stdarray_s.crbegin();

	for (; cr_m_item_s_2 != c_marray_s.crend();
	     ++cr_m_item_s_2, ++cr_std_item_s_2)
		CHECK_EQ(*(cr_m_item_s_2), *(cr_std_item_s_2));

	///<- int

	std_array<int, 10> stdarray_i = {1, 2, 3, 4, 5, 6, 7, 8};
	m_array<int, 10>   marray_i = {1, 2, 3, 4, 5, 6, 7, 8};

	const std_array<int, 10> c_stdarray_i = {1, 2, 3, 4, 5, 6, 7, 8};
	const m_array<int, 10>   c_marray_i = {1, 2, 3, 4, 5, 6, 7, 8};

	///<- begin()
	auto m_item_i = marray_i.begin();
	auto std_item_i = stdarray_i.begin();

	for (; m_item_i != marray_i.end(); ++m_item_i, ++std_item_i)
		CHECK_EQ(*m_item_i, *std_item_i);

	///<- cbegin()
	auto c_m_item_i_2 = c_marray_i.cbegin();
	auto c_std_item_i_2 = c_stdarray_i.cbegin();

	for (; c_m_item_i_2 != c_marray_i.cend(); ++c_m_item_i_2, ++c_std_item_i_2)
		CHECK_EQ(*c_m_item_i_2, *c_std_item_i_2);

	///<- rbegin()
	auto rm_item_i = marray_i.rbegin();
	auto rstd_item_i = stdarray_i.rbegin();

	for (; rm_item_i != marray_i.rend(); ++rm_item_i, ++rstd_item_i)
		CHECK_EQ(*rm_item_i, *rstd_item_i);

	///<- crbegin()
	auto cr_m_item_i_2 = c_marray_i.crbegin();
	auto cr_std_item_i_2 = c_stdarray_i.crbegin();

	for (; cr_m_item_i_2 != c_marray_i.crend();
	     ++cr_m_item_i_2, ++cr_std_item_i_2)
		CHECK_EQ(*cr_m_item_i_2, *cr_std_item_i_2);
}

TEST_CASE(" swap.. ") {

	///<- string

	std_array<string, 10> stdarray_s_1 = {"a", "b", "c"};
	std_array<string, 10> stdarray_s_2 = {"m", "n"};
	m_array<string, 10>   marray_s_1 = {"a", "b", "c"};
	m_array<string, 10>   marray_s_2 = {"m", "n"};

	stdarray_s_1.swap(stdarray_s_2);
	marray_s_1.swap(marray_s_2);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);

	swap(stdarray_s_1, stdarray_s_2);
	swap(marray_s_1, marray_s_2);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_s_1[i], stdarray_s_1[i]);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_s_2[i], stdarray_s_2[i]);

	///<- int

	std_array<int, 10> stdarray_i_1 = {1, 2, 3};
	std_array<int, 10> stdarray_i_2 = {7, 8};
	m_array<int, 10>   marray_i_1 = {1, 2, 3};
	m_array<int, 10>   marray_i_2 = {7, 8};

	stdarray_i_1.swap(stdarray_i_2);
	marray_i_1.swap(marray_i_2);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);

	swap(stdarray_i_1, stdarray_i_2);
	swap(marray_i_1, marray_i_2);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_i_1[i], stdarray_i_1[i]);

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(marray_i_2[i], stdarray_i_2[i]);
}

TEST_CASE(" compare ") {

	std_array<string, 10> stdarray_s_1 = {"b", "d", "h"};

	std_array<std_array<string, 10>, 10> stdarray_s{{}};

	stdarray_s[0] = {"b", "d", "h"};
	stdarray_s[1] = {"a", "d", "h"};
	stdarray_s[2] = {"c", "d", "h"};
	stdarray_s[3] = {"c", "d"};
	stdarray_s[4] = {"e", "f"};
	stdarray_s[5] = {"c", "d", "a", "a"};
	stdarray_s[6] = {"e", "f", "g", "h"};

	m_array<string, 10> marray_s_1 = {"b", "d", "h"};

	m_array<m_array<string, 10>, 10> marray_s{{}};

	marray_s[0] = {"b", "d", "h"};
	marray_s[1] = {"a", "d", "h"};
	marray_s[2] = {"c", "d", "h"};
	marray_s[3] = {"c", "d"};
	marray_s[4] = {"e", "f"};
	marray_s[5] = {"c", "d", "a", "a"};
	marray_s[6] = {"e", "f", "g", "h"};

	for (int i = 0; i < marray_s.size(); ++i) {
		CHECK_EQ(stdarray_s_1 == stdarray_s[i], marray_s_1 == marray_s[i]);
		CHECK_EQ(stdarray_s_1 != stdarray_s[i], marray_s_1 != marray_s[i]);
		CHECK_EQ(stdarray_s_1 > stdarray_s[i], marray_s_1 > marray_s[i]);
		CHECK_EQ(stdarray_s_1 < stdarray_s[i], marray_s_1 < marray_s[i]);
		CHECK_EQ(stdarray_s_1 >= stdarray_s[i], marray_s_1 >= marray_s[i]);
		CHECK_EQ(stdarray_s_1 <= stdarray_s[i], marray_s_1 <= marray_s[i]);
	}

	///<- int

	std_array<int, 10> stdarray_i_1 = {5, 6, 7};

	std_array<std_array<int, 10>, 10> stdarray_i{{}};
	stdarray_i[0] = {5, 6, 7};
	stdarray_i[1] = {1, 2, 3};
	stdarray_i[2] = {8, 9, 0};
	stdarray_i[3] = {1, 2};
	stdarray_i[4] = {7, 8};
	stdarray_i[5] = {1, 2, 3, 4};
	stdarray_i[6] = {7, 8, 9, 10};

	m_array<int, 10> marray_i_1 = {5, 6, 7};

	m_array<m_array<int, 10>, 10> marray_i{{}};
	marray_i[0] = {5, 6, 7};
	marray_i[1] = {1, 2, 3};
	marray_i[2] = {8, 9, 0};
	marray_i[3] = {1, 2};
	marray_i[4] = {7, 8};
	marray_i[5] = {1, 2, 3, 4};
	marray_i[6] = {7, 8, 9, 10};

	for (int i = 0; i < marray_i.size(); ++i) {
		CHECK_EQ(stdarray_i_1 == stdarray_i[i], marray_i_1 == marray_i[i]);
		CHECK_EQ(stdarray_i_1 != stdarray_i[i], marray_i_1 != marray_i[i]);
		CHECK_EQ(stdarray_i_1 > stdarray_i[i], marray_i_1 > marray_i[i]);
		CHECK_EQ(stdarray_i_1 < stdarray_i[i], marray_i_1 < marray_i[i]);
		CHECK_EQ(stdarray_i_1 >= stdarray_i[i], marray_i_1 >= marray_i[i]);
		CHECK_EQ(stdarray_i_1 <= stdarray_i[i], marray_i_1 <= marray_i[i]);
	}
}

/*
TEST_CASE(" no-member function ") {

	///<- string

	std_array<string, 10> stdarray_s = {"a", "b", "c", "d", "e"};
	m_array<string, 10>   marray_s = {"a", "b", "c"};

	// T&
	m_array<string, 10> ms_s = {
	    mSTL::get<0>(marray_s),
	    mSTL::get<3>(marray_s),
	    mSTL::get<6>(marray_s),
	    mSTL::get<9>(marray_s),
	};

	m_array<string, 10> stds_s = {
	    std::get<0>(stdarray_s),
	    std::get<3>(stdarray_s),
	    std::get<6>(stdarray_s),
	    std::get<9>(stdarray_s),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_s[i], stds_s[i]);

	// 测试编译期行为是否有效
	CHECK_EQ(mSTL::get<0>(marray_s), "a");
	CHECK_EQ(mSTL::get<3>(marray_s), "d");
	CHECK_EQ(mSTL::get<6>(marray_s), string());
	CHECK_EQ(mSTL::get<9>(marray_s), string());

	CHECK_EQ(marray_s[0], "c");
	CHECK_EQ(marray_s[3], "d");
	CHECK_EQ(marray_s[6], "e");
	CHECK_EQ(marray_s[9], "f");

	// 修改
	mSTL::get<0>(marray_s) = "c";
	mSTL::get<3>(marray_s) = "d";
	mSTL::get<6>(marray_s) = "e";
	mSTL::get<9>(marray_s) = "f";

	// 测试编译期行为是否有效
	CHECK_EQ(mSTL::get<0>(marray_s), "c");
	CHECK_EQ(mSTL::get<3>(marray_s), "d");
	CHECK_EQ(mSTL::get<6>(marray_s), "e");
	CHECK_EQ(mSTL::get<9>(marray_s), "f");

	const std_array<string, 10> c_stdarray_s = {"a", "b", "c", "d", "e"};
	const m_array<string, 10>   c_marray_s = {"a", "b", "c"};

	// const T&
	m_array<const string, 10> c_ms_s = {
	    mSTL::get<0>(c_marray_s),
	    mSTL::get<3>(c_marray_s),
	    mSTL::get<6>(c_marray_s),
	    mSTL::get<9>(c_marray_s),
	};

	m_array<const string, 10> c_stds_s = {
	    std::get<0>(c_stdarray_s),
	    std::get<3>(c_stdarray_s),
	    std::get<6>(c_stdarray_s),
	    std::get<9>(c_stdarray_s),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_s[i], stds_s[i]);

	// const T&&
	m_array<string, 10> ms_s_move = {
	    std::move(std::move(mSTL::get<0>(marray_s))),
	    std::move(std::move(mSTL::get<3>(marray_s))),
	    std::move(std::move(mSTL::get<6>(marray_s))),
	    std::move(std::move(mSTL::get<9>(marray_s))),
	};

	m_array<string, 10> stds_s_move = {
	    std::move(std::move(std::get<0>(stdarray_s))),
	    std::move(std::move(std::get<3>(stdarray_s))),
	    std::move(std::move(std::get<6>(stdarray_s))),
	    std::move(std::move(std::get<9>(stdarray_s))),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_s[i], stds_s[i]);

	///<- int

	std_array<int, 10> stdarray_i = {1, 2, 3, 4, 5};
	m_array<int, 10>   marray_i = {1, 2, 3};

	// T&
	m_array<int, 10> ms_i = {
	    mSTL::get<0>(marray_i),
	    mSTL::get<3>(marray_i),
	    mSTL::get<6>(marray_i),
	    mSTL::get<9>(marray_i),
	};

	m_array<int, 10> stds_i = {
	    std::get<0>(stdarray_i),
	    std::get<3>(stdarray_i),
	    std::get<6>(stdarray_i),
	    std::get<9>(stdarray_i),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_i[i], stds_i[i]);

	// 测试编译期行为是否有效
	CHECK_EQ(mSTL::get<0>(marray_i), 1);
	CHECK_EQ(mSTL::get<3>(marray_i), 4);
	CHECK_EQ(mSTL::get<6>(marray_i), 0);
	CHECK_EQ(mSTL::get<9>(marray_i), 0);

	CHECK_EQ(marray_i[0], 9);
	CHECK_EQ(marray_i[3], 10);
	CHECK_EQ(marray_i[6], 11);
	CHECK_EQ(marray_i[9], 12);

	// 修改
	mSTL::get<0>(marray_i) = 9;
	mSTL::get<3>(marray_i) = 10;
	mSTL::get<6>(marray_i) = 11;
	mSTL::get<9>(marray_i) = 12;

	// 测试编译期行为是否有效
	CHECK_EQ(mSTL::get<0>(marray_i), 9);
	CHECK_EQ(mSTL::get<3>(marray_i), 10);
	CHECK_EQ(mSTL::get<6>(marray_i), 11);
	CHECK_EQ(mSTL::get<9>(marray_i), 12);

	const std_array<int, 10> c_stdarray_i = {1, 2, 3, 4, 5};
	const m_array<int, 10>   c_marray_i = {1, 2, 3};

	// const T&
	m_array<int, 10> c_ms_i = {
	    mSTL::get<0>(c_marray_i),
	    mSTL::get<3>(c_marray_i),
	    mSTL::get<6>(c_marray_i),
	    mSTL::get<9>(c_marray_i),
	};

	m_array<int, 10> c_itds_i = {
	    std::get<0>(c_stdarray_i),
	    std::get<3>(c_stdarray_i),
	    std::get<6>(c_stdarray_i),
	    std::get<9>(c_stdarray_i),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_i[i], stds_i[i]);

	// const T&&
	m_array<int, 10> ms_i_move = {
	    std::move(mSTL::get<0>(std::move(marray_i))),
	    std::move(mSTL::get<3>(std::move(marray_i))),
	    std::move(mSTL::get<6>(std::move(marray_i))),
	    std::move(mSTL::get<9>(std::move(marray_i))),
	};

	m_array<int, 10> stds_i_move = {
	    std::move(std::get<0>(std::move(stdarray_i))),
	    std::move(std::get<3>(std::move(stdarray_i))),
	    std::move(std::get<6>(std::move(stdarray_i))),
	    std::move(std::get<9>(std::move(stdarray_i))),
	};

	for (int i = 0; i < 10; i++)
		CHECK_EQ(ms_i[i], stds_i[i]);
}

*/

MSTL_TEST_NAMESPACE_END
