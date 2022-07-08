//#include "fat_table.hpp"
#include "fat_table_mod.hpp"
using namespace std;

void hello_window() {
    cout << "Enter following buttons to choose mods for fat_16 table:" << endl;
	cout << "a - new | " << "c - change | ";
	cout << "r - remove | " << "d - clean table" << endl;
	cout << "s - display | " << "f - find | ";
	cout << "q - quit" << endl;
}

int main() {
	// fat_table instructions
	/*
	alloc_memory();

	srand(time(0));
    hello_window();	

	while (!cin.fail())
		direction();

	getchar();
	*/

	//fat_table_mod instructions

	srand(time(0));

	initialization();
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("| Command menu - { [n - new], [m - modify], [d - delete], [c - clean], [s - show], [f - find], [e - exit], [q - show files]} |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");

	while (!cin.fail())
		direction();

	getchar();

	return 0;
}