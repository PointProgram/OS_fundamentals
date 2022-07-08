#include "fat_table.hpp"

vector<FAT_file> fat;
vector<int> mem;

void alloc_memory() {
	int cnt = 4;
	for (int i = 0; i < FAT_16; i++) {
		mem.push_back(0x0000);
		if ((rand() % 100) > 50 && i % cnt == 0) {
			mem[i] = 0x00F7;
		}
	}
	mem[0] = 0x00F7;
	mem[1] = 0xFFFF;
}

void change_size(string name, int size) {
	long int clust_n = FAT_16;
	FAT_file* file = &fat[0];

	
	for (int i = 0; i < fat.size(); i++) {
		if (name.compare(fat[i].name) == 0) {
			clust_n = fat[i].initial_claster;
			file = &fat[i];
		}
	}

	if (clust_n != FAT_16) {
		int clust_left = FAT_16 - 2, sum_size = size - file->file_size, val_shift = file->file_size - 1;

		for (int i = 0; i < fat.size(); i++)
			clust_left -= fat[i].file_size;

		if (clust_left < sum_size && sum_size > 0) {
			cout << "Size allocation fails, exceeding size value..." << endl;
			return;
		}
		
		while (val_shift != 0) {
			clust_n = mem[clust_n];
			val_shift--;
		}

		clust_left = clust_n;
		int cluster = clust_left + 1;
		if (sum_size > 0) {
			while (sum_size) {
				if (mem[cluster] == 0x0000) {
					mem[clust_left] = cluster;
					clust_left = cluster;
					sum_size--;
				}
				cluster++;
			}
			mem[clust_left] = 0xFFFF;
		}
		else {
			clust_left += sum_size;
			cluster += sum_size + 1;
			mem[clust_left] = 0xFFFF;
			clust_left++;
			while (-sum_size)
			{
				mem[clust_left] = 0;
				clust_left = cluster;
				sum_size++;
				//while(mem[cluster] == 0x00F7)
				cluster++;
			}
		}
		for (int i = 0; i < fat.size(); i++)
			if (name.compare(fat[i].name) == 0) {
				fat[i].file_size = size;
			}
	}
	else {
		cout << "ERROR, file [" << name << "] isn't found" << endl;
	}
}

void erase_(string name) {
	for (int i = 0; i < fat.size(); i++) {
		if (name.compare(fat[i].name) == 0) {
			int clust_left = fat[i].initial_claster, clust_spare, sum_size = fat[i].file_size;
			
			if (sum_size > 0) {
				while (sum_size) {
					clust_spare = mem[clust_left];
					mem[clust_left] = 0x0000;
					clust_left = clust_spare;
					sum_size--;
				}
				
			}
			
			fat.erase(fat.begin() + i);
			break;
		}
	}
}

void remove_all() {
	for (int i = 0; i < fat.size(); i++) {
			int clust_left = fat[i].initial_claster, clust_spare, sum_size = fat[i].file_size;
			if (sum_size > 0) {
				while (sum_size) {
					clust_spare = mem[clust_left];
					mem[clust_left] = 0x0000;
					clust_left = clust_spare;
					sum_size--;
				}

			}
			fat.erase(fat.begin() + i);
		}
}

void push_(string name, int size) {
	for (int i = 0; i < fat.size(); i++) {
		if (name.compare(fat[i].name) == 0) {
			cout << "Record with such name is present in the memory..." << endl;
			return;
		}
	}

	FAT_file fatt;
	int clust_n = FAT_16 - 2, size_val = 2;
	fatt.file_size = size;
	fatt.name = name;

	for (int i = 0; i < fat.size(); i++) {
		clust_n -= fat[i].file_size;
	}

	if (clust_n < size) {
		std::cout << "No more space left..." << std::endl;
		return;
	}

	fatt.initial_claster = size_val;
	while (size_val < FAT_16 - 1 && mem[size_val] != 0x0000) {
		fatt.initial_claster = ++size_val;
	}

	clust_n = fatt.initial_claster;
	size_val = clust_n + 1;

	while (size != 1)
	{
		if (mem[size_val] == 0x0000)
		{
			mem[clust_n] = size_val;
			clust_n = size_val;
			size--;
		}
		size_val++;
	}
	mem[clust_n] = 0xFFFF;
	fat.push_back(fatt);
}


void display_FAT_16(string name)
{
	if (!fat.size()) 
		cout << "FAT table is empty..." << endl;

	int clust_n, cnt;
	for (int i = 0; i < fat.size(); i++) {
		if (!name.empty() && name.compare(fat[i].name) != 0) {
			continue;
		}
		cnt = fat[i].file_size;
		clust_n = fat[i].initial_claster;
		std::cout << "Name: " << fat[i].name << "; size: " << cnt << "; initial cluster: " << std::hex << clust_n << std::endl;
		
		cout << "Content ->| ";
		while (cnt != 0) {
			cout << hex << clust_n << " | ";
			clust_n = mem[clust_n];
			cnt--;
		}
		cout << endl;
		if (!name.empty())
			break;
	}

	if (name.empty()) {
		cout << "Unfortunately file with such name doesen`t exists..." << endl;
	}
}

void direction()
{
	string name;
	int size;
	char key;
	cin >> key;
	if (key == 'a') {
		cout << "Name of file: ";
		cin >> name;
		cout << "Size: ";
		cin >> dec >> size;

		push_(name, size);
	}
	else if (key == 'c') {
		cout << "Name of file: ";
		cin >> name;
		cout << "Size: ";
		cin >> dec >> size;

		change_size(name, size);
	}
	else if (key == 'r')
	{
		cout << "Name of file:";
		cin >> name;

		erase_(name);
	}
	else if (key == 's')
	{
		display_FAT_16("");
	}
	else if (key == 'f')
	{
		cout << "Name of file: ";
		cin >> name;

		display_FAT_16(name);
	}
	else if (key == 'd') {
		remove_all();
	}
	else if (key == 'q')
		exit(0);
	else {
		cout << "wrong key code..." << endl;
	}
	cout << endl;
}
