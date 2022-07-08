#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <bitset>
#include <string>
#include <array>
#include <ctime>


const int address_bit_num = 5;
const int data_bit_num = 8;
const int RAM_size = 32;
const int CACHE_size = 16;


typedef struct CACHE_cnt {
	std::bitset<data_bit_num> data;
	std::bitset<address_bit_num> address;
	bool modification_feature = false;
	bool empty_vault = true;
	int index;
} CACHE;

typedef struct RAM_cnt {
	std::bitset<data_bit_num> data;
	bool fill_ = false;
} RAM;

class model {
public:
	double p = 1.0;
	int it_amount;
private:
	int t, t1 = 3 * 8, t2 = 3;

	int cache_hit = 0, cache_miss = 0;
	std::array<RAM, RAM_size> RAM_store;
	std::array<CACHE, CACHE_size> cache_store;
public:

	double show_probability() {
		return (double)cache_hit / ((double)cache_hit + (double)cache_miss);p;
	}
	void show_RAM_table();
	double tm();
	void show_extra_data();
	void data_modification(std::bitset<address_bit_num> adr, std::bitset<data_bit_num> data, bool print);
	int power_of(int base, int power);
	void show_case_table();
	void back_write(std::bitset<address_bit_num> adr, std::bitset<data_bit_num> data, int pos, bool write_through, bool print);
};


void model::back_write(std::bitset<address_bit_num> adr, std::bitset<data_bit_num> data, int pos, bool write_back, bool print) {
	CACHE vault;
	vault.data = data;
	vault.address = adr;
	vault.empty_vault = false;
	vault.empty_vault = false;
	vault.index = pos;

	if (!cache_store.at(pos).empty_vault) {
		if (print) {
			std::cout << "-OLD RECORD REWRITING: copying to RAM";
			std::cout << "\t\tREMOVED ELEMENT - [address | data]: " << cache_store[pos].address << " | " << cache_store[pos].data << " |" << std::endl;
			RAM_store.at(adr.to_ulong()).data = cache_store[pos].data;
			RAM_store.at(adr.to_ulong()).fill_ = true;
		}
		vault.modification_feature = true;

	}
	else {
		if (print) {
			std::cout << "\t-DATA WRITING: modifying cache" << std::endl;
		}
		vault.modification_feature = false;
	}
	cache_store.at(pos) = vault;

}

double model::tm() {
	p = (double)cache_hit / ((double)cache_hit + (double)cache_miss);
	return t2 * p + t1 * (1.0 - p);
}

int find_pow(int num) {
	int i = 1;
	while (num != 2) {
		num /= 2;
		i++;
	};
	return i;
}

void model::data_modification(std::bitset<address_bit_num> adr, std::bitset<data_bit_num> data, bool print) {
	bool hit = false, write_back = false;

	int pow_number = find_pow(CACHE_size);
	int tag_index, cache_index;

	tag_index = pow_number;
	cache_index = address_bit_num - tag_index;

	std::string tag = adr.to_string(), dat;
	tag = tag.substr(0, tag_index);
	dat = (adr.to_string()).substr(tag_index, tag_index + cache_index);


	int indexx = std::stoi(tag, nullptr, 2);
	if (cache_store[indexx].address == adr && !cache_store[indexx].empty_vault) {
		if (print)
			std::cout << "\t-DATA SEARCHING: cache-hit" << std::endl;
		write_back = true;
		hit = true;
		cache_hit++;
		back_write(adr, data, indexx, write_back, print);

	}
	else
	{
		if (print)
			std::cout << "\t-DATA SEARCHING: cache-miss" << std::endl;
		cache_miss++;
		if (print)
			std::cout << "\t-DATA WRITING: adding record to RAM table" << std::endl;

		back_write(adr, data, indexx, write_back, print);

		RAM_store.at(adr.to_ulong()).data = data;
		RAM_store.at(adr.to_ulong()).fill_ = true;
	}
}



int model::power_of(int base, int power) {
	int i = 0, sum = 1;
	if (power < 0)
		return 0;
	while (i < power) {
		sum *= base;
		i++;
	}
	return sum;
}

void model::show_case_table() {
	std::cout << "CACHE TABLE: " << std::endl;
	printf("-------------------------------------------------\n");
	printf("|%5s|%20s|%20s|\n", "Index", "Cache Address", "Data");
	printf("-------------------------------------------------\n");
	for (int i = 0; i < cache_store.size(); i++) {
		if (!cache_store[i].empty_vault) {
			std::cout << std::setfill(' ') << "|" << std::setw(5) << cache_store[i].index << "|" << std::setw(20) \
				<< cache_store[i].address << "|" << std::setw(20) << cache_store[i].data << "|" << std::endl;
		}
	}
	printf("-------------------------------------------------\n");
}

void execution(model& ch_, int limit, int& i) {
	std::bitset<address_bit_num> adr_ = std::rand() % RAM_size;
	std::bitset<data_bit_num> data_ = std::rand() % ch_.power_of(2, data_bit_num);

	printf("**************************************(OPERATION START)********************************************\n");

	ch_.data_modification(adr_, data_, true);

	if (limit == 0 || limit <= i) {
		std::cout << "\t\t" << "Address - [" << adr_ << "] to_int(" << adr_.to_ulong() << ") | " << "data - [" << data_ << "] to_int(" << data_.to_ulong() << ")" << std::endl;



		printf("\n\t-EXTRA INFO: \n\t\tIteration num - %d | Average access time - %lf | Hit ratio - %lf\n", ch_.it_amount, ch_.tm(), ch_.show_probability());

		printf("**************************************(OPERATION END)**********************************************\n\n");

		printf("------------------------------------------------------------------------------------------------------------------------------\n");
		printf("| Command menu - { [c - coninue], [i - show info], [t - show tables + c], [n - execute N iterations], [e - exit] } |\n");
		printf("------------------------------------------------------------------------------------------------------------------------------\n");
	}

	i++;
	ch_.it_amount++;
}

void model::show_RAM_table() {
	std::cout << "RAM TABLE: " << std::endl;
	printf("---------------------------------\n");
	printf("|%15s|%15s|\n", "RAM Adress", "Data");
	printf("---------------------------------\n");
	for (int i = 0; i < RAM_store.size(); i++) {
		if (RAM_store[i].fill_) {
			std::cout << std::setfill(' ') << "|" << std::setw(15) \
				<< std::bitset<address_bit_num>(i).to_string() << "|" << std::setw(15) << RAM_store[i].data << "|" << std::endl;
		}
	}
	printf("---------------------------------\n");
}

void model::show_extra_data() {
	printf("Start information:\n");
	printf("\tSizes: [memory - %d], [data - %d], [RAM table - %d], [cache table - %d]\n", address_bit_num, data_bit_num, RAM_size, CACHE_size);
	printf("\tStatistics: [hit ratio - %lf ( hits - %d | misses %d )&(cache time - %d | RAM time - %d) ]\n", show_probability(), cache_hit, cache_miss, t2, t1);
	printf("_________________________________________________________________________________________________________________\n\n");
}



int get_key_press()
{
	char key;
	std::cout << "Press any key from the menu +=> ";
	std::cin >> key;
	return (int)key;
}

int main() {
	//controlled random seed
	srand(177);
	//srand(time(0));

	model cache;

	int i = 0, limit = 0;
	bool wait = true;
	short button_pressed;

	cache.show_extra_data();

	while (true) {
		if (wait) {
			execution(cache, limit, i);
			wait = false;
		}

		switch (get_key_press())
		{
			case 'e':
				exit(1);
				break;
			case 'i':
				cache.show_extra_data();
				break;
			case 't':
				cache.show_case_table();
				cache.show_RAM_table();
				break;
			case 'f':
				cache.show_extra_data();
				cache.show_case_table();
				cache.show_RAM_table();
				break;
			case 'n':
				std::cout << "Type iteration number: " << std::endl;
				std::cin >> limit;
				while (i < limit) {
					execution(cache, limit, i);
				}
				break;
			default:
				wait = true;
				break;
		}
	}

	return 1;
}
