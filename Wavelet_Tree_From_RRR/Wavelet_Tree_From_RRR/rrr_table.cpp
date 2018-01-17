#include "stdafx.h"
#include "rrr_table.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>


RRRTable::RRRTable(int b) {
	
	vector<uint8_t> FirstRank(b, 0);
	Block_and_Rank_at_bit_index_t Block_and_Rank_at_bit_index_Zero(0, FirstRank);
	class_t FirstClass;
	FirstClass.push_back(Block_and_Rank_at_bit_index_Zero);
	lookup_table.push_back(FirstClass);

	for (int i = 1; i <= b; i++) {
		class_t subtable;
		
		uint32_t v, initial;

		v = initial = firstElement(i);
		int block = firstElement(b);

		//cout << v << "-" << block << "|";


		while (v >= initial) {
			vector<uint8_t> culRank;

			uint32_t x = v;
			uint32_t z = 0;
			int rankCC = 0;

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

	//cout << lookup_table[0][0].first << "!!!";

	for (int i = 0; i < lookup_table[3].size(); i++) {
		cout << lookup_table[3][i].first << "|";
		for (int j = 0; j < lookup_table[3][i].second.size(); j++) {
			cout << ((int)lookup_table[3][i].second[j]);
		}
		cout << "!" << endl;
		//cout << lookup_table[2][i].first << " ";
	}
}

int RRRTable::nextPermutation(uint32_t v) {
	int t = (v | (v - 1)) + 1;
	int w = t | ((((t & (~t + 1)) / (v & (~v + 1))) >> 1) - 1);
	return w;
}

int RRRTable::firstElement(uint32_t c) {
	return (1 << c) - 1;
}


