#include "cache_container.hpp"

void execution(cache_container &ch_, int limit, int &i, bool loop) {

	std::bitset<memory_val_size> adr_ = std::rand() % memory_table_size;
	std::bitset<data_val_size> data_ = std::rand() % ch_.pow_(2, data_val_size);

	if (i == limit || loop)
		std::cout << "Request info: " << "memory address - [" << adr_ << "] (" << adr_.to_ulong() << ") | " << "data - [" << data_ << "] (" << data_.to_ulong() << ")" << std::endl;
	
    ch_.cache_handling(adr_, data_, true);
	
    if (i == limit || loop) {
		std::cout << "Average access time: " << ch_.access_time() << "; | Iteration num - " << ch_.iteration_times << " | ";
		std::cout << "Hit ratio - " << std::setprecision(9) << ch_.p << std::endl;
		std::cout << "Press: _c_ > continue | _q_ > exit | _i_ > display and continue...^" << std::endl;
		std::cout << "_t_ > show tables and continue | _l_ > jump to N operation...\n" << std::endl;
		if (i == limit && !loop) {
			ch_.display_info();
			ch_.display_cache();
			ch_.display_RAM();
		}
	}

	i++;
	ch_.iteration_times++;
}

int main() {
	std::bitset<memory_val_size> adr;

	//set to get random seed, so that every time program launch you would get unique results
	//srand(time(0));
	
	// set to get fixed value of time seed to check whether programas works as it is supposed to be
	srand(255);

	char key_press;
	int ascii_value;
	cache_container ch_;

	// 2-bit values of memory and data addresses 
	std::bitset<memory_val_size> adr_;
	std::bitset<data_val_size> data_;

	//display main interface with functionality explained
	ch_.display_info();

	int i = 0, limit = 0;
	bool loop = true;

	//enter endless loop to get sequential results until user decides to stop the execution
	while (true) {
		if (loop) {
			while (true) {

				//request for the action
				execution(ch_, limit, i, loop);

				std::cin >> key_press;
				ascii_value = key_press;
				
				//selecting proper option after entering value from the keyboard
				if (ascii_value == 'c') {
					continue;
				}
				else if (ascii_value == 'q') {
					exit(1);
				}
				else if (ascii_value == 'i') {
					ch_.display_info();
					continue;
				}
				else if (ascii_value == 't') {
					ch_.display_cache();
					ch_.display_RAM();
					continue;
				}
				else if (ascii_value == 'f') {
					ch_.display_info();
					ch_.display_cache();
					ch_.display_RAM();
					continue;
				}
				else if (ascii_value == 'l') {
					loop = false;
					break;
				}
			}
		}
		else { // enter loop which doesen's stop until reaching limit value set by the user
			std::cout << "Type iteration number: " << std::endl;
			std::cin >> limit;
			while (i < limit) {
				execution(ch_, limit, i, loop);
			}
			loop = true;
		}
	}

	return 0;
}
