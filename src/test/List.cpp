#include "multimap/List.h"
#include <iostream>
#include "catch.hpp"

typedef std::list<int> STLList;

TEST_CASE("List") {

	List<int> list;

	REQUIRE(list.empty());
	REQUIRE(list == STLList());

	list.push_back(10);
	list.push_back(20);
	list.push_back(30);
	REQUIRE((list == STLList{10, 20, 30}));

	list.erase(list.front());
	REQUIRE((list == STLList{20, 30}));

	list.erase(list.back());
	REQUIRE(list == STLList{20});

	list.erase(list.front());
	REQUIRE(list.empty());
	REQUIRE(list == STLList{});

	list.push_back(10);
	list.push_back(20);
	list.push_back(30);
	REQUIRE(list.front()->next()->next() == list.back());
	REQUIRE(list.back()->prev()->prev() == list.front());

	int i = 0;
	list.push_back(i);
}
