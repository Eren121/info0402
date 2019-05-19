#include "counter/Counter.h"
#include "catch.hpp"
#include "multimap/Multimap.h"

/// Test des fuites mémoires

void buildMemory() {

	Multimap<int, Counter> multimap;

	for(int i = 0; i < 100; i++) {
		multimap.insert({i, Counter()});
	}

	for(int i = 0; i < 10; i++) {
		multimap.erase(i);
	}
}

TEST_CASE("Le compteur doit être égal à zéro après la destruction de la map", "[bstree]") {

	Counter::resetCount();

	buildMemory();
	REQUIRE(Counter::check());
}
