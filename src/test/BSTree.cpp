#include "multimap/BSTree.h"
#include <iostream>
#include "catch.hpp"

BSTree<int> buildBSTree() {

	BSTree<int> bstree;
	auto* node = *bstree.create_root(20);
	node->insert_left(10);
	node->insert_right(30)->insert_right(40);

	//		20
	//  10	   30
	//            40

	return bstree;
}

TEST_CASE("Hauteur d'un BSTree", "[bstree]") {

	BSTree<int> bstree;
	int requiredHeight = -1;

	SECTION("Cas 1: arbre vide") {
		requiredHeight = 0;
	}

	SECTION("Cas 2: arbre non-vide") {
		requiredHeight = 2;
		bstree = buildBSTree();
	}

	REQUIRE(bstree.height() == requiredHeight);
}

TEST_CASE("Insertion dans un BSTree", "[bstree]") {

	BSTree<int> bstree = buildBSTree();
	REQUIRE(bstree.equals({10, 20, 30, 40}));
}

TEST_CASE("Suppression dans un BSTree", "[bstree]") {

	BSTree<int> bstree;
	bstree.create_root(20);

	SECTION("Pas de fils") {

		bstree.erase(bstree.root());
		REQUIRE(bstree == std::initializer_list<int>{});
	}

	SECTION("Fils gauche uniquement") {

		bstree.root()->insert_left(10);
		bstree.erase(bstree.root());
		REQUIRE(bstree == std::initializer_list<int>{10});
	}

	SECTION("Fils droit uniquement") {

		bstree.root()->insert_right(30);
		bstree.erase(bstree.root());
		REQUIRE(bstree == std::initializer_list<int>{30});
	}

	SECTION("Deux fils") {

		bstree.root()->insert_left(10);
		bstree.root()->insert_right(30);

		SECTION("File droit sans fils") {

			bstree.erase(bstree.root());
			REQUIRE(bstree == std::initializer_list<int>{10, 30});
		}

		SECTION("Fils droit avec fils droit") {

			bstree.root()->right()->insert_right(40);
			bstree.erase(bstree.root());
			REQUIRE(bstree == std::initializer_list<int>{10, 30, 40});
		}

		SECTION("Fils droit avec fils gauche") {

			bstree.root()->right()->insert_left(25);
			bstree.erase(bstree.root());
			REQUIRE(bstree == std::initializer_list<int>{10, 25, 30});
		}
	}
}
