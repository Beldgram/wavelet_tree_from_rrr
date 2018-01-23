#include "stdafx.h"
#include "rrr_table.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>


RRRTable::RRRTable(int b) {
	
	//The first element of the table, when popcount equals 0
	vector<uint8_t> FirstRank(b, 0);
	Block_and_Rank_at_bit_index_t Block_and_Rank_at_bit_index_Zero(0, FirstRank);
	class_t FirstClass;
	FirstClass.push_back(Block_and_Rank_at_bit_index_Zero);
	lookup_table.push_back(FirstClass);

	//Filling the rest of the table, following http://alexbowe.com/popcount-permutations/
	for (int i = 1; i <= b; i++) {
		
		class_t subtable;		
		uint32_t v, initial;
		
		//First Permutations
		v = initial = firstElement(i);
		uint32_t block = firstElement(b);

		//cout << v << "-" << block << "|";

		//Filling the block value and cumulative rank for the class
		while (v >= initial) {
			vector<uint8_t> culRank;

			uint32_t x = v;
			uint32_t z = 0;
			int rankCC = 0;

			//Cumulative rank for a single block value
			for (int j = b - 1; j >= 0; j--) {
				z = (x >> j) & 1;

				rankCC += z;

				culRank.push_back(rankCC);
			}			
			//cout << v << endl;
			Block_and_Rank_at_bit_index_t el(v, culRank);
			subtable.push_back(el);

			v = nextPermutation(v) & block;
		}
		lookup_table.push_back(subtable);
	}

}

//Used for generating the lookup table, code used from http://alexbowe.com/rrr/ 

// Generates the next permutation given the previous lexicographical value
uint32_t RRRTable::nextPermutation(uint32_t v) {
	int t = (v | (v - 1)) + 1;
	int w = t | ((((t & (~t + 1)) / (v & (~v + 1))) >> 1) - 1);
	return w;
}

// Generates the first permutation
uint32_t RRRTable::firstElement(uint32_t c) {
	return (1 << c) - 1;
}

//Acquires the Offset for the RRR blocks
uint32_t RRRTable::GetOffset(uint8_t classindex, uint32_t block) {

	for (int i = 0; i < lookup_table[classindex].size(); ++i) {
		if (block == lookup_table[classindex][i].first) {
			return (uint32_t)i;
		}
	}

	return 0;
}

//Sets the pointer to the created table
lookup_table_t& RRRTable::getTable() {
	return lookup_table;
}

