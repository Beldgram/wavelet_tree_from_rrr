#pragma once
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

typedef vector<vector<pair<uint32_t, vector<uint32_t> > > > table_t;
typedef pair<uint32_t, vector<uint32_t> > element_t;
typedef vector<pair<uint32_t, vector<uint32_t> > > class_t;


class RRRTable {
public:
	RRRTable(uint32_t block_size);
	static uint32_t initialBlockLength;
private:
	table_t table_;
	vector<uint32_t> bitsForOffset;
	uint32_t nextPermutation(uint32_t v);
	uint32_t firstElement(uint32_t c);
};