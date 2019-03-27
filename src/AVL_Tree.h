#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct AVL_Node
{
	struct AVL_Node* parent;
	struct AVL_Node* left;
	struct AVL_Node* right;
	int h;
	void* value;
} AVL_NODE;

typedef int(*AVL_COMPARAISON)(void* left, void* right);
typedef void(*AVL_OPERATION)(AVL_NODE* node);

typedef struct AVL_Tree
{
	struct AVL_Node* root;
	AVL_COMPARAISON compare;
} AVL_TREE;

int CompareDefaultAVL(void* left, void* right);

AVL_TREE* CreateAVL();

void PostfixAVL(AVL_NODE* t, AVL_OPERATION operation);

void PrefixAVL(AVL_NODE* t, AVL_OPERATION operation);

void InfixAVL(AVL_NODE* t, AVL_OPERATION operation);

void DestroyNodeAVL(AVL_NODE* node);

void DestroyAVL(AVL_TREE* t);

void VerifierEquilibreAVL(AVL_NODE* inserted);

int InsertAVL(AVL_TREE* arbre, void* valeur);

int RemoveAVL(AVL_TREE* arbre, void* valeur);

void AfficherAVL(AVL_TREE* arbre);


// Rotations
void RightRotation(AVL_NODE* t);
void LeftRotation(AVL_NODE* t);

#endif // AVL_TREE_H
