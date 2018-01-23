#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "rrr_table.h"

using namespace std;

//type of the new blocks and super blocks
typedef pair<uint32_t, uint32_t> block;

class RRR {

public:

	//Constructor
	RRR(string &bits);

	//Returns the nuber of bits with value 1 from the start of the
	//sequnce to the given index
	uint64_t rank1(uint64_t index);

	//Returns the nuber of bits with value 0 from the start of the
	//sequnce to the given index
	uint64_t rank0(uint64_t index);

	//Returns the index of the target-th bit with value 1
	uint64_t select1(uint64_t target);

	//Returns the index of the target-th bit with value 0
	uint64_t select0(uint64_t target);

	//Returns the value at the index
	uint8_t RRR::access(uint64_t index);

private:

	//How much bits there are in a single block
	int block_size;

	//How much blocks there are in a single superblock
	int blocks_per_superblock;

	//Blocks and superblocks
	std::vector<block> converted_blocks;
	std::vector<block> converted_super_blocks;

	//Lookup table
	lookup_table_t lookup_table;
};