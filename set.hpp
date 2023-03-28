/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 01:45:47 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:33:33 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_HPP
# define SET_HPP

# include <memory>
# include <functional>
# include "stddef.h"
# include "utility.hpp"
# include "type_traits.hpp"
# include "map_tree.hpp"
# include "set_iterator.hpp"
# include "reverse_iterator.hpp"

namespace ft
{
	template <class K,
		class C = std::less<K>,
		class A = std::allocator<K> >
	class set
	{
		public:
			typedef K												key_type;
			typedef K												value_type;
			typedef size_t											size_type;
			typedef ptrdiff_t										difference_type;
			typedef C												key_compare;
			typedef C												value_compare;
			typedef A												allocator_type;
			typedef value_type&										reference;
			typedef const value_type&								const_reference;
			typedef typename A::pointer								pointer;
			typedef typename A::const_pointer						const_pointer;
			typedef ft::set_iterator<value_type>					iterator;
			typedef ft::set_iterator<const value_type>				const_iterator;
			typedef ft::reverse_iterator<iterator>					reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;
			typedef struct ft::map_node								node_type;
			typedef typename A::template rebind<node_type>::other	tree_allocator_type;

			explicit set(const key_compare& compare = key_compare(),
				const allocator_type &allocator = allocator_type())
			{
				_compare = compare;
				_allocator = allocator;
				_size = 0;
			}

			template <class InputIterator>
			set(InputIterator first, InputIterator last,
				const key_compare& compare = key_compare(),
				const allocator_type &allocator = allocator_type())
			{
				_compare = compare;
				_allocator = allocator;
				_size = 0;
				insert(first, last);
			}

			set(const set &other)
			{
				_compare = other.key_comp();
				_allocator = other.get_allocator();
				_size = 0;
				insert(other.begin(), other.end());
			}

			set &operator=(const set &other)
			{
				if (this != &other)
				{
					clear();
					_compare = other.key_comp();
					_allocator = other.get_allocator();
					insert(other.begin(), other.end());
				}
				return (*this);
			}

			~set()
			{
				clear();
			}

			// -----===== ITERATORS =====-----

			iterator begin()
			{
				return (iterator(_tree.get_first()));
			}

			const_iterator begin() const
			{
				return (const_iterator(_tree.get_first()));
			}

			iterator end()
			{
				if (_size == 0)
					return (begin());
				return (iterator(_tree.get_last()->right));
			}

			const_iterator end() const
			{
				if (_size == 0)
					return (begin());
				return (const_iterator(_tree.get_last()->right));
			}

			reverse_iterator rbegin()
			{
				if (_size == 0)
					return (rend());
				return (reverse_iterator(end()));
			}

			const_reverse_iterator rbegin() const
			{
				if (_size == 0)
					return (rend());
				return (const_reverse_iterator(end()));
			}

			reverse_iterator rend()
			{
				return (reverse_iterator(begin()));
			}

			const_reverse_iterator rend() const
			{
				return (const_reverse_iterator(begin()));
			}

			// -----===== CAPACITY =====-----

			size_type size() const
			{
				return (_size);
			}

			size_type max_size() const
			{
				size_type ptr_max;

				ptr_max = std::numeric_limits<ptrdiff_t>::max();
				if (ptr_max < _allocator.max_size())
					return (ptr_max);
				return (_allocator.max_size());
			}

			bool empty() const
			{
				return (_size == 0);
			}

			// -----===== MODIFIERS =====-----

			ft::pair<iterator, bool> insert(const value_type &value)
			{
				ft::pair<iterator, bool> pair;
				node_type *new_node;
				node_type *node;
				bool equal;
				bool less;

				if (_tree.get_root() == NULL)
				{
					new_node = _tree.add_left(construct(value), NULL);
					_size++;
					pair.first = iterator(new_node);
					pair.second = true;
					return (pair);
				}
				node = find_nearest_node(value, _tree.get_root(), &equal, &less);
				if (equal)
				{
					pair.first = iterator(node);
					pair.second = false;
					return (pair);
				}
				if (less)
				{
					new_node = _tree.add_left(construct(value), node);
					_size++;
					pair.first = iterator(new_node);
					pair.second = true;
					return (pair);
				}
				new_node = _tree.add_right(construct(value), node);
				_size++;
				pair.first = iterator(new_node);
				pair.second = true;
				return (pair);
			}

			iterator insert(iterator position, const value_type &value)
			{
				(void)position;
				return (insert(value).first);
			}

			template <class InputIterator>
			void insert(InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, void>::type* = NULL)
			{
				while (first != last)
				{
					insert(*first);
					++first;
				}
			}

			void erase(iterator position)
			{
				node_type *node;

				node = position.base();
				destroy(node->data);
				_tree.remove(node);
				_size--;
			}

			size_type erase(const key_type &key)
			{
				iterator it;
				bool equal;

				it = find_first_ge(key, _tree.get_root(), &equal);
				if (!equal)
					return (0);
				erase(it);
				return (1);
			}

			void erase(iterator first, iterator last)
			{
				iterator it;

				while (first != last)
				{
					it = first;
					++first;
					erase(it);
				}
			}

			void swap(set &other)
			{
				if (this == &other)
					return ;
				std::swap(_compare, other._compare);
				std::swap(_allocator, other._allocator);
				std::swap(_size, other._size);
				_tree.swap(other._tree);
			}

			void clear()
			{
				while (_size > 0)
					erase(begin());
			}

			// -----===== OBSERVERS =====-----

			key_compare key_comp() const
			{
				return (key_compare(_compare));
			}

			value_compare value_comp() const
			{
				return (value_compare(_compare));
			}

			// -----===== OPERATIONS =====-----

			iterator find(const key_type &key)
			{
				node_type *node;
				bool equal;

				node = find_nearest_node(key, _tree.get_root(), &equal);
				if (!equal)
					return (end());
				return (iterator(node));
			}

			const_iterator find(const key_type &key) const
			{
				node_type *node;
				bool equal;

				node = find_nearest_node(key, _tree.get_root(), &equal);
				if (!equal)
					return (end());
				return (const_iterator(node));
			}

			size_type count(const key_type &key) const
			{
				if (find(key) != end())
					return (1);
				return (0);
			}

			iterator lower_bound(const key_type &key)
			{
				bool equal;

				return (find_first_ge(key, _tree.get_root(), &equal));
			}

			const_iterator lower_bound(const key_type &key) const
			{
				bool equal;

				return (const_iterator(find_first_ge(key, _tree.get_root(), &equal)));
			}

			iterator upper_bound(const key_type &key)
			{
				iterator it;
				bool equal;

				it = find_first_ge(key, _tree.get_root(), &equal);
				if (equal)
					++it;
				return (it);
			}

			const_iterator upper_bound(const key_type &key) const
			{
				iterator it;
				bool equal;

				it = find_first_ge(key, _tree.get_root(), &equal);
				if (equal)
					++it;
				return (const_iterator(it));
			}

			ft::pair<iterator,iterator> equal_range(const key_type &key)
			{
				ft::pair<iterator, iterator> pair;

				pair.first = lower_bound(key);
				pair.second = upper_bound(key);
				return (pair);
			}

			ft::pair<const_iterator,const_iterator> equal_range(const key_type &key) const
			{
				ft::pair<const_iterator, const_iterator> pair;

				pair.first = lower_bound(key);
				pair.second = upper_bound(key);
				return (pair);
			}

			// -----===== ALLOCATOR =====-----

			allocator_type get_allocator() const
			{
				return (allocator_type(_allocator));
			}

		private:
			uintptr_t construct(const value_type &value)
			{
				value_type *data;

				data = _allocator.allocate(1);
				_allocator.construct(data, value);
				return (reinterpret_cast<uintptr_t>(data));
			}

			void destroy(uintptr_t data)
			{
				_allocator.destroy(reinterpret_cast<value_type *>(data));
				_allocator.deallocate(reinterpret_cast<value_type *>(data), 1);
			}

			value_type *node_data(node_type *node) const
			{
				return (reinterpret_cast<value_type *>(node->data));
			}

			iterator find_first_ge(const key_type &key, node_type *node, bool *equal,
									node_type *prev = NULL) const
			{
				*equal = false;
				if (node == NULL)
					return (end());
				if (_compare(key, *(node_data(node))))
				{
					if (_tree.is_nil(node->left))
						return (iterator(node));
					return (find_first_ge(key, node->left, equal, node));
				}
				if (_compare(*(node_data(node)), key))
				{
					if (_tree.is_nil(node->right))
					{
						if (prev == NULL)
							return (end());
						return (iterator(prev));
					}
					return (find_first_ge(key, node->right, equal, prev));
				}
				*equal = true;
				return (iterator(node));
			}

			node_type *find_nearest_node(const key_type &key, node_type *node, bool *equal, bool *less = NULL) const
			{
				if (node == NULL)
				{
					if (less != NULL)
						*less = false;
					*equal = false;
					return (NULL);
				}
				if (_compare(key, *(node_data(node))))
				{
					if (_tree.is_nil(node->left))
					{
						if (less != NULL)
							*less = true;
						*equal = false;
						return (node);
					}
					else
						return (find_nearest_node(key, node->left, equal, less));
				}
				if (_compare(*(node_data(node)), key))
				{
					if (_tree.is_nil(node->right))
					{
						if (less != NULL)
							*less = false;
						*equal = false;
						return (node);
					}
					else
						return (find_nearest_node(key, node->right, equal, less));
				}
				if (less != NULL)
					*less = false;
				*equal = true;
				return (node);
			}

			key_compare							_compare;
			allocator_type						_allocator;
			size_type							_size;
			ft::map_tree<tree_allocator_type>	_tree;
	};

	template <class K, class C, class A>
	void swap(ft::set<K, C, A> &lhs, ft::set<K, C, A> &rhs)
	{
		if (&lhs == &rhs)
			return ;
		lhs.swap(rhs);
	}

	template <class K, class C, class A>
	bool operator==(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class K, class C, class A>
	bool operator!=(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class K, class C, class A>
	bool operator<(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end()));
	}

	template <class K, class C, class A>
	bool operator<=(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		return (!(lhs > rhs));
	}

	template <class K, class C, class A>
	bool operator>(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		return (rhs < lhs);
	}

	template <class K, class C, class A>
	bool operator>=(const ft::set<K, C, A> &lhs, const ft::set<K, C, A> &rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
