#ifndef BSTREE_H
#define BSTREE_H

#include "List.h"
#include "AnsiColor.h"
#include <ostream>
#include <sstream>
#include <utility>

// Lightweight wrapper for binary search tree

template<typename T>
class BSTree;

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const BSTree<T>& bstree);

template<typename T>
class BSTreeNode {
public:

	std::size_t size() const {
		int res = 1;
		if(m_left) res += m_left->size();
		if(m_right) res += m_right->size();
		return res;
	}

	bool is_root() const { return m_parent == nullptr; }
	bool is_leaf() const { return m_left == nullptr && m_right == nullptr; }
	bool has_left() const { return m_left != nullptr; }
	bool has_right() const { return m_right != nullptr; }

	const T& data() const { return *m_data; }
	T& data() { return *m_data; }

	const BSTreeNode* parent() const {
		return m_parent;
	}

	BSTreeNode* parent() {
		return m_parent;
	}

	const BSTreeNode* left() const {
		return m_left;
	}

	BSTreeNode* left() {
		return m_left;
	}

	const BSTreeNode* right() const {
		return m_right;
	}

	BSTreeNode* right() {
		return m_right;
	}

	int height() const {
		int hl = 0;
		int hr = 0;

		if(m_left != nullptr) {
			hl = 1 + m_left->height();
		}
		if(m_right != nullptr) {
			hr = 1 + m_right->height();
		}

		if(hl > hr) return hl;
		else return hr;
	}

	BSTreeNode* max() {
		BSTreeNode* tmp = this;
		while(tmp->m_right != nullptr) tmp = tmp->m_right;
		return tmp;
	}

	BSTreeNode* min() {
		BSTreeNode* tmp = this;
		while(tmp->m_left != nullptr) tmp = tmp->m_left;
		return tmp;
	}

	BSTreeNode* next() {

		BSTreeNode *tmp = this;
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

	BSTreeNode* prev() {

		BSTreeNode *tmp = this;
		if(m_right != nullptr) {
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

	template<typename U>
	BSTreeNode* insert_left(U&& t) {
		m_left = new BSTreeNode(std::forward<U>(t), this);
		return m_left;
	}

	template<typename U>
	BSTreeNode* insert_right(U&& t) {
		m_right = new BSTreeNode(std::forward<U>(t), this);
		return m_right;
	}

private:
	template<typename U>
	BSTreeNode(U&& t, BSTreeNode* parentNode) :
		m_data(new T(std::forward<U>(t))),
		m_parent(parentNode),
		m_left(nullptr),
		m_right(nullptr) {}

	~BSTreeNode() {
		delete m_data;
		delete m_left;
		delete m_right;
	}

	BSTreeNode* detach() {

		BSTreeNode* tmp = nullptr;

		if(m_left && m_right) {
			tmp = next();
			swap(*this, *tmp);
			return tmp->detach();
		}
		else if(m_left) {
			swap(*this, *m_left);
			return m_left->detach();
		}
		else if(m_right) {
			swap(*this, *m_right);
			return m_right->detach();
		}
		else {

			if(m_parent) {

				if(m_parent->m_left == this) m_parent->m_left = nullptr;
				else m_parent->m_right = nullptr;
			}

			return this;
		}
	}

	T* m_data;
	BSTreeNode* m_parent;
	BSTreeNode* m_left;
	BSTreeNode* m_right;

	friend class BSTree<T>;
	friend std::ostream& operator<< <T>(std::ostream& lhs, const BSTree<T>& bstree);

	friend void swap(BSTreeNode& a, BSTreeNode& b) {
		T* tmp = a.m_data;
		a.m_data = b.m_data;
		b.m_data = tmp;
	}
};

template<typename T>
class BSTree {
public:
	typedef BSTreeNode<T> Node;

	BSTree() : m_root(nullptr) {}
	BSTree(const BSTree&) = delete;
	BSTree(BSTree&& o) : m_root(o.m_root) { o.m_root = nullptr; }

	~BSTree() {
		delete m_root;
	}

	BSTree& operator=(const BSTree&) = delete;
	BSTree& operator=(BSTree&& o) {
		Node* tmp = m_root;
		m_root = o.m_root;
		o.m_root = tmp;
		return *this;
	}

	std::size_t size() const {

		if(m_root) return m_root->size();
		else return 0;
	}

	bool empty() const { return m_root == nullptr; }
	int height() const { return m_root != nullptr ? m_root->height() : 0; }
	Node& operator*() { return *m_root; }
	const Node& operator*() const { return *m_root; }
	Node* operator->() { return m_root; }
	const Node* operator->() const { return m_root; }

	template<typename U>
	Node** create_root(U&& value) {
		m_root = new Node(std::forward<U>(value), nullptr);
		return &m_root;
	}

	const Node* root() const {
		return m_root;
	}
	Node* root() {
		return m_root;
	}


	void erase(Node* node) {

		Node* to_delete = node->detach();

		if(to_delete == m_root)
			m_root = nullptr;

		delete to_delete;
	}

	Node* front() {
		return m_root ? m_root->min() : nullptr;
	}

	const Node* front() const {
		return m_root ? m_root->min() : nullptr;
	}

	Node* back() {
		return m_root ? m_root->max() : nullptr;
	}

	const Node* back() const {
		return m_root ? m_root->max() : nullptr;
	}

	// Infixe
	bool equals(const std::initializer_list<T>& initl) {

		Node* tmp = front();

		for(const T& val : initl) {

			if(!tmp || tmp->data() != val) {
				return false;
			}

			tmp = tmp->next();
		}

		return !tmp;
	}

private:
	Node* m_root;

	friend std::ostream& operator<< <T>(std::ostream& lhs, const BSTree<T>& bstree);
};

#define LENGTH 128 // Largeur d'une ligne
#define N_ELEMENT 16 // Nombre de caractères à afficher par élément

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const BSTree<T>& bstree) {
	typedef BSTreeNode<T> Node;

	lhs << "\n\n\033[4mArbre :\033[0m\n\n";

	int length = 128;
	char str[100];
	char out[100];
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

	if(bstree.m_root != NULL)
	{
		file.push_back(bstree.m_root);
		h = bstree.m_root->height();
	}

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
				stream << node->data();

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

	::operator delete(eol);
	return lhs;
}

template class BSTree<int>;
template class BSTreeNode<int>;

#endif // BSTREE_H
