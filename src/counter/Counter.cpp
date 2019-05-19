#include "Counter.h"
#include <iostream>
#include "catch.hpp"

#define TRACE_ENABLED 0

int Counter::constructions = 0;
int Counter::destructions = 0;
int Counter::assignements = 0;
int Counter::move_assignments = 0;
int Counter::copies = 0;
int Counter::moves = 0;
int Counter::balance = 0;
int Counter::nextID = 0;

Counter::Counter(bool b) : currentID(++nextID), trace(b && TRACE_ENABLED) {

	if(trace) {

		std::cout << "Creating n." << currentID << "(" << balance << ")" << std::endl;
	}

	balance++;
	constructions++;
}

Counter::~Counter() {

	if(trace) {

		std::cout << "Destroying n." << currentID << "(" << balance << ")" << std::endl;
	}

	balance--;
	destructions++;
}

Counter::Counter(const Counter& other) : currentID(++nextID), trace(other.trace) {

	if(trace) {

		std::cout << "Creating n." << currentID << " ( constructeur par copie de n." << other.currentID << ") "
				  << "(" << balance << ")" << std::endl;
	}

	balance++;
	copies++;
}

Counter::Counter(Counter&& other) : currentID(++nextID), trace(other.trace) {

	if(trace) {

		std::cout << "Creating n." << currentID << " ( constructeur par copie de n." << other.currentID << ") "
				  << "(" << balance << ")" << std::endl;
	}

	balance++;
	moves++;
}

void Counter::printCount() {

	std::cout << Catch::getCurrentContext().getResultCapture()->getCurrentTestName() << std::endl;
	std::cout << balance << " Instances living" << std::endl;
	std::cout << nextID << " Total instances created" << std::endl;
	std::cout << constructions << " constructions\n";
	std::cout << copies << " copies\n";
	std::cout << moves << " moves\n";
	std::cout << assignements << " assignments\n";
	std::cout << move_assignments << " move assignments\n";
	std::cout << destructions << " destructions\n";

	if(balance == 0) {

		std::cout << "No memory leaks" << std::endl;
	}

	std::cout << "--------------" << std::endl;
}

void Counter::resetCount() {

	nextID = 0;
	constructions = 0;
	destructions = 0;
	copies = 0;
	moves = 0;
	assignements = 0;
	move_assignments = 0;
	balance = 0;
}

bool Counter::check() {
	bool noLeaks = (balance == 0);

	printCount();
	resetCount();

	return noLeaks;
}
