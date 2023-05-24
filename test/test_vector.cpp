#include <iterator>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "test_basic.h"

#include "../include/doctest.h"
#include "../src/vector.h"

#include <string>
#include <vector>

MSTL_TEST_NAMESPACE_BEGIN

template <class T>
using m_vector = mSTL::vector<T>;
template <class T>
using std_vector = std::vector<T>;
using string = std::string;

TEST_CASE(" test construct vector()") {

	m_vector<string>   empty_m;
	std_vector<string> empty;

	CHECK_EQ(empty_m.data(), nullptr);
	CHECK_EQ(empty_m.data(), empty.data());

	CHECK_EQ(empty_m.size(), empty.size());
	CHECK_EQ(empty_m.capacity(), empty.capacity());

	CHECK_EQ(empty.capacity(), 0);
	CHECK_EQ(empty.size(), 0);
}

TEST_CASE(" test vector(const size_type count, value_type()) ") {

	///<- string
	m_vector<string>   mvec_s(10);
	std_vector<string> stdvec_s(10);

	CHECK_EQ(mvec_s.size(), 10);
	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	for (int i = 0; i < mvec_s.size(); ++i)
		CHECK_EQ(mvec_s[i], string());

	// zero 情况
	m_vector<string>   mvec_s_zero(0);
	std_vector<string> stdvec_s_zero(0);

	CHECK_EQ(mvec_s_zero.size(), 0);
	CHECK_EQ(mvec_s_zero.size(), stdvec_s_zero.size());
	CHECK_EQ(mvec_s_zero.capacity(), stdvec_s_zero.capacity());

	///<- int
	m_vector<int>   mvec_i(10);
	std_vector<int> stdvec_i(10);

	CHECK_EQ(mvec_i.size(), 10);
	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	for (int i = 0; i < mvec_i.size(); ++i)
		CHECK_EQ(mvec_i[i], 0);

	// zero 情况
	m_vector<int>   mvec_i_zero(0);
	std_vector<int> stdvec_i_zero(0);

	CHECK_EQ(mvec_i_zero.size(), 0);
	CHECK_EQ(mvec_i_zero.size(), stdvec_i_zero.size());
	CHECK_EQ(mvec_i_zero.capacity(), stdvec_i_zero.capacity());
}

TEST_CASE(" test vector(const size_type count, const_reference value) ") {

	// string
	m_vector<string>   mvec_s(20, "dasdasfs");
	std_vector<string> stdvec_s(20, "dasdasfs");

	CHECK_EQ(mvec_s.size(), 20);
	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	for (int i = 0; i < mvec_s.size(); ++i) {
		CHECK_EQ(mvec_s[i], "dasdasfs");
		CHECK_EQ(mvec_s[i], stdvec_s[i]);
	}

	// int
	m_vector<double>   mvec_i(static_cast<size_t>(20), 12);
	std_vector<double> stdvec_i(20, 12);

	CHECK_EQ(mvec_i.size(), 20);
	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	for (int i = 0; i < mvec_i.size(); ++i) {
		CHECK_EQ(mvec_i[i], 12);
		CHECK_EQ(mvec_i[i], stdvec_i[i]);
	}
}

TEST_CASE(" vector(InputIterator first, InputIterator last) ") {

	///<- string
	std_vector<string> stdvec_s(10, "dasdasfs");
	m_vector<string>   mvec_s_1(stdvec_s.begin(), stdvec_s.end());
	m_vector<string>   mvec_s_2(mvec_s_1.begin(), mvec_s_1.end());

	CHECK_EQ(mvec_s_1.size(), 10);
	CHECK_EQ(mvec_s_2.size(), 10);
	CHECK_EQ(mvec_s_1.size(), stdvec_s.size());
	CHECK_EQ(mvec_s_2.size(), mvec_s_1.size());
	CHECK_EQ(mvec_s_1.capacity(), stdvec_s.capacity());
	CHECK_EQ(mvec_s_2.capacity(), mvec_s_1.capacity());

	for (int i = 0; i < mvec_s_1.size(); ++i) {
		CHECK_EQ(mvec_s_1[i], "dasdasfs");
		CHECK_EQ(mvec_s_1[i], stdvec_s[i]);
	}

	for (int i = 0; i < mvec_s_2.size(); ++i) {
		CHECK_EQ(mvec_s_2[i], "dasdasfs");
		CHECK_EQ(mvec_s_2[i], mvec_s_1[i]);
	}

	///<- int
	std_vector<int> stdvec_i(10, 12);
	m_vector<int>   mvec_i_1(stdvec_i.begin(), stdvec_i.end());
	m_vector<int>   mvec_i_2(mvec_i_1.begin(), mvec_i_1.end());

	CHECK_EQ(mvec_i_1.size(), 10);
	CHECK_EQ(mvec_i_2.size(), 10);
	CHECK_EQ(mvec_i_1.size(), stdvec_i.size());
	CHECK_EQ(mvec_i_2.size(), mvec_i_1.size());
	CHECK_EQ(mvec_i_1.capacity(), stdvec_i.capacity());
	CHECK_EQ(mvec_i_2.capacity(), mvec_i_1.capacity());

	for (int i = 0; i < mvec_i_1.size(); ++i) {
		CHECK_EQ(mvec_i_1[i], 12);
		CHECK_EQ(mvec_i_1[i], stdvec_i[i]);
	}

	for (int i = 0; i < mvec_i_2.size(); ++i) {
		CHECK_EQ(mvec_i_2[i], 12);
		CHECK_EQ(mvec_i_2[i], mvec_i_1[i]);
	}
}

TEST_CASE(" copy contruct and operator= ") {

	///<- string

	std_vector<string> stdvec_s(10, "dasdasfs");

	///<- copy construct

	// copy
	m_vector<string> mvec_s(stdvec_s.begin(), stdvec_s.end());
	m_vector<string> mvec_s_1(mvec_s);
	m_vector<string> mvec_s_2 = mvec_s;

	CHECK_EQ(mvec_s_1.size(), 10);
	CHECK_EQ(mvec_s_2.size(), 10);
	CHECK_EQ(mvec_s_1.size(), mvec_s.size());
	CHECK_EQ(mvec_s_2.size(), mvec_s.size());
	CHECK_EQ(mvec_s_1.capacity(), mvec_s.capacity());
	CHECK_EQ(mvec_s_2.capacity(), mvec_s.capacity());

	for (int i = 0; i < mvec_s_1.size(); ++i) {
		CHECK_EQ(mvec_s_1[i], "dasdasfs");
		CHECK_EQ(mvec_s_1[i], mvec_s[i]);
	}

	for (int i = 0; i < mvec_s_2.size(); ++i) {
		CHECK_EQ(mvec_s_2[i], "dasdasfs");
		CHECK_EQ(mvec_s_2[i], mvec_s[i]);
	}

	// move
	m_vector<string> mvec_s_3(std::move(mvec_s_1));
	m_vector<string> mvec_s_4 = std::move(mvec_s_2);

	CHECK_EQ(mvec_s_3.size(), 10);
	CHECK_EQ(mvec_s_4.size(), 10);
	CHECK_EQ(mvec_s_3.capacity(), mvec_s.capacity());
	CHECK_EQ(mvec_s_4.capacity(), mvec_s.capacity());

	CHECK_EQ(mvec_s_1.size(), 0);
	CHECK_EQ(mvec_s_2.size(), 0);
	CHECK_EQ(mvec_s_1.capacity(), 0);
	CHECK_EQ(mvec_s_2.capacity(), 0);
	CHECK_EQ(mvec_s_1.data(), nullptr);
	CHECK_EQ(mvec_s_2.data(), nullptr);

	for (int i = 0; i < mvec_s_3.size(); ++i) {
		CHECK_EQ(mvec_s_3[i], "dasdasfs");
		CHECK_EQ(mvec_s_3[i], mvec_s[i]);
	}

	for (int i = 0; i < mvec_s_4.size(); ++i) {
		CHECK_EQ(mvec_s_4[i], "dasdasfs");
		CHECK_EQ(mvec_s_4[i], mvec_s[i]);
	}

	///<- operator=

	// copy
	m_vector<string> mvec_s_5(1, "ok");
	mvec_s_5 = mvec_s_3;

	CHECK_EQ(mvec_s_5.size(), 10);
	CHECK_EQ(mvec_s_5.size(), mvec_s_3.size());
	CHECK_EQ(mvec_s_5.capacity(), mvec_s_3.capacity());

	for (int i = 0; i < mvec_s_5.size(); ++i) {
		CHECK_EQ(mvec_s_5[i], "dasdasfs");
		CHECK_EQ(mvec_s_5[i], mvec_s_3[i]);
	}

	// move
	m_vector<string> mvec_s_6(1, "ok");
	mvec_s_6 = std::move(mvec_s_4);

	CHECK_EQ(mvec_s_6.size(), 10);
	CHECK_EQ(mvec_s_6.capacity(), mvec_s.capacity());

	CHECK_EQ(mvec_s_4.size(), 0);
	CHECK_EQ(mvec_s_4.capacity(), 0);
	CHECK_EQ(mvec_s_4.data(), nullptr);

	for (int i = 0; i < mvec_s_6.size(); ++i) {
		CHECK_EQ(mvec_s_6[i], "dasdasfs");
		CHECK_EQ(mvec_s_6[i], mvec_s[i]);
	}

	///<- int
	std_vector<int> stdvec_i(10, 12);

	///<- copy construct

	// copy
	m_vector<int> mvec_i(stdvec_i.begin(), stdvec_i.end());
	m_vector<int> mvec_i_1(mvec_i);
	m_vector<int> mvec_i_2 = mvec_i;

	CHECK_EQ(mvec_i_1.size(), 10);
	CHECK_EQ(mvec_i_2.size(), 10);
	CHECK_EQ(mvec_i_1.size(), mvec_i.size());
	CHECK_EQ(mvec_i_2.size(), mvec_i.size());
	CHECK_EQ(mvec_i_1.capacity(), mvec_i.capacity());
	CHECK_EQ(mvec_i_2.capacity(), mvec_i.capacity());

	for (int i = 0; i < mvec_i_1.size(); ++i) {
		CHECK_EQ(mvec_i_1[i], 12);
		CHECK_EQ(mvec_i_1[i], mvec_i[i]);
	}

	for (int i = 0; i < mvec_i_2.size(); ++i) {
		CHECK_EQ(mvec_i_2[i], 12);
		CHECK_EQ(mvec_i_2[i], mvec_i[i]);
	}

	// move
	m_vector<int> mvec_i_3(std::move(mvec_i_1));
	m_vector<int> mvec_i_4 = std::move(mvec_i_2);

	CHECK_EQ(mvec_i_3.size(), 10);
	CHECK_EQ(mvec_i_4.size(), 10);
	CHECK_EQ(mvec_i_3.capacity(), mvec_i.capacity());
	CHECK_EQ(mvec_i_4.capacity(), mvec_i.capacity());

	CHECK_EQ(mvec_i_1.size(), 0);
	CHECK_EQ(mvec_i_2.size(), 0);
	CHECK_EQ(mvec_i_1.capacity(), 0);
	CHECK_EQ(mvec_i_2.capacity(), 0);
	CHECK_EQ(mvec_i_1.data(), nullptr);
	CHECK_EQ(mvec_i_2.data(), nullptr);

	for (int i = 0; i < mvec_i_3.size(); ++i) {
		CHECK_EQ(mvec_i_3[i], 12);
		CHECK_EQ(mvec_i_3[i], mvec_i[i]);
	}

	for (int i = 0; i < mvec_i_4.size(); ++i) {
		CHECK_EQ(mvec_i_4[i], 12);
		CHECK_EQ(mvec_i_4[i], mvec_i[i]);
	}

	///<- operator=

	// copy
	m_vector<int> mvec_i_5(static_cast<size_t>(1), 12);
	mvec_i_5 = mvec_i_3;

	CHECK_EQ(mvec_i_5.size(), 10);
	CHECK_EQ(mvec_i_5.size(), mvec_i_3.size());
	CHECK_EQ(mvec_i_5.capacity(), mvec_i_3.capacity());

	for (int i = 0; i < mvec_i_5.size(); ++i) {
		CHECK_EQ(mvec_i_5[i], 12);
		CHECK_EQ(mvec_i_5[i], mvec_i_3[i]);
	}

	// move
	m_vector<int> mvec_i_6(static_cast<size_t>(1), 12);
	mvec_i_6 = std::move(mvec_i_4);

	CHECK_EQ(mvec_i_6.size(), 10);
	CHECK_EQ(mvec_i_6.capacity(), mvec_i.capacity());

	CHECK_EQ(mvec_i_4.size(), 0);
	CHECK_EQ(mvec_i_4.capacity(), 0);
	CHECK_EQ(mvec_i_4.data(), nullptr);

	for (int i = 0; i < mvec_i_6.size(); ++i) {
		CHECK_EQ(mvec_i_6[i], 12);
		CHECK_EQ(mvec_i_6[i], mvec_i[i]);
	}
}

TEST_CASE(" initializer_list ") {

	///<- string
	std_vector<string> stdvec_s = {"a", "b", "c", "d", "e"};
	m_vector<string>   mvec_s_1({"a", "b", "c", "d", "e"});
	m_vector<string>   mvec_s_2 = {"a", "b", "c", "d", "e"};

	CHECK_EQ(mvec_s_1.size(), 5);
	CHECK_EQ(mvec_s_2.size(), 5);
	CHECK_EQ(mvec_s_1.size(), stdvec_s.size());
	CHECK_EQ(mvec_s_2.size(), stdvec_s.size());
	CHECK_EQ(mvec_s_1.capacity(), stdvec_s.capacity());
	CHECK_EQ(mvec_s_2.capacity(), stdvec_s.capacity());

	for (int i = 0; i < mvec_s_1.size(); ++i) {
		CHECK_EQ(mvec_s_1[i], stdvec_s[i]);
	}

	for (int i = 0; i < mvec_s_2.size(); ++i) {
		CHECK_EQ(mvec_s_2[i], stdvec_s[i]);
	}

	///<- int
	std_vector<int> stdvec_i = {1, 2, 3, 4, 5};
	m_vector<int>   mvec_i_1({1, 2, 3, 4, 5});
	m_vector<int>   mvec_i_2 = {1, 2, 3, 4, 5};

	CHECK_EQ(mvec_i_1.size(), 5);
	CHECK_EQ(mvec_i_2.size(), 5);
	CHECK_EQ(mvec_i_1.size(), stdvec_i.size());
	CHECK_EQ(mvec_i_2.size(), stdvec_i.size());
	CHECK_EQ(mvec_i_1.capacity(), stdvec_i.capacity());
	CHECK_EQ(mvec_i_2.capacity(), stdvec_i.capacity());

	for (int i = 0; i < mvec_i_1.size(); ++i) {
		CHECK_EQ(mvec_i_1[i], stdvec_i[i]);
	}

	for (int i = 0; i < mvec_i_2.size(); ++i) {
		CHECK_EQ(mvec_i_2[i], stdvec_i[i]);
	}
}

TEST_CASE(" assign ") {

	///<- string

	std_vector<string> stdvec_s(5, "dasdasfs");

	m_vector<string> mvec_s_1(1, "ok");
	m_vector<string> mvec_s_2(1, "ok");
	m_vector<string> mvec_s_3(1, "ok");

	mvec_s_1.assign(5, "dasdasfs");
	mvec_s_2.assign(stdvec_s.begin(), stdvec_s.end());
	mvec_s_3.assign(
	    {"dasdasfs", "dasdasfs", "dasdasfs", "dasdasfs", "dasdasfs"});

	CHECK_EQ(mvec_s_1.size(), 5);
	CHECK_EQ(mvec_s_2.size(), 5);
	CHECK_EQ(mvec_s_3.size(), 5);
	CHECK_EQ(mvec_s_1.capacity(), 5);
	CHECK_EQ(mvec_s_2.capacity(), 5);
	CHECK_EQ(mvec_s_3.capacity(), 5);

	for (int i = 0; i < mvec_s_1.size(); ++i) {
		CHECK_EQ(mvec_s_1[i], "dasdasfs");
	}

	for (int i = 0; i < mvec_s_2.size(); ++i) {
		CHECK_EQ(mvec_s_2[i], "dasdasfs");
	}

	for (int i = 0; i < mvec_s_3.size(); ++i) {
		CHECK_EQ(mvec_s_3[i], "dasdasfs");
	}

	std_vector<string> stdvec_s_1(5, "aaaaa");

	mvec_s_1.assign(2, "aaaaa");
	mvec_s_2.assign(stdvec_s_1.begin(), stdvec_s_1.begin() + 2);
	mvec_s_3.assign({"aaaaa", "aaaaa"});

	CHECK_EQ(mvec_s_1.size(), 2);
	CHECK_EQ(mvec_s_2.size(), 2);
	CHECK_EQ(mvec_s_3.size(), 2);
	CHECK_EQ(mvec_s_1.capacity(), 5);
	CHECK_EQ(mvec_s_2.capacity(), 5);
	CHECK_EQ(mvec_s_3.capacity(), 5);

	for (int i = 0; i < mvec_s_1.size(); ++i) {
		CHECK_EQ(mvec_s_1[i], "aaaaa");
	}

	for (int i = 0; i < mvec_s_2.size(); ++i) {
		CHECK_EQ(mvec_s_2[i], "aaaaa");
	}

	for (int i = 0; i < mvec_s_3.size(); ++i) {
		CHECK_EQ(mvec_s_3[i], "aaaaa");
	}

	///<- int
	std_vector<int> stdvec_i(5, 12);

	m_vector<int> mvec_i_1(static_cast<size_t>(1), 0);
	m_vector<int> mvec_i_2(static_cast<size_t>(1), 0);
	m_vector<int> mvec_i_3(static_cast<size_t>(1), 0);

	mvec_i_1.assign(static_cast<size_t>(5), 12);
	mvec_i_2.assign(stdvec_i.begin(), stdvec_i.end());
	mvec_i_3.assign({12, 12, 12, 12, 12});

	CHECK_EQ(mvec_i_1.size(), 5);
	CHECK_EQ(mvec_i_2.size(), 5);
	CHECK_EQ(mvec_i_3.size(), 5);
	CHECK_EQ(mvec_i_1.capacity(), 5);
	CHECK_EQ(mvec_i_2.capacity(), 5);
	CHECK_EQ(mvec_i_3.capacity(), 5);

	for (int i = 0; i < mvec_i_1.size(); ++i) {
		CHECK_EQ(mvec_i_1[i], 12);
	}

	for (int i = 0; i < mvec_i_2.size(); ++i) {
		CHECK_EQ(mvec_i_2[i], 12);
	}

	for (int i = 0; i < mvec_i_3.size(); ++i) {
		CHECK_EQ(mvec_i_3[i], 12);
	}

	std_vector<int> stdvec_i_1(5, 10);

	mvec_i_1.assign(static_cast<size_t>(2), 10);
	mvec_i_2.assign(stdvec_i_1.begin(), stdvec_i_1.begin() + 2);
	mvec_i_3.assign({10, 10});

	CHECK_EQ(mvec_i_1.size(), 2);
	CHECK_EQ(mvec_i_2.size(), 2);
	CHECK_EQ(mvec_i_3.size(), 2);
	CHECK_EQ(mvec_i_1.capacity(), 5);
	CHECK_EQ(mvec_i_2.capacity(), 5);
	CHECK_EQ(mvec_i_3.capacity(), 5);

	for (int i = 0; i < mvec_i_1.size(); ++i) {
		CHECK_EQ(mvec_i_1[i], 10);
	}

	for (int i = 0; i < mvec_i_2.size(); ++i) {
		CHECK_EQ(mvec_i_2[i], 10);
	}

	for (int i = 0; i < mvec_i_3.size(); ++i) {
		CHECK_EQ(mvec_i_3[i], 10);
	}
}

TEST_CASE(" element access ") {

	///<- string

	std_vector<string> stdvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};
	m_vector<string>   mvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	const std_vector<string> c_stdvec_s = {"a", "b", "c", "d",
	                                       "e", "f", "g", "h"};
	const m_vector<string> c_mvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	for (int i = 0; i < mvec_s.size(); ++i) {
		CHECK_EQ(mvec_s.at(i), stdvec_s.at(i));
		CHECK_EQ(c_mvec_s.at(i), c_stdvec_s.at(i));

		CHECK_EQ(mvec_s[i], stdvec_s[i]);
		CHECK_EQ(c_mvec_s[i], c_stdvec_s[i]);
	}

	CHECK_EQ(mvec_s.front(), stdvec_s.front());
	CHECK_EQ(c_mvec_s.front(), c_stdvec_s.front());

	CHECK_EQ(mvec_s.back(), stdvec_s.back());
	CHECK_EQ(c_mvec_s.back(), c_stdvec_s.back());

	CHECK_EQ(*(mvec_s.data()), *(stdvec_s.data()));
	CHECK_EQ(*(c_mvec_s.data()), *(c_stdvec_s.data()));

	///<- int

	std_vector<int> stdvec_i = {1, 2, 3, 4, 5, 6, 7, 8};
	m_vector<int>   mvec_i = {1, 2, 3, 4, 5, 6, 7, 8};

	const std_vector<int> c_stdvec_i = {1, 2, 3, 4, 5, 6, 7, 8};
	const m_vector<int>   c_mvec_i = {1, 2, 3, 4, 5, 6, 7, 8};

	for (int i = 0; i < mvec_i.size(); ++i) {
		CHECK_EQ(mvec_i.at(i), stdvec_i.at(i));
		CHECK_EQ(c_mvec_i.at(i), c_stdvec_i.at(i));

		CHECK_EQ(mvec_i[i], stdvec_i[i]);
		CHECK_EQ(c_mvec_i[i], c_stdvec_i[i]);
	}

	CHECK_EQ(mvec_i.front(), stdvec_i.front());
	CHECK_EQ(c_mvec_i.front(), c_stdvec_i.front());

	CHECK_EQ(mvec_i.back(), stdvec_i.back());
	CHECK_EQ(c_mvec_i.back(), c_stdvec_i.back());

	CHECK_EQ(*(mvec_i.data()), *(stdvec_i.data()));
	CHECK_EQ(*(c_mvec_i.data()), *(c_stdvec_i.data()));
}

TEST_CASE(" iterator ") {

	///<- string

	std_vector<string> stdvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};
	m_vector<string>   mvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	const std_vector<string> c_stdvec_s = {"a", "b", "c", "d",
	                                       "e", "f", "g", "h"};
	const m_vector<string> c_mvec_s = {"a", "b", "c", "d", "e", "f", "g", "h"};

	///<- begin()
	auto m_item_s = mvec_s.begin();
	auto std_item_s = stdvec_s.begin();

	for (; m_item_s != mvec_s.end(); ++m_item_s, ++std_item_s)
		CHECK_EQ(*m_item_s, *std_item_s);

	///<- begin() const
	auto c_m_item_s_1 = c_mvec_s.begin();
	auto c_std_item_s_1 = c_stdvec_s.begin();

	for (; c_m_item_s_1 != c_mvec_s.end(); ++c_m_item_s_1, ++c_std_item_s_1)
		CHECK_EQ(*c_m_item_s_1, *c_std_item_s_1);

	///<- cbegin()
	auto c_m_item_s_2 = c_mvec_s.cbegin();
	auto c_std_item_s_2 = c_stdvec_s.cbegin();

	for (; c_m_item_s_2 != c_mvec_s.cend(); ++c_m_item_s_2, ++c_std_item_s_2)
		CHECK_EQ(*c_m_item_s_2, *c_std_item_s_2);

	///<- rbegin()
	auto rm_item_s = mvec_s.rbegin();
	auto rstd_item_s = stdvec_s.rbegin();

	for (; rm_item_s != mvec_s.rend(); ++rm_item_s, ++rstd_item_s)
		CHECK_EQ((*rm_item_s), (*rstd_item_s));

	///<- rbegin() const
	auto cr_m_item_s_1 = c_mvec_s.rbegin();
	auto cr_std_item_s_1 = c_stdvec_s.rbegin();

	for (; cr_m_item_s_1 != c_mvec_s.rend(); ++cr_m_item_s_1, ++cr_std_item_s_1)
		CHECK_EQ(*(cr_m_item_s_1), *(cr_std_item_s_1));

	///<- crbegin()
	auto cr_m_item_s_2 = c_mvec_s.crbegin();
	auto cr_std_item_s_2 = c_stdvec_s.crbegin();

	for (; cr_m_item_s_2 != c_mvec_s.crend();
	     ++cr_m_item_s_2, ++cr_std_item_s_2)
		CHECK_EQ(*(cr_m_item_s_2), *(cr_std_item_s_2));

	///<- int

	std_vector<int> stdvec_i = {1, 2, 3, 4, 5, 6, 7, 8};
	m_vector<int>   mvec_i = {1, 2, 3, 4, 5, 6, 7, 8};

	const std_vector<int> c_stdvec_i = {1, 2, 3, 4, 5, 6, 7, 8};
	const m_vector<int>   c_mvec_i = {1, 2, 3, 4, 5, 6, 7, 8};

	///<- begin()
	auto m_item_i = mvec_i.begin();
	auto std_item_i = stdvec_i.begin();

	for (; m_item_i != mvec_i.end(); ++m_item_i, ++std_item_i)
		CHECK_EQ(*m_item_i, *std_item_i);

	///<- begin() const
	auto c_m_item_i_1 = c_mvec_i.begin();
	auto c_std_item_i_1 = c_stdvec_i.begin();

	for (; c_m_item_i_1 != c_mvec_i.end(); ++c_m_item_i_1, ++c_std_item_i_1)
		CHECK_EQ(*c_m_item_i_1, *c_std_item_i_1);

	///<- cbegin()
	auto c_m_item_i_2 = c_mvec_i.cbegin();
	auto c_std_item_i_2 = c_stdvec_i.cbegin();

	for (; c_m_item_i_2 != c_mvec_i.cend(); ++c_m_item_i_2, ++c_std_item_i_2)
		CHECK_EQ(*c_m_item_i_2, *c_std_item_i_2);

	///<- rbegin()
	auto rm_item_i = mvec_i.rbegin();
	auto rstd_item_i = stdvec_i.rbegin();

	for (; rm_item_i != mvec_i.rend(); ++rm_item_i, ++rstd_item_i)
		CHECK_EQ(*rm_item_i, *rstd_item_i);

	///<- rbegin() const
	auto cr_m_item_i_1 = c_mvec_i.rbegin();
	auto cr_std_item_i_1 = c_stdvec_i.rbegin();

	for (; cr_m_item_i_1 != c_mvec_i.rend(); ++cr_m_item_i_1, ++cr_std_item_i_1)
		CHECK_EQ(*cr_m_item_i_1, *cr_std_item_i_1);

	///<- crbegin()
	auto cr_m_item_i_2 = c_mvec_i.crbegin();
	auto cr_std_item_i_2 = c_stdvec_i.crbegin();

	for (; cr_m_item_i_2 != c_mvec_i.crend();
	     ++cr_m_item_i_2, ++cr_std_item_i_2)
		CHECK_EQ(*cr_m_item_i_2, *cr_std_item_i_2);
}

TEST_CASE(" capacity.. and clear ") {

	///<- string

	std_vector<string> stdvec_s;
	m_vector<string>   mvec_s;

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());
	CHECK(mvec_s.empty());

	// max_size ....

	for (int i = 0; i <= 100; i += 10) {
		stdvec_s.reserve(i);
		mvec_s.reserve(i);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		CHECK_EQ(mvec_s.capacity(), i);
		CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	for (int i = 200; i <= 400; i += 40) {
		stdvec_s.reserve(i);
		mvec_s.reserve(i);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		CHECK_EQ(mvec_s.capacity(), i);
		CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	for (int i = 0; i <= 200; i += 50) {
		stdvec_s.reserve(i);
		mvec_s.reserve(i);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		CHECK_EQ(mvec_s.capacity(), 400);
		CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	stdvec_s.shrink_to_fit();
	mvec_s.shrink_to_fit();
	CHECK_EQ(mvec_s.capacity(), mvec_s.size());
	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	stdvec_s.clear();
	mvec_s.clear();

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	stdvec_s = {"a", "b", "c", "d", "e"};
	mvec_s = {"a", "b", "c", "d", "e"};

	stdvec_s.reserve(20);
	mvec_s.reserve(20);

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	for (int i = 0; i < mvec_s.size(); ++i)
		CHECK_EQ(stdvec_s[i], mvec_s[i]);

	stdvec_s.shrink_to_fit();
	mvec_s.shrink_to_fit();

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	for (int i = 0; i < mvec_s.size(); ++i)
		CHECK_EQ(stdvec_s[i], mvec_s[i]);

	stdvec_s.clear();
	mvec_s.clear();

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(mvec_s.capacity(), stdvec_s.capacity());

	///<- int

	std_vector<int> stdvec_i;
	m_vector<int>   mvec_i;

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());
	CHECK(mvec_i.empty());

	// max_size ....

	for (int i = 0; i <= 100; i += 10) {
		stdvec_i.reserve(i);
		mvec_i.reserve(i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		CHECK_EQ(mvec_i.capacity(), i);
		CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	for (int i = 200; i <= 400; i += 40) {
		stdvec_i.reserve(i);
		mvec_i.reserve(i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		CHECK_EQ(mvec_i.capacity(), i);
		CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	for (int i = 0; i <= 200; i += 50) {
		stdvec_i.reserve(i);
		mvec_i.reserve(i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		CHECK_EQ(mvec_i.capacity(), 400);
		CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	stdvec_i.shrink_to_fit();
	mvec_i.shrink_to_fit();
	CHECK_EQ(mvec_i.capacity(), mvec_i.size());
	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	stdvec_i.clear();
	mvec_i.clear();

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	stdvec_i = {1, 2, 3, 4, 5};
	mvec_i = {1, 2, 3, 4, 5};

	stdvec_i.reserve(20);
	mvec_i.reserve(20);

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	for (int i = 0; i < mvec_i.size(); ++i)
		CHECK_EQ(stdvec_i[i], mvec_i[i]);

	stdvec_i.shrink_to_fit();
	mvec_i.shrink_to_fit();

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());

	for (int i = 0; i < mvec_i.size(); ++i)
		CHECK_EQ(stdvec_i[i], mvec_i[i]);

	stdvec_i.clear();
	mvec_i.clear();

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
	CHECK_EQ(mvec_i.capacity(), stdvec_i.capacity());
}

TEST_CASE(" push_back && pop_back && emplace_back && resize ") {

	///<- string
	std_vector<string> stdvec_s;
	m_vector<string>   mvec_s;

	// push_back && pop_back
	for (int i = 0; i < 10; ++i) {
		stdvec_s.push_back("assdfgg");
		mvec_s.push_back("assdfgg");

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	string tmp_s = "assdfgg";

	for (int i = 0; i < 10; ++i) {
		stdvec_s.push_back(tmp_s);
		mvec_s.push_back(tmp_s);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	for (int i = 0; i < 20; ++i) {
		stdvec_s.pop_back();
		mvec_s.pop_back();

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	CHECK(mvec_s.empty());

	// emplace_back
	for (int i = 0; i < 20; ++i) {
		stdvec_s.emplace_back(string("assdfgg"));
		mvec_s.emplace_back(string("assdfgg"));

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	// resize
	for (int i = 20; i <= 100; i += 10) {
		stdvec_s.resize(i);
		mvec_s.resize(i);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		for (int j = 0; j < mvec_s.size(); j += 10)
			CHECK_EQ(mvec_s[j], stdvec_s[j]);
	}

	for (int i = 100; i >= 0; i -= 20) {
		stdvec_s.resize(i);
		mvec_s.resize(i);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		for (int j = 0; j < mvec_s.size(); j += 10)
			CHECK_EQ(mvec_s[j], stdvec_s[j]);
	}

	CHECK(mvec_s.empty());

	///<- int

	std_vector<int> stdvec_i;
	m_vector<int>   mvec_i;

	// push_back && pop_back
	for (int i = 0; i < 10; ++i) {
		stdvec_i.push_back(12);
		mvec_i.push_back(12);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	int tmp_i = 12;

	for (int i = 0; i < 10; ++i) {
		stdvec_i.push_back(tmp_i);
		mvec_i.push_back(tmp_i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	CHECK_EQ(mvec_i.size(), 20);

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	for (int i = 0; i < 20; ++i) {
		stdvec_i.pop_back();
		mvec_i.pop_back();

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	CHECK(mvec_i.empty());

	// emplace_back
	for (int i = 0; i < 20; ++i) {
		stdvec_i.emplace_back();
		mvec_i.emplace_back();

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	// resize
	for (int i = 20; i <= 100; i += 10) {
		stdvec_i.resize(i);
		mvec_i.resize(i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		for (int j = 0; j < mvec_i.size(); j += 10)
			CHECK_EQ(mvec_i[j], stdvec_i[j]);
	}

	for (int i = 100; i >= 0; i -= 20) {
		stdvec_i.resize(i);
		mvec_i.resize(i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		for (int j = 0; j < mvec_i.size(); j += 10)
			CHECK_EQ(mvec_i[j], stdvec_i[j]);
	}

	CHECK(mvec_i.empty());
}

TEST_CASE(" insert && earse && emplace ") {

	///<- string

	std_vector<string> stdvec_s = {"a", "b", "c", "d", "e"};
	m_vector<string>   mvec_s = {"a", "b", "c", "d", "e"};

	for (int i = 0; i < 5; ++i) {
		auto t_s_1 = stdvec_s.insert((stdvec_s.begin() + i), "f");
		auto t_s_2 = mvec_s.insert((mvec_s.begin() + i), "f");

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	string m_s = "f";
	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_s.insert((stdvec_s.begin() + i), m_s);
		auto t_s_2 = mvec_s.insert((mvec_s.begin() + i), m_s);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_s.insert((stdvec_s.begin() + i),
		                             static_cast<size_t>(2), m_s);
		auto t_s_2 =
		    mvec_s.insert((mvec_s.begin() + i), static_cast<size_t>(2), m_s);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	std_vector<string> stdvec_s_tmp = {"x", "y", "z", "h"};

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_s.insert((stdvec_s.begin() + i),
		                             stdvec_s_tmp.begin(), stdvec_s_tmp.end());
		auto t_s_2 = mvec_s.insert((mvec_s.begin() + i), stdvec_s_tmp.begin(),
		                           stdvec_s_tmp.end());

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_s.insert((stdvec_s.begin() + i), {"1", "2", "3"});
		auto t_s_2 = mvec_s.insert((mvec_s.begin() + i), {"1", "2", "3"});

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_s.emplace((stdvec_s.begin() + i), "f");
		auto t_s_2 = mvec_s.emplace((mvec_s.begin() + i), "f");

		auto t_s_3 = stdvec_s.emplace((stdvec_s.begin() + i), m_s);
		auto t_s_4 = mvec_s.emplace((mvec_s.begin() + i), m_s);

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		// WARN_EQ(mvec_s.capacity(), stdvec_s.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
		CHECK_EQ(*t_s_3, *t_s_4);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	for (int i = 0; i < 30; ++i) {
		auto t_s_1 = stdvec_s.erase((stdvec_s.begin() + i));
		auto t_s_2 = mvec_s.erase((mvec_s.begin() + i));

		CHECK_EQ(mvec_s.size(), stdvec_s.size());
		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	stdvec_s.erase(stdvec_s.begin(), stdvec_s.end());
	mvec_s.erase(mvec_s.begin(), mvec_s.end());

	CHECK_EQ(mvec_s.size(), stdvec_s.size());

	///<- int

	std_vector<int> stdvec_i = {1, 2, 3, 4, 5};
	m_vector<int>   mvec_i = {1, 2, 3, 4, 5};

	for (int i = 0; i < 5; ++i) {
		auto t_s_1 = stdvec_i.insert((stdvec_i.begin() + i), 6);
		auto t_s_2 = mvec_i.insert((mvec_i.begin() + i), 6);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 10; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	int m_i = 6;
	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_i.insert((stdvec_i.begin() + i), m_i);
		auto t_s_2 = mvec_i.insert((mvec_i.begin() + i), m_i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_i.insert((stdvec_i.begin() + i),
		                             static_cast<size_t>(2), m_i);
		auto t_s_2 =
		    mvec_i.insert((mvec_i.begin() + i), static_cast<size_t>(2), m_i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	std_vector<int> stdvec_i_tmp = {13, 15, 17, 19};

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_i.insert((stdvec_i.begin() + i),
		                             stdvec_i_tmp.begin(), stdvec_i_tmp.end());
		auto t_s_2 = mvec_i.insert((mvec_i.begin() + i), stdvec_i_tmp.begin(),
		                           stdvec_i_tmp.end());

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_i.insert((stdvec_i.begin() + i), {1, 2, 3});
		auto t_s_2 = mvec_i.insert((mvec_i.begin() + i), {1, 2, 3});

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	for (int i = 0; i < 10; ++i) {
		auto t_s_1 = stdvec_i.emplace((stdvec_i.begin() + i), 6);
		auto t_s_2 = mvec_i.emplace((mvec_i.begin() + i), 6);

		auto t_s_3 = stdvec_i.emplace((stdvec_i.begin() + i), m_i);
		auto t_s_4 = mvec_i.emplace((mvec_i.begin() + i), m_i);

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		// WARN_EQ(mvec_i.capacity(), stdvec_i.capacity());

		CHECK_EQ(*t_s_1, *t_s_2);
		CHECK_EQ(*t_s_3, *t_s_4);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	for (int i = 0; i < 30; ++i) {
		auto t_s_1 = stdvec_i.erase((stdvec_i.begin() + i));
		auto t_s_2 = mvec_i.erase((mvec_i.begin() + i));

		CHECK_EQ(mvec_i.size(), stdvec_i.size());
		CHECK_EQ(*t_s_1, *t_s_2);
	}

	for (int i = 0; i < 20; ++i)
		CHECK_EQ(mvec_i[i], stdvec_i[i]);

	stdvec_i.erase(stdvec_i.begin(), stdvec_i.end());
	mvec_i.erase(mvec_i.begin(), mvec_i.end());

	CHECK_EQ(mvec_i.size(), stdvec_i.size());
}

TEST_CASE(" swap.. ") {

	///<- string

	std_vector<string> stdvec_s_1 = {"a", "b", "c"};
	std_vector<string> stdvec_s_2 = {"m", "n"};
	m_vector<string>   mvec_s_1 = {"a", "b", "c"};
	m_vector<string>   mvec_s_2 = {"m", "n"};

	stdvec_s_1.swap(stdvec_s_2);
	mvec_s_1.swap(mvec_s_2);

	CHECK_EQ(mvec_s_1.size(), stdvec_s_1.size());
	CHECK_EQ(mvec_s_1.capacity(), stdvec_s_1.capacity());

	for (int i = 0; i < mvec_s_1.size(); ++i)
		CHECK_EQ(mvec_s_1[i], stdvec_s_1[i]);

	for (int i = 0; i < mvec_s_2.size(); ++i)
		CHECK_EQ(mvec_s_2[i], stdvec_s_2[i]);

	swap(stdvec_s_1, stdvec_s_2);
	swap(mvec_s_1, mvec_s_2);

	CHECK_EQ(mvec_s_1.size(), stdvec_s_1.size());
	CHECK_EQ(mvec_s_1.capacity(), stdvec_s_1.capacity());

	for (int i = 0; i < mvec_s_1.size(); ++i)
		CHECK_EQ(mvec_s_1[i], stdvec_s_1[i]);

	for (int i = 0; i < mvec_s_2.size(); ++i)
		CHECK_EQ(mvec_s_2[i], stdvec_s_2[i]);

	///<- int

	std_vector<int> stdvec_i_1 = {1, 2, 3};
	std_vector<int> stdvec_i_2 = {7, 8};
	m_vector<int>   mvec_i_1 = {1, 2, 3};
	m_vector<int>   mvec_i_2 = {7, 8};

	stdvec_i_1.swap(stdvec_i_2);
	mvec_i_1.swap(mvec_i_2);

	CHECK_EQ(mvec_i_1.size(), stdvec_i_1.size());
	CHECK_EQ(mvec_i_1.capacity(), stdvec_i_1.capacity());

	for (int i = 0; i < mvec_i_1.size(); ++i)
		CHECK_EQ(mvec_i_1[i], stdvec_i_1[i]);

	for (int i = 0; i < mvec_i_2.size(); ++i)
		CHECK_EQ(mvec_i_2[i], stdvec_i_2[i]);

	swap(stdvec_i_1, stdvec_i_2);
	swap(mvec_i_1, mvec_i_2);

	CHECK_EQ(mvec_i_1.size(), stdvec_i_1.size());
	CHECK_EQ(mvec_i_1.capacity(), stdvec_i_1.capacity());

	for (int i = 0; i < mvec_i_1.size(); ++i)
		CHECK_EQ(mvec_i_1[i], stdvec_i_1[i]);

	for (int i = 0; i < mvec_i_2.size(); ++i)
		CHECK_EQ(mvec_i_2[i], stdvec_i_2[i]);
}

TEST_CASE(" compare ") {

	std_vector<string> stdvec_s_1 = {"b", "d", "h"};

	std_vector<std_vector<string>> stdvec_s = {
	    {"b", "d", "h"}, {"a", "d", "h"},      {"c", "d", "h"},     {"c", "d"},
	    {"e", "f"},      {"c", "d", "a", "a"}, {"e", "f", "g", "h"}};

	m_vector<string> mvec_s_1 = {"b", "d", "h"};

	m_vector<m_vector<string>> mvec_s = {
	    {"b", "d", "h"}, {"a", "d", "h"},      {"c", "d", "h"},     {"c", "d"},
	    {"e", "f"},      {"c", "d", "a", "a"}, {"e", "f", "g", "h"}};

	for (int i = 0; i < mvec_s.size(); ++i) {
		CHECK_EQ(stdvec_s_1 == stdvec_s[i], mvec_s_1 == mvec_s[i]);
		CHECK_EQ(stdvec_s_1 != stdvec_s[i], mvec_s_1 != mvec_s[i]);
		CHECK_EQ(stdvec_s_1 > stdvec_s[i], mvec_s_1 > mvec_s[i]);
		CHECK_EQ(stdvec_s_1 < stdvec_s[i], mvec_s_1 < mvec_s[i]);
		CHECK_EQ(stdvec_s_1 >= stdvec_s[i], mvec_s_1 >= mvec_s[i]);
		CHECK_EQ(stdvec_s_1 <= stdvec_s[i], mvec_s_1 <= mvec_s[i]);
	}

	///<- int

	std_vector<int> stdvec_i_1 = {5, 6, 7};

	std_vector<std_vector<int>> stdvec_i = {
	    {5, 6, 7}, {1, 2, 3},    {8, 9, 0},    {1, 2},
	    {7, 8},    {1, 2, 3, 4}, {7, 8, 9, 10}};

	m_vector<int> mvec_i_1 = {5, 6, 7};

	m_vector<m_vector<int>> mvec_i = {{5, 6, 7},    {1, 2, 3}, {8, 9, 0},
	                                  {1, 2},       {7, 8},    {1, 2, 3, 4},
	                                  {7, 8, 9, 10}};

	for (int i = 0; i < mvec_i.size(); ++i) {
		CHECK_EQ(stdvec_i_1 == stdvec_i[i], mvec_i_1 == mvec_i[i]);
		CHECK_EQ(stdvec_i_1 != stdvec_i[i], mvec_i_1 != mvec_i[i]);
		CHECK_EQ(stdvec_i_1 > stdvec_i[i], mvec_i_1 > mvec_i[i]);
		CHECK_EQ(stdvec_i_1 < stdvec_i[i], mvec_i_1 < mvec_i[i]);
		CHECK_EQ(stdvec_i_1 >= stdvec_i[i], mvec_i_1 >= mvec_i[i]);
		CHECK_EQ(stdvec_i_1 <= stdvec_i[i], mvec_i_1 <= mvec_i[i]);
	}
}

// 此用例要求 C++20 版本
#if 0
TEST_CASE(" others(earse earse_if) ") {
	std_vector<string> stdvec_s;
	m_vector<string>   mvec_s;

	for (int i = 0; i < 40; ++i) {
		stdvec_s.insert((stdvec_s.begin() + i * 5), {"p", "q", "r", "s", "t"});
		mvec_s.insert((mvec_s.begin() + i * 5), {"p", "q", "r", "s", "t"});
	}

	string p_s = "p";

	size_t msize_s_1 = erase(mvec_s, p_s);
	size_t stdsize_s_1 = std::erase(stdvec_s, p_s);

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(msize_s_1, stdsize_s_1);

	for (int i = 0; i < 40; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	string r_s = "r";

	size_t msize_s_2 = erase(mvec_s, r_s);
	size_t stdsize_s_2 = std::erase(stdvec_s, r_s);

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(msize_s_1, stdsize_s_1);

	for (int i = 0; i < 40; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	/*

	string t_s = "t";

	size_t msize_s_3 = erase_if(mvec_s, [t_s](string s) { return s == t_s; });
	size_t stdsize_s_3 =
	    std::erase_if(stdvec_s, [t_s](string s) { return s == t_s; });

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(msize_s_1, stdsize_s_1);

	for (int i = 0; i < 40; ++i)
		CHECK_EQ(mvec_s[i], stdvec_s[i]);

	size_t msize_s_4 = erase_if(mvec_s, [](string s) { return true; });
	size_t stdsize_s_4 = std::erase_if(stdvec_s, [](string s) { return true; });

	CHECK_EQ(mvec_s.size(), stdvec_s.size());
	CHECK_EQ(msize_s_1, stdsize_s_1);

	CHECK_EQ(mvec_s.size(), 0);
	*/
}
#endif

// come from TinySTL
MSTL_TEST_NAMESPACE_END
