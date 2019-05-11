#include "multimap/Multimap.h"
#include "catch.hpp"
#include <ctime>

std::multimap<int, int> initRandom(std::size_t size = 1000, unsigned int seed = static_cast<unsigned int>(time(nullptr)), int maxValue = 10000) {
	std::multimap<int, int> m;
	srand(seed);
	for(std::size_t i = 0; i < size; i++) {
		int key = rand() % maxValue;
		int val = rand() % maxValue;
		m.insert({key, val});
	}
	return m;
}

TEST_CASE("Algorithme std::for_each") {

	const unsigned int seed = 12345;
	std::multimap<int, int> map = initRandom();
	Multimap<int, int> mymap(map);
	std::function<void(std::pair<const int, int>&)> transform;

	SECTION("Remplacer les valeurs aléatoirement") {
		transform = [](std::pair<const int, int>& i) { i.second = rand(); };
	}

	SECTION("Inverser les valeurs") {
		transform = [](std::pair<const int, int>& i) { i.second = -i.second; };
	}

	srand(seed);
	std::for_each(map.begin(), map.end(), transform);
	srand(seed);
	std::for_each(mymap.begin(), mymap.end(), transform);
	REQUIRE(mymap == map);
}

TEST_CASE("Algorithme std::count, std::count_if") {

	unsigned int seed = 12345;
	std::multimap<int, int> map = initRandom(1000, seed, 10);
	Multimap<int, int> mymap(map);
	std::function<bool(std::pair<const int, int> const&)> predicate;

	SECTION("std::count") {

		for(int i = 0; i < 10; i++) {
			for(int j = 0; j < 10; j++) {
				std::pair<const int, int> val = {i, j};
				REQUIRE(std::count(map.begin(), map.end(), val) ==
						std::count(mymap.begin(), mymap.end(), val));
			}
		}
	}

	SECTION("std::count_if") {

		SECTION("Compter le nombre de clé inférieures à la valeur") {
			predicate = [](const std::pair<const int, int>&p) { return p.first < p.second; };
		}

		SECTION("Compter le nombre d'éléments tel que cle + valeur < 10") {
			predicate = [](const std::pair<const int, int>&p) { return p.first + p.second < 10; };
		}

		REQUIRE(std::count_if(map.begin(), map.end(), predicate) ==
				std::count_if(mymap.begin(), mymap.end(), predicate));
	}
}

TEST_CASE("Algorithme std::match") {

	unsigned int seed = 12345;
	std::multimap<int, int> map = initRandom(1000, seed, 10);
	Multimap<int, int> mymap(map);

	REQUIRE(std::equal(mymap.begin(), mymap.end(), map.begin()));
	map.erase(map.begin());
	REQUIRE(!std::equal(mymap.begin(), mymap.end(), map.begin()));
}

TEST_CASE("Algorithme std::remove, std::remove_if") {

}
