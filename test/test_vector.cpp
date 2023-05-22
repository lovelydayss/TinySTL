#include <cstddef>
#include <initializer_list>
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

	m_vector<string>   mvec(10);
	std_vector<string> stdvec(10);

	CHECK_EQ(mvec.size(), 10);
	CHECK_EQ(mvec.size(), stdvec.size());
	CHECK_EQ(mvec.capacity(), stdvec.capacity());

	for (int i = 0; i < mvec.size(); i++)
		CHECK_EQ(mvec[i], string());

	// zero 情况
	m_vector<string>   mvec_zero(0);
	std_vector<string> stdvec_zero(0);

	CHECK_EQ(mvec_zero.size(), 0);
	CHECK_EQ(mvec_zero.size(), stdvec_zero.size());
	CHECK_EQ(mvec_zero.capacity(), stdvec_zero.capacity());
}

TEST_CASE(" test vector(const size_type count, const_reference value) ") {

	m_vector<string>   mvec(20, "dasdasfs");
	std_vector<string> stdvec(20, "dasdasfs");

	CHECK_EQ(mvec.size(), 20);
	CHECK_EQ(mvec.size(), stdvec.size());
	CHECK_EQ(mvec.capacity(), stdvec.capacity());

	for (int i = 0; i < mvec.size(); i++) {
		CHECK_EQ(mvec[i], "dasdasfs");
		CHECK_EQ(mvec[i], stdvec[i]);
	}
}

TEST_CASE(" vector(InputIterator first, InputIterator last) ") {

	std_vector<string> stdvec(10, "dasdasfs");
	m_vector<string>   mvec_1(stdvec.begin(), stdvec.end());
	m_vector<string>   mvec_2(mvec_1.begin(), mvec_1.end());

	CHECK_EQ(mvec_1.size(), 10);
	CHECK_EQ(mvec_2.size(), 10);
	CHECK_EQ(mvec_1.size(), stdvec.size());
	CHECK_EQ(mvec_2.size(), mvec_1.size());
	CHECK_EQ(mvec_1.capacity(), stdvec.capacity());
	CHECK_EQ(mvec_2.capacity(), mvec_1.capacity());

	for (int i = 0; i < mvec_1.size(); i++) {
		CHECK_EQ(mvec_1[i], "dasdasfs");
		CHECK_EQ(mvec_1[i], stdvec[i]);
	}

	for (int i = 0; i < mvec_2.size(); i++) {
		CHECK_EQ(mvec_2[i], "dasdasfs");
		CHECK_EQ(mvec_2[i], mvec_1[i]);
	}
}

TEST_CASE(" copy contruct and operator= ") {

	std_vector<string> stdvec(10, "dasdasfs");

	// copy
	m_vector<string> mvec(stdvec.begin(), stdvec.end());
	m_vector<string> mvec_1(mvec);
	m_vector<string> mvec_2 = mvec;

	CHECK_EQ(mvec_1.size(), 10);
	CHECK_EQ(mvec_2.size(), 10);
	CHECK_EQ(mvec_1.size(), mvec.size());
	CHECK_EQ(mvec_2.size(), mvec.size());
	CHECK_EQ(mvec_1.capacity(), mvec.capacity());
	CHECK_EQ(mvec_2.capacity(), mvec.capacity());

	for (int i = 0; i < mvec_1.size(); i++) {
		CHECK_EQ(mvec_1[i], "dasdasfs");
		CHECK_EQ(mvec_1[i], mvec[i]);
	}

	for (int i = 0; i < mvec_2.size(); i++) {
		CHECK_EQ(mvec_2[i], "dasdasfs");
		CHECK_EQ(mvec_2[i], mvec[i]);
	}

	// move

	m_vector<string> mvec_3(std::move(mvec_1));
	m_vector<string> mvec_4 = std::move(mvec_2);

	CHECK_EQ(mvec_3.size(), 10);
	CHECK_EQ(mvec_4.size(), 10);
	CHECK_EQ(mvec_3.capacity(), mvec.capacity());
	CHECK_EQ(mvec_4.capacity(), mvec.capacity());

	CHECK_EQ(mvec_1.size(), 0);
	CHECK_EQ(mvec_2.size(), 0);
	CHECK_EQ(mvec_1.capacity(), 0);
	CHECK_EQ(mvec_2.capacity(), 0);
	CHECK_EQ(mvec_1.data(), nullptr);
	CHECK_EQ(mvec_2.data(), nullptr);

	for (int i = 0; i < mvec_3.size(); i++) {
		CHECK_EQ(mvec_3[i], "dasdasfs");
		CHECK_EQ(mvec_3[i], mvec[i]);
	}

	for (int i = 0; i < mvec_4.size(); i++) {
		CHECK_EQ(mvec_4[i], "dasdasfs");
		CHECK_EQ(mvec_4[i], mvec[i]);
	}
}

TEST_CASE(" initializer_list ") {

	std_vector<string> stdvec = {"a", "b", "c", "d", "e"};
	m_vector<string>   mvec_1({"a", "b", "c", "d", "e"});
	m_vector<string>   mvec_2 = {"a", "b", "c", "d", "e"};

	CHECK_EQ(mvec_1.size(), 5);
	CHECK_EQ(mvec_2.size(), 5);
	CHECK_EQ(mvec_1.size(), stdvec.size());
	CHECK_EQ(mvec_2.size(), stdvec.size());
	CHECK_EQ(mvec_1.capacity(), stdvec.capacity());
	CHECK_EQ(mvec_2.capacity(), stdvec.capacity());

	for (int i = 0; i < mvec_1.size(); i++) {
		CHECK_EQ(mvec_1[i], stdvec[i]);
	}

	for (int i = 0; i < mvec_2.size(); i++) {
		CHECK_EQ(mvec_2[i], stdvec[i]);
	}
}

TEST_CASE(" assign ") {

	std_vector<string> stdvec(5, "dasdasfs");

	m_vector<string> mvec_1(1, "ok");
	m_vector<string> mvec_2(1, "ok");
	m_vector<string> mvec_3(1, "ok");

	mvec_1.assign(5, "dasdasfs");
	mvec_2.assign(stdvec.begin(), stdvec.end());
	mvec_3.assign({"dasdasfs", "dasdasfs", "dasdasfs", "dasdasfs", "dasdasfs"});

	CHECK_EQ(mvec_1.size(), 5);
	CHECK_EQ(mvec_2.size(), 5);
	CHECK_EQ(mvec_3.size(), 5);
	CHECK_EQ(mvec_1.capacity(), 5);
	CHECK_EQ(mvec_2.capacity(), 5);
	CHECK_EQ(mvec_3.capacity(), 5);

	for (int i = 0; i < mvec_1.size(); i++) {
		CHECK_EQ(mvec_1[i], "dasdasfs");
	}

	for (int i = 0; i < mvec_2.size(); i++) {
		CHECK_EQ(mvec_2[i], "dasdasfs");
	}

	for (int i = 0; i < mvec_3.size(); i++) {
		CHECK_EQ(mvec_3[i], "dasdasfs");
	}

	std_vector<string> stdvec_1(5, "aaaaa");

	mvec_1.assign(2, "aaaaa");
	mvec_2.assign(stdvec_1.begin(), stdvec_1.begin() + 2);
	mvec_3.assign({"aaaaa", "aaaaa"});

	CHECK_EQ(mvec_1.size(), 2);
	CHECK_EQ(mvec_2.size(), 2);
	CHECK_EQ(mvec_3.size(), 2);
	CHECK_EQ(mvec_1.capacity(), 5);
	CHECK_EQ(mvec_2.capacity(), 5);
	CHECK_EQ(mvec_3.capacity(), 5);

	for (int i = 0; i < mvec_1.size(); i++) {
		CHECK_EQ(mvec_1[i], "aaaaa");
	}

	for (int i = 0; i < mvec_2.size(); i++) {
		CHECK_EQ(mvec_2[i], "aaaaa");
	}

	for (int i = 0; i < mvec_3.size(); i++) {
		CHECK_EQ(mvec_3[i], "aaaaa");
	}
}

// come from TinySTL
MSTL_TEST_NAMESPACE_END
