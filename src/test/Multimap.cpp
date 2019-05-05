#include "multimap/Multimap.h"
#include "catch.hpp"

typedef std::pair<const int, int> pair;
typedef typename Multimap<int, int>::iterator iterator;

TEST_CASE("lower_bound doit retourner la première clé qui n'est pas plus petite", "multimap") {

	Multimap<int, int> map;
	int key;
	std::pair<int, int> required;
	iterator it;

	SECTION("Si la multimap est vide, lower_bound doit toujours retourner un itérateur de fin") {

		key = 0;
		it = map.lower_bound(key);
		REQUIRE(it == map.end());
	}

	SECTION("Si la multimap n'est pas vide: ") {

		map.insert({0, 0});
		map.insert({0, 1});
		map.insert({-2, 2});
		map.insert({-2, 3});
		map.insert({1, 4});
		map.insert({1, 5});

		SECTION("Il n'y a pas d'éléments supérieur ou égal") {

			key = 2;
			it = map.lower_bound(key);
			REQUIRE(it == map.end());
		}

		SECTION("Il y a au moins un élément supérieur ou égal") {

			SECTION("Cas 1: strictement supérieur") {
				key = -1;
				required = {0, 0};
			}

			SECTION("Cas 2: égaux") {
				key = 1;
				required = {1, 4};
			}

			it = map.lower_bound(key);
			REQUIRE(it != map.end());
			REQUIRE(*it == pair(required));
		}
	}
}

TEST_CASE("après une construction par défaut, une multimap est vide", "multimap") {

	Multimap<int, int> map;
	REQUIRE(map.empty());
	REQUIRE(map.begin() == map.end());
	REQUIRE(map.size() == 0);
}

TEST_CASE("l'ordre des valeur dont les clés sont identique est l'ordre d'insertion", "multimap") {

	Multimap<int, int> map;

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

	std::cout << map;
	REQUIRE(map.equals({{-1, 0}, {-1, -1}, {0, 0}, {0, 1}, {1, 0}, {1, -1}}));
}
