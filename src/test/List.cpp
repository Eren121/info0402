#include "multimap/List.h"
#include <iostream>
#include "catch.hpp"

TEST_CASE("List") {

	List<int> list;
	REQUIRE(list.empty());
	REQUIRE(equals(list, {}));

	list.push_back(10);
	list.push_back(20);
	list.push_back(30);
	REQUIRE(equals(list, {10, 20, 30}));

	list.erase(list.front());
	REQUIRE(equals(list, {20, 30}));

	list.erase(list.back());
	REQUIRE(equals(list, {20}));

	list.erase(list.front());
	REQUIRE(list.empty());
	REQUIRE(equals(list, {}));

	list.push_back(10);
	list.push_back(20);
	list.push_back(30);
	REQUIRE(list.front()->next()->next() == list.back());
	REQUIRE(list.back()->prev()->prev() == list.front());
}
