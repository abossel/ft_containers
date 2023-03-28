/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_tree2.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 08:39:05 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:15:24 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_TREE_HPP
# define MAP_TREE_HPP

# include <stdint.h>
# include <stddef.h>
# include <iostream>
# include <string>
# include <memory>

# define MAP_FLAG_NIL 0x1
# define MAP_FLAG_RED 0x2
# define MAP_FLAG_FIRST 0x4
# define MAP_FLAG_LAST 0x8

namespace ft
{
	struct map_node
	{
		uintptr_t		data;
		unsigned int	flags;
		struct map_node	*parent;
		struct map_node	*left;
		struct map_node	*right;
	};

	template <class A = std::allocator<struct map_node> >
	class map_tree
	{
		public:
			typedef struct map_node	node_type;
			typedef A				allocator_type;

			map_tree(const allocator_type &allocator = allocator_type())
			{
				_allocator = allocator;
				_empty.data = 0;
				_tree = NULL;
				_current_first = NULL;
				_current_last = NULL;
			}

			map_tree(const map_tree &other)
			{
				_allocator = other._allocator;
				_empty.data = 0;
				_tree = copy_tree(other._tree);
				_current_first = find_first();
				_current_last = find_last();
			}

			~map_tree()
			{
				delete_all(_tree);
			}

			node_type *get_root() const
			{
				return (_tree);
			}

			node_type *get_first() const
			{
				return (_current_first);
			}

			node_type *get_last() const
			{
				return (_current_last);
			}

			static bool is_nil(node_type *node)
			{
				return ((node->flags & MAP_FLAG_NIL) != 0);
			}

			static bool is_first(node_type *node)
			{
				return ((node->flags & MAP_FLAG_FIRST) != 0);
			}

			static bool is_last(node_type *node)
			{
				return ((node->flags & MAP_FLAG_LAST) != 0);
			}

			static bool is_leaf(node_type *node)
			{
				return (is_nil(node->left) && is_nil(node->right));
			}

			static bool is_root(node_type *node)
			{
				return (node->parent == NULL);
			}

			static bool is_left_child(node_type *node)
			{
				return (node == node->parent->left);
			}

			static bool is_right_child(node_type *node)
			{
				return (node == node->parent->right);
			}

			static node_type *next_node(node_type *node)
			{
				if (is_nil(node))
				{
					if (is_first(node->parent))
						node = node->parent;
				}
				else if (is_last(node))
					node = node->right;
				else if (!is_nil(node->right))
				{
					node = node->right;
					while (!is_nil(node->left))
						node = node->left;
				}
				else
				{
					while (is_right_child(node))
						node = node->parent;
					node = node->parent;
				}
				return (node);
			}

			static node_type *prev_node(node_type *node)
			{
				if (is_nil(node))
				{
					if (is_last(node->parent))
						node = node->parent;
				}
				else if (is_first(node))
					node = node->left;
				else if (!is_nil(node->left))
				{
					node = node->left;
					while (!is_nil(node->right))
						node = node->right;
				}
				else
				{
					while (is_left_child(node))
						node = node->parent;
					node = node->parent;
				}
				return (node);
			}

			void remove(node_type *node)
			{
				if (is_leaf(node))
					remove_leaf(node);
				else if (is_nil(node->left) || is_nil(node->right))
					remove_branch_single(node);
				else
					remove_branch_right(node);
			}

			node_type *add_left(uintptr_t data, node_type *node)
			{
				node_type *new_left;

				if (node == NULL)
				{
					new_root(data);
					new_left = get_root();
				}
				else
				{
					new_left = new_node(data, node);
					delete_all(new_left->left);
					new_left->left = node->left;
					new_left->left->parent = new_left;
					node->left = new_left;
					if (is_first(node))
						set_first(node->left);
				}
				fix_rb_insert(new_left);
				return (new_left);
			}

			node_type *add_right(uintptr_t data, node_type *node)
			{
				node_type *new_right;

				if (node == NULL)
				{
					new_root(data);
					new_right = get_root();
				}
				else
				{
					new_right = new_node(data, node);
					delete_all(new_right->right);
					new_right->right = node->right;
					new_right->right->parent = new_right;
					node->right = new_right;
					if (is_last(node))
						set_last(node->right);
				}
				fix_rb_insert(new_right);
				return (new_right);
			}

			void swap(map_tree &other)
			{
				std::swap(_allocator, other._allocator);
				std::swap(_tree, other._tree);
				std::swap(_current_first, other._current_first);
				std::swap(_current_last, other._current_last);
			}

		private:
			/*
			 * returns true if a node is red
			 */
			bool is_red(node_type *node) const
			{
				return ((node->flags & MAP_FLAG_RED) != 0);
			}

			/*
			 * sets the colour of a node to red
			 */
			void set_red(node_type *node) const
			{
				unsigned int flag;

				flag = MAP_FLAG_RED;
				node->flags |= flag;
			}

			/*
			 * sets the colour of a node to black
			 */
			void set_black(node_type *node) const
			{
				unsigned int flag;

				flag = MAP_FLAG_RED;
				node->flags &= ~flag;
			}

			/*
			 * set a node to be the first node
			 * removes the flag from the previous first node
			 * call this function before deleting the old first node
			 */
			void set_first(node_type *node)
			{
				unsigned int flag;

				flag = MAP_FLAG_FIRST;
				if (_current_first != NULL)
					_current_first->flags &= ~flag;
				_current_first = node;
				_current_first->flags |= flag;
			}

			/*
			 * set a node to be the last node
			 * removes the flag from the previous last node
			 * call this function before deleting the old last node
			 */
			void set_last(node_type *node)
			{
				unsigned int flag;

				flag = MAP_FLAG_LAST;
				if (_current_last != NULL)
					_current_last->flags &= ~flag;
				_current_last = node;
				_current_last->flags |= flag;
			}

			/*
			 * create a new empty node
			 */
			node_type *new_empty(node_type *parent = NULL,  unsigned int flags = 0)
			{
				node_type *node;

				node = _allocator.allocate(1);
				_allocator.construct(node, _empty);
				node->parent = parent;
				node->data = 0;
				node->flags = flags;
				node->left = NULL;
				node->right = NULL;
				return (node);
			}

			/*
			 * create a new node with nil nodes
			 */
			node_type *new_node(uintptr_t data, node_type *parent = NULL)
			{
				node_type *node;

				node = new_empty(parent, MAP_FLAG_RED);
				node->data = data;
				node->left = new_empty(node, MAP_FLAG_NIL);
				node->right = new_empty(node, MAP_FLAG_NIL);
				return (node);
			}

			/*
			 * create a new root node
			 */
			void new_root(uintptr_t data)
			{
				_tree = new_node(data, NULL);
				set_first(_tree);
				set_last(_tree);
			}

			/*
			 * make a shallow copy of the tree
			 */
			node_type *copy_tree(node_type *other, node_type *parent = NULL)
			{
				node_type *node;

				if (other == NULL)
					return (NULL);
				node = new_empty(parent, other->flags);
				node->data = other->data;
				node->left = copy_tree(other->left, node);
				node->right = copy_tree(other->right, node);
				return (node);
			}

			/*
			 * print out the tree
			 * the depth parameter is used for recursion and is not required
			 */
			void print_tree(node_type *node, unsigned int depth = 0)
			{
				if (node == NULL)
				{
					std::cout << "null" << std::endl;
					return ;
				}
				if (!is_nil(node->right))
					print_tree(node->right, depth + 1);
				std::cout << std::string(depth, ' ') << "0x" << std::hex << node->data;
				if (is_first(node))
					std::cout << " F";
				if (is_last(node))
					std::cout << " L";
				if (node->left->parent != node)
					std::cout << " XL";
				if (node->right->parent != node)
					std::cout << " XR";
				std::cout << std::endl;
				if (!is_nil(node->left))
					print_tree(node->left, depth + 1);
			}

			/*
			 * find the smallest node in the tree
			 */
			node_type *find_first() const
			{
				node_type *node;

				node = _tree;
				if (node != NULL)
					while (!is_nil(node->left))
						node = node->left;
				return (node);
			}

			/*
			 * find the largest node in the tree
			 */
			node_type *find_last() const
			{
				node_type *node;

				node = _tree;
				if (node != NULL)
					while (!is_nil(node->right))
						node = node->right;
				return (node);
			}

			/*
			 * set the parent of old_child to point to new_child
			 */
			void change_child(node_type *old_child, node_type *new_child)
			{
				if (is_root(old_child))
					_tree = new_child;
				else if (is_left_child(old_child))
					old_child->parent->left = new_child;
				else if (is_right_child(old_child))
					old_child->parent->right = new_child;
			}

			/*
			 * swap a branch with another branch lower in the tree
			 */
			void swap_branch_leaf(node_type *branch, node_type *leaf)
			{
				node_type *node;
				bool is_red_branch;

				// keep the node colours in the same position
				is_red_branch = is_red(branch);
				if (is_red(leaf))
					set_red(branch);
				else
					set_black(branch);
				if (is_red_branch)
					set_red(leaf);
				else
					set_black(leaf);
				// set the branch parent to point to the leaf
			 	change_child(branch, leaf);
				// don't change the leaf parent if it is the branch
			 	if (leaf->parent != branch)
			 		change_child(leaf, branch);
				// special case if the branch and leaf are parent and child
				if (branch->left == leaf)
					branch->left = branch;
				if (branch->right == leaf)
					branch->right = branch;
				if (leaf->parent == branch)
					leaf->parent = leaf;
				// swap the internal pointers
				node = branch->left;
				branch->left = leaf->left;
				leaf->left = node;
				node = branch->right;
				branch->right = leaf->right;
				leaf->right = node;
				node = branch->parent;
				branch->parent = leaf->parent;
				leaf->parent = node;
				// reset the child parent pointers
				branch->left->parent = branch;
				branch->right->parent = branch;
				leaf->left->parent = leaf;
				leaf->right->parent = leaf;
			}

			/*
			 * delete a node and all of it's childen
			 */
			void delete_all(node_type *node)
			{
				if (node == NULL)
					return ;
				delete_all(node->left);
				delete_all(node->right);
				_allocator.destroy(node);
				_allocator.deallocate(node, 1);
			}

			/*
			 * delete a leaf node
			 */
			void remove_leaf(node_type *node)
			{
				node_type *next;
				bool fix_next;

				next = NULL;
				fix_next = !is_red(node);
				if (is_root(node))
				{
					fix_next = false;
					_tree = NULL;
					_current_first = NULL;
					_current_last = NULL;
				}
				else if (is_left_child(node))
				{
					next = node->left;
					node->parent->left = node->left;
					node->left->parent = node->parent;
					node->left = NULL;
					if (is_first(node))
						set_first(node->parent);
				}
				else if (is_right_child(node))
				{
					next = node->right;
					node->parent->right = node->right;
					node->right->parent = node->parent;
					node->right = NULL;
					if (is_last(node))
						set_last(node->parent);
				}
				delete_all(node);
				if (fix_next)
					fix_rb_remove(next);
			}

			/*
			 * delete a branch node with only 1 child node
			 */
			void remove_branch_single(node_type *branch)
			{
				node_type *node;
				node_type *next;
				bool fix_next;

				next = NULL;
				fix_next = !is_red(branch);
				if (!is_nil(branch->left))
				{
					next = branch->left;
					if (is_last(branch))
					{
						node = branch->left;
						while (!is_nil(node->right))
							node = node->right;
						set_last(node);
					}
					change_child(branch, branch->left);
					branch->left->parent = branch->parent;
					branch->left = NULL;
					delete_all(branch);
				}
				else if (!is_nil(branch->right))
				{
					next = branch->right;
					if (is_first(branch))
					{
						node = branch->right;
						while (!is_nil(node->left))
							node = node->left;
						set_first(node);
					}
					change_child(branch, branch->right);
					branch->right->parent = branch->parent;
					branch->right = NULL;
					delete_all(branch);
				}
				if (fix_next)
					fix_rb_remove(next);
			}

			void rotate_left(node_type *node)
			{
				node_type *node_right;

				node_right = node->right;
				node->right = node_right->left;
				node->right->parent = node;
				change_child(node, node_right);
				node_right->left = node;
				node_right->parent = node->parent;
				node->parent = node_right;
			}

			void rotate_right(node_type *node)
			{
				node_type *node_left;

				node_left = node->left;
				node->left = node_left->right;
				node->left->parent = node;
				change_child(node, node_left);
				node_left->right = node;
				node_left->parent = node->parent;
				node->parent = node_left;
			}

			void fix_rb_insert(node_type *node)
			{
				node_type *parent;
				node_type *grand;
				node_type *uncle;

				// case 1: new node is the root
				// set node black and stop recursion
				if (is_root(node))
				{
					set_black(node);
					return ;
				}
				// case 2: parent node is red and the root
				// if parent is black nothing left to do
				// don't need to check if parent is root
				// because root is always black
				parent = node->parent;
				if (!is_red(parent))
					return ;
				// case 3: parent and uncle nodes are red
				// get the grand-parent and uncle
				grand = parent->parent;
				if (is_left_child(parent))
					uncle = grand->right;
				else
					uncle = grand->left;
				// if uncle is red change colours
				// run fix on grand-parent
				if (is_red(uncle))
				{
					set_black(parent);
					set_red(grand);
					set_black(uncle);
					fix_rb_insert(grand);
				}
				// case 4: parent node is red
				// uncle node is black
				// inserted node is inner grand-child
				else if (is_left_child(parent))
				{
					if (is_right_child(node))
					{
						rotate_left(parent);
						parent = node;
					}
					rotate_right(grand);
					set_black(parent);
					set_red(grand);
				}
				// case 5: parent node is red
				// uncle node is black
				// inserted node is outer grand-child
				else
				{
					if (is_left_child(node))
					{
						rotate_right(parent);
						parent = node;
					}
					rotate_left(grand);
					set_black(parent);
					set_red(grand);
				}
			}

			node_type *get_sibling(node_type *node) const
			{
				if (is_left_child(node))
					return (node->parent->right);
				return (node->parent->left);
			}

			void fix_rb_remove(node_type *node)
			{
				node_type *parent;
				node_type *sibling;

				// case 1: end recursion at root
				if (is_root(node))
				{
					set_black(node);
					return ;
				}
				// case 2: red sibling
				parent = node->parent;
				sibling = get_sibling(node);
				if (is_red(sibling))
				{
					set_black(sibling);
					set_red(parent);
					if (is_left_child(node))
						rotate_left(parent);
					else
						rotate_right(parent);
					sibling = get_sibling(node);
				}
				// case 3+4: black sibling with two black children
				if (!is_red(sibling->left) && !is_red(sibling->right))
				{
					// parent might be different after rotation
					parent = node->parent;
					set_red(sibling);
					// case 3: black sibling with two black children + red parent
					if (is_red(parent))
						set_black(parent);
					// case 4: black sibling with two black children + black parent
					else
						fix_rb_remove(parent);
				}
				// case 5+6: black sibling with at least one red child
				else
				{
					// case 5: Black sibling with at least one red child + "outer nephew" is black
					if (is_left_child(node) && !is_red(sibling->right))
					{
						set_black(sibling->left);
						set_red(sibling);
						rotate_right(sibling);
						sibling = node->parent->right;
					}
					else if (is_right_child(node) && !is_red(sibling->left))
					{
						set_black(sibling->right);
						set_red(sibling);
						rotate_left(sibling);
						sibling = node->parent->left;
					}
					// case 6: black sibling with at least one red child + "outer nephew" is red
					// parent might be different after rotation
					parent = node->parent;
					if (is_red(parent))
						set_red(sibling);
					else
						set_black(sibling);
					set_black(parent);
					if (is_left_child(node))
					{
						set_black(sibling->right);
						rotate_left(parent);
					}
					else
					{
						set_black(sibling->left);
						rotate_right(parent);
					}
				}
			}

			/*
			 * remove a branch node by swapping with a leaf node on the left side
			 */
			void remove_branch_left(node_type *node)
			{
				node_type *leaf;

				leaf = node->left;
				while (!is_nil(leaf->right))
					leaf = leaf->right;
				if (is_last(node))
					set_last(leaf);
				swap_branch_leaf(node, leaf);
				if (is_leaf(node))
					remove_leaf(node);
				else
					remove_branch_single(node);
			}

			/*
			 * remove a branch node by swapping with a leaf node on the right side
			 */
			void remove_branch_right(node_type *node)
			{
				node_type *leaf;

				leaf = node->right;
				while (!is_nil(leaf->left))
					leaf = leaf->left;
				if (is_first(node))
					set_first(leaf);
				swap_branch_leaf(node, leaf);
				if (is_leaf(node))
					remove_leaf(node);
				else
					remove_branch_single(node);
			}

			node_type		*_tree;
			node_type		*_current_first;
			node_type		*_current_last;
			allocator_type	_allocator;
			node_type		_empty;
	};
}

#endif
