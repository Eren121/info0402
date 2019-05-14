#ifndef BSTREE_H
#define BSTREE_H

#include "List.h"
#include "AnsiColor.h"
#include <ostream>
#include <sstream>
#include <utility>

/// Prédéclarations
/// --------------

// Lightweight wrapper for binary search tree

template<typename T>
class BinaryNode;

/// Arbre contenant des noeuds génériques
template<typename T, typename Node = BinaryNode<T>>
class BSTree;

template<typename T, typename Node>
std::ostream& operator<<(std::ostream& lhs, const BSTree<T, Node>& bstree);

/// Noeud binaire basique
/// ---------------------

template<typename T, typename Node>
class BSTreeNode {
public:

    std::size_t size() const {
		std::size_t res = 1;
        if(m_left) res += m_left->size();
        if(m_right) res += m_right->size();
        return res;
	}

    /// Retourne vrai si le noeud est la racine, faux sinon
    bool is_root() const { return m_parent == nullptr; }

    /// Retourne vrai si le noeud est une feuille, faux sinon
    bool is_leaf() const { return m_left == nullptr && m_right == nullptr; }

    /// Retourne vrai si le noeud a un fils gauche
    bool has_left() const { return m_left != nullptr; }

    /// Retourne vrai si le noeud a un fils droit
    bool has_right() const { return m_right != nullptr; }

    /// Récupère la donnée (constant)
	const T& data() const {

		ASSERT(m_data);
		return *m_data;
	}

    /// Récupère la donnée (non-constant)
	T& data() {

		ASSERT(m_data);
		return *m_data;
	}

    /// Récupère le parent du noeud courant (constant)
    const Node* parent() const {
        ASSERT(m_parent != nullptr);
        return m_parent;
    }

    /// Récupère le parent du noeud courant (non-constant)
    Node* parent() {
        ASSERT(m_parent != nullptr);
        return m_parent;
    }

    /// Récupère le fils gauche (constant)
    const Node* left() const {
        ASSERT(m_left != nullptr);
        return m_left;
    }

    /// Récupère le fils gauche (non-constant)
    Node* left() {
        ASSERT(m_left != nullptr);
        return m_left;
    }

    /// Récupère le fils droit (constant)
    const Node* right() const {
        ASSERT(m_right != nullptr);
        return m_right;
    }

    /// Récupère le fils droit (non-constant)
    Node* right() {
        ASSERT(m_right != nullptr);
        return m_right;
    }

    /// Hauteur du noeud
	std::size_t height() const {
		std::size_t hl = 0;
		std::size_t hr = 0;

        if(m_left != nullptr) {
            hl = 1 + m_left->height();
        }
        if(m_right != nullptr) {
            hr = 1 + m_right->height();
        }

        if(hl > hr) return hl;
        else return hr;
    }

    /// Récupère le noeud fils le plus grand = le plus à droite
    /// Retourne lui-même s'il n'a pas de fils
    Node* max() {
        Node* tmp = static_cast<Node*>(this);
        while(tmp->m_right != nullptr) tmp = tmp->m_right;
        return tmp;
    }

    /// Récupère le noeud fils le plus petit = le plus à gauche
    /// Retourne lui-même s'il n'a pas de fils
    Node* min() {
        Node* tmp = static_cast<Node*>(this);
        while(tmp->m_left != nullptr) tmp = tmp->m_left;
        return tmp;
    }

    /// Récupère le noeud suivant (ou NULL si c'est le dernier noeud)
    Node* next() {

        Node *tmp = static_cast<Node*>(this);
        if(m_right != nullptr) {
            return m_right->min();
        }
        while(tmp != nullptr) {
            if(tmp->m_parent && tmp->m_parent->m_left == tmp) {
                tmp = tmp->m_parent;
                break;
            }
            tmp = tmp->m_parent;
        }
        return tmp;
    }

    const Node* next() const {
        return const_cast<BSTreeNode*>(this)->next();
    }

    /// Récupère le noeud précédent (ou NULL si c'est le premier noeud)
    Node* prev() {

		Node *tmp = static_cast<Node*>(this);
        if(m_left != nullptr) {
            return m_left->max();
        }
        while(tmp != nullptr) {
            if(tmp->m_parent && tmp->m_parent->m_right == tmp) {
                tmp = tmp->m_parent;
                break;
            }
            tmp = tmp->m_parent;
        }
        return tmp;
    }

    const Node* prev() const {
        return const_cast<BSTreeNode*>(this)->prev();
    }

    /// Insère un noeud à gauche
    template<typename U>
    Node* insert_left(U&& t) {
        ASSERT(m_left == nullptr);
        m_left = new Node(std::forward<U>(t), static_cast<Node*>(this));
        return m_left;
    }

    /// Insère un noeud à droite
    template<typename U>
    Node* insert_right(U&& t) {
        ASSERT(m_right == nullptr);
        m_right = new Node(std::forward<U>(t), static_cast<Node*>(this));
        return m_right;
    }

protected:
    template<typename U>
	BSTreeNode(U&& t, Node* parentNode = nullptr) :
        m_data(new T(std::forward<U>(t))),
        m_parent(parentNode),
        m_left(nullptr),
        m_right(nullptr) {}

	virtual ~BSTreeNode() {
        delete m_data;
		m_data = nullptr;
		m_parent = nullptr;
		m_left = nullptr;
		m_right = nullptr;
	}

    T* m_data;
    Node* m_parent;
    Node* m_left;
    Node* m_right;

    friend class BSTree<T, Node>;
    friend std::ostream& operator<< <T, Node>(std::ostream& lhs, const BSTree<T, Node>& bstree);

	void deepEraseChildren() {

		if(m_left) {

			m_left->deepEraseChildren();
			delete m_left;
			m_left = nullptr;
		}

		if(m_right) {

			m_right->deepEraseChildren();
			delete m_right;
			m_right = nullptr;
		}
	}

	void replaceChild(Node* oldChild, Node* newChild) {

		if(m_left == oldChild) {
			m_left = newChild;
		}
		else if(m_right == oldChild) {
			m_right = newChild;
		}
		else {
			ASSERT_THROW("oldChild ne correspond à aucun des fils du noeud");
		}
	}
};

template<typename T>
class BinaryNode : public BSTreeNode<T, BinaryNode<T>> {

    using BSTreeNode<T, BinaryNode<T>>::BSTreeNode;
};

/**
 * Arbre binaire
 * @tparam T type stocké
 * @tparam N type du noeud
 */
template<typename T, typename N>
class BSTree {
public:
	/**
	 * Type du noeud
	 */
	using Node = N;

	/**
	 * Constructeur par défaut
	 * Créer un arbre vide
	 */
	BSTree() :
		m_root(nullptr)
	{}

	/**
	 * Constructeur par copie
	 */
    BSTree(const BSTree&) = delete;

	/**
	 * Constructeur par déplacement
	 */
	BSTree(BSTree&& o) :
		m_root(o.m_root) {

		o.m_root = nullptr;
	}

	/**
	 * Destructeur
	 */
    ~BSTree() {

		if(m_root) {

			m_root->deepEraseChildren();
			delete m_root;
			m_root = nullptr;
		}
    }

	/**
	 * Assignation par copie
	 */
    BSTree& operator=(const BSTree&) = delete;

	/**
	 * Assignation par déplacement
	 */
    BSTree& operator=(BSTree&& o) {

        Node* tmp = m_root;

        m_root = o.m_root;
        o.m_root = tmp;

		return *this;
    }

	/**
	 * Récupère la taille de l'arbre (récursif)
	 *
	 * @return le nombre de noeuds de l'arbre
	 */
    std::size_t size() const {

		if(m_root) {

			return m_root->size();
		}
		else {

			return 0;
		}
    }

	/**
	 * Tester si l'arbre est vide (c'est-à-dire s'il contient 0 noeuds).
	 *
	 * @return true si l'arbre est vide, false sinon
	 */
	bool empty() const {

		return m_root == nullptr;
	}

	/**
	 * Récupère la hauteur de l'arbre (récursif)
	 *
	 * @return la hauteur de l'arbre. 0 si l'arbre est vide
	 */
	std::size_t height() const {

		if(m_root) {

			return m_root->height();
		}
		else {

			return 0;
		}
	}

	/**
	 * Récupération des données (non constant)
	 */
	Node& operator*() {

		ASSERT(m_root);
		return *m_root;
	}

	/**
	 * Récupération des données (constant)
	 */
	const Node& operator*() const {

		ASSERT(m_root);
		return *m_root;
	}

	/**
	 * Récupération des données pointées (non constant)
	 */
	Node* operator->() {

		ASSERT(m_root);
		return m_root;
	}

	/**
	 * Récupération des données pointées (constant)
	 */
	const Node* operator->() const {

		ASSERT(m_root);
		return m_root;
	}

	/**
	 * Créer une racine par référence universelle (copie ou déplacement). L'arbre doit être vide.
	 * @param value La valeur de la racine
	 */
    template<typename U>
    Node** create_root(U&& value) {

		ASSERT(!m_root);

		m_root = new Node(std::forward<U>(value));
        return &m_root;
    }

	/**
	 * Récupère la racine de l'arbre (constant)
	 * L'arbre ne doit pas être vide
	 */
    const Node* root() const {

		ASSERT(m_root);
        return m_root;
    }

	/**
	 * Récupère la racine de l'arbre (non constant)
	 * L'arbre ne doit pas être vide
	 */
    Node* root() {

		ASSERT(m_root);
		return m_root;
    }

private:

	/**
	 * Retire un noeud qui possède 0 ou 1 enfant de l'arbre, sans le supprimer
	 * Le noeud en lui-même n'est pas modifié mais seulement les éléments voisins.
	 *
	 * L'arbre ne doit pas être vide
	 */
	void detach(Node* node) {

		ASSERT(m_root);

		Node* tmp = nullptr;

		// Vérifier que le noeud ne possède bien qu'1 ou 0 fils

		ASSERT(!node->m_left || !node->m_right);

		// S'il n'a qu'un seul fils,
		// on le remplace par son fils
		// Et on le supprime

		if(node->m_left || node->m_right) {

			tmp = node->m_left;
			if(!tmp) tmp = node->m_right;

			tmp->m_parent = node->m_parent;

			// S'il a un parent, actualiser le fils du parent

			if(tmp->m_parent) {

				tmp->m_parent->replaceChild(node, tmp);
			}

			// Sinon, si c'est la racine, actualiser la racine

			else {

				m_root = tmp;
			}
		}

		// Sinon, s'il n'a ni fils gauche ni fils droit,
		// C'est une feuille
		// Par conséquent on peut le supprimer directement

		else {

			// S'il a un parent, actualiser le fils du parent

			if(node->m_parent) {

				node->m_parent->replaceChild(node, nullptr);
			}

			// Si c'est la racine, actualiser la racine
			// L'arbre est vide

			else {

				m_root = nullptr;
			}
		}
	}

public:

	/**
	 * Supprime un noeud de l'arbre
	 * L'arbre ne doit pas être vide
	 *
	 * @param node le noeud à supprimer
	 */
    void erase(Node* node) {

		ASSERT(node && m_root);
		Node* tmp = nullptr;

		// S'il a un fils gauche et un fils droit,
		// On l'échange avec son plus proche successeur (il n'a plus qu'au maximum un seul fils)
		// Et on le supprime

		if(node->m_left && node->m_right) {

			tmp = node->next();
			detach(tmp);

			/// Insérer le noeud

			// Fils gauche

			tmp->m_left = node->m_left;
			if(tmp->m_left) {

				ASSERT(tmp->m_left->m_parent == node);
				tmp->m_left->m_parent = tmp;
			}

			// Fils droit

			tmp->m_right = node->m_right;
			if(tmp->m_right) {

				ASSERT(tmp->m_right->m_parent == node);
				tmp->m_right->m_parent = tmp;
			}

			// Parent

			tmp->m_parent = node->m_parent;
			if(tmp->m_parent) {

				tmp->m_parent->replaceChild(node, tmp);
			}

			// Si c'est la racine que l'on supprime, actualiser au prochain élément

			if(node == m_root) {

				m_root = tmp;
			}
		}
		else {

			detach(node);
		}

		delete node;
	}

	/**
	 * Récupérer l'élément le plus petit de l'arbre (non constant)
	 * L'arbre ne doit pas être vide
	 */
    Node* front() {

		ASSERT(m_root);
		return m_root->min();
	}

	/**
	 * Récupérer l'élément le plus petit de l'arbre (constant)
	 * L'arbre ne doit pas être vide
	 */
    const Node* front() const {

		ASSERT(m_root);
		return m_root->min();
	}

	/**
	 * Récupérer l'élément le plus grand de l'arbre (non constant)
	 * L'arbre ne doit pas être vide
	 */
	Node* back() {

		ASSERT(m_root);
		return m_root->max();
	}

	/**
	 * Récupérer l'élément le plus petit de l'arbre (constant)
	 * L'arbre ne doit pas être vide
	 */
    const Node* back() const {

		ASSERT(m_root);
		return m_root->max();
	}

	/**
	 * Comparaison avec une liste d'initialisation.
	 * Fournit une méthode equals() car on ne peut pas comparer directement une liste d'initialisation littérale avec l'opérateur ==.
	 *
	 * @param initl la liste d'initialisation à comparer
	 *
	 * @return true si la liste d'initialisation correspond à l'arbre dans l'ordre infixe, false sinon.
	 **/
	bool equals(const std::initializer_list<T>& initl) const {
		return *this == initl;
	}

	/**
	 * Comparaison avec une liste d'initialisation
	 *
	 * @param initl la liste d'initialisation à comparer
	 *
	 * @return false si la liste d'initialisation correspond à l'arbre dans l'ordre infixe, true sinon.
	 **/
	bool operator!=(const std::initializer_list<T>& initl) const {
		return !(*this != initl);
	}

	/**
	 * Comparaison avec une liste d'initialisation.
	 *
	 * @param initl la liste d'initialisation à comparer
	 *
	 * @return true si la liste d'initialisation correspond à l'arbre dans l'ordre infixe, false sinon.
	 **/
	bool operator==(const std::initializer_list<T>& initl) const {

		const Node* tmp = (!empty() ? front() : nullptr);

        for(const T& val : initl) {

            if(!tmp || tmp->data() != val) {
                return false;
            }

            tmp = tmp->next();
        }

        return !tmp;
    }

	/**
	 * Vider entièrement l'arbre.
	 * Si l'arbre était déjà vide, ne fait rien.
	 */
    void clear() {

		if(m_root) {

			m_root->deepEraseChildren();
			delete m_root;
			m_root = nullptr;
		}
    }

private:
	/**
	 * Pointeur vers l'élément racine
	 */
    Node* m_root;

	/**
	 * Affichage de l'arbre dans un flux (en infixé, ou sous forme d'arbre)
	 */
    friend std::ostream& operator<< <T, Node>(std::ostream& lhs, const BSTree<T, Node>& bstree);
};

#define LENGTH 128 // Largeur d'une ligne
#define N_ELEMENT 16 // Nombre de caractères à afficher par élément

template<typename T, typename Node>
std::ostream& operator<<(std::ostream& lhs, const BSTree<T, Node>& bstree) {

    lhs << "\n\n\033[4mArbre :\033[0m\n\n";

    constexpr int length = 128;
    char str[100];
    char out[length];
    int strl;

    Node* eol = reinterpret_cast<Node*>(::operator new(sizeof(Node))); // Noeud spécial pour signifier la fin d'une ligne
    Node* node = nullptr;
    List<Node*> file;
    // Nombre d'éléments à afficher sur la profondeur actuelle.
    // Multiple de 2
    // Pour la racine: n=1. Pour la profondeur 1, n=2, etc...
    int n = 1;

    // Largeur que doit occuper chaque noeud à la ligne n
    int larg = length;

    // Niveau du noeud actuel
    int niv = 0;

    // Profondeur de l'arbre
    int h = 0;

	if(!bstree.empty())
    {
        file.push_back(bstree.m_root);
        h = bstree.m_root->height();
        file.push_back(eol);

        while(!file.empty())
        {
            node = file.front()->data();
            file.erase(file.front());

            if(node != eol)
            {
                if(node != NULL)
                {
                    if(node->m_left != NULL || niv < h)
                    {
                        file.push_back(node->m_left);
                    }
                    if(node->m_right != NULL || niv < h)
                    {
                        file.push_back(node->m_right);
                    }


                    std::ostringstream stream;
                    stream << *node->m_data;

                    sprintf(str, BOLD("%s"), stream.str().c_str());
                }
                else
                {
                    if(niv < h)
                    {
                        file.push_back(nullptr);
                        file.push_back(nullptr);
                    }

                    str[0] = '*';
                    str[1] = '\0';
                }

                strl = strlenAnsi(str);

                if(niv == 0) {

                    sprintf(out, BG(41, "%*s"), (larg+1) / 2 - ((strl+1) / 2), " ");
                    lhs << out;
                    sprintf(out, BG(41, "%s"), str);
                    lhs << out;
                    sprintf(out, BG(41, "%*s"), larg / 2 - (strl / 2), "");
                    lhs << out;
                }
                else if(niv % 2 == 0) {

                    sprintf(out, BG(100, "%*s"), (larg+1) / 2 - ((strl+1) / 2), " ");
                    lhs << out;
                    sprintf(out, BG(100, "%s"), str);
                    lhs << out;
                    sprintf(out, BG(100, "%*s"), larg / 2 - (strl / 2), "");
                    lhs << out;
                }
                else {

                    sprintf(out, "%*s", (larg+1) / 2 - ((strl+1) / 2), " ");
                    lhs << out;
                    sprintf(out, "%s", str);
                    lhs << out;
                    sprintf(out, "%*s", larg / 2 - (strl / 2), "");
                    lhs << out;
                }
            }
            else
            {
                // Fin de ligne

                // Toute la ligne suivant a été empilée, on rajoute un retour à la ligne
                // Si la file est vide, c'est qu'on a atteint la fin de l'arbre. Sinon, on empile un nouveau retour à la ligne.

                if(!file.empty())
                {
                    file.push_back(eol);
                    niv++;
                    n *= 2;
                    larg = length / n;
                }

                printf("\n");
            }
        }
    }
    else {

        sprintf(str, "(vide)");
        strl = strlenAnsi(str);
        sprintf(out, BG(41, "%*s%*s"), (larg+1) / 2 - ((strl+1) / 2), str, (larg+1) / 2 - (strl+1) / 2, " ");
        lhs << out;
    }

    ::operator delete(eol);
    return lhs;
}

template class BSTree<int>;

#endif // BSTREE_H
