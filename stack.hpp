/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 01:30:40 by abossel           #+#    #+#             */
/*   Updated: 2023/02/07 22:28:40 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft
{
	template <class T, class C = ft::vector<T> >
	class stack
	{
		public:
			typedef C							container_type;
			typedef typename C::value_type		value_type;
			typedef typename C::reference		reference;
			typedef typename C::const_reference	const_reference;
			typedef typename C::size_type		size_type;

			explicit stack(const container_type &container = container_type())
			{
				c = container;
			}

			~stack()
			{
				c.clear();
			}

			stack &operator=(const stack &other)
			{
				if (this != &other)
					c = other.c;
				return (*this);
			}

			// -----===== CAPACITY =====-----

			size_type size() const
			{
				return (c.size());
			}

			bool empty() const
			{
				return (c.empty());
			}

			// -----===== ELEMENT ACCESS =====-----

			reference top()
			{
				return (c.back());
			}

			// -----===== MODIFIERS =====-----

			void push(const value_type &value)
			{
				c.push_back(value);
			}

			void pop()
			{
				c.pop_back();
			}

			// -----===== OPERATORS =====-----

			bool operator==(const stack<T, C> &other) const
			{
				return (c == other.c);
			}

			bool operator<(const stack<T, C> &other) const
			{
				return (c < other.c);
			}

		protected:
			container_type	c;
	};

	template <class T, class C>
	bool operator==(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		return (lhs.operator==(rhs));
	}

	template <class T, class C>
	bool operator<(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		return (lhs.operator<(rhs));
	}

	template <class T, class C>
	bool operator!=(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class T, class C>
	bool operator<=(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		if (lhs == rhs)
			return (true);
		if (lhs < rhs)
			return (true);
		return (false);
	}

	template <class T, class C>
	bool operator>(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		return (!(lhs <= rhs));
	}

	template <class T, class C>
	bool operator>=(const stack<T, C> &lhs, const stack<T, C> &rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
