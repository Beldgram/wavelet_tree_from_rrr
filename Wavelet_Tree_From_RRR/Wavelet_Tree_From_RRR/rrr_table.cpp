#include "stdafx.h"
#include "rrr_table.h"
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iostream>

uint32_t RRRTable::initialBlockLength = 0;

RRRTable::RRRTable(uint32_t b) {


	
}

uint32_t RRRTable::nextPermutation(uint32_t v) {
	uint32_t t = (v | (v - 1)) + 1;
	uint32_t w = t | ((((t & (~t + 1)) / (v & (~v + 1))) >> 1) - 1);
	return w;
}


