#include "multimap/multimap.h"
#include "catch/catch.hpp"
#include <map>
#include <iostream>

TEST_CASE("S'il y a plusieurs valeurs pour la clé, "
		  "find doit retourner un itérateur vers le premier elément inséré",
		  "[find]") {
	std::multimap<int, int> map;
	int res = -1;
	SECTION("0 en premier") {
		map = {{0, 0}, {0, 1}};
		res = 0;
	}
	SECTION("1 en premier") {
		map = {{0, 1}, {0, 0}};
		res = 1;
	}
	auto it = map.find(0);
	REQUIRE(it->second == res);
}
TEST_CASE("[begin]") {
	multimap<int, int> map;
	REQUIRE(map.begin() == map.end());
}
TEST_CASE("[end]") {
	multimap<int, int> map;
	REQUIRE(map.begin() == map.end());

	REQUIRE_THROWS(*map.end());
	REQUIRE_THROWS(++map.end());
}
TEST_CASE("[empty]") {
	multimap<int, int> map;
	REQUIRE(map.empty());
}
TEST_CASE("[lower_bound]") {
	multimap<int, int> map;
	REQUIRE(map.lower_bound(0) == map.end());
}
TEST_CASE("[insert]") {
	multimap<int, int> map;
	map.insert({1, 2});
}
