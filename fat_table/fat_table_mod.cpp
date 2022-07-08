#include "fat_table_mod.hpp"


vector<FAT_file> fat_table;
vector<int> memory_table;

void add_file(string name, int size) {
	FAT_file file;
	int clster_numbers = clusters_amount - 2, next_clst = 2, previous_clst;

	for (size_t i = 0; i < fat_table.size(); i++) {
		if (name.compare(fat_table[i].filename) == 0) {
			cout << "This file name is already used..." << endl;
			return;
		}
	}

	for (size_t i = 0; i < fat_table.size(); i++)
		clster_numbers -= fat_table[i].filesize;

	if (clster_numbers < size) {
		std::cout << "There is no free space..." << std::endl;
		return;
	}

	file.filesize = size;
	file.filename = name;
	file.initial_cluster = next_clst;

	while (next_clst < clusters_amount - 1 && memory_table[next_clst] != 0x0000) {
		next_clst++;
		file.initial_cluster = next_clst;
	}

	previous_clst = file.initial_cluster;
	next_clst = previous_clst + 1;

	while (size != 1) {
		if (memory_table[next_clst] == 0) {
			memory_table[previous_clst] = next_clst;
			previous_clst = next_clst;
			size--;
		}
		next_clst++;
	}
	memory_table[previous_clst] = 0xFFFF;
	file.final_cluster = previous_clst;
	fat_table.push_back(file);
}

void initialization() {
	std::random_device devision;
	std::mt19937 rng(devision());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 1000); // distribution in range [1, 6]
	auto defect_max = dist6(rng);

	for (int i = 0; i < clusters_amount; i++) {
		memory_table.push_back(0x0000);
		if ((rand() % 100) < 20 && defect_max != 0) {
			memory_table[i] = 0x00F7;
		}
	}
	memory_table[0] = 0x00F7;
	memory_table[1] = 0xFFFF;
}

void modify_file_size(string name, int size) {
	
	FAT_file file = fat_table[0];
	int cluster = clusters_amount;

	for (size_t i = 0; i < fat_table.size(); i++) {
		if (name.compare(fat_table[i].filename) == 0) {
			cluster = fat_table[i].initial_cluster;
			file = fat_table[i];
			break;
		}
	}
	if (cluster == clusters_amount) {
		cout << "File \"" << name << "\" is damaged..." << endl;
		return;
	}

	int cluster_limit = clusters_amount - 2, left_items = size - file.filesize, val_shift = file.filesize - 1;

	for (size_t i = 0; i < fat_table.size(); i++)
		cluster_limit -= fat_table[i].filesize;

	if (cluster_limit < left_items && left_items > 0) {
		cout << "Size of file exceeds table left memory..." << endl;
		return;
	}

	while (val_shift != 0) {
		cluster = memory_table[cluster];
		val_shift--;
	}

	cluster_limit = cluster;
	//get the final cluster position
	cluster = cluster_limit + 1;
	//add new cluster to the end of the final cluster
	int lmt = 2;
	if (left_items > 0) {
		while (left_items) {
			if (memory_table[cluster] == 0) {
				memory_table[cluster_limit] = cluster;
				cluster_limit = cluster;
				left_items--;
			}
			cluster++;
		}
		lmt = cluster_limit;
		memory_table[cluster_limit] = 0xFFFF;

	}
	else {
		cluster_limit += left_items;
		cluster += left_items + 1;
		lmt = cluster_limit;
		memory_table[cluster_limit] = 0xFFFF;
		cluster_limit++;
		while (-left_items) {
			memory_table[cluster_limit] = 0;
			cluster_limit = cluster++;
			left_items++;
		}
	}
	for (size_t i = 0; i < fat_table.size(); i++)
		if (name.compare(fat_table[i].filename) == 0) {
			fat_table[i].filesize = size;
			fat_table[i].final_cluster = lmt;
		}
}

void delete_file(string name) {
	for (size_t i = 0; i < fat_table.size(); i++) {
		if (name.compare(fat_table[i].filename) == 0) {
			int cluster_limit = fat_table[i].initial_cluster, clust_spare, table_size = fat_table[i].filesize;
			while (table_size) {
				clust_spare = memory_table[cluster_limit];
				memory_table[cluster_limit] = 0;
				cluster_limit = clust_spare;
				table_size--;
			}

			fat_table.erase(fat_table.begin() + i);
			break;
		}
	}
}

void clean_table() {
	for (size_t i = 0; i < fat_table.size(); i++) {
		int cluster_limit = fat_table[i].initial_cluster, clust_spare, sum_size = fat_table[i].filesize;
		while (sum_size) {
			clust_spare = memory_table[cluster_limit];
			memory_table[cluster_limit] = 0;
			cluster_limit = clust_spare;
			sum_size--;
		}
		fat_table.erase(fat_table.begin() + i);
	}
}

void show_table(string name)
{
	if (!fat_table.size())
		cout << "FAT-16 table is empty..." << endl;

	int left_clust, cnt;
	for (size_t i = 0; i < fat_table.size(); i++) {
		if (!name.empty() && name.compare(fat_table[i].filename) != 0)
			continue;
		
		cnt = fat_table[i].filesize;
		left_clust = fat_table[i].initial_cluster;
		cout << ("FAT-16 TABLE:\n");
		std::cout << "Name: " << fat_table[i].filename << "; size: " << cnt << "; initial cluster: " << std::hex << left_clust << std::endl;

		
		cout << "CHAIN OF CONNECTIONS:\n";
		while (cnt != 0) {
			cout << "[" << hex << i  << " ]" << hex << "[ " << left_clust << " ]->";
			left_clust = memory_table[left_clust];
			cnt--;
		}
		cout << endl;
		if (!name.empty())
			break;
	}

	if (name.empty()) {
		cout << "File with such name doesen`t exists..." << endl;
	}
}

void show_all()
{
	int final_cls = 1;
	cout << ("FAT-16 TABLE: up->index, down->value\n");
	for (size_t i = 0; i < fat_table.size();i++) {
		if(fat_table[i].final_cluster > final_cls)
			final_cls = fat_table[i].final_cluster;
	}
	for (int i = 0; i <=final_cls; i++) {
		printf("[%4X]-", i);
	}
	cout << endl;
	for (int i = 0; i <= final_cls; i++) {
		printf("[%4X]-", memory_table[i]);
	}
}

void show_files()
{
	if (!fat_table.size()) {
		cout << "FAT-16 table is empty..." << endl;
		return;
	}

	cout << ("FAT-16 TABLE: [FILE][SIZE]\n");
	for (size_t i = 0; i < fat_table.size(); i++) {
		cout << "[ " << fat_table[i].filename << " ]" << "[ " << fat_table[i].filesize << " ]-";
	}
	cout << endl;
}

void direction()
{
	string filename;
	int filesize;
	char button_key;
	cin >> button_key;
	switch (button_key)
	{
		case 'n':
			cout << "[FILE NAME] - ";
			cin >> filename;
			cout << "[FILE SIZE] - ";
			cin >> std::dec >> filesize;
			add_file(filename, filesize);
			cout << "FILE is ADDED\n";
			break;
		case 'm':
			cout << "[FILE NAME] - ";
			cin >> filename;
			cout << "[FILE SIZE] - ";
			cin >> std::dec >> filesize;
			modify_file_size(filename, filesize);
			cout << "FILE is MODIFIED\n";
			break;
		case 'd':
			cout << "[FILE NAME] - ";
			cin >> filename;
			delete_file(filename);
			cout << "FILE is DELETED\n";
			break;
		case 's':
			show_all();
			break;
		case 'c':
			clean_table();
			cout << "TABLE is CLEANED\n";
			break;
		case 'f':
			cout << "[FILE NAME] - ";
			cin >> filename;
			show_table(filename);
			break;
		case 'e':
			exit(1);
			break;
		case 'q':
			show_files();
			break;
		default:
			cout << "You pressed wronk key, please try again..." << endl;
			break;
	}
	
	cout << endl;
}