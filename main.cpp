/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 01:12:54 by abossel           #+#    #+#             */
/*   Updated: 2023/03/28 18:48:08 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>
#include "vector.hpp"
#include <stack>
#include "stack.hpp"
#include <set>
#include "set.hpp"
#include <map>
#include "map.hpp"
#include "utility.hpp"

#define STD_TEST
#define FT_TEST

#ifdef STD_TEST
static const bool g_std_test = true;
#else
static const bool g_std_test = false;
#endif

#ifdef FT_TEST
static const bool g_ft_test = true;
#else
static const bool g_ft_test = false;
#endif

#define CON_RED "\033[0;31m"
#define CON_GREEN "\033[0;32m"
#define CON_YELLOW "\033[0;33m"
#define CON_PURPLE "\033[0;35m"
#define CON_RESET "\033[0m"

typedef std::vector<ft::pair<int, std::string> > svector;
typedef ft::vector<ft::pair<int, std::string> > fvector;
typedef svector::iterator svector_iterator;
typedef fvector::iterator fvector_iterator;
typedef std::reverse_iterator<svector_iterator> svector_riterator;
typedef ft::reverse_iterator<fvector_iterator> fvector_riterator;

typedef std::map<int, std::string> smap;
typedef ft::map<int, std::string> fmap;
typedef smap::iterator smap_iterator;
typedef fmap::iterator fmap_iterator;
typedef std::reverse_iterator<smap_iterator> smap_riterator;
typedef ft::reverse_iterator<fmap_iterator> fmap_riterator;

typedef std::set<std::string> sset;
typedef ft::set<std::string> fset;
typedef sset::iterator sset_iterator;
typedef fset::iterator fset_iterator;
typedef std::reverse_iterator<sset_iterator> sset_riterator;
typedef ft::reverse_iterator<fset_iterator> fset_riterator;

typedef std::stack<int> sstack;
typedef ft::stack<int> fstack;

class crc32
{
	public:
		crc32()
		{
			generate();
			reset();
		}

		~crc32()
		{
		}

		void add(unsigned char byte)
		{
			_crc = ~_crc;
			_crc = (_crc >> 8) ^ _lookup[(_crc & 0xff) ^ byte];
			_crc = ~_crc;
		}

		void add(int n)
		{
			unsigned int i;

			for (i = 0; i < sizeof(int); i++)
			{
				add((unsigned char)(n & 0xff));
				n >>= 8;
			}
		}

		void add(std::string s)
		{
			unsigned int i;

			for (i = 0; i < s.size(); i++)
				add((unsigned char)s.c_str()[i]);
		}

		unsigned int crc()
		{
			return (_crc);
		}

		void reset()
		{
			_crc = 0;
		}

	private:
		void generate()
		{
			unsigned int rem;
			unsigned int i;
			unsigned int j;

			for (i = 0; i < 256; i++) {
				rem = i;
				for (j = 0; j < 8; j++) {
					if (rem & 1) {
						rem >>= 1;
						rem ^= 0xedb88320;
					} else
						rem >>= 1;
				}
				_lookup[i] = rem;
			}
		}

		unsigned int _lookup[256];
		unsigned int _crc;
};

unsigned int stoui(std::string s)
{
    std::stringstream ss(s);
    unsigned int n;

	n = 0;
    ss >> n;
    return (n);
}

/*
 * convert an int to a string
 */
std::string itos(int i)
{
	std::stringstream ss;

	ss << i;
	return (ss.str());
}

/*
 * get the current time in milliseconds
 */
time_t millis()
{
	struct timeval tv;
	time_t ms;

	gettimeofday(&tv, NULL);
	ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (ms);
}

/*
 * print the time difference in ms between now and start
 */
void print_time_diff(time_t start)
{
	std::cout << CON_YELLOW << "time: " << (millis() - start) << "ms." << CON_RESET << std::endl;
}

/*
 * print the time difference in ms between now and start
 */
void print_crc32(crc32 check)
{
	std::cout << CON_PURPLE << "info: crc32 " << check.crc() << "." << CON_RESET << std::endl;
}

/*
 * compare a std and ft vector
 */
bool compare_vectors(svector &sv, fvector &fv)
{
	svector_iterator sit;
	fvector_iterator fit;
	svector_riterator rsit;
	fvector_riterator rfit;
	crc32 check;

	if (g_std_test && !g_ft_test)
	{
		check.reset();
		for (sit = sv.begin(); sit != sv.end(); sit++)
		{
			check.add((*sit).first);
			check.add((*sit).second);
		}
		print_crc32(check);
	}

	if (g_ft_test && !g_std_test)
	{
		check.reset();
		for (fit = fv.begin(); fit != fv.end(); fit++)
		{
			check.add((*fit).first);
			check.add((*fit).second);
		}
		print_crc32(check);
	}

	if (!g_ft_test || !g_std_test)
		return (true);

	if (sv.size() != fv.size())
	{
		std::cout << CON_RED << "error: vector sizes don't match."
			<< CON_RESET << std::endl;
		return (false);
	}
	sit = sv.begin();
	fit = fv.begin();
	while (sit != sv.end() && fit != fv.end())
	{
		if ((*sit).first != (*fit).first || (*sit).second != (*fit).second)
		{
			std::cout << CON_RED << "error: vector elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		sit++;
		fit++;
	}
	if (sit != sv.end() || fit != fv.end())
	{
		std::cout << CON_RED << "error: vector iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	rsit = sv.rbegin();
	rfit = fv.rbegin();
	while (rsit != sv.rend() && rfit != fv.rend())
	{
		if ((*rsit).first != (*rfit).first || (*rsit).second != (*rfit).second)
		{
			std::cout << CON_RED << "error: vector reverse iterator elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		rsit++;
		rfit++;
	}
	if (rsit != sv.rend() || rfit != fv.rend())
	{
		std::cout << CON_RED << "error: vector reverse iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	std::cout << CON_GREEN << "info: vectors match."
		<< CON_RESET << std::endl;
	return (true);
}

/*
 * use the insert(position, value) function to insert into the vector
 */
void compare_vector_insert(svector &sv, fvector &fv, unsigned int size, unsigned int seed)
{
	unsigned int i;
	time_t start;
	int	r;

	if (g_std_test)
	{
		std::cout << "info: std::vector::insert(position, value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			if (i % 2 == 0)
				sv.insert(sv.begin() + (sv.size() / 2), ft::make_pair(r, itos(r)));
			else
				sv.insert(sv.begin() + (rand() % sv.size()), ft::make_pair(r, itos(r)));
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::insert(position, value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			if (i % 2 == 0)
				fv.insert(fv.begin() + (fv.size() / 2), ft::make_pair(r, itos(r)));
			else
				fv.insert(fv.begin() + (rand() % fv.size()), ft::make_pair(r, itos(r)));
		}
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the insert(position, first, last) function to insert into the vector
 */
void compare_vector_insert_range(svector &sv, fvector &fv,
	unsigned int size, unsigned int iter, unsigned int seed)
{
	svector sv2;
	fvector fv2;
	unsigned int i;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::vector::insert(position, first, last) with "
			<< size << " elements and " << iter << " iterations." << std::endl;
		sv2 = sv;
		sv2.resize(size);
		srand(seed);
		start = millis();
		for (i = 0; i < iter; i++)
		{
			if (i % 2 == 0)
				sv.insert(sv.begin() + (sv.size() / 2), sv2.begin(), sv2.end());
			else
				sv.insert(sv.begin() + (rand() % sv.size()), sv2.begin(), sv2.end());
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::insert(position, first, last) with "
			<< size << " elements and " << iter << " iterations." << std::endl;
		fv2 = fv;
		fv2.resize(size);
		srand(seed);
		start = millis();
		for (i = 0; i < iter; i++)
		{
			if (i % 2 == 0)
				fv.insert(fv.begin() + (fv.size() / 2), fv2.begin(), fv2.end());
			else
				fv.insert(fv.begin() + (rand() % fv.size()), fv2.begin(), fv2.end());
		}
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the erase() function
 */
void compare_vector_erase(svector &sv, fvector &fv, unsigned int seed)
{
	svector_iterator sit;
	fvector_iterator fit;
	svector::size_type ssize;
	fvector::size_type fsize;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::vector::erase(position) and std::erase(first, last) with "
			<< sv.size() << " elements." << std::endl;
		srand(seed);
		start = millis();
		while (sv.size() != 0)
		{
			ssize = sv.size();
			if (ssize > 30 && ssize % 2 == 0)
			{
				sit = sv.begin() + (ssize / 2);
				sv.erase(sit, sit + 10);
			}
			else
				sv.erase(sv.begin() + (rand() % ssize));
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::erase(position) and ft::erase(first, last) with "
			<< fv.size() << " elements." << std::endl;
		srand(seed);
		start = millis();
		while (fv.size() != 0)
		{
			fsize = fv.size();
			if (fsize > 30 && fsize % 2 == 0)
			{
				fit = fv.begin() + (fsize / 2);
				fv.erase(fit, fit + 10);
			}
			else
				fv.erase(fv.begin() + (rand() % fsize));
		}
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the clear() function
 */
void compare_vector_clear(svector &sv, fvector &fv)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::vector::clear() with " << sv.size()
			<< " elements." << std::endl;
		start = millis();
		sv.clear();
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::clear() with " << fv.size()
			<< " elements." << std::endl;
		start = millis();
		fv.clear();
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the resize() function
 */
void compare_vector_resize(svector &sv, fvector &fv,
	unsigned int size)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::vector::resize() from " << sv.size()
			<< " to " << size << " elements." << std::endl;
		start = millis();
		sv.resize(size);
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::resize() from " << fv.size()
			<< " to " << size << " elements." << std::endl;
		start = millis();
		fv.resize(size);
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the push_back(value) function
 */
void compare_vector_push_back(svector &sv, fvector &fv,
	unsigned int size, unsigned int seed)
{
	unsigned int i;
	time_t start;
	int	r;

	if (g_std_test)
	{
		std::cout << "info: std::vector::push_back(value) with "
			<< size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			sv.push_back(ft::make_pair(r, itos(r)));
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::push_back(value) with "
			<< size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			fv.push_back(ft::make_pair(r, itos(r)));
		}
		print_time_diff(start);
	}
	compare_vectors(sv, fv);
}

/*
 * use the copy operator
 */
void compare_vector_copy_operator(svector &sv, fvector &fv, unsigned int iter)
{
	unsigned int i;
	time_t start;
	svector sv2;
	fvector fv2;

	if (g_std_test)
	{
		std::cout << "info: std::vector::operator=(other) with "
			<< iter << " iterations." << std::endl;
		start = millis();
		for (i = 0; i < iter; i++)
		{
			sv2 = sv;
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::vector::operator=(other) with "
			<< iter << " iterations." << std::endl;
		start = millis();
		for (i = 0; i < iter; i++)
		{
			fv2 = fv;
		}
		print_time_diff(start);
	}
	compare_vectors(sv2, fv2);
}

/*
 * compare a std and ft map
 */
bool compare_maps(smap &sm, fmap &fm)
{
	smap_iterator sit;
	fmap_iterator fit;
	smap_riterator rsit;
	fmap_riterator rfit;
	crc32 check;

	if (g_std_test && !g_ft_test)
	{
		check.reset();
		for (sit = sm.begin(); sit != sm.end(); sit++)
		{
			check.add((*sit).first);
			check.add((*sit).second);
		}
		print_crc32(check);
	}

	if (g_ft_test && !g_std_test)
	{
		check.reset();
		for (fit = fm.begin(); fit != fm.end(); fit++)
		{
			check.add((*fit).first);
			check.add((*fit).second);
		}
		print_crc32(check);
	}

	if (!g_std_test || !g_ft_test)
		return (true);

	if (sm.size() != fm.size())
	{
		std::cout << CON_RED << "error: map sizes don't match."
			<< CON_RESET << std::endl;
		return (false);
	}
	sit = sm.begin();
	fit = fm.begin();
	while (sit != sm.end() && fit != fm.end())
	{
		if ((*sit).first != (*fit).first || (*sit).second != (*fit).second)
		{
			std::cout << CON_RED << "error: map elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		sit++;
		fit++;
	}
	if (sit != sm.end() || fit != fm.end())
	{
		std::cout << CON_RED << "error: map iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	rsit = sm.rbegin();
	rfit = fm.rbegin();
	while (rsit != sm.rend() && rfit != fm.rend())
	{
		if ((*rsit).first != (*rfit).first || (*rsit).second != (*rfit).second)
		{
			std::cout << CON_RED << "error: map reverse iterator elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		rsit++;
		rfit++;
	}
	if (rsit != sm.rend() || rfit != fm.rend())
	{
		std::cout << CON_RED << "error: map iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	std::cout << CON_GREEN << "info: maps match."
		<< CON_RESET << std::endl;
	return (true);
}

/*
 * use the clear() function
 */
void compare_map_clear(smap &sm, fmap &fm)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::map::clear() with " << sm.size()
			<< " elements." << std::endl;
		start = millis();
		sm.clear();
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::clear() with " << fm.size()
			<< " elements." << std::endl;
		start = millis();
		fm.clear();
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * use the erase(key) function
 */
void compare_map_erase_key(smap &sm, fmap &fm, unsigned int skip)
{
	smap_iterator sit;
	fmap_iterator fit;
	unsigned int skip_count;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::map::erase(key) erase every "
			<< skip << " element." << std::endl;
		start = millis();
		sit = sm.begin();
		skip_count = 0;
		while (sit != sm.end())
		{
			if (skip_count == skip)
			{
				skip_count = 0;
				sm.erase((sit++)->first);
				continue ;
			}
			skip_count++;
			sit++;
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::erase(key) erase every "
			<< skip << " element." << std::endl;
		start = millis();
		fit = fm.begin();
		skip_count = 0;
		while (fit != fm.end())
		{
			if (skip_count == skip)
			{
				skip_count = 0;
				fm.erase((fit++)->first);
				continue ;
			}
			skip_count++;
			fit++;
		}
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * use the erase(first, last) function
 */
void compare_map_erase_range(smap &sm, fmap &fm)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::map::erase(first, last)." << std::endl;
		start = millis();
		sm.erase(sm.begin(), sm.end());
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::erase(first, last)." << std::endl;
		start = millis();
		fm.erase(fm.begin(), fm.end());
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * use the insert(value) function to insert into the map
 */
void compare_map_insert(smap &sm, fmap &fm, unsigned int size, unsigned int seed)
{
	smap_iterator sit;
	fmap_iterator fit;
	unsigned int i;
	time_t start;
	int	r;

	if (g_std_test)
	{
		std::cout << "info: std::map::insert(value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			if (i % 10 != 0)
				sm.insert(std::make_pair(r, itos(r)));
			else
			{
				sit = sm.insert(sm.begin(), std::make_pair(r, itos(r)));
				if (sit->first != r)
				std::cout << CON_RED << "error: return iterators don't match data."
					<< CON_RESET << std::endl;
			}
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::insert(value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = rand();
			if (i % 10 != 0)
				fm.insert(ft::make_pair(r, itos(r)));
			else
			{
				fit = fm.insert(fm.begin(), ft::make_pair(r, itos(r)));
				if (fit->first != r)
				std::cout << CON_RED << "error: return iterators don't match data."
					<< CON_RESET << std::endl;
			}
		}
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * use the insert(position, first, last) function to insert into the map
 */
void compare_map_insert_range(smap &sm, fmap &fm,
	smap_iterator sfirst, smap_iterator slast,
	fmap_iterator ffirst, fmap_iterator flast)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::map::insert(first, last)." << std::endl;
		start = millis();
		sm.insert(sfirst, slast);
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::insert(first, last)." << std::endl;
		start = millis();
		fm.insert(ffirst, flast);
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * use the find(key) function to search for a value
 */
void compare_map_count(smap &sm, fmap &fm,
	unsigned int size, unsigned int seed)
{
	unsigned int found;
	unsigned int i;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::map::count(key) "
			<< size << " times." << std::endl;
		srand(seed);
		found = 0;
		start = millis();
		for (i = 0; i < size; i++)
			found += sm.count(rand() % 10000);
		std::cout << "info: found " << found << " matches." << std::endl;
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::map::count(key) "
			<< size << " times." << std::endl;
		srand(seed);
		found = 0;
		start = millis();
		for (i = 0; i < size; i++)
			found += fm.count(rand() % 10000);
		std::cout << "info: found " << found << " matches." << std::endl;
		print_time_diff(start);
	}
	compare_maps(sm, fm);
}

/*
 * compare a std and ft set
 */
bool compare_sets(sset &sm, fset &fm)
{
	sset_iterator sit;
	fset_iterator fit;
	sset_riterator rsit;
	fset_riterator rfit;
	crc32 check;

	if (g_std_test && !g_ft_test)
	{
		check.reset();
		for (sit = sm.begin(); sit != sm.end(); sit++)
		{
			check.add(*sit);
			check.add(*sit);
		}
		print_crc32(check);
	}

	if (g_ft_test && !g_std_test)
	{
		check.reset();
		for (fit = fm.begin(); fit != fm.end(); fit++)
		{
			check.add(*fit);
			check.add(*fit);
		}
		print_crc32(check);
	}

	if (!g_std_test || !g_ft_test)
		return (true);

	if (sm.size() != fm.size())
	{
		std::cout << CON_RED << "error: set sizes don't match."
			<< CON_RESET << std::endl;
		return (false);
	}
	sit = sm.begin();
	fit = fm.begin();
	while (sit != sm.end() && fit != fm.end())
	{
		if ((*sit) != (*fit) || (*sit) != (*fit))
		{
			std::cout << CON_RED << "error: set elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		sit++;
		fit++;
	}
	if (sit != sm.end() || fit != fm.end())
	{
		std::cout << CON_RED << "error: set iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	rsit = sm.rbegin();
	rfit = fm.rbegin();
	while (rsit != sm.rend() && rfit != fm.rend())
	{
		if ((*rsit) != (*rfit) || (*rsit) != (*rfit))
		{
			std::cout << CON_RED << "error: set reverse iterator elements don't match."
				<< CON_RESET << std::endl;
			return (false);
		}
		rsit++;
		rfit++;
	}
	if (rsit != sm.rend() || rfit != fm.rend())
	{
		std::cout << CON_RED << "error: set iterators are not at end."
			<< CON_RESET << std::endl;
		return (false);
	}
	std::cout << CON_GREEN << "info: sets match."
		<< CON_RESET << std::endl;
	return (true);
}

/*
 * use the clear() function
 */
void compare_set_clear(sset &sm, fset &fm)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::set::clear() with " << sm.size()
			<< " elements." << std::endl;
		start = millis();
		sm.clear();
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::clear() with " << fm.size()
			<< " elements." << std::endl;
		start = millis();
		fm.clear();
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the erase(key) function
 */
void compare_set_erase_key(sset &sm, fset &fm, unsigned int skip)
{
	sset_iterator sit;
	fset_iterator fit;
	unsigned int skip_count;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::set::erase(key) erase every "
			<< skip << " element." << std::endl;
		start = millis();
		sit = sm.begin();
		skip_count = 0;
		while (sit != sm.end())
		{
			if (skip_count == skip)
			{
				skip_count = 0;
				sm.erase(sit++);
				continue ;
			}
			skip_count++;
			sit++;
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::erase(key) erase every "
			<< skip << " element." << std::endl;
		start = millis();
		fit = fm.begin();
		skip_count = 0;
		while (fit != fm.end())
		{
			if (skip_count == skip)
			{
				skip_count = 0;
				fm.erase(fit++);
				continue ;
			}
			skip_count++;
			fit++;
		}
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the erase(first, last) function
 */
void compare_set_erase_range(sset &sm, fset &fm)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::set::erase(first, last)." << std::endl;
		start = millis();
		sm.erase(sm.begin(), sm.end());
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::erase(first, last)." << std::endl;
		start = millis();
		fm.erase(fm.begin(), fm.end());
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the insert(value) function to insert into the set
 */
void compare_set_insert(sset &sm, fset &fm, unsigned int size, unsigned int seed)
{
	sset_iterator sit;
	fset_iterator fit;
	unsigned int i;
	time_t start;
	std::string r;

	if (g_std_test)
	{
		std::cout << "info: std::set::insert(value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = itos(rand());
			if (i % 10 != 0)
				sm.insert(r);
			else
			{
				sit = sm.insert(sm.begin(), r);
				if (*sit != r)
				std::cout << CON_RED << "error: return iterators don't match data."
					<< CON_RESET << std::endl;
			}
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::insert(value) with " << size << " elements." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
		{
			r = itos(rand());
			if (i % 10 != 0)
				fm.insert(r);
			else
			{
				fit = fm.insert(fm.begin(), r);
				if (*fit != r)
				std::cout << CON_RED << "error: return iterators don't match data."
					<< CON_RESET << std::endl;
			}
		}
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the insert(position, first, last) function to insert into the set
 */
void compare_set_insert_range(sset &sm, fset &fm,
	sset_iterator sfirst, sset_iterator slast,
	fset_iterator ffirst, fset_iterator flast)
{
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::set::insert(first, last)." << std::endl;
		start = millis();
		sm.insert(sfirst, slast);
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::insert(first, last)." << std::endl;
		start = millis();
		fm.insert(ffirst, flast);
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the find(key) function to search for a value
 */
void compare_set_count(sset &sm, fset &fm,
	unsigned int size, unsigned int seed)
{
	unsigned int found;
	unsigned int i;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::set::count(key) "
			<< size << " times." << std::endl;
		srand(seed);
		found = 0;
		start = millis();
		for (i = 0; i < size; i++)
			found += sm.count(itos(rand() % 10000));
		std::cout << "info: found " << found << " matches." << std::endl;
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::count(key) "
			<< size << " times." << std::endl;
		srand(seed);
		found = 0;
		start = millis();
		for (i = 0; i < size; i++)
			found += fm.count(itos(rand() % 10000));
		std::cout << "info: found " << found << " matches." << std::endl;
		print_time_diff(start);
	}
	compare_sets(sm, fm);
}

/*
 * use the copy operator
 */
void compare_set_copy_operator(sset &ss, fset &fs, unsigned int iter)
{
	unsigned int i;
	time_t start;
	sset ss2;
	fset fs2;

	if (g_std_test)
	{
		std::cout << "info: std::set::operator=(other) with "
			<< iter << " iterations." << std::endl;
		start = millis();
		for (i = 0; i < iter; i++)
		{
			ss2 = ss;
		}
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::set::operator=(other) with "
			<< iter << " iterations." << std::endl;
		start = millis();
		for (i = 0; i < iter; i++)
		{
			fs2 = fs;
		}
		print_time_diff(start);
	}
	compare_sets(ss2, fs2);
}

void compare_stacks(sstack &ss, fstack &fs)
{
	crc32 check;

	if (g_std_test && !g_ft_test)
	{
		check.reset();
		while (ss.size() != 0)
		{
			check.add(ss.top());
			ss.pop();
		}
		print_crc32(check);
	}
	if (g_ft_test && !g_std_test)
	{
		check.reset();
		while (fs.size() != 0)
		{
			check.add(fs.top());
			fs.pop();
		}
		print_crc32(check);
	}

	if (!g_ft_test || !g_std_test)
		return ;

	if (ss.size() != fs.size())
	{
		std::cout << CON_RED << "error: stacks are not the same size."
			<< CON_RESET << std::endl;
		return ;
	}
	while (fs.size() != 0)
	{
		if (ss.top() != fs.top())
		{
			std::cout << CON_RED << "error: stacks are not the same."
				<< CON_RESET << std::endl;
			return ;
		}
		ss.pop();
		fs.pop();
	}
	if (!ss.empty() || !fs.empty())
	{
		std::cout << CON_RED << "error: stacks are not empty."
			<< CON_RESET << std::endl;
		return ;
	}
	std::cout << CON_GREEN << "info: stacks match."
		<< CON_RESET << std::endl;
}

void compare_stack_push(sstack &ss, fstack &fs, unsigned int size, unsigned int seed)
{
	unsigned int i;
	time_t start;

	if (g_std_test)
	{
		std::cout << "info: std::stack::push(value) " << size
			<< " elements on the stack." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
			ss.push(rand());
		print_time_diff(start);
	}
	if (g_ft_test)
	{
		std::cout << "info: ft::stack::push(value) " << size
			<< " elements on the stack." << std::endl;
		srand(seed);
		start = millis();
		for (i = 0; i < size; i++)
			fs.push(rand());
		print_time_diff(start);
	}
}

int main(int argc, char **argv)
{
	svector sv;
	fvector fv;
	smap sm;
	fmap fm;
	smap sm2;
	fmap fm2;
	sset ss;
	fset fs;
	sset ss2;
	fset fs2;
	sstack sst;
	fstack fst;
	unsigned int seed;

	if (argc == 1)
		seed = time(NULL);
	else if (argc == 2)
		seed = stoui(argv[1]);
	else
	{
		std::cout << "usage: " << argv[0] << " [seed]" << std::endl;
		return (0);
	}

	std::cout << "info: seed " << seed << "." << std::endl;
	compare_vector_push_back(sv, fv, 1000000, seed);
	compare_vector_copy_operator(sv, fv, 1000);
	compare_vector_clear(sv, fv);
	compare_vector_insert(sv, fv, 100000, seed);
	compare_vector_insert_range(sv, fv, 1000, 1000, seed);
	compare_vector_resize(sv, fv, 200000);
	compare_vector_resize(sv, fv, 300000);
	compare_vector_erase(sv, fv, seed);

	std::cout << std::endl;
	compare_map_insert(sm, fm, 10000000, seed);
	compare_map_insert_range(sm2, fm2, sm.begin(), sm.end(), fm.begin(), fm.end());
	compare_map_erase_key(sm2, fm2, 10);
	compare_map_erase_range(sm2, fm2);
	compare_map_count(sm, fm, 10000000, seed);
	compare_map_clear(sm, fm);

	std::cout << std::endl;
	compare_set_insert(ss, fs, 3000000, seed);
	compare_set_insert_range(ss2, fs2, ss.begin(), ss.end(), fs.begin(), fs.end());
	compare_set_erase_key(ss2, fs2, 10);
	compare_set_erase_range(ss2, fs2);
	compare_set_count(ss, fs, 100000, seed);
	compare_set_copy_operator(ss, fs, 10);
	compare_set_clear(ss, fs);

	std::cout << std::endl;
	compare_stack_push(sst, fst, 100000000, seed);
	compare_stacks(sst, fst);
	return (0);
}
