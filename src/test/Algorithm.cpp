#include "multimap/Multimap.h"
#include "catch.hpp"
#include <ctime>

std::multimap<int, int> initRandom(std::size_t size = 10000, int maxKey = 1000) {

	std::multimap<int, int> m;

	for(std::size_t i = 0; i < size; i++) {

		int key = rand() % maxKey;
		int val = rand() % maxKey;
		m.insert({key, val});
	}

	return m;
}

namespace tests {

	template<typename T>
	void for_each(T& mmap, unsigned int seed) {

		// Graîne pour avoir la même séquence à chaque appel

		srand(seed);

		// Remplir de façon aléatoire la map

		std::for_each(mmap.begin(), mmap.end(), [](typename T::value_type& val) {
			val.second = rand() % 1000000;
		});

		// Inverser les valeurs

		std::for_each(mmap.begin(), mmap.end(), [](typename T::value_type& val) {
			val.second = -val.second;
		});

		// Doubler chaque valeur

		std::for_each(mmap.begin(), mmap.end(), [](typename T::value_type& val) {
			val.second = val.second + val.second;
		});

		// Soustraire à chaque valeur sa clé

		std::for_each(mmap.begin(), mmap.end(), [](typename T::value_type& val) {
			val.second -= val.first;
		});
	}

	// Compte le nomre d'éléments dont la clé et la valeur valent zéro

	template<typename T>
	typename T::difference_type count(const T& mmap) {

		return std::count(mmap.begin(), mmap.end(), std::pair<const int, int>(0, 0));
	}

	// Compte le nombre d'éléments dont la clé est paire

	template<typename T>
	typename T::difference_type count_if(const T& mmap) {

		return std::count_if(mmap.begin(), mmap.end(), [](const typename T::value_type& val) {
			return val.first % 2 == 0;
		});
	}

	template<typename T>
	T unique_copy(T& mmap) {

		T output;
		mmap.insert({0, 0}); // unique
		mmap.insert({0, 0});
		mmap.insert({1, 1}); // unique
		mmap.insert({1, 2}); // unique
		mmap.insert({1, 2});
		mmap.insert({3, 3}); // unique

		std::unique_copy(mmap.begin(), mmap.end(), std::insert_iterator<T>(output, output.begin()));
		return output;
	}
}



TEST_CASE("Algorithme std::for_each", "[multimap][algorithm]") {

	// Initialiser la graine au début
	// Sinon les appel à rand() sont donc différent pour les deux maps
	const unsigned int seed = time(0);

	std::multimap<int, int> map = initRandom();
	Multimap<int, int> mymap(map);

	tests::for_each(mymap, seed);
	tests::for_each(map, seed);

	REQUIRE(mymap == map);
}

TEST_CASE("Algorithmes std::count", "[multimap][algorithm]") {

	std::multimap<int, int> map = initRandom(100, 5);
	Multimap<int, int> mymap(map);

	REQUIRE(tests::count(map) == tests::count(mymap));
}

TEST_CASE("Algorithme std::count_if", "[multimap][algorithm]") {

	std::multimap<int, int> map = initRandom();
	Multimap<int, int> mymap(map);

	REQUIRE(tests::count_if(map) == tests::count_if(mymap));
}

TEST_CASE("Algorithme std::equal", "[multimap][algorithm]") {

	std::multimap<int, int> map = initRandom();
	Multimap<int, int> mymap(map);

	REQUIRE(std::equal(mymap.begin(), mymap.end(), map.begin()));

	map.erase(map.begin());

	REQUIRE(!std::equal(mymap.begin(), mymap.end(), map.begin()));
}

TEST_CASE("Algorithme std::unique_copy via std::insert_iterator", "[multimap][algorithm]") {

	std::multimap<int, int> map;
	Multimap<int, int> mymap(map);

	// Compare les deux conteneurs obtenus
	REQUIRE(tests::unique_copy(map) == tests::unique_copy(mymap));
}
