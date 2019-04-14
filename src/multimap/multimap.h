#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <functional> // less
#include <utility> // pair

template<class Key,
		 class Value,
		 class Compare = std::less<Key>>
class multimap;

template<class Key,
		 class Value,
		 class Compare = std::less<Key>>
class multimap_iterator {
private:
	typedef multimap<Key, Value, Compare> container;
	typedef typename container::Node Node;
	typedef typename container::SingleNode SingleNode;
	typedef typename container::value_type Pair;
	friend class multimap<Key, Value, Compare>;
	container& source;
	Node* current;
	SingleNode* single;
public:
	multimap_iterator(container& m) : multimap_iterator(m, nullptr, nullptr) {}
	multimap_iterator(container& m, Node* node, SingleNode* s) : source(m), current(node), single(s) {}
	Pair& operator*() const {
		if(!current || !single) {
			throw std::logic_error("cannot dereference end() iterator");
		}
		return single->value;
	}
	multimap_iterator& operator++() {
		if(!current || !single) {
			throw std::logic_error("cannot dereference end() iterator");
		}
		if(single->next) {
			single = single->next;
		}
		else {
			if(current->right) {
				current = current->right;
				while(current->left) {
					current = current->left;
				}
			}
			else {
				while(current->parent && current->parent->right == current) {
					current = current->parent;
				}
				current = current->parent;
				if(current) {
					single = current->first;
				}
				else {
					single = nullptr;
				}
			}
		}
		return *this;
	}
	bool operator==(const multimap_iterator& o) {
		return &o.source == &source && current == o.current && single == o.single;
	}
};

template<class Key,
		 class Value,
		 class Compare>
class multimap {
private:
	struct Node;
public:
	/// Ordre des déclarations (alias/prototypes)
	/// Selon https://en.cppreference.com/w/cpp/container/multimap
	/// Trié par surcharge, dans l'ordre aussi
	/// Mis à part structure interne
	typedef Key								key_type;
	typedef Value							mapped_type;
	typedef std::pair<const Key, Value>		value_type;
	typedef std::size_t						size_type;
	typedef std::ptrdiff_t					difference_type;
	typedef Compare							key_compare;
	typedef Value&							reference;
	typedef const Value&					const_reference;
	typedef Value*							pointer;
	typedef const Value*					const_pointer;
	typedef multimap_iterator<Key, Value, Compare> iterator;
	typedef Node							node_type;
	friend class multimap_iterator<Key, Value, Compare>;

private:
	// Structure interne
	struct SingleNode {
		SingleNode *next, *prev;
		value_type value;
		SingleNode(const value_type& v) : next(nullptr), prev(nullptr), value(v) {}
	};
	struct Node {
		Node *parent, *right, *left;
		SingleNode *first, *last;
		Node(const value_type& value) : parent(nullptr), right(nullptr), left(nullptr), first(new SingleNode(value)), last(first) {}
		~Node() {
			for(SingleNode* cur = first; cur != nullptr; cur = cur->next) {
				delete cur;
			}
		}
	};
	Compare comp;
	Node* root;
public:
	multimap() : comp(), root(nullptr) {}
	iterator begin() {
		return root ? iterator(*this, root, root->first) : end();
	}
	iterator end() {
		return iterator(*this);
	}
	iterator lower_bound(const Key& key) {
		Node* last_greather = nullptr;
		Node* cur = root;
		while(cur) {
			if(comp(cur->first->value.first, key)) {
				cur = cur->right;
			}
			else {
				last_greather = cur;
				cur = cur->left;
			}
		}
		cur = last_greather;
		return cur ? iterator(*this, cur, cur->first) : end();
	}
	iterator insert(iterator hint, const value_type& pair) {
		iterator prev = hint;
		iterator e = end();
		if(hint == e) {
			if(empty()) {
				root = new Node(pair);
			}
			else {
				--prev;
				if(comp(prev->first, pair->first)) {
					insert(pair);//bad hint
				}
				else {
					if(hint.current->fg) {
						prev.current->fd = new Node(pair);
						prev.current->fd->parent = prev.current;
					}
					else {
						hint.current->fg = new Node(pair);
						hint.current->fg->parent = hint.current;
					}
				}
			}
		}
		else  {
			if(comp(hint->first, pair.first)) {
				--prev;
				if(prev == e) {
					hint.current->fg = new Node(pair);
					hint.current->fg->parent = hint.current;
				}
				else if(comp(pair.first, prev->first)) {
					insert(pair);//bad hint
				}
				else if(comp(prev->first, pair.first)) {
					Node* tmp = new Node(pair);
					if(!prev.current->left) {
						tmp->parent = prev.current;
						prev.current->left = tmp;
					}
					else {
						iterator prev2 = prev; --prev2;
						tmp->parent = prev2.current;
						prev2.current->right = tmp;
					}
				}
				else {
					SingleNode* tmp = new SingleNode(pair);
					tmp->prev = prev.current->last;
					tmp->prev->next = tmp;
					prev.current->last = tmp;
					if(!tmp->prev) {
						prev.current->first = tmp;
					}
				}
			}
			else {
				insert(pair);//bad hint
			}
		}
	}
	iterator insert(const value_type& pair) {
		Node** cur = &root;

		while(cur) {
			if(comp(pair.first, (*cur)->first->value.first)) {
				cur = &(*cur)->right;
			}
			else if(comp((*cur)->first->value.first, pair.first)) {
				cur = &(*cur)->left;
			}
			else {
				SingleNode* tmp = new SingleNode(pair);
				tmp->prev = (*cur)->last;
				tmp->prev->next = tmp;
				(*cur)->last = tmp;
			}
		}

	}
	bool empty() const {
		return !root;
	}
};

#endif // MULTIMAP_H
