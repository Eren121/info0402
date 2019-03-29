#include "Counter.h"
#include <iostream>

int Counter::count = 0;
int Counter::balance = 0;

Counter::Counter(bool b) : currentID(++count), trace(b) {

	if(trace) {

		std::cout << "Creating n." << currentID << "(" << balance << ")" << std::endl;
	}

	balance++;
}

Counter::~Counter() {

	if(trace) {

		std::cout << "Destroying n." << currentID << "(" << balance << ")" << std::endl;
	}

	balance--;
}

Counter::Counter(const Counter& other) : currentID(++count), trace(other.trace) {

	if(trace) {

		std::cout << "Creating n." << currentID << " ( constructeur par copie de n." << other.currentID << ") "
				  << "(" << balance << ")" << std::endl;
	}

	balance++;
}

void Counter::printCount() {

	std::cout << balance << " Instances living" << std::endl;
	std::cout << count << " Total instances created" << std::endl;

	if(balance == 0) {

		std::cout << "No memory leaks" << std::endl;
	}
}

void Counter::resetCount() {

	count = 0;
	balance = 0;
}

bool Counter::check() {
	bool noLeaks = (balance == 0);

	printCount();
	resetCount();

	return noLeaks;
}
