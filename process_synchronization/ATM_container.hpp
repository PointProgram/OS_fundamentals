#pragma once

#include <map>
#include <stdlib.h>

using namespace std;

const int repeat_limit = 5;

class ATM {
private:
	int X;
	int Y;
	int redundant;
	map<int, int> banc_account;
	int cash[7] = { 1, 2, 5, 10, 20, 50, 100 };
	map<int, int>::iterator itr;
	bool bound[2] = { true, false };
public:
	ATM() : X((rand() % 1000) + 1), Y((rand() % 10000) + 1), redundant(0){
		init_bancomat();
	}
	void init_bancomat();
	void A();
	void B();

};
