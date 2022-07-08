#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
#include <string>
#include <ctime>


using namespace std;

const int clusters_amount = 65536;

struct FAT_file {
	string filename;
	short attribute_file;
	short reserved_field;
	int creation_time;
	int creation_date;
	int last_access_date;
	int reserved;
	int last_modification_time;
	int last_modification_date;
	int initial_cluster;
	int final_cluster;
	unsigned int filesize;
};

void initialization();
void direction();