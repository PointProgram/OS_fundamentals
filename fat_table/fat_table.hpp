#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;


const int FAT_16 = 65536;

struct FAT_file {
	string name;
	short attribute;
	short reserved_field;
	int creation_time;
	int creation_date;
	int last_access_date;
	int reserved;
	int last_modification_time;
	int last_modification_date;
	int initial_claster;
	unsigned int file_size;
};

void alloc_memory();
void direction();
