#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "rrr_table.h"

using namespace std;

typedef pair<int, int> block;

class RRR {

public:
	RRR(string &bits);
	uint64_t rank1(uint64_t index);
	uint64_t rank0(uint64_t index);
	uint64_t select1(uint64_t target);
	uint64_t select0(uint64_t target);
	uint8_t RRR::access(uint64_t index);
private:
	int block_size;
	int blocks_per_superblock;
	std::vector<block> converted_blocks;
	std::vector<block> converted_super_blocks;
	lookup_table_t lookup_table;
};