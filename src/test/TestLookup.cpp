#include "multimap/Multimap.h"
#include "catch.hpp"
#include <iostream>

/// Test des accesseurs

TEMPLATE_PRODUCT_TEST_CASE("Recherche d'éléments",
						   "[multimap][std::multimap][template][lookup]",
						   (std::multimap, Multimap), ((int, int))) {

	TestType map;
	auto it1 = map.insert({0, 0});
			   map.insert({0, 1});
	auto it2 = map.insert({1, 0});
			   map.insert({1, 1});
			   map.insert({1, 2});

	SECTION("La fonction find() doit chercher un élément") {

		REQUIRE(map.find(0) == it1);
		REQUIRE(map.find(1) == it2);
		REQUIRE(map.find(2) == map.end());
	}

	SECTION("La fonction count() doit retourner le nombre d'éléments") {

		REQUIRE(map.count(0) == 2);
		REQUIRE(map.count(1) == 3);
		REQUIRE(map.count(2) == 0);

	}

	SECTION("La fonction upper_bound() doit retourner l'élément supérieur le plus petit") {

		REQUIRE(map.upper_bound(0) == it2);
		REQUIRE(map.upper_bound(1) == map.end());
	}
}

TEMPLATE_PRODUCT_TEST_CASE("La fonction contains() doit vérifier l'existence d'un élément",
						   "[multimap][template][lookup]",
						   (Multimap), ((int, int))) {

	TestType map;
	map.insert({0, 0});
	map.insert({0, 1});
	map.insert({1, 0});
	map.insert({1, 1});
	map.insert({1, 2});

	REQUIRE(map.contains(0));
	REQUIRE(map.contains(1));
	REQUIRE(!map.contains(2));
}

TEMPLATE_PRODUCT_TEST_CASE("La fonction lower_bound() doit retourner la première clé qui n'est pas plus petite",
                           "[multimap][std::multimap][template][lookup]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType map;
    int key;
    std::pair<int, int> required;
    typename TestType::iterator it;

    SECTION("Si la multimap est vide, lower_bound doit toujours retourner un itérateur de fin") {

        key = 0;
        it = map.lower_bound(key);
        REQUIRE(it == map.end());
    }

    SECTION("Si la multimap n'est pas vide") {

        map.insert({0, 0});
        map.insert({0, 1});
        map.insert({-2, 2});
        map.insert({-2, 3});
        map.insert({1, 4});
        map.insert({1, 5});

        SECTION("Et qu'il n'y a pas d'éléments supérieur ou égal, lower_bound doit retourner un itérateur de fin") {

            key = 2;
            it = map.lower_bound(key);
            REQUIRE(it == map.end());
        }

        SECTION("Et qu'il y a au moins un élément supérieur ou égal") {

            SECTION("Cas 1: S'il y a une clé strictement supérieur, lower_bound() doit retourner le plus petit d'entre-eux inséré et si les clés sont égales, celui inséré en premier") {

                key = -1;
                required = {0, 0};
            }

            SECTION("Cas 2: S'il y a un élément égal, lower_bound() doit le premier inséré") {

                key = 1;
                required = {1, 4};
            }

            it = map.lower_bound(key);
            REQUIRE(it != map.end());
            REQUIRE(*it == std::pair<const int, int>(required));
        }
    }
}

TEMPLATE_PRODUCT_TEST_CASE("Test de l'affichage",
						   "[multimap][lookup]",
						   (Multimap), ((int, int))) {

	TestType map{{0, 0}, {1, 1}, {0, 2}, {-1, 3}, {3, 5}, {-1, 6}};
	std::cout << map;
}
