#include "Counter.h"
#include <iostream>

int Counter::count = 0;
int Counter::balance = 0;

Counter::Counter(bool b) : currentID(++count), trace(b) {

	if(trace) {

		std::cout << "Creating n°" << currentID << std::endl;
	}

	balance++;
}

Counter::~Counter() {

	if(trace) {

		std::cout << "Destroying n°" << currentID << std::endl;
	}

	balance--;
}

Counter::Counter(const Counter& other) : currentID(++count), trace(other.trace) {

	if(trace) {

		std::cout << "Creating n°" << currentID << " ( constructeur par copie de n°" << other.currentID << ") " << std::endl;
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
}
