// Wavelet_Tree_From_RRR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rrr.h"
#include "rrr_table.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>


#include "wavelet_tree.h"


using namespace std;

int main()
{

	const std::string input("Peter Piper_picked_a_peck_of_pickled_peppers$");

	dictionary abc(input);

	using trivial_wtree = wtree<trivial_bitvector>;
	trivial_wtree w(input, abc);
	std::cout << "rank(10, p) = " << w.rank(10, 'P') << "\n";



	uint32_t x = 5;
	char scaning;
	string bitovi1 = "0011101000111011011000000101100";
	string bitovi = "001101100100110110010101010101011010101011000110110010011011001010101010101101010101100011011001001101100101010101010110101010110001101100100110110010101010101011010101011000110110010011011001010101010101101010101100011011001001101100101010101010110101010110001101100100110110010101010101011010101011000110110010011011001010101010101101010101101111111";
	RRR RRRa(bitovi1);
	uint64_t y = RRRa.access(x);
	cout << endl << "Select 1 je : " << y;
	//RRRTable RRRTa(x);
	scanf_s(&scaning);
    return 0;
}

