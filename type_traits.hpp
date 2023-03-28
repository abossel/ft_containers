/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 01:45:47 by abossel           #+#    #+#             */
/*   Updated: 2023/02/24 10:59:39 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

# include <limits>

namespace ft
{
	// enable_if

	template <bool B, class T>
	struct enable_if
	{
		typedef T type;
	};

	template <class T>
	struct enable_if<false, T>
	{
	};

	// is_integral

	template <typename T>
	struct is_integral
	{
		static const bool value = std::numeric_limits<T>::is_integer;
	};
}

#endif
