/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 08:00:56 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:39:34 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_ITERATOR_HPP
# define MAP_ITERATOR_HPP

# include <iterator>
# include <stddef.h>
# include "map_tree.hpp"

namespace ft
{
	template <class T>
	class map_iterator : public std::iterator<std::bidirectional_iterator_tag,
		T, ptrdiff_t, T*, T&>
	{
		public:
			typedef std::bidirectional_iterator_tag	iterator_category;
			typedef T								value_type;
			typedef ptrdiff_t						difference_type;
			typedef T*								pointer;
			typedef T&								reference;
			typedef struct ft::map_node				node_type;

			map_iterator() { _current = NULL; }
			map_iterator(node_type *other) { _current = other; }
			template <typename U>
			map_iterator(map_iterator<U> const &other) { _current = other.base(); }
			map_iterator(map_iterator const &other) { _current = other.base(); }
			template <typename U>
			map_iterator &operator=(const map_iterator<U> &other) { _current = other.base(); return *this; }
			map_iterator &operator=(const map_iterator &other) { _current = other.base(); return *this; }
			reference operator*() const { return *(reinterpret_cast<pointer>(_current->data)); }
			pointer operator->() const { return reinterpret_cast<pointer>(_current->data); }
			map_iterator &operator++() { next_node(); return *this; }
			map_iterator &operator--() { prev_node(); return *this; }
			map_iterator operator++(int) { map_iterator tmp(*this); next_node(); return tmp; }
			map_iterator operator--(int) { map_iterator tmp(*this); prev_node(); return tmp; }
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
	bool operator==(const ft::map_iterator<I1> &lhs, const ft::map_iterator<I2> &rhs)
	{
		return (lhs.base() == rhs.base());
	}

	template <class I1, class I2>
	bool operator!=(const ft::map_iterator<I1> &lhs, const ft::map_iterator<I2> &rhs)
	{
		return (lhs.base() != rhs.base());
	}
}

#endif
