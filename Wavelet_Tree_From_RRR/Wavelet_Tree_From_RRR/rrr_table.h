#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

typedef pair<uint32_t, vector<uint8_t>> Block_and_Rank_at_bit_index_t;
typedef vector<Block_and_Rank_at_bit_index_t> class_t;
typedef vector<class_t> lookup_table_t;


class RRRTable {
public:
	RRRTable(int block_size);
private:
	lookup_table_t lookup_table;
	int nextPermutation(uint32_t v);
	int firstElement(uint32_t c);
};