#include "multimap/multimap.h"
#include "catch.hpp"

typedef std::pair<const int, int> pair;
typedef typename multimap<int, int>::iterator iterator;

TEST_CASE("après une construction par défaut, une multimap est vide", "multimap") {

	multimap<int, int> map;
	REQUIRE(map.empty());
	REQUIRE(map.begin() == map.end());
	REQUIRE(map.size() == 0);
}

TEST_CASE("l'ordre des valeur dont les clés sont identique est l'ordre d'insertion", "multimap") {

	multimap<int, int> map;
	iterator it;

	// Insertion à la racine:
	map.insert({0, 0});
	// Insertion à gauche:
	map.insert({-1, 0});
	// Insertion à droite
	map.insert({1, 0});

	// Insertion double racine:
	map.insert({0, 1});
	// Insertion double à gauche (valeur inférieure):
	map.insert({-1, -1});
	// Insertion double à droite (valeur inférieure):
	map.insert({1, -1});

	auto p = pair(1, 10);
	map.insert(p);
	map.dump();
	REQUIRE(map.equals({{-1, 0}, {-1, -1}, {0, 0}, {0, 1}, {1, 0}, {1, -1}}));
}
