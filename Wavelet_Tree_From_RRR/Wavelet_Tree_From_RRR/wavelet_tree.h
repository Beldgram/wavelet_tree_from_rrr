#include "stdafx.h"
#include <stdint.h>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include <iterator>
#include <cassert>
#include <fstream>
#include <string>
#include "rrr.h"

struct dictionary {
	explicit dictionary(const std::string& input)
	{
		std::set<char> abc;
		std::copy(std::cbegin(input), std::cend(input), std::inserter(abc, std::end(abc)));
		m_abc = std::string(std::cbegin(abc), std::cend(abc));
	}

	// linear lookup of bit value in the dictionary
	bool get_bit(char c) const
	{
		return m_abc.find(c) >= m_abc.length() / 2;
	}

	size_t length() const
	{
		return m_abc.length();
	}

	// TODO: implement as references (without copying data) for large dictionaries
	dictionary left() const
	{
		return dictionary(m_abc.substr(0, m_abc.length() / 2));
	}

	dictionary right() const
	{
		return dictionary(m_abc.substr(m_abc.length() / 2, m_abc.length() - m_abc.length() / 2));
	}

private:
	std::string m_abc;
};

struct trivial_bitvector {
	trivial_bitvector(const std::string& input, const dictionary& abc)
		: m_bitvec(input.length())
	{
		// construct the bit vector from the input string using the dictionary we got from a higher level
		// we presume that if the character from the input string is in the left half of the dictionary then its bit
		// is 0, else its bit is 1, this checking is implemented in the get_bit() function
		std::transform(std::cbegin(input), std::cend(input), std::begin(m_bitvec), [&](char c) {
			return abc.get_bit(c);
		});
	}

	// helper function for trivial bit rank computation on a bit vector
	int rank(int idx, bool val) const
	{
		auto pos = std::cbegin(m_bitvec);
		std::advance(pos, idx);

#ifndef NDEBUG
		std::copy(std::cbegin(m_bitvec), std::cend(m_bitvec), std::ostream_iterator<bool>(std::cout, ""));
		std::cout << "\n";
		std::transform(std::cbegin(m_bitvec), pos, std::ostream_iterator<char>(std::cout, ""), [val](bool b) {
			return val == b ? '^' : '_';
		});
#endif

		return static_cast<int>(std::count(std::cbegin(m_bitvec), pos, val));
	}

private:
	std::vector<bool> m_bitvec;
};

template <typename Tbvec>
struct wtree {
	wtree(const std::string& input, const dictionary& abc)
		: m_root(std::make_unique<node>(input, abc))
	{
	}

	int rank(int idx, char elem) const
	{
		// the input range is by one larger then the position in the string
		return m_root->rank(idx + 1, elem);
	}

private:
	struct node {

		node(const std::string& input, const dictionary& abc) : m_abc(abc), m_bv(input, abc)
		{
#ifndef NDEBUG
			m_input = input;
#endif

			// if the dictionary contains 1 or 2 elements then this is a terminal node
			size_t abc_len = abc.length();
			if (abc_len <= 2) return;

			// separate the input string into the left and right subtree part using the bit vector flags
			std::string left, right;
			for (char c : input) {
				if (m_abc.get_bit(c)) {
					right.push_back(c);
				}
				else {
					left.push_back(c);
				}
			}

			// if there are more than two elements in the input dictionary than the left and right nodes must not be empty
			// to get a balanced tree;assert checks if its argument (which must have scalar type) compares equal to zero.
			//If it does, assert outputs implementation-specific diagnostic information on the standard error output and calls std::abort.
			assert(!left.empty());
			assert(!right.empty());

			// split the dictionary into two halves
			m_left = std::make_unique<node>(left, abc.left());
			m_right = std::make_unique<node>(right, abc.right());
		}

		int rank(int idx, char elem) const
		{
			// first we compute to which bit in the bitvector this character corresponds by querying the local dictionary
			bool bit = m_abc.get_bit(elem);

#ifndef NDEBUG
			std::cout << m_input << "\n";
#endif

			int r = m_bv.rank(idx, bit);

#ifndef NDEBUG
			std::cout << " = " << r << "\n----\n";
#endif

			// if this is a terminal node then we are at the end and return the final rank value
			// since this is a balanced tree m_left and m_right must both be null, and we don't need to check both here
			if (!m_left) return r;

			// recurse to the left or right subtree depending on what bit the character corresponds to in the local dictionary
			if (bit) {
				return m_right->rank(r, elem);
			}
			else {
				return m_left->rank(r, elem);
			}
		}

#ifndef NDEBUG
		std::string m_input;
#endif

		// each node contains it's dictionary subset and the bit vector
		dictionary m_abc;
		Tbvec m_bv;

		std::unique_ptr<node> m_left;
		std::unique_ptr<node> m_right;
	};

	std::unique_ptr<node> m_root;
};

