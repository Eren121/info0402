#include "AVL_Tree.h"
#include "File.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 *      Fonction de comparaison pour l'insertion d'une valeur à comparer aux valeurs déjà existantes.
 *      Utilisé car les noeuds d'un arbre AVL sont triées.
 *      Chaque enfant gauche est inférieur à son parent, et chaque enfant droit est supérieur à son parent
 *
 *      Retourne 1 si "right" est supérieur à "left"
 *               0 si "left" est supérieur à "right"
 */
typedef int(*AVL_COMPARAISON)(void* left, void* right);

/**
 *      Opération sur un noeud
 *      Utilisé comme fonction de rappel aux parcours postfixe, préfixe et infixe.
 */
typedef void(*AVL_OPERATION)(AVL_NODE* node);

/**
 *      Fonction de comparaison des valeurs de l'arbre par défaut
 */
int CompareDefaultAVL(void* left, void* right)
{
	return (intptr_t)left < (intptr_t)right;
}

/**
 *      Créé un arbre AVL vide.
 *      Initialise la fonction de comparaison avec un pointeur de fonction sur "compare_int".
 */
AVL_TREE* CreateAVL()
{
	AVL_TREE* t = malloc(sizeof(AVL_TREE));
    t->root = NULL;
	t->compare = &CompareDefaultAVL;

    return t;
}

/**
 *      Traitement d'une operation postfixe, c'est-à-dire que l'on écrit ou traite un noeud après avoir traité ses fils.
 *      Appel récursif.
 *      L'opération est effectué dans l'ordre: gauche -> droite -> racine.
**/
void PostfixAVL(AVL_NODE* t, AVL_OPERATION operation)
{
    if(t == NULL)
    {
        printf("Erreur (PostfixAVL): noeud non alloué");
        return;
    }

    if(t->left != NULL)
    {
        PostfixAVL(t->left, operation);
    }
    if(t->right != NULL)
    {
        PostfixAVL(t->right, operation);
    }

    operation(t);
}


/**
 *      Traitement d'une operation préfixe, c'est-à-dire que l'on traite les fils après avoir traité le noeud.
 *      Appel récursif.
 *      L'opération est effectué dans l'ordre: racine -> gauche -> droite
**/
void PrefixAVL(AVL_NODE* t, AVL_OPERATION operation)
{
    if(t == NULL)
    {
        printf("Erreur (PrefixAVL): noeud non alloué");
        return;
    }

    operation(t);

    if(t->left != NULL)
    {
        PostfixAVL(t->left, operation);
    }
    if(t->right != NULL)
    {
        PostfixAVL(t->right, operation);
    }
}

/**
 *      Traitement d'une operation infixe, c'est-à-dire que l'on traite d'abord le fils gauche, puis le noeud courant, puis le fils droit.
 *      Appel récursif.
 *      L'opération est effectué dans l'ordre: gauche -> racine -> droite
**/
void InfixAVL(AVL_NODE* t, AVL_OPERATION operation)
{
	if(t == NULL)
	{
		printf("Erreur (PrefixAVL): noeud non alloué");
		return;
	}

	if(t->left != NULL)
	{
		PostfixAVL(t->left, operation);
	}

	operation(t);

	if(t->right != NULL)
	{
		PostfixAVL(t->right, operation);
	}
}

/**
 *      Désalloue un noeud de l'arbre et ses descendants.
 *      Chaque descendant du noeud est aussi désalloué.
 *      Utilisé pour désallouer l'arbre AVL.
 *
 *      Récursif, GDR.
 */
void DestroyNodeAVL(AVL_NODE* node)
{
    if(node == NULL)
    {
        printf("Erreur (DestroyNodeAVL): noeud non alloué\n");
        return;
    }

    free(node);
}

/**
 *      Désalloue l'arbre entier
 */
void DestroyAVL(AVL_TREE* t)
{
    if(t == NULL)
    {
        printf("Erreur (DestroyAVL): arbre non alloué");
        return;
    }

    if(t->root != NULL)
    {
		PostfixAVL(t->root, &DestroyNodeAVL);
    }

    free(t);
}

/**
 *      Vérifie si l'arbre est équilibré après insertion.
 *      Ajoute 1 à la hauteur de chaque ascendant
 *      Si la différence de hauteur entre l'enfant gauche et droite est supérieure à 1, l'arbre n'est pas équilibré.
 *
 *
 *               Racine        |
 *                / \          |
 *               /   \         |
 *        Gauche      Droite   | Hauteur droite de la racine
 *           .          .      |
 *                      .      |
 */
void VerifierEquilibreAVL(AVL_NODE* t)
{
	AVL_NODE* node = NULL;
	int hg  = 0;
	int hd = 0;
	int bal;

    if(t == NULL)
    {
        printf("Erreur (VerifierEquilibreAVL): noeud non alloué");
		return;
	}

	node = t->parent;

	while(node != NULL)
	{
		if(node->left != NULL)
		{
			hg  = 1 + node->left->h;
		}
		else
		{
			hg = 0;
		}

		if(node->right != NULL)
		{
			hd = 1 + node->right->h;
		}
		else
		{
			hd = 0;
		}

		bal = hd - hg;
		node->h = bal > 0 ? hd : hg;

		if(bal == -2 || bal == 2)
		{
			printf("Arbre non-équilibré\n");
			// Arbre non équilibré
		}

		node = node->parent;
	}
}

/**
 *      Ajoute une valeur dans l'arbre. La valeur est insérée de façon trié.
 *      Retourne 1 si la valeur n'existait pas et a été inséré
 *               0 si la valeur existait déjà ou qu'il y a eu une erreur (arbre nul)
 */
int InsertAVL(AVL_TREE* arbre, void* valeur)
{
	// Pointeur vers un pointeur de noeud
	// On modifie directement le pointeur du parent
	AVL_NODE** node = NULL;

	// Parent du noeud inséré
	AVL_NODE* parent = NULL;

	// Niveau du noeud que l'on ajoute
	int niv = 0;

	if(arbre == NULL)
	{
		printf("Erreur (InsertAVL): arbre non alloué\n");
		return 0;
	}

	node = &arbre->root;

	while(*node != NULL)
	{
		parent = *node;
		niv++;

		if(arbre->compare(valeur, parent->value)) // Si valeur < node
		{
			node = &parent->left;
		}
		else if(arbre->compare(parent->value, valeur)) // Si valeur > node
		{
			node = &parent->right;
		}
		else
		{
			// Sinon la valeur existe déjà dans l'arbre
			printf("Erreur: la valeur existe déjà dans l'arbre (InsertAVL)\n");
			return 0;
		}
	}

	*node = malloc(sizeof(AVL_NODE));
	(*node)->parent = parent;
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->value = valeur;
	(*node)->h = 0;

	VerifierEquilibreAVL(*node);

	// Actualiser la hauteur de l'arbre (= hauteur de la racine) si nécessaire
	if(arbre->root->h < niv)
	{
		arbre->root->h = niv;
	}

	return 1;
}

/**
 *      Supprime une valeur de l'arbre.
 *      Retourne: 1 si l'arbre contenait la valeur et elle a été supprimée
 *                0 si l'arbre ne contenait pas la valeur.
 */
int RemoveAVL(AVL_TREE* arbre, void* valeur);

/**
 *		Affiche un arbre sur plusieurs lignes en arborescence
 *		On considère que les valeurs pointées sont de type int
 *		Parcours en largeur
 *
 *			     A
 *			B        C
 *       D    E  NULL   G
 */

#define LENGTH 100 // Largeur d'une ligne
#define N_ELEMENT 10 // Nombre de caractères à afficher par élément

void AfficherAVL(AVL_TREE* arbre)
{
	AVL_NODE* eol = malloc(sizeof(AVL_NODE)); // Noeud spécial pour signifier la fin d'une ligne

	AVL_NODE* node = NULL;
	FIFO* file = CreerFile();
	int n = 1; // Nombre d'éléments à afficher sur la profondeur actuelle.
			   // Multiple de 2
			   // Pour la racine: n=1. Pour la profondeur 1, n=2, etc...
	int larg = LENGTH; // Largeur que doit occuper chaque noeud à la ligne n
	int niv = 0; // Niveau du noeud actuel
	int h = 0; // Hauteur de l'arbre
	char str[N_ELEMENT];

	if(arbre == NULL)
	{
		printf("Erreur (AfficherAVL): arbre non alloué");
	}

	if(arbre->root != NULL)
	{
		Enfiler(file, arbre->root);
		h = arbre->root->h;
	}

	Enfiler(file, eol);

	while(!FileEstVide(file))
	{
		node = Defiler(file);

		if(node != eol)
		{
			if(node != NULL)
			{
				if(node->left != NULL || niv < h)
				{
					Enfiler(file, node->left);
				}
				if(node->right != NULL || niv < h)
				{
					Enfiler(file, node->right);
				}

				sprintf(str, "%d(%d)", (int)node->value, node->h);
			}
			else
			{
				if(niv < h)
				{
					Enfiler(file, NULL);
					Enfiler(file, NULL);
				}

				str[0] = 'N';
				str[1] = 'I';
				str[2] = 'L';
				str[3] = '\0';
			}

			printf("%*s%*s", larg / 2 + (N_ELEMENT / 2), str, larg / 2 - (N_ELEMENT / 2), "");
		}
		else
		{
			// Fin de ligne

			// Toute la ligne suivant a été empilée, on rajoute un retour à la ligne
			// Si la file est vide, c'est qu'on a atteint la fin de l'arbre. Sinon, on empile un nouveau retour à la ligne.

			if(!FileEstVide(file))
			{
				Enfiler(file, eol);
				niv++;
				n *= 2;
				larg = LENGTH / n;
			}

			printf("\n");
		}
	}

	free(eol);
	DetruireFile(file);
}

void LeftRotation(AVL_NODE *t)
{

}

void RightRotation(AVL_NODE *a)
{
	AVL_NODE* b = a->left;

	a->left = b->right;

	if(a->left != NULL)
	{
		a->left->parent = a;
	}

	b->right = a;

	if(b->right != NULL)
	{
		b->right->parent = b;
	}

	a->parent = b;
}
