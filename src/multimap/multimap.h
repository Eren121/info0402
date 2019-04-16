#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <iostream>
#include <functional> // less
#include <utility> // pair
#include "List.h"
#include "BSTree.h"

template<typename U, typename V>
std::ostream& operator<<(std::ostream& lhs, const std::pair<U, V>& pair) {

	lhs << "{" << pair.first << "," << pair.second << "}";
	return lhs;
}

template<class Key,
		 class Value,
		 class Compare = std::less<Key>>
class multimap;

template<class Key,
		 class Value,
		 class Compare = std::less<Key>>
class multimap_iterator {
private:
	typedef multimap<Key, Value, Compare>	container;
	typedef typename container::Node		Node;
	typedef typename container::SingleNode	SingleNode;
	typedef typename container::value_type	Pair;
	friend class multimap<Key, Value, Compare>;

	enum Position {
		FRONT, BACK
	};

	enum ChildPosition {
		LEFT, RIGHT
	};

	multimap_iterator child(ChildPosition pos) const {
		multimap_iterator res;

		switch(pos) {

			case LEFT:

				if(current->has_left())
					res = multimap_iterator(*source, *current->left());
				else
					res = multimap_iterator(*source);
				break;

			case RIGHT:

				if(current->has_right())
					res = multimap_iterator(*source, *current->right());
				else
					res = multimap_iterator(*source);
				break;
		}

		return res;
	}

	template<typename U>
	multimap_iterator insert(U&& t, ChildPosition pos) {
		multimap_iterator ret;

		switch(pos) {

			case LEFT:
				current->insert_left(List<Pair>());
				ret = multimap_iterator(*source, *current->left(), std::forward<U>(t));
				break;

			case RIGHT:
				current->insert_right(List<Pair>());
				ret = multimap_iterator(*source, *current->right(), std::forward<U>(t));
				break;
		}

		return ret;
	}

	template<typename U>
	multimap_iterator push_back(U&& t) {
		current->data().push_back(std::forward<U>(t));
		return multimap_iterator(*source, *current, BACK);
	}

	container* source;
	Node* current;
	SingleNode* single;

public:
	// Construit un itérateur invalide dont la seule action valide est de l'assigner à un autre itérateur
	multimap_iterator() :
		source(nullptr),
		current(nullptr),
		single(nullptr)
	{}
	multimap_iterator(container& m) :
		source(&m),
		current(nullptr),
		single(nullptr)
	{}

	multimap_iterator(container& m, Node& node, SingleNode& s) :
		source(&m),
		current(&node),
		single(&s)
	{}

	multimap_iterator(container &m, Node& node, Position pos = FRONT) :
		source(&m),
		current(&node),
		single(nullptr) {
		switch(pos) {
			case FRONT:
				single = node.data().front();
				break;
			case BACK:
				single = node.data().back();
			break;
		}
	}

	template<typename U>
	multimap_iterator(container &m, Node& node, U&& t) :
		source(&m),
		current(&node),
		single(nullptr) {
		current->data().push_back(std::forward<U>(t));
		single = current->data().front();
	}

	multimap_iterator(const multimap_iterator& o) :
		source(o.source),
		current(o.current),
		single(o.single)
	{}

	const Key& key() const {
		return single->data().first;
	}

	Pair& operator*() const {
		if(!current || !single) {
			throw std::logic_error("cannot dereference end() iterator");
		}
		return single->data();
	}

	Pair* operator->() const {
		return &this->operator *();
	}

	multimap_iterator& operator++() {
		if(!current || !single) {
			throw std::logic_error("cannot decrement end() iterator");
		}
		if(!single->is_last()) {
			single = single->next();
		}
		else {
			current = current->next();

			if(current) {
				single = current->data().front();
			}
			else {
				single = nullptr;
			}
		}
		return *this;
	}

	multimap_iterator& operator--() {
		if(!current) { // --end()
			current = source->tree.back();
			single = current->data().back();
		}
		else {
			if(single->is_first()) {
				current = current->prev();
				single  = current->data().back();
			}
			else {
				single = single->prev();
			}
		}
		return *this;
	}

	multimap_iterator& operator=(const multimap_iterator& o) {

		if(this != &o) {
			source = o.source;
			current = o.current;
			single = o.single;
		}

		return *this;
	}

	bool operator==(const multimap_iterator& o) {
		return source == o.source && current == o.current && single == o.single;
	}

	bool operator!=(const multimap_iterator& o) {
		return !(*this == o);
	}
};

template<class Key,
		 class Value,
		 class Compare>
class multimap {
public:
	/// Ordre des déclarations (alias/prototypes)
	/// Selon https://en.cppreference.com/w/cpp/container/multimap
	/// Trié par surcharge, dans l'ordre aussi
	/// Mis à part structure interne
	typedef Key										key_type;
	typedef Value									mapped_type;
	typedef std::pair<const Key, Value>				value_type;
	typedef std::size_t								size_type;
	typedef std::ptrdiff_t							difference_type;
	typedef Compare									key_compare;
	typedef Value&									reference;
	typedef const Value&							const_reference;
	typedef Value*									pointer;
	typedef const Value*							const_pointer;
	typedef multimap_iterator<Key, Value, Compare>	iterator;

	friend class multimap_iterator<Key, Value, Compare>;

private:
	typedef	BSTree<List<value_type>>				Tree;
	typedef typename Tree::Node						Node;
	typedef typename List<value_type>::Node			SingleNode;

	Compare comp;
	Tree tree;

public:
	multimap() : comp(), tree() {}
	iterator begin() {
		return tree.empty() ? end() : iterator(*this, *tree.front(), iterator::FRONT);
	}
	iterator end() {
		return iterator(*this);
	}

	iterator lower_bound(const Key& key) {

		iterator last_not_less = end();
		iterator it = begin();

		while(it != end()) {

			if(comp(it->first, key)) {
				it = it.child(iterator::RIGHT);
			}
			else {
				last_not_less = it;
				it = it.child(iterator::LEFT);
			}
		}

		return last_not_less;
	}

	iterator insert(value_type&& pair) {
		return insert<value_type&&>(std::move(pair));
	}

	template<typename U>
	iterator insert(U&& pair) {

		iterator it, e = end(), tmp = end(), res = end();

		if(empty()) {

			tree.create_root(List<value_type>());
			res = iterator(*this, *tree.root(), std::forward<U>(pair));
		}
		else {

			it = iterator(*this, *tree.root());

			while(it != e) {

				tmp = it;

				if(comp(pair.first, it->first)) {

					it = it.child(iterator::LEFT);
					if(it == e) {
						res = tmp.insert(std::forward<U>(pair), iterator::LEFT);
						break;
					}
				}
				else if(comp(it->first, pair.first)) {

					it = it.child(iterator::RIGHT);
					if(it == e) {
						res = tmp.insert(std::forward<U>(pair), iterator::RIGHT);
						break;
					}
				}
				else {
					res = tmp.push_back(std::forward<U>(pair));
					break;
				}
			}
		}

		return res;
	}

	bool empty() const {
		return tree.empty();
	}

	size_type size() const {
		return static_cast<size_type>(tree.size());
	}

	bool equals(const std::initializer_list<value_type>& il) {
		iterator it = begin(), e = end();

		for(value_type pair : il) {

			if(it == e || *it != pair) {
				break;
			}

			++it;
		}

		return it == e;
	}

	void dump() const {

		iterator it = const_cast<multimap*>(this)->begin();
		iterator e = const_cast<multimap*>(this)->end();

		std::cout << "Infixe: ";

		while(it != e) {
			std::cout << *it;
			++it;

			if(it != e) {
				std::cout << ", ";
			}
		}

		std::cout << std::endl;
		std::cout << "Arbre:" << std::endl;
		std::cout << tree << std::endl;
	}
};

template class multimap<int, int>;

#endif // MULTIMAP_H
