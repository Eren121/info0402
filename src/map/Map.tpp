#include "Map.h"

template<typename Key,
		 typename T>
Map<Key, T>::Map() :
	tree(rb_tree_create(comparator)) {
}


template<typename Key,
		 typename T>
auto Map<Key, T>::at(const Map::key_type &key)
-> mapped_type& {
	iterator it = lower_bound(key);
	if(it == end() || key < it->first) {
		throw std::out_of_range("Aucune valeur trouvée");
	}

	return it->second;
}

/*template<typename Key,
		 typename T>
void
Map<Key, T>::insert(const T& t) {

	rb_tree_insert(tree, new T(t));
}*/

/**
 *	Supprime un élément de la Map, retourne le nombre d'éléments supprimés (en l'occurence, zéro ou un)
 *	@return 1 si l'élément a été trouvé et supprimé
 *			0 si l'élément n'a pas été trouvé
 */

template<typename Key,
		 typename T>
auto
Map<Key, T>::erase(const key_type& t)
-> size_type {
	int n = rb_tree_remove_with_cb(tree, const_cast<key_type*>(&t), remover);

	if(n == 1) {

		return 1;
	}
	else {

		return 0;
	}
}

template<typename Key,
		 typename T>
Map<Key, T>::~Map() {
	rb_tree_dealloc(tree, remover);
	tree = nullptr;
}

template<typename Key,
		 typename T>
auto
Map<Key, T>::begin()
-> iterator {
	return iterator(*this, iterator::begin);
}

template<typename Key,
		 typename T>
auto
Map<Key, T>::end()
-> iterator {
	return iterator(*this, iterator::end);
}

template<typename Key,
		 typename T>
bool
Map<Key, T>::empty() const {
	return rb_tree_size(tree) == 0;
}

template<typename Key,
		 typename T>
auto
Map<Key, T>::size() const
-> size_type {
	return rb_tree_size(tree);
}

template<typename Key,
		 typename T>
auto Map<Key, T>::find(const key_type &key)
-> iterator {
	iterator it = lower_bound(key), e = end();
	if(it == e || key < it->first) {
		return e;
	}

	return it;
}

#include <iostream>

template<typename Key,
		 typename T>
auto Map<Key, T>::insert(value_type &&val, int)
-> std::pair<iterator, bool> {
	iterator it = lower_bound(val.first);

	if(it != end() && !(val.first < it->first)) {
		return std::make_pair(it, false);

	} else {

		// On insère dans la map une copie de la std::pair
		rb_node* node = rb_node_create(new value_type(std::move(val)));

		if(!rb_tree_insert_node(tree, node)) {

			throw new std::bad_alloc();
		}

		return std::make_pair(find(val.first), true);
	}
}

template<typename Key,
		 typename T>
auto Map<Key, T>::insert(const value_type &val)
-> std::pair<iterator, bool> {
	iterator it = lower_bound(val.first);

	if(it != end() && !(val.first < it->first)) {
		return std::make_pair(it, false);

	} else {

		// On insère dans la map une copie de la std::pair
		rb_node* node = rb_node_create(new value_type(val));

		if(!rb_tree_insert_node(tree, node)) {

			throw new std::bad_alloc();
		}

		return std::make_pair(find(val.first), true);
	}
}

template<typename Key,
		 typename T>
auto Map<Key, T>::insert(iterator hint, const Map::value_type& value)
-> iterator {
	// 1 2 3 4 5
	// ^ cas 1: hint est sur begin()

	// 1 2 3 4 5
	//           ^ cas 2: hint est sur end()

	// 1 2 3 4 5
	//     ^ cas 3: hint est entre begin() et end()

	iterator e = end();
	iterator b = begin();

	if(hint == e) {

		if(empty()) {

		}
		else {
			--hint;
			if(hint->first < value.first) {

			}
		}
	}
	else if(hint == b) {
		if(value.first < hint->first) {

		}
	}
	else {
		iterator prev = hint;
		--prev;

		if(prev->first < value.first && value.first < hint->first) {

		}
	}

	// Insertion non optimisée
	//...
}

template<typename Key, typename T>
void Map<Key, T>::clear() noexcept {
	rb_tree_dealloc(tree, remover);
	tree = nullptr;

	tree = rb_tree_create(comparator);
}

template<typename Key,
		 typename T>
auto Map<Key, T>::lower_bound(const key_type& key)
-> iterator {
	///TODO dichotomie
	iterator e = end();

	for(iterator it = begin(); it != e; ++it) {
		if(!(it->first < key)) {
			return it;
		}
	}

	return e;
}

template<typename Key,
		 typename T>
auto Map<Key, T>::upper_bound(const key_type& key)
-> iterator {
	iterator it = lower_bound(key), e = end();

	while(it != e && !(key < it->first))
		++it;

	return it;
}

template<typename Key, typename T>
typename Map<Key, T>::iterator
Map<Key, T>::insert(value_type *value) {
	iterator it = lower_bound(value->first);

	if(value == nullptr)
		throw new std::bad_alloc();

	if(it != end() && !(value->first < it->first)) {
		return it;

	} else {

		// On insère dans la map une copie de la std::pair
		rb_node* node = rb_node_create(value);

		if(!rb_tree_insert_node(tree, node)) {

			throw new std::bad_alloc();
		}

		return find(value->first);
	}
}

template<typename Key,
		 typename T>
auto Map<Key, T>::count(const key_type& key)
-> size_type {
	iterator it = lower_bound(key), e = end();
	int c = 0;

	while(it != e && !(key < it->first)) {
		++c;
		++it;
	}

	return c;
}

template<typename Key, typename T>
auto Map<Key, T>::operator[](const key_type &key)
-> mapped_type& {
	iterator it = lower_bound(key);

	if(it == end() || key < it->first) {
		return insert(new value_type(std::piecewise_construct, std::tuple<const key_type&>(key), std::tuple<>()))->second;
	}

	return it->second;
}
