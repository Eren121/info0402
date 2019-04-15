#ifndef LIST_H
#define LIST_H

#include <ostream>
#include <utility>

///  Lightweight wrapper for list

template<typename T> class List;

template<class T>
class ListNode {
public:
	bool is_last() const { return m_next == nullptr; }
	bool is_first() const { return m_prev == nullptr; }

	const T& data() const { return m_data; }
	T& data() { return m_data; }
	const ListNode* next() const { return m_next; }
	ListNode* next() { return m_next; }
	const ListNode* prev() const { return m_prev; }
	ListNode* prev() { return m_prev; }

private:
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

	bool empty() const { return m_first == nullptr; }
	Node* front() { return m_first; }
	const Node* front() const { return m_first; }
	Node* back() { return m_last; }
	const Node* back() const { return m_last; }

	template<typename U>
	void push_back(U&& value) {

		Node* tmp = new Node(std::forward<U>(value), m_last);
		if(tmp->is_first()) m_first = tmp;
		m_last = tmp;
	}

	void erase(Node* node) {

		if(node->is_first()) {
			m_first = node->next();
		}

		if(node->is_last()) {
			m_last = node->prev();
		}

		delete node;
	}

private:
	Node* m_first;
	Node* m_last;
};

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const List<T>& list) {

	const ListNode<T>* node = list.front();

	while(node != nullptr) {

		lhs << node->data();

		if(!node->is_last()) {
			lhs << ",";
		}

		node = node->next();
	}

	return lhs;
}

template<typename T>
bool equals(const List<T>& list, const std::initializer_list<T>& array) {

	const ListNode<T>* node = list.front();

	for(const T& obj : array) {

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

template class List<int>;
template class ListNode<int>;

#endif // LIST_H
