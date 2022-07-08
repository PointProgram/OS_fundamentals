#include "cache_container.hpp"

bool time_number_ref = true;

// calculating the average time of access to the memory
double cache_container::access_time() {
	p = (double)cache_hit / ((double)cache_hit + (double)cache_miss);
	return t2 * p + t1 * (1.0 - p);
}

// writing new values to the records of the cache memory
void cache_container::cache_write(std::bitset<memory_val_size> adr, std::bitset<data_val_size> data, int pos, bool write_through, bool out) {
	cache store;
	store.data = data;
	store.address = adr;
	store.valid_bit = true;

    // if rewriting command wasn't set go there
	if (!write_through) {
        // if cache store isn't full, add new value to the memory puddle
		if (cache_store.size() < cache_table_size) {
			store.last_time_reference = iteration_times;
			store.reference_numbers = 1;
			cache_store.push_back(store);
		}
        // otherwise find a record where an old memory should be rewrited
		else {
			if(out)
				std::cout << "---->>> displacement of the record ";
			
            // if time reference is set to True, then choose a record with the least time of addressing
			if (time_number_ref) {
				if (out)
					std::cout << "that has not been addressed for a long time" << std::endl;
				int index_time_record = cache_store[0].last_time_reference, ind = 0;
				for (size_t i = 1; i < cache_store.size(); i++) {
					if (index_time_record > cache_store[i].last_time_reference) {
						index_time_record = cache_store[i].last_time_reference;
						ind = i;
					}
				}
				if (out)
					std::cout << "Deleted an el.[address; data; last reference]: " << cache_store[ind].address << " | " << cache_store[ind].data << " | " << cache_store[ind].last_time_reference << std::endl;
				cache_store.erase((cache_store.begin() + ind));
			}
            // if time reference is set to False, then choose a record with the least time of calls
			else {
				if (out)
					std::cout << "with the least number of appeals" << std::endl;
				int index_amount_records = cache_store[0].reference_numbers, ind = 0;
				for (size_t i = 1; i < cache_store.size(); i++) {
					if (index_amount_records < cache_store[i].reference_numbers) {
						index_amount_records = cache_store[i].reference_numbers;
						ind = i;
					}
				}
				if (out)
					std::cout << "Deleted an el.[address; data; reference times]: " << cache_store[ind].address << " | " << cache_store[ind].data << " | " << cache_store[ind].reference_numbers << std::endl;
				cache_store.erase(cache_store.begin() + ind);
			}
			store.last_time_reference = iteration_times;
			store.reference_numbers = 1;
			cache_store.push_back(store);
		}
	}
	else { // rewriting the cache store memory with new values
		if (out)
			std::cout << "---->>> write through: cache and RAM records rewriting" << std::endl;
		cache_store[pos].address = store.address;
		cache_store[pos].data = store.data;
		cache_store[pos].valid_bit = store.valid_bit;
		cache_store[pos].last_time_reference = iteration_times;
		cache_store[pos].reference_numbers++;
	}
}

// checking whether required data is present in cache memory
// If so, cache-hit takes place, and data is read and transferred to the request source
// If not, data is simultaneously being read from the main memory and transferred 
// from the main memory to the request source, and copied to the cache memory 
void cache_container::cache_handling(std::bitset<memory_val_size> adr, std::bitset<data_val_size> data, bool out) {
	bool hit = false, write_through = false;

    // iterate throught the cache memory
	for (size_t i = 0; i < cache_store.size(); i++) {

		if (cache_store[i].address == adr) {
			write_through = true;
			if (out)
				std::cout << "***cache-hit***" << std::endl;
			hit = true;
			cache_hit++;
			cache_write(adr, data, i, write_through, out);

			RAM_store.at(adr.to_ulong()).data = data;
			RAM_store.at(adr.to_ulong()).data_presence = true;
			break;
		}
	}
	if (!hit) {
		if (out)
			std::cout << "***cache-miss***" << std::endl;
		cache_miss++;
		if (out) {
			std::cout << "---->>> write through: RAM record writing" << std::endl;
			std::cout << "---->>> copying data to the cache" << std::endl;
		}
		cache_write(adr, data, 0, write_through, out);

		RAM_store.at(adr.to_ulong()).data = data;
		RAM_store.at(adr.to_ulong()).data_presence = true;
	}
}

// bring the value to the power of
int cache_container::pow_(int base, int power) {
	int i = 0, sum = 1;
	if (power < 0)
		return 0;
	while (i < power) {
		sum *= base;
		i++;
	}
	return sum;
}

//show cache values in the memory table
void cache_container::display_cache() {
	std::cout << std::endl;
	std::cout << std::setw(35+4) << std::setfill('-') << "cache table|" << std::endl;
	std::cout << std::setfill(' ') << "| " << std::setw(5) << "index" << "|" << std::setw(15) \
		<< "cache address " << "|" << std::setw(15) << " data | " << std::endl;
	for (size_t i = 0; i < cache_store.size(); i++) {
		std::cout << std::setfill(' ') << "|" << std::setw(6) << cache_store[i].address.to_ulong() << "|" << std::setw(15) \
			<< cache_store[i].address << "|" << std::setw(14) << cache_store[i].data << "| " << cache_store[i].last_time_reference << std::endl;
	}
}

//show RAM values in the memory table
void cache_container::display_RAM() {
	std::cout << std::endl;
	std::cout << std::setw(32) << std::setfill('-') << "RAM table|" << std::endl;
	std::cout << std::setfill(' ') << "|" << std::setw(15) \
		<< "RAM address " << "|" << std::setw(15) << " data |" << std::endl;
	for (size_t i = 0; i < RAM_store.size(); i++) {
		if (RAM_store[i].data_presence) {
			std::cout << std::setfill(' ') << "|" << std::setw(15) \
				<< i << "|" << std::setw(14) << RAM_store[i].data << "|" << std::endl;
		}
	}
}

// show main info about the memory status
void cache_container::display_info() {
	std::cout << "\tMain info:" << std::endl;
	std::cout << "[- (memory bit size) =>> " << memory_val_size << " ]" << std::endl;
	std::cout << "[- (data bit size) =>> " << data_val_size << " ]" << std::endl;
	std::cout << "[- (RAM table size) =>> " << memory_table_size << " ]" << std::endl;
	std::cout << "[- (cache table size) =>> " << cache_table_size << " ]" << std::endl;
	std::cout << "[- {hit ratio} -- " << p << " {hit_n} -- " << cache_hit << " {miss_n} -- " << cache_miss << " ]" << std::endl;
	std::cout << "[- {T RAM} -- " << t1 << " {T cache} -- " << t2 << " ]" << std::endl;
	std::cout << std::endl;
}