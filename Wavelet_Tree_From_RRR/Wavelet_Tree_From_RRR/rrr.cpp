#include "stdafx.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include "rrr.h"

using namespace std;

//Constructor
RRR::RRR(string &bits){

	//Lenght of the input string
	int size = bits.length();

	//How much bits there are in a single block
	block_size =floor(log2(size) / 2);

	//How much bits there are in a single super block
	int super_block_size = pow(floor(log2(size)),2);

	//How much blocks there are in a single superblock
	blocks_per_superblock = super_block_size / block_size;

	//Creation and acquiring of the lookup table
	RRRTable RRR_table_object(block_size); 
	lookup_table = RRR_table_object.getTable();

	//Initial values
	int block_class = 0;
	int block_offset = 0;
	uint16_t content = 0;
	int super_block_class = 0;
	int super_block_offset = 0;
	int n = 0;
	int s = 0;
	int x = 0;

	//First super block 0-0
	converted_super_blocks.push_back(block(super_block_class, super_block_offset));

	//Block creation
	for (int i = 0; i < size; i++) {
		char element = bits[i];
		n++;
		s++;
		
		//Class increment
		if (element == 1) {
			block_class++;
			content = (content << 1) + 1;
		}
		else {
			//Saving for the block
			content = (content << 1) + 0;
		}
		if (n == block_size || (i + 1) == bits.size()) {
			//cout << block_class;
			//cout << "-";
			//block_offset++;

			//Acquiring from the lookup table
			block_offset = RRR_table_object.GetOffset(block_class, content);

			//cout << block_offset << "|";

			converted_blocks.push_back(block(block_class, block_offset));
			super_block_class += block_class;
			block_class = 0;
			//cout << "OVO JE CONTENT  " << content << endl;
			content = 0;

			n = 0;

			//Super block
			if (s == super_block_size || (i + 1) == bits.size()) {
				//cout << super_block_class;
				//cout << "-";
				super_block_offset++;
				//cout << super_block_offset << "|";
				converted_super_blocks.push_back(block(super_block_class, super_block_offset));
				s = 0;
			}

		}
	}
	//cout << endl << endl;

}

//Returns the nuber of bits with value 1 from the start of the
//sequnce to the given index
//Based of the pseudo code from http://alexbowe.com/rrr/
uint64_t RRR::rank1(uint64_t index){

	// Determine index of block and super block which contains bit with index given as parameter
	uint64_t block_index = floor(index / block_size);
	uint64_t super_block_index = floor(block_index / blocks_per_superblock);

	//Sum of the popcounts of the previous superblocks
	uint32_t rank_sum = converted_super_blocks[super_block_index].first;

	//First block of the selected superblock
	uint32_t current_block_index = blocks_per_superblock * super_block_index;

	//cout << block_index << "/"<< blocks_per_superblock << "|||" << super_block_index << "|||" << rank_sum << "|||" << current_block_index << " END" << endl;

	// Iterate through superblock and add popcounts of it's blocks until
	// block which contain bit with given index is reached
	for (int i = current_block_index; i < block_index; i++) {
		//cout << "RANK_sum je " << rank_sum << " + " << converted_blocks[i].first << endl;
		rank_sum += converted_blocks[i].first;
	}

	//Find the index of the last block
	int last_block_index = index % block_size;

	//cout << lookup_table[converted_blocks[block_index].first][converted_blocks[block_index].second].second[last_block_index];

	//Find the remaining sum from the lookup table
	rank_sum += lookup_table[converted_blocks[block_index].first][converted_blocks[block_index].second].second[last_block_index];

	//cout << endl << "Rank je : " << rank_sum;

	return rank_sum;

	return 0;
}


//Returns the nuber of bits with value 1 from the start of the
//sequnce to the given index
uint64_t RRR::rank0(uint64_t index) {
	return index + 1 - rank1(index);
}


//Returns the index of the target-th bit with value 1
uint64_t RRR::select1(uint64_t target) {
	

	uint32_t index = 0;
	uint32_t super_block_index = 0;
	uint32_t block_index = 0;

	//cout << endl << "super_block slecet " << converted_super_blocks[super_block_index + 1].first << endl;
	//cout << endl << "super_block slecet " << converted_blocks.size() << endl;

	// Find superblock whose value is lower than target value, but value of next is
	// bigger than target value
	while (super_block_index + 1 < converted_super_blocks.size()) {
		if (target >= converted_super_blocks[super_block_index].first && target < converted_super_blocks[super_block_index + 1].first) {
			break;
		}
		super_block_index++;
	}

	//cout << endl << "super index slecet " << super_block_index << endl;

	//Curent popcount and first block of the super block
	uint32_t rankSum = converted_super_blocks[super_block_index].first;
	uint32_t current_block_index = blocks_per_superblock * super_block_index;

	// Iterate through blocks and add their popcounts to the rankSum until value of
	// s is higher than target
	while (current_block_index < blocks_per_superblock) {
		uint32_t s = rankSum + converted_blocks[current_block_index].first;

		if (s >= target) {
			break;
		}
		else
		{
			rankSum += converted_blocks[current_block_index].first;
			current_block_index++;
		}
	}
	//cout << endl << "current_block_index slecet" << current_block_index << endl;
	
	//current index
	index = current_block_index * block_size;
	//cout << endl << "Index slecet" << index << endl;
	//cout << endl << "RabkSUM slecet" << rankSum << endl;

	//Value inside the block index
	uint32_t y = lookup_table[converted_blocks[current_block_index].first][converted_blocks[current_block_index].second].first;
	int j = 0;
	uint32_t z = 0;

	// Iterate through the last value and add all 1-es
	for (int j = block_size - 1; j >= 0; j--) {
		
		//cout << endl << "X  " << y << endl;
		

		z = (y >> j) & 1;

		//cout << endl << "Z slecet " << z << endl;

		rankSum += z;
		
		//cout << endl << "RabkSUM slecet " << rankSum << endl;
		if (rankSum == target) {
			break;
		}
		index++;
		
		
	}

	return index;
}

//Returns the index of the target-th bit with value 0
uint64_t RRR::select0(uint64_t target) {

	uint32_t index = 0;
	uint32_t super_block_index = 0;
	uint32_t block_index = 0;

	//cout << endl << "current_block_index slecet" << block_size*blocks_per_superblock - converted_super_blocks[super_block_index + 1].first << endl;
	
	// Find superblock whose value of rank 0 is lower than target value, but value of next is
	// bigger than target value
	while (super_block_index + 1 < converted_super_blocks.size()) {
		if (target < (block_size*blocks_per_superblock - converted_super_blocks[super_block_index+1].first)) {
			break;
		}
		super_block_index++;
	}

	uint32_t rankSum = block_size*blocks_per_superblock - converted_super_blocks[super_block_index].first;
	uint32_t current_block_index = blocks_per_superblock * super_block_index;
	//cout << endl << "RabkSUM slecet" << rankSum << endl;
	//cout << endl << "current_block_index slecet -----" << current_block_index << endl;
	uint32_t s = 0;


	// Iterate through blocks and add their popcounts to the rankSum until value of
	// s is higher than target
	while (current_block_index < blocks_per_superblock) {
		s = rankSum + (block_size - converted_blocks[current_block_index].first);
		cout << endl << "S--> " << s << endl;

		if (s >= target) {
			break;
		}
		else
		{
			rankSum += (block_size - converted_blocks[current_block_index].first);
			current_block_index++;
		}
	}

	//cout << endl << "current_block_index slecet" << current_block_index << endl;
	index = current_block_index * block_size;
	//cout << endl << "Index slecet" << index << endl;
	//cout << endl << "RabkSUM slecet" << rankSum << endl;

	//Value inside the block index
	uint32_t y = lookup_table[converted_blocks[current_block_index].first][converted_blocks[current_block_index].second].first;
	int j = 0;
	uint32_t z = 0;

	// Iterate through the last value and add all 0-es
	for (int j = block_size - 1; j >= 0; j--) {

		//cout << endl << "X  " << y << endl;


		z = (~(y >> j)) & 1;

		//cout << endl << "Z slecet " << z << endl;

		rankSum += z;

		//cout << endl << "RabkSUM slecet " << rankSum << endl;
		if (rankSum == target) {
			break;
		}
		index++;


	}


	return 0;
}

uint8_t RRR::access(uint64_t index) {
	
	// Determine index of block in which is given index
	uint32_t block_index = index / block_size;

	// Determine index of bit on given index inside a block
	int index_in_block = index % block_size;

	// Find the value of the original block
	uint32_t x = lookup_table[converted_blocks[block_index].first][converted_blocks[block_index].second].first;

	//Get the value in the index
	uint8_t y = (x >> (block_size - 1 - index_in_block)) & 1;


	return y;
}
