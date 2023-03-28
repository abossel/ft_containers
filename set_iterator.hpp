/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 01:48:02 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:33:26 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_ITERATOR_HPP
# define SET_ITERATOR_HPP

# include <iterator>
# include "stddef.h"
# include "map_tree.hpp"

namespace ft
{
	template <class T>
	class set_iterator : public std::iterator<std::bidirectional_iterator_tag,
		T, ptrdiff_t, T*, T&>
	{
		public:
			typedef std::bidirectional_iterator_tag	iterator_category;
			typedef T								value_type;
			typedef ptrdiff_t						difference_type;
			typedef T*								pointer;
			typedef T&								reference;
			typedef struct ft::map_node				node_type;

			set_iterator() { _current = NULL; }
			set_iterator(node_type *other) { _current = other; }
			template <typename U>
			set_iterator(set_iterator<U> const &other) { _current = other.base(); }
			set_iterator(set_iterator const &other) { _current = other.base(); }
			template <typename U>
			set_iterator &operator=(const set_iterator<U> &other) { _current = other.base(); return *this; }
			set_iterator &operator=(const set_iterator &other) { _current = other.base(); return *this; }
			reference operator*() const { return *(reinterpret_cast<pointer>(_current->data)); }
			pointer operator->() const { return reinterpret_cast<pointer>(_current->data); }
			set_iterator &operator++() { next_node(); return *this; }
			set_iterator &operator--() { prev_node(); return *this; }
			set_iterator operator++(int) { set_iterator tmp(*this); next_node(); return tmp; }
			set_iterator operator--(int) { set_iterator tmp(*this); prev_node(); return tmp; }
			node_type *base() const { return _current; }

		private:
			void next_node()
			{
				_current = ft::map_tree<>::next_node(_current);
			}

			void prev_node()
			{
				_current = ft::map_tree<>::prev_node(_current);
			}

			node_type *_current;
	};

	template <class I1, class I2>
	bool operator==(const ft::set_iterator<I1> &lhs, const ft::set_iterator<I2> &rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <class I1, class I2>
	bool operator!=(const ft::set_iterator<I1> &lhs, const ft::set_iterator<I2> &rhs)
	{
		return (lhs.base() != rhs.base());
	}
}

#endif
