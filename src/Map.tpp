#include "Map.h"
template<typename Key,
		 typename T>
Map<Key, T>::Map() :
	tree(rb_tree_create(comparator)) {

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

	int n = rb_tree_remove_with_cb(tree, &t, remover);

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

	///TODO dichotomie

	for(auto it = begin(); it != end(); ++it) {
		if(!(it->first < key) && !(key < it->first))
			return it;
	}

	return end();
}

template<typename Key, typename T>
auto Map<Key, T>::insert(const value_type &val)
-> std::pair<iterator, bool> {

	iterator it = find(val.first);

	if(it == end()) {

		// On insère dans la map une copie de la std::pair
		rb_node* node = rb_node_create(new value_type(val));

		if(!rb_tree_insert_node(tree, node)) {

			throw new std::exception();
		}

		return std::make_pair(find(val.first), false);
	}
	else
		return std::make_pair(it, true);
}

template<typename Key,
		 typename T>
auto Map<Key, T>::insert(iterator hint, const Map::value_type& value)
-> iterator
{
	if(hint == begin())
		return end();

	// Pour que l'insertion soit possible,
	// Soit hint vaut end(),
	// Soit hint pointe vers un élément de clé supérieure à la clé de value

	/// TODO...

	if(value.first < hint->first) {

		iterator it(hint);
		--it;

		if(it->first < value.first) {


		}
	}

	//return it;
}

template<typename Key,
		 typename T>
auto Map<Key, T>::at(const Map::key_type &key) -> mapped_type& {

	///TODO dichotomie

	for(auto it = begin(); it != end(); ++it) {
		if(!(it->first < key) && !(key < it->first))
			return it->second;
	}

	throw std::out_of_range("Aucune valeur trouvée");
}

template<typename Key,
		 typename T>
auto Map<Key, T>::lower_bound(const key_type& key)
-> iterator {

	///TODO dichotomie

	for(auto it = begin(); it != end(); ++it) {
		if(!(it->first < key)) {
			return it;
		}
	}

	return end();
}

template<typename Key,
		 typename T>
auto Map<Key, T>::upper_bound(const key_type& key)
-> iterator {

	///TODO dichotomie

	for(auto it = begin(); it != end(); ++it) {
		if(key < it->first) {
			return it;
		}
	}

	return end();
}

template<typename Key,
		 typename T>
auto Map<Key, T>::count(const key_type& key)
-> size_type {

	///TODO dichotomie
	int c = 0;

	for(auto it = begin(); it != end(); ++it) {
		if(!(key < it->first) &&) {
			return it;
		}
	}

	return c;
}