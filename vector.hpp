/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 01:30:40 by abossel           #+#    #+#             */
/*   Updated: 2023/02/28 17:08:13 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include <limits>
# include <algorithm>
# include <stdexcept>
# include <stddef.h>
# include "utility.hpp"
# include "type_traits.hpp"
# include "vector_iterator.hpp"
# include "reverse_iterator.hpp"

namespace ft
{
	template <class T, class A = std::allocator<T> >
	class vector
	{
		public:
			typedef T											value_type;
			typedef A											allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer 			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef ft::vector_iterator<value_type>				iterator;
			typedef ft::vector_iterator<const value_type>		const_iterator;
			typedef ft::reverse_iterator<iterator> 				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator> 		const_reverse_iterator;
			typedef ptrdiff_t									difference_type;
			typedef size_t										size_type;

			explicit vector(const allocator_type &alloc = allocator_type())
			{
				_allocator = alloc;
				init();
			}

			explicit vector(size_type count, const value_type &value = value_type(),
				const allocator_type &alloc = allocator_type())
			{
				_allocator = alloc;
				init();
				assign(count, value);
			}

			template <class InputIterator>
			vector(InputIterator first, InputIterator last,
				const allocator_type &alloc = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, void>::type* = NULL)
			{
				_allocator = alloc;
				init();
				assign(first, last);
			}

			vector(const vector &other)
			{
				_allocator = other._allocator;
				init(other._capacity);
				for (size_type i = 0; i < other._size; i++)
					push_back(other[i]);
			}

			~vector()
			{
				destroy();
				deallocate();
			}

			vector &operator=(const vector &other)
			{
				if (this != &other)
				{
					destroy();
					deallocate();
					_allocator = other._allocator;
					init(other._capacity);
					for (size_type i = 0; i < other._size; i++)
						push_back(other[i]);
				}
				return (*this);
			}

			// -----===== ITERATORS =====-----

			iterator begin()
			{
				return (iterator(_array));
			}

			const_iterator begin() const
			{
				return (const_iterator(_array));
			}

			iterator end()
			{
				if (_size == 0)
					return (begin());
				return (iterator(&_array[_size]));
			}

			const_iterator end() const
			{
				if (_size == 0)
					return (begin());
				return (const_iterator(&_array[_size]));
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

			void resize(size_type size, value_type value = value_type())
			{
				if (_size == size)
					return ;
				else if (_size > size)
					destroy(size);
				else if (_size < size)
				{
					reserve(size);
					while(_size < size)
						push_back(value);
				}
			}

			size_type capacity() const
			{
				return (_capacity);
			}

			bool empty() const
			{
				return (_size == 0);
			}

			void reserve(size_type capacity)
			{
				size_type size2;
				T *array2;

				if (capacity > max_size())
					throw (std::length_error("resize larger than max size"));
				if (capacity <= _capacity)
					return ;
				size2 = _size;
				array2 = _allocator.allocate(capacity, _array);
				for (size_type i = 0; i < _size; i++)
					_allocator.construct(&array2[i], _array[i]);
				destroy();
				deallocate();
				_capacity = capacity;
				_size = size2;
				_array = array2;
			}

			// -----===== ELEMENT ACCESS =====-----

			reference operator[](size_type index)
			{
				return (_array[index]);
			}

			const_reference operator[](size_type index) const
			{
				return (_array[index]);
			}

			reference at(size_type index)
			{
				if (index >= _size)
					throw (std::out_of_range("index out of range"));
				return (_array[index]);
			}

			const_reference at(size_type index) const
			{
				if (index >= _size)
					throw (std::out_of_range("index out of range"));
				return (_array[index]);
			}

			reference front()
			{
				return (_array[0]);
			}

			const_reference front() const
			{
				return (_array[0]);
			}

			reference back()
			{
				return (_array[_size - 1]);
			}

			const_reference back() const
			{
				return (_array[_size - 1]);
			}

			// -----===== MODIFIERS =====-----

			template<class InputIterator>
			void assign(InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, void>::type* = NULL)
			{
				destroy();
				while (first != last)
				{
					push_back(*first);
					++first;
				}
			}

			void assign(size_type count, const value_type &value)
			{
				destroy();
				reserve(count);
				for (size_type i = 0; i < count; i++)
					push_back(value);
			}

			void push_back(const value_type &value)
			{
				if (_size == _capacity)
					reserve(grow_capacity());
				_allocator.construct(&_array[_size], value);
				_size++;
			}

			void pop_back()
			{
				if (_size > 0)
					destroy(_size - 1);
			}

			iterator insert(iterator position, const value_type& value)
			{
				position = shift(position, 1);
				_allocator.construct(&(*position), value);
				return (position);
			}

			void insert(iterator position, size_type count, const value_type& value)
			{
				if (count == 0)
					return ;
				position = shift(position, count);
				for (size_type i = 0; i < count; i++)
					_allocator.construct(&(*(position + i)), value);
			}

			template <class InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, void>::type* = NULL)
			{
				size_type shift_size;

				if (first == last)
					return ;
				shift_size = _size;
				if (_size == 0)
					shift_size = 2;
				if (_size > max_size() / 2)
					shift_size = max_size() - _size;
				position = shift(position, shift_size);
				while (first != last)
				{
					_allocator.construct(&(*position), *first);
					first++;
					position++;
					shift_size--;
					if (shift_size == 0)
					{
						insert(position, first, last);
						return ;
					}
				}
				rshift(position, shift_size);
			}

			iterator erase(iterator position)
			{
				_allocator.destroy(&(*position));
				rshift(position, 1);
				return (position);
			}

			iterator erase(iterator first, iterator last)
			{
				size_type count;
				iterator it;

				count = 0;
				for (it = first; it != last; it++)
				{
					_allocator.destroy(&(*it));
					count++;
				}
				rshift(first, count);
				return (first);
			}

			void swap(vector &other)
			{
				if (this == &other)
					return ;
				std::swap(_allocator, other._allocator);
				std::swap(_capacity, other._capacity);
				std::swap(_size, other._size);
				std::swap(_array, other._array);
			}

			void clear()
			{
				destroy();
				deallocate();
				init();
			}

			// -----===== ALLOCATOR =====-----

			allocator_type get_allocator() const
			{
				return (allocator_type(_allocator));
			}

		private:
			void init(size_type capacity = 0)
			{
				_capacity = capacity;
				_size = 0;
				_array = NULL;
				if (capacity != 0)
					_array = _allocator.allocate(_capacity);
			}

			void destroy(size_type start = 0)
			{
				for (size_type i = start; i < _size; i++)
					_allocator.destroy(&_array[i]);
				_size = start;
			}

			void deallocate()
			{
				if (_array != NULL)
					_allocator.deallocate(_array, _capacity);
				_array = NULL;
			}

			size_type grow_capacity() const
			{
				size_type max;

				if (_capacity == 0)
					return (2);
				max = max_size();
				if (_capacity < max / 2)
					return (_capacity * 2);
				return (max);
			}

			iterator shift(iterator position, size_type count)
			{
				size_type capacity2;
				size_type size2;
				size_type index;
				size_type delta;
				iterator it;
				T *array2;

				size2 = _size + count;
				if (count == 0)
					return (position);
				if (size2 <= _capacity)
				{
					for (it = end() - 1; it >= position; --it)
					{
						_allocator.construct(&(*(it + count)), *it);
						_allocator.destroy(&(*it));
					}
					_size = size2;
					return (position);
				}
				capacity2 = grow_capacity();
				if (capacity2 < size2)
					capacity2 = size2;
				if (capacity2 > max_size())
					throw (std::length_error("resize larger than max size"));
				delta = position - begin();
				array2 = _allocator.allocate(capacity2, _array);
				index = 0;
				for (it = begin(); it != position; ++it)
				{
					_allocator.construct(&array2[index], *it);
					index++;
				}
				for (it = position; it != end(); it++)
				{
					_allocator.construct(&array2[index + count], *it);
					index++;
				}
				destroy();
				deallocate();
				_capacity = capacity2;
				_size = size2;
				_array = array2;
				return (begin() + delta);
			}

			void rshift(iterator position, size_type count)
			{
				iterator it;

				if (count == 0)
					return ;
				for (it = position; it != end() - count; ++it)
				{
					_allocator.construct(&(*it), *(it + count));
					_allocator.destroy(&(*(it + count)));
				}
				_size -= count;
			}

			allocator_type	_allocator;
			value_type		*_array;
			size_type		_capacity;
			size_type		_size;
	};

	template <class T, class A>
	void swap(vector<T, A> &lhs, vector<T, A> &rhs)
	{
		if (&lhs == &rhs)
			return ;
		lhs.swap(rhs);
	}

	template <class T, class A>
	bool operator==(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class A>
	bool operator!=(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <class T, class A>
	bool operator<(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end()));
	}

	template <class T, class A>
	bool operator<=(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		return (!(lhs > rhs));
	}
	
	template <class T, class A>
	bool operator>(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class A>
	bool operator>=(const vector<T, A> &lhs, const vector<T, A> &rhs)
	{
		return (!(lhs < rhs));
	}
}

#endif
