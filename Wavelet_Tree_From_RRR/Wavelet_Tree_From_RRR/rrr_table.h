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
	uint32_t GetOffset(uint8_t popcount, uint32_t block);
	lookup_table_t& RRRTable::getTable();
private:
	lookup_table_t lookup_table;
	uint32_t nextPermutation(uint32_t v);
	uint32_t firstElement(uint32_t c);
};