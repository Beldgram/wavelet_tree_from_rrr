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

	std::vector<block> blocks;

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


	for (int i = 0; i < size; i++) {
		char element = bits[i];
		n++;
		if (element == '1') {
			block_class++;
		}
		if (n == block_size) {
			cout << block_class;
			//blocks.push_back((block_class, block_offset));
			block_class = 0;
			cout << "-";
			block_offset++;
			cout << block_offset << "|";
			n = 0;
		}

	}
	cout << endl << endl;


	for (int i = 0; i < size; i++) {
		char element = bits[i];
		s++;
		if (element == '1') {
			super_block_class++;
		}
		if (s == super_block_size) {
			cout << super_block_class;
			block_class = 0;
			cout << "-";
			super_block_offset++;
			cout << super_block_offset << "|";
			s = 0;
		}

	}
}