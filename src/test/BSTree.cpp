#include "multimap/BSTree.h"
#include <iostream>
#include "catch.hpp"

BSTree<int> buildBSTree() {

	BSTree<int> bstree;
	BSTreeNode<int>** node = bstree.root(20);
	(*node)->insert_left(10);
	(*node)->insert_right(30)->insert_right(40);

	//		20
	//  10	   30
	//            40

	return bstree;
}

TEST_CASE("Hauteur", "BSTree") {

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

TEST_CASE("Insertion", "BSTree") {

	BSTree<int> bstree = buildBSTree();
	REQUIRE(bstree.equals({10, 20, 30, 40}));
}

TEST_CASE("Suppression", "BSTree") {

	using std::cout;
	using std::endl;

	BSTree<int> bstree = buildBSTree();

	bstree.erase(*bstree);
	REQUIRE(bstree.equals({10, 30, 40}));

	bstree.erase(bstree->left());
	REQUIRE(bstree.equals({30, 40}));

	bstree.erase(bstree->right());
	REQUIRE(bstree.equals({30}));

	bstree.erase(*bstree);
	REQUIRE(bstree.equals({}));
}
