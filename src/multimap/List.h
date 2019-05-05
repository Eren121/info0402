#ifndef LIST_H
#define LIST_H

#include "Defines.h"
#include <ostream> // std::ostream
#include <utility> // std::pair
#include <list> // std::list

// Prédéclaration
template<typename T> class List;


/// Classe légère implémentant une liste doublement chaînée
/// Ne contient pas les fonctionalités de la STL et les itérateurs
template<class T>
class ListNode {
public:

	/// Retourne true si c'est le dernier élément de la liste, false sinon
	bool is_last() const { return m_next == nullptr; }

	/// Retourne true si c'est le premier élément de la liste, false sinon
	bool is_first() const { return m_prev == nullptr; }

	/// Récupère la donnée (constant)
	const T& data() const { return m_data; }

	/// Récupère la donnée (non-constant)
	T& data() { return m_data; }

	/// Récupère le prochain élément (constant)
	/// Ou NULL si c'est le dernier élément
	const ListNode* next() const {
		return m_next;
	}

	/// Récupère le prochain élément (non-constant)
	/// Ou NULL si c'est le dernier élément
	ListNode* next() {
		return m_next;
	}

	/// Récupère l'élément précédent (constant)
	/// Ou NULL si c'est le dernier élément
	const ListNode* prev() const {
		return m_prev;
	}

	/// Récupère l'élément précédent (non-constant)
	/// Ou NULL si c'est le dernier élément
	ListNode* prev() {
		return m_prev;
	}

private:
	/// Ajoute un élément à la liste, et initialise le noeud sur cet élément
	template<typename U>
	ListNode(U&& t, ListNode* previousNode) :
		m_data(std::forward<U>(t)),
		m_prev(previousNode),
		m_next(nullptr) {

		if(!is_first()) {
			m_prev->m_next = this;
		}
	}

	~ListNode() {

		if(!is_first()) {
			m_prev->m_next = m_next;
		}

		if(!is_last()) {
			m_next->m_prev = m_prev;
		}
	}

	T m_data;
	ListNode* m_prev;
	ListNode* m_next;

	friend class List<T>;
};

template<class T>
class List {
public:
	typedef ListNode<T> Node;

	List() : m_first(nullptr), m_last(nullptr) {}
	~List() {
		Node* tmp = m_first;

		while(tmp != nullptr) {

			tmp = m_first->next();
			delete m_first;
			m_first = tmp;
		}
	}

	bool empty() const {
		return m_first == nullptr;
	}

	Node* front() {
		return m_first;
	}

	const Node* front() const {
		return m_first;
	}

	Node* back() {
		return m_last;
	}

	const Node* back() const {
		return m_last;
	}

	template<typename U>
	void push_back(U&& value) {

		Node* tmp = new Node(std::forward<U>(value), m_last);
		if(tmp->is_first()) m_first = tmp;
		m_last = tmp;
	}


	void erase(Node* node) {

		ASSERT(node != nullptr);

		if(node->is_first()) {
			m_first = node->next();
		}

		if(node->is_last()) {
			m_last = node->prev();
		}

		delete node;
	}

	std::size_t size() const {
		std::size_t res = 0;
		Node* tmp = m_first;
		for(; tmp; tmp = tmp->next(), res++);
		return res;
	}

	bool operator!=(const std::list<T>& list) const {
		return !(*this == list);
	}

	bool operator==(const std::list<T>& list) const {

		const ListNode<T>* node = front();

		for(const T& obj : list) {

			if(node == nullptr) {
				return false;
			}

			if(node->data() != obj) {
				return false;
			}

			node = node->next();
		}

		return node == nullptr;
	}

	friend std::ostream& operator<<(std::ostream& lhs, const List& list) {

		const ListNode<T>* node = list.front();

		using namespace pair_operators;

		while(node != nullptr) {

			lhs << node->data();

			if(!node->is_last()) {
				lhs << ",";
			}

			node = node->next();
		}

		return lhs;
	}
private:
	Node* m_first;
	Node* m_last;
};

/// Force l'instanciation des templates
template class List<int>;
template class ListNode<int>;

#endif // LIST_H
