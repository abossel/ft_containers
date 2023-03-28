/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:06:04 by abossel           #+#    #+#             */
/*   Updated: 2023/02/22 22:26:30 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP
# define UTILITY_HPP

# include <limits>

namespace ft
{
	// equal

	template <class InputIterator1, class InputIterator2>
	bool equal (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1!=last1)
		{
			if (!(*first1 == *first2))
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}

	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool equal (InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, BinaryPredicate pred)
	{
		while (first1 != last1)
		{
			if (!pred(*first1, *first2))
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}

	// lexicographical_compare

	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return (false);
			else if (*first1 < *first2)
				return (true);
			++first1;
			++first2;
		}
  		return (first2 != last2);
	}

	template <class InputIterator1, class InputIterator2, class Compare>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1))
				return (false);
			else if (comp(*first1, *first2))
				return (true);
			++first1;
			++first2;
		}
  		return (first2 != last2);
	}

	// pair

	template <class U, class V>
	struct pair
	{
		public:
			typedef U first_type;
			typedef V second_type;

			pair()
				: first(), second()
			{
			}

			pair(const pair<U, V> &other)
				: first(other.first), second(other.second)
			{
			}

			template <class U2, class V2>
			pair(const pair<U2, V2> &other)
				: first(other.first), second(other.second)
			{
			};

			pair(const first_type &a, const second_type &b)
				: first(a), second(b)
			{
			}

			pair &operator=(const pair &other)
			{
				first = other.first;
				second = other.second;
				return (*this);
			}

			U first;
			V second;
	};

	template <class U, class V>
	bool operator==(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}

	template <class U, class V>
	bool operator!=(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class U, class V>
	bool operator<(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (lhs.first < rhs.first
				|| (!(rhs.first < lhs.first) && lhs.second < rhs.second));
	}

	template <class U, class V>
	bool operator<=(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <class U, class V>
	bool operator>(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (rhs < lhs);
	}

	template <class U, class V>
	bool operator>=(const pair<U, V> &lhs, const pair<U, V> &rhs)
	{
		return (!(lhs < rhs));
	}

	// make_pair

	template<class U, class V>
	pair<U, V> make_pair(U a, V b)
	{
		return (pair<U, V>(a, b));
	}
}

#endif
