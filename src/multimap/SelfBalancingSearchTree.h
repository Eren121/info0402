#ifndef SELFBALANCINGSEARCHTREE_H
#define SELFBALANCINGSEARCHTREE_H

#include "AVL_Tree.h"
#include "Liste.h"
#include "Pile.h"

// Structures de données choisie pour la multimap.
// Lightweight C++ wrapper for C structures

namespace structures {

	typedef AVL_TREE		BSTree;
	typedef AVL_NODE		BSTreeNode;
	typedef LISTE			List;
	typedef LISTE_ELEMENT	ListNode;
	typedef PILE			Stack;
	typedef void			UserData;

	struct list {
		List* create();								// Créé une liste
		void destroy(List*);						// Détruit une liste (mais pas les valeurs)
		ListNode* first(List*);						// Premier élément d'une liste
		ListNode* last(List*);						// Dernier élément d'une liste
		ListNode* next(ListNode*);					// Prochain élément (retourne NULL si pas de suivant)
		ListNode* previous(ListNode*);				// Précédent élément (retourne NULL si pas de précédent)
		void erase(List*, ListNode*);				// Détruit un noeud
		void push(List*, UserData*);				// Ajoute un élément à la fin de la liste
		void insert(List*, UserData*, ListNode*);	// Insère un élément avant
		UserData* data(ListNode*);
	};

	struct stack {
		Stack* create();							// Créé une pile
		void destroy(Stack*);						// Détruit une pile (mais pas les valeurs)
		void push(Stack*, UserData*);				// Empile
		UserData* pop(Stack*);						// Dépile
		bool empty(Stack*);
	};

	struct bst {
		BSTree* create();							// Créer un arbre
		void destroy(BSTree*);						// Détruit une arbre (mais pas les valeurs)



		UserData data(BSTreeNode*);
		BSTreeNode* left(BSTreeNode*);
		BSTreeNode* right(BSTreeNode*);
	};
}

#endif // SELFBALANCINGSEARCHTREE_H
