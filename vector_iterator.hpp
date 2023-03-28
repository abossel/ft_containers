/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_iterator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 21:00:40 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:41:46 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_ITERATOR_HPP
# define VECTOR_ITERATOR_HPP

# include <iterator>
# include <stddef.h>

namespace ft
{
	template <class T>
	class vector_iterator : public std::iterator<std::random_access_iterator_tag,
		T, ptrdiff_t, T*, T&>
	{
		public:
			typedef std::random_access_iterator_tag	iterator_category;
			typedef T								value_type;
			typedef ptrdiff_t						difference_type;
			typedef T*								pointer;
			typedef T&								reference;

			vector_iterator() { _pointer = NULL; }
			vector_iterator(pointer other) { _pointer = other; }
			template <typename U>
			vector_iterator(vector_iterator<U> const &other) { _pointer = other.base(); }
			vector_iterator(vector_iterator const &other) { _pointer = other.base(); }
			template <typename U>
			vector_iterator &operator=(const vector_iterator<U> &other) { _pointer = other.base(); return *this; }
			vector_iterator &operator=(const vector_iterator &other) { _pointer = other.base(); return *this; }
			vector_iterator &operator=(pointer other) { _pointer = other; return *this; }
			vector_iterator &operator+=(difference_type add) { _pointer += add; return *this; }
			vector_iterator &operator-=(difference_type sub) { _pointer -= sub; return *this; }
			reference operator*() const { return *_pointer; }
			pointer operator->() const { return _pointer; }
			reference operator[](difference_type index) const { return _pointer[index]; }
			vector_iterator &operator++() { ++_pointer; return *this; }
			vector_iterator &operator--() { --_pointer; return *this; }
			vector_iterator operator++(int) { vector_iterator tmp(*this); ++_pointer; return tmp; }
			vector_iterator operator--(int) { vector_iterator tmp(*this); --_pointer; return tmp; }
			vector_iterator operator+(difference_type add) const { return vector_iterator(_pointer + add); }
			vector_iterator operator-(difference_type sub) const { return vector_iterator(_pointer - sub); }
			template <typename U>
			difference_type operator-(const vector_iterator<U>& other) const {return _pointer - other.base(); }
			difference_type operator-(const vector_iterator& other) const {return _pointer - other.base(); }
			template <typename U>
			friend vector_iterator operator+(difference_type add, const vector_iterator<U>& other) { return vector_iterator(other._pointer + add); }
			friend vector_iterator operator+(difference_type add, const vector_iterator& other) { return vector_iterator(other._pointer + add); }
			template <typename U>
			friend vector_iterator operator-(difference_type sub, const vector_iterator<U>& other) { return vector_iterator(other._pointer - sub); }
			friend vector_iterator operator-(difference_type sub, const vector_iterator& other) { return vector_iterator(other._pointer - sub); }
			pointer base() const { return _pointer; }
		private:
			pointer _pointer;
	};

	template <class I1, class I2>
	bool operator==(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <class I1, class I2>
	bool operator!=(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template <class I1, class I2>
	bool operator>(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() > rhs.base());
	}

	template <class I1, class I2>
	bool operator<(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() < rhs.base());
	}

	template <class I1, class I2>
	bool operator>=(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() >= rhs.base());
	}

	template <class I1, class I2>
	bool operator<=(const ft::vector_iterator<I1> &lhs, const ft::vector_iterator<I2> &rhs)
	{
		return (lhs.base() <= rhs.base());
	}
}

#endif
