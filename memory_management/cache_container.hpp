#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <ctime>
#include <conio.h>

const int memory_val_size = 10;
const int data_val_size = 8;
const int memory_table_size = 1024;
const int cache_table_size = 128;

// user defined struct for cache handling
typedef struct cache_type {
	std::bitset<data_val_size> data;
	std::bitset<memory_val_size> address;
	bool valid_bit = false;
	int last_time_reference;
	int reference_numbers = 1;
} cache;

// user defined struct for RAM handling
typedef struct memory_type {
	std::bitset<data_val_size> data;
	bool data_presence = false;
} RAM;


class cache_container {
public:
	// number of times the cache memory has been accessed
	int iteration_times = 0;
	// probability of the cache hit
	double p = 1.0;
private:
	// t1 - access time to the main memory
	// t2 - access time to the cache memory
	// t - average access time to the memory
	int t, t1 = 2 * 10, t2 = 2;
	
	//numbers of cache hits and cache misses
	int cache_hit = 0, cache_miss = 0;

	// container for keeping all memory records of the RAM store
	std::array<RAM, memory_table_size> RAM_store;
	// container for keeping all memory recoreds of the CACHE store
	std::vector<cache> cache_store;
public:
	cache_container() {};
	// checking whether required data is present in cache memory
	void cache_handling(std::bitset<memory_val_size> adr, std::bitset<data_val_size> data, bool out);
	// bring the value to the power of
	int pow_(int base, int power);
	//show cache values in the memory table
	void display_cache();
	//show RAM values in the memory table
	void display_RAM();
	//calculating the average time of access to the memory
	double access_time();
	// show main info about the memory status
	void display_info();
	// writing new values to the records of the cache memory
	void cache_write(std::bitset<memory_val_size> adr, std::bitset<data_val_size> data, int pos, bool write_through, bool out);
};