#include "stdafx.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include "rrr.h"

using namespace std;

RRR::RRR(string &bits){

	int size = bits.length();
	block_size =floor(log2(size) / 2);
	int super_block_size = pow(floor(log2(size)),2);
	blocks_per_superblock = super_block_size / block_size;


	//cout << "Size " << size << endl;
	//cout << "Blokovi " << block_size << endl;
	//cout << "Super Blokovi " << super_block_size << endl;
	//cout << "F " << blocks_per_superblock << endl;

	RRRTable RRR_table_object(block_size); 
	lookup_table = RRR_table_object.getTable();

	int block_class = 0;
	int block_offset = 0;
	uint16_t content = 0;
	int super_block_class = 0;
	int super_block_offset = 0;
	int n = 0;
	int s = 0;
	int x = 0;


	for (int i = 0; i < size; i++) {
		cout << bits[i];
		x++;
		if (x == block_size) {
			cout << "|";
			x = 0;
		}
	}
	cout << endl << endl;

	converted_super_blocks.push_back(block(super_block_class, super_block_offset));

	for (int i = 0; i < size; i++) {
		char element = bits[i];
		n++;
		s++;
		
		if (element == '1') {
			block_class++;
			content = (content << 1) + 1;
		}
		else {
			content = (content << 1) + 0;
		}
		if (n == block_size) {
			cout << block_class;
			cout << "-";
			//block_offset++;

			block_offset = RRR_table_object.GetOffset(block_class, content); 

			cout << block_offset << "|";
			converted_blocks.push_back(block(block_class, block_offset));
			super_block_class += block_class;
			block_class = 0;
			cout << "OVO JE CONTENT  " << content << endl;
			content = 0;

			n = 0;
		}

		if (s == super_block_size) {
			cout << super_block_class;
			cout << "-";
			super_block_offset++;
			cout << super_block_offset << "|";
			converted_super_blocks.push_back(block(super_block_class, super_block_offset));
			s = 0;
		}
		
		
	}
	cout << endl << endl;


	for (int i = 0; i < size; i++) {
		char element = bits[i];
		
		if (element == '1') {
			super_block_class++;
		}
		

	}
	
	

	cout << "PROVJERA" << endl << endl;
	for (auto b : converted_blocks) {
		cout << b.first << "-" << b.second << "|"  ;
	}
	cout << endl << converted_super_blocks.size() << endl;
	for (auto b : converted_super_blocks) {
		cout << b.first << "-" << b.second << "|" ;
	}
}


uint64_t RRR::rank1(uint64_t index){

	//cout << endl << "RANK 1-> Index" << index << endl;

	uint64_t block_index = floor(index / block_size);
	uint64_t super_block_index = floor(block_index / blocks_per_superblock);


	int rank_sum = converted_super_blocks[super_block_index].first;


	uint32_t current_block_index = blocks_per_superblock * super_block_index;

	//cout << block_index << "/"<< blocks_per_superblock << "|||" << super_block_index << "|||" << rank_sum << "|||" << current_block_index << " END" << endl;

	for (int i = current_block_index; i < block_index; i++) {
		//cout << "RANK_sum je " << rank_sum << " + " << converted_blocks[i].first << endl;
		rank_sum += converted_blocks[i].first;
	}

	int last_block_index = index % block_size;

	//cout << lookup_table[converted_blocks[block_index].first][converted_blocks[block_index].second].second[last_block_index];

	rank_sum += lookup_table[converted_blocks[block_index].first][converted_blocks[block_index].second].second[last_block_index];

	//cout << endl << "Rank je : " << rank_sum;

	return rank_sum;

	return 0;
}


uint64_t RRR::rank0(uint64_t index) {
	return index + 1 - rank1(index);
}

uint64_t RRR::select1(uint64_t index) {

}

uint8_t RRR::access(uint64_t index) {
	
	return 0;
}
