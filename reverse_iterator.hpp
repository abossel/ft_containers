/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 21:09:19 by abossel           #+#    #+#             */
/*   Updated: 2023/02/23 01:29:44 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include <iterator>
# include "iterator_traits.hpp"

namespace ft
{
	template <class T>
	class reverse_iterator : public std::iterator<typename ft::iterator_traits<T>::iterator_category,
		typename ft::iterator_traits<T>::value_type,
		typename ft::iterator_traits<T>::difference_type,
		typename ft::iterator_traits<T>::pointer,
		typename ft::iterator_traits<T>::reference>
	{
		public:
			typedef T													iterator_type;
			typedef typename ft::iterator_traits<T>::value_type			value_type;
			typedef typename ft::iterator_traits<T>::difference_type	difference_type;
			typedef typename ft::iterator_traits<T>::pointer			pointer;
			typedef typename ft::iterator_traits<T>::reference			reference;
			typedef typename ft::iterator_traits<T>::iterator_category	iterator_category;

			reverse_iterator() { current = iterator_type(); }
			explicit reverse_iterator(iterator_type other) { current = other; }
			template <typename I>
			reverse_iterator(reverse_iterator<I> const &other) { current = other.base(); }
			reverse_iterator(reverse_iterator const &other) { current = other.base(); }
			template <typename I>
			reverse_iterator &operator=(const reverse_iterator<I> &other) { current = other.base(); return *this; }
			reverse_iterator &operator=(const reverse_iterator &other) { current = other.base(); return *this; }
			reverse_iterator &operator+=(difference_type add) { current -= add; return *this; }
			reverse_iterator &operator-=(difference_type sub) { current += sub; return *this; }
			reference operator*() const
			{
				iterator_type it = current;
				--it;
				return *it;
			}
			pointer operator->() const
			{
				iterator_type it = current;
				--it;
				return &(*it);
			}
			reference operator[](difference_type index) const
			{
				iterator_type it = current;
				--it;
				return *(it - index);
			}
			reverse_iterator &operator++() { --current; return *this; }
			reverse_iterator &operator--() { ++current; return *this; }
			reverse_iterator operator++(int) { reverse_iterator tmp(*this); --current; return tmp; }
			reverse_iterator operator--(int) { reverse_iterator tmp(*this); ++current; return tmp; }
			reverse_iterator operator+(difference_type add) const { return reverse_iterator(current - add); }
			reverse_iterator operator-(difference_type sub) const { return reverse_iterator(current + sub); }
			template <typename I>
			difference_type operator-(const reverse_iterator<I> &other) const {return other.base() - current; }
			difference_type operator-(const reverse_iterator &other) const {return other.base() - current; }
			template <typename I>
			friend reverse_iterator operator+(difference_type add, const reverse_iterator<I> &other) { return reverse_iterator(other.base() - add); }
			friend reverse_iterator operator+(difference_type add, const reverse_iterator &other) { return reverse_iterator(other.base() - add); }
			template <typename I>
			friend reverse_iterator operator-(difference_type sub, const reverse_iterator<I> &other) { return reverse_iterator(other.base() + sub); }
			friend reverse_iterator operator-(difference_type sub, const reverse_iterator &other) { return reverse_iterator(other.base() + sub); }
			iterator_type base() const { return current; }

		protected:
			iterator_type current;
	};

	template <class I1, class I2>
	bool operator==(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <class I1, class I2>
	bool operator!=(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() != rhs.base());
	}

	template <class I1, class I2>
	bool operator>(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() < rhs.base());
	}

	template <class I1, class I2>
	bool operator<(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() > rhs.base());
	}

	template <class I1, class I2>
	bool operator>=(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() <= rhs.base());
	}

	template <class I1, class I2>
	bool operator<=(const ft::reverse_iterator<I1> &lhs, const ft::reverse_iterator<I2> &rhs)
	{
		return (lhs.base() >= rhs.base());
	}
}

#endif
