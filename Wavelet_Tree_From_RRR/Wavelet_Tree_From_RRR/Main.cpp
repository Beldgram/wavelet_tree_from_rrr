// Wavelet_Tree_From_RRR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rrr.h"
#include "rrr_table.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#pragma warning(disable:4996)
#ifdef __linux__ 
	#include "stdlib.h"
	#include "stdio.h"
#else
	#include "windows.h"
	#include "psapi.h"
#endif

#include "wavelet_tree.h"

using namespace std;
using namespace std::chrono;



int main(int argc, char** argv) {
	// check arguments

	char* inputFile = argv[1];

	// open file in which operation outputs will be written
	

	printf("Loading file %s\n", inputFile);

	FILE *handler = fopen(inputFile, "r");
	if (handler == NULL) {
		fprintf(stderr, "Couldn't open file %s\n", inputFile);
		exit(1);
	}
	fseek(handler, 0, SEEK_END);
	long size = ftell(handler);
	fclose(handler);

	// Read data
	ifstream inputStream(inputFile, ios::in);
	string line;
	char* buffer = new char[size];
	int index = 0;
	while (getline(inputStream, line)) {
		if (line[0] == '>' || line[0] == ';' || line.empty())
			continue;

		// Read line
		memcpy(buffer + index, line.c_str(), line.length());
		index += line.length();
	}
	buffer[index] = '\0';
	string input(buffer);

	// Deallocate buffer used for string construction
	delete[] buffer;
	printf("File loaded\n");

	printf("Building Wavelet tree\n");

	dictionary abc(input);

	using trivial_wtree = wtree<trivial_bitvector>;
	high_resolution_clock::time_point startTime = high_resolution_clock::now();


	trivial_wtree w(input, abc);

	high_resolution_clock::time_point endTime = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(endTime - startTime).count();
	printf("Done building tree - %ld ms (%ld us)\n", duration / 1000, duration);

	w.rank(10, 'A');

	//std::cout << "rank(10, p) = " << w.rank(10, 'P') << "\n";
	
    return 0;
}

