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
	int block_size =floor(log2(size) / 2);
	int super_block_size = pow(floor(log2(size)),2);
	int blocks_per_superblock = super_block_size / block_size;

	typedef pair<int, int> block;
	std::vector<block> converted_blocks;
	std::vector<block> converted_super_blocks;

	cout << "Size " << size << endl;
	cout << "Blokovi " << block_size << endl;
	cout << "Super Blokovi " << super_block_size << endl;
	cout << "F " << blocks_per_superblock << endl;

	int block_class = 0;
	int block_offset = 0;
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
		}
		if (n == block_size) {
			cout << block_class;
			cout << "-";
			block_offset++;
			cout << block_offset << "|";
			converted_blocks.push_back(block(block_class, block_offset));
			super_block_class += block_class;
			block_class = 0;
			
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
	
	

	cout << endl << endl;
	for (auto b : converted_blocks) {
		cout << b.first << "-" << b.second << "|"  ;
	}
	cout << endl << converted_super_blocks.capacity() << endl;
	for (auto b : converted_super_blocks) {
		cout << b.first << "-" << b.second << "|" ;
	}
}