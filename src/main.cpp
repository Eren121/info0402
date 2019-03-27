#include "catch.hpp"
#include <map>
#include <iostream>
#include "Map.h"
#include <type_traits>

TEMPLATE_TEST_CASE("Map<K, T>", "[map]", int) {

	typedef TestType		Key;
	typedef TestType		T;
	typedef Map<Key, T>		map;
	
	map m;

	SECTION("Map vide") {

		REQUIRE(m.empty());				// Après le constructeur par défaut, la map doit être vide
		REQUIRE(m.begin() == m.end());	// Quand la map est vide, l'itérateur de fin doi être géal à l'itérateur de début
		REQUIRE(m.size() == 0);			// Quand la map est vide, sa taille est zéro
		REQUIRE(m.find(3) == m.end());  // Quand la map est vide, la recherche de n'importe quel valeur doit retourner l'itérateur de fin
	}

	SECTION("Insertion") {

		m.insert(m.begin(), std::make_pair(2, 2));
	}

	SECTION("lower_bound") {


		REQUIRE(m.lower_bound(2) == m.end());

		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 20));
		m.insert(std::make_pair(4, 30));
		REQUIRE(m.size() == 3);
		REQUIRE(!m.empty());

		std::cout << m.lower_bound(0)->first << std::endl;

		REQUIRE(*m.lower_bound(0) == typename decltype(m)::value_type(0, 10));
		REQUIRE(*m.lower_bound(1) == typename decltype(m)::value_type(2, 20));
		REQUIRE(*m.lower_bound(2) == typename decltype(m)::value_type(2, 20));
		REQUIRE(*m.lower_bound(4) == typename decltype(m)::value_type(4, 30));
		REQUIRE(m.lower_bound(5) == m.end());
	}

	SECTION("upper_bound") {

		REQUIRE(m.upper_bound(2) == m.end());
		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 20));
		m.insert(std::make_pair(4, 30));

		REQUIRE(*m.upper_bound(-1) == std::pair<const Key, T>(0, 10));
		REQUIRE(*m.upper_bound(0) == std::pair<const Key, T>(2, 20));
		REQUIRE(*m.upper_bound(2) == std::pair<const Key, T>(4, 30));
		REQUIRE(*m.upper_bound(3) == std::pair<const Key, T>(4, 30));
		REQUIRE(m.upper_bound(4) == m.end());
		REQUIRE(m.upper_bound(5) == m.end());
	}

	SECTION("count") {

		REQUIRE(m.count(0) == 0);
		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 10));

		REQUIRE(m.count(0) == 1);
		REQUIRE(m.count(1) == 0);
		REQUIRE(m.count(2) == 1);
	
	}
}
