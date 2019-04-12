#include <map>
#include "catch/catch.hpp"

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
