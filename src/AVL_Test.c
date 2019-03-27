#include "AVL_Tree.h"
#include <time.h>
#include <stdlib.h>

int main()
{
	AVL_TREE* tree = CreateAVL();

	srand(time(0));

	// arbre non équilibré: cas 1:

	InsertAVL(tree, 2);
	InsertAVL(tree, 3);
	InsertAVL(tree, 1);
	InsertAVL(tree, 0);
	InsertAVL(tree, -1);

	RightRotation(tree->root->left);

	AfficherAVL(tree);
	DestroyAVL(tree);
	return 0;
}
