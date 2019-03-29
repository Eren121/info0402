#include <map>
#include "catch.hpp"
#include "Map.h"
#include "Counter.h"

TEST_CASE("Counter") {

	{
		Counter a,b,c,d,e;
	}

	Counter::check();
}

TEMPLATE_PRODUCT_TEST_CASE("Fuites mémoires", "[map]", (std::map, Map), ((int, Counter))) {

	typedef TestType Container;

	{ // Destructeur
		Container map;

		for(int i = 0; i < 100; i++) {
			map[i] = Counter();
		}
	}

	bool ok = Counter::check();
	REQUIRE(ok);

	{ // Clear
		Container map;

		for(int i = -20; i < 100; i += 11) {
			map.insert({i, Counter()});
			map[i+3] = map[i];
			map[i] = map[i-1];
			map.erase(i / 2);
		}

		map.clear();

		bool ok = Counter::check();
		REQUIRE(ok);
	}
}

TEMPLATE_PRODUCT_TEST_CASE("All map methods", "[map]", (std::map, Map), ((int, int))) {

	typedef TestType Container;
	typedef typename Container::iterator iterator;
	typedef typename Container::value_type value_type;
	typedef typename Container::key_type Key;
	typedef typename Container::mapped_type T;
	Container m;


	SECTION("begin") {

		REQUIRE(m.begin() == m.end());	// Quand la map est vide, l'itérateur de fin doi être égal à l'itérateur de début

		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(1, 10));
		m.insert(std::make_pair(2, 20));

		iterator it = m.begin();
		REQUIRE(it != m.end());
		REQUIRE(*it == std::pair<const int, int>(0, 10));
		++it;
		REQUIRE(it != m.end());
		REQUIRE(*it == std::pair<const int, int>(1, 10));
		++it;
		REQUIRE(it != m.end());
		REQUIRE(*it == std::pair<const int, int>(2, 20));
		++it;
		REQUIRE(it == m.end());

	}

	SECTION("insert") {

		const std::pair<const Key, T>& value = std::make_pair(0, 10);
		std::pair<iterator, bool> pair = m.insert(value);

		REQUIRE(*pair.first == std::pair<const int, int>(0, 10));
		REQUIRE(pair.second == true);

		pair = m.insert(std::make_pair(1, 10));

		REQUIRE(*pair.first == std::pair<const int, int>(1, 10));
		REQUIRE(pair.second == true);

		pair = m.insert(std::make_pair(2, 20));

		REQUIRE(*pair.first == std::pair<const int, int>(2, 20));
		REQUIRE(pair.second == true);

		auto pair2 = m.insert(std::make_pair(2, 20));
		REQUIRE(pair2.first == pair.first);
		REQUIRE(pair2.second == false);

		auto pair3 = m.insert(std::make_pair(2, 30));
		REQUIRE(pair3.first == pair.first);
		REQUIRE(pair3.second == false);
	}

	SECTION("operator[]") {

		m[-1] = 1;
		m[100] = 2;
		m[1000] = 3;

		REQUIRE(m.size() == 3);

		iterator it = m.begin();
		REQUIRE(*it == std::pair<const int, int>(-1, 1));
		++it;
		REQUIRE(*it == std::pair<const int, int>(100, 2));
		++it;
		REQUIRE(*it == std::pair<const int, int>(1000, 3));

		m[1000] = 10;
		REQUIRE(m.size() == 3);
		REQUIRE(it->second == 10);
	}

	SECTION("size") {

		REQUIRE(m.size() == 0);			// Quand la map est vide, sa taille est zéro

		m.insert(std::make_pair(0, 10));
		REQUIRE(m.size() == 1);
		m.insert(std::make_pair(2, 20));
		REQUIRE(m.size() == 2);
		m.insert(std::make_pair(4, 30));
		REQUIRE(m.size() == 3);
		m.insert(std::make_pair(0, 10));
		REQUIRE(m.size() == 3);
	}

	SECTION("empty") {

		REQUIRE(m.empty());				// Après le constructeur par défaut, la map doit être vide

		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 20));
		m.insert(std::make_pair(4, 30));
		REQUIRE(!m.empty());
	}

	SECTION("clear") {

		m.clear(); // Il ne doit rien se passer après avoir vidé une map vide
		REQUIRE(m.empty());

		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 20));
		m.insert(std::make_pair(4, 30));
		m.clear();
		REQUIRE(m.empty());

	}

	SECTION("find") {

		REQUIRE(m.find(3) == m.end());  // Quand la map est vide, la recherche de n'importe quel valeur doit retourner l'itérateur de fin

		m[0] = 10;
		m[100] = -3;

		iterator it = m.find(0);
		REQUIRE(it != m.end());
		REQUIRE(*it == value_type(0, 10));

		for(int i = 20; i < 105; i += 5) {
			it = m.find(i);
			if(i == 100) {
				REQUIRE(it != m.end());
				REQUIRE(*it == value_type(100, -3));
			}
			else {
				REQUIRE(it == m.end());
			}
		}
	}

	SECTION("lower_bound") {

		REQUIRE(m.lower_bound(2) == m.end());

		m.insert(std::make_pair(0, 10));
		m.insert(std::make_pair(2, 20));
		m.insert(std::make_pair(4, 30));

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

	SECTION("at") {

		REQUIRE_THROWS_AS(m.at(0), std::out_of_range);
	}

	SECTION("Une map est toujours triée") {

		{
			m.insert(std::make_pair(0, 10));
			m.insert(std::make_pair(2, 20));
			m.insert(std::make_pair(4, 30));
			auto it = m.begin();

			REQUIRE(*it == std::pair<const int, int>(0, 10));
			++it;
			REQUIRE(*it == std::pair<const int, int>(2, 20));
			++it;
			REQUIRE(*it == std::pair<const int, int>(4, 30));
		}

		{
			m.clear();
			m.insert(std::make_pair(2, 20));
			m.insert(std::make_pair(0, 10));
			m.insert(std::make_pair(4, 30));
			auto it = m.begin();

			REQUIRE(*it == std::pair<const int, int>(0, 10));
			++it;
			REQUIRE(*it == std::pair<const int, int>(2, 20));
			++it;
			REQUIRE(*it == std::pair<const int, int>(4, 30));
		}


		{
			m.clear();
			m.insert(std::make_pair(2, 20));
			m.insert(std::make_pair(0, 10));
			m.insert(std::make_pair(4, 30));

			m.erase(2);
			auto it = m.begin();

			REQUIRE(*it == std::pair<const int, int>(0, 10));
			++it;
			REQUIRE(*it == std::pair<const int, int>(4, 30));
		}
	}
}
