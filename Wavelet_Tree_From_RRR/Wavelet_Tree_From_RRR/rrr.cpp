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
	int blocks =floor(log2(size) / 2);
	int super_blocks = pow(floor(log2(size)),2);
	int blocks_per_superblock = super_blocks / blocks;
	cout << "Size " << size << endl;
	cout << "Blokovi " << blocks << endl;
	cout << "Super Blokovi " << super_blocks << endl;
	cout << "F " << blocks_per_superblock << endl;
}