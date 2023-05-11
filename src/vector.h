#ifndef VECTOR_H
#define VECTOR_H

#include <limits>

#include "algorithm.h"
#include "allocator.h"
#include "iterator.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "initializer_list.h"

MSTL_NAMESPACE_BEGIN

template <class T, class Alloc = allocator<T>>
class vector {
public:
	typedef T                 value_type;
	typedef Alloc         	  allocator_type;
	typedef size_t            size_type;
	typedef ptrdiff_t         difference_type;
	typedef value_type&       reference;
	typedef const value_type& const_reference;
	typedef value_type*       pointer;
	typedef const value_type* const_pointer;

	typedef _iterator<random_access_iterator_tag, value_type> iterator;
	typedef _iterator<random_access_iterator_tag, const value_type> const_iterator;

	typedef _reverse_iterator<iterator>       reverse_iterator;
	typedef _reverse_iterator<const_iterator> const_reverse_iterator;

private:
	pointer start_;
	pointer finish_;
	pointer end_of_storage_;

public:
	
	// (construct)(destruct)(copy)(operator=)(assign) ......
	
	vector() = default;
	explicit vector(const allocator_type& alloc = allocator_type())
	    : start_(nullptr)
	    , finish_(nullptr)
	    , end_of_storage_(nullptr) {}
	
	
	explicit vector(const size_type count);
	explicit vector(const size_type count, const value_type& value = T(), const allocator_type& alloc = allocator_type());

	template <class InputIterator>
	vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());

	// vector(initializer_list<T> init, const allocator_type& alloc = allocator_type());

	vector(const vector& other);
	vector(vector&& other);
	vector& operator=(const vector& other);
	vector& operator=(vector&& other);
	// vector& operator=(initializer_list<T> ilist);

	void assign(size_type count, const T& value);
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last);
	// void assign(initializer_list<T> ilist);

	~vector() noexcept;

	// allocator_type get_allocator() { return allocator_type(); }

	// Element access

	reference at(size_type pos) { return *(begin() + pos); }
	const_reference at(size_type pos) const { return *(begin() + pos); }

	reference operator[](const difference_type i) { return *(begin() + i); }
	const_reference operator[](const difference_type i) const { return *(cbegin() + i); }

	reference front() { return *(begin()); }
	const_reference front() const { return *(begin()); }

	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	pointer data() noexcept { return (start_); }
	const_pointer data() const noexcept { return (start_); }

	// Iterators ?

	iterator begin() noexcept { return (start_); }
	const_iterator begin() const noexcept { return (start_); }
	const_iterator cbegin() const noexcept { return (start_); }

	iterator end() noexcept { return (finish_); }
	const_iterator end() const noexcept { return (finish_); }
	const_iterator cend() const noexcept { return (finish_); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(finish_); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(finish_); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(finish_); }

	reverse_iterator rend() noexcept { return reverse_iterator(start_); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(start_); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(start_);}

	// Capacity

	bool empty() const noexcept { return start_ == finish_; }
	size_type size() const noexcept { return static_cast<size_type>(finish_ - start_); }
	// size_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }

	void reserve(size_type n);
	size_type capacity() const noexcept { return static_cast<size_type>(end_of_storage_ - start_); }
	void shrink_to_fit();

	// Modifiers

	void clear() noexcept;
	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, T&& value);
	iterator insert(const_iterator pos, size_type count, const_reference value);
	template <class InputIterator>
	iterator insert(const_iterator pos, InputIterator first, InputIterator last);
	// iterator insert(const_iterator pos, initializer_list<T> ilist);

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args);

	iterator erase(const_iterator position);
	iterator erase(const_iterator first, const_iterator last);

	void push_back(const_reference value);
	void push_back(T&& value);

	template <class... Args>
	reference emplace_back(Args&&... args);

	void pop_back();

	void resize(size_type count);
	void resize(size_type count, const value_type& value);

	void swap(vector& other);

private:
	template <class InputIterator>
	void insert_aux(iterator      position,
	                InputIterator first,
	                InputIterator last,
	                std::false_type);
	template <class Integer>
	void insert_aux(iterator          position,
	                Integer           n,
	                const value_type& value,
	                std::true_type);

};

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

template <class T, class Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs);

template <class T, class Alloc, class U>
typename vector<T, Alloc>::size_type erase(vector<T, Alloc>& c, const U& value);

template <class T, class Alloc, class Pred>
typename vector<T, Alloc>::size_type erase_if(vector<T, Alloc>& c, Pred pred);

MSTL_NAMESPACE_END

#endif