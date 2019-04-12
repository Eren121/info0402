#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "SelfBalancingSearchTree.h"

template<typename Key, typename Value>
class multimap {
public:
	multimap();

private:
	// Type de donné abstrait vers les structures à utiliser
	using BalancedTree = structures::BalancedTree;
	using BalancedTreeNode = structures::BalancedTreeNode;

	// Arbre dont chaque noeud est une liste de pointeurs vers value_type
	BalancedTree* tree;
};

#include "multimap_constructors.h"

#endif // MULTIMAP_H
