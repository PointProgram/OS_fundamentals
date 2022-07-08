#include "ATM_container.hpp"

#include <iostream>
#include <vector>
#include <atomic>


#include <chrono>

atomic_flag lock_stream = ATOMIC_FLAG_INIT;

void ATM::init_bancomat() {
	banc_account.insert(pair<int, int>(cash[0], 1));
	banc_account.insert(pair<int, int>(cash[1], 7));
	banc_account.insert(pair<int, int>(cash[2], 3));
	banc_account.insert(pair<int, int>(cash[3], 5));
	banc_account.insert(pair<int, int>(cash[4], 1));
	banc_account.insert(pair<int, int>(cash[5], 9));
	banc_account.insert(pair<int, int>(cash[6], 5));
}

void ATM::A() {
	while (true) {
		while (lock_stream.test_and_set()) {}
		//critical section A
		if (bound[0]) {
			cout << "\n---\tA process IN <<<<" << endl;
			cout << "Client sends a request for payment from a cash machine" << endl;
			cout << "The amount of money in the account " << Y << endl;
			cout << "Number of banknotes denomination in the cash machine" << endl;

			for (itr = banc_account.begin(); itr != banc_account.end(); itr++) {
				cout <<"[" << itr->first << "] - " << itr->second << "; ";
			}
			cout << endl;

			X = (rand() % 1000) + 1;
			cout << "Client requires " << X << endl;
			redundant++;

			if (X < Y) {
				cout << "Possible to make a payment" << endl;
				
				
				lock_stream.clear();
				bound[0] = false;
				bound[1] = true;
			}
			else if (Y == 0) {
				cout << "There are no money in your account" << endl;
				exit(1);
			}
			else {
				cout << "Not enough money in the account" << endl;
				//X = (rand() % 1000) + 1;
				//cout << "Client requires " << X << endl;
				bound[0] = true;
				bound[1] = false;
				lock_stream.clear();
			}

			if (redundant > repeat_limit) {
				cout << "{ Unfortunately you exceeded your requests \nfor money payoff(requests contain a larger amount \nthan it is in the account) }\n";
				exit(1);
			}
		}
		else {
			lock_stream.clear();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
	
}

void ATM::B() {
	while (true) {
		while (lock_stream.test_and_set()) {}

		//critical section B
		if (bound[1]) {
			cout << "\n---\tB process IN <<<<" << endl;
			redundant = 0;
			map<int, int> tmp_ack(banc_account.begin(), banc_account.end());
			int i = 0, sum = 0;
			for (i = 0, itr = tmp_ack.begin(); itr != tmp_ack.end(); ++itr, ++i) {
				itr->second = 0;
			}

			for (int i = 6; i >= 0; i--) {
				itr = banc_account.find(cash[i]);
				int cnt = X / itr->first;
				while ((sum + cnt * itr->first) > X) {
					cnt--;
				}
				if (cnt <= itr->second) {
					sum += cnt * itr->first;
					tmp_ack.find(itr->first)->second = cnt;
				}
				else {
					sum += itr->second * itr->first;
					tmp_ack.find(itr->first)->second = itr->second;
				}

				//cout << "Sum after " << i << "iteration is " << sum << endl;

				if (sum < X) {
					continue;
				}
				else if (sum == X) {
					break;
				}

			}
			if (sum == X) {
				cout << "Successfull transaction:" << endl;
				cout << "Sum consist of: ";
				for (itr = tmp_ack.begin(); itr != tmp_ack.end(); itr++) {
					if (itr->second != 0) {
						cout << itr->second << "x" << itr->first << " ";
					}
				}
				cout << " = " << sum << endl;
				cout << endl;
				//cout << "Sum after " << i << "iteration is " << sum << endl;
				map<int, int>::iterator it;
				for (itr = banc_account.begin(), it = tmp_ack.begin(); itr != banc_account.end(); ++itr, it++) {
					itr->second -= it->second;
				}
				Y -= X;
			}
			else {
				cout << "There is no money in the cash machine for that payoff" << endl;
			}
			for (i = 0, itr = banc_account.begin(); itr != banc_account.end(); ++itr) {
				if (itr->second == 0)
					i++;
			}
			if (i == 7) {
				cout << "Cash machine is empty" << endl;
				exit(1);
			}

			lock_stream.clear();
			bound[0] = true;
			bound[1] = false;
		}
		else {
			lock_stream.clear();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}
