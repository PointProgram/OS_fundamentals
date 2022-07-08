#include "ATM_container.hpp"
#include <thread>

int main() {
	//srand((unsigned)time(0));
	srand(123456);

	ATM  a;
	std::thread first (&ATM::A, &a);
	std::thread second(&ATM::B, &a);

	first.join();
	second.join();

	return 1;
}
