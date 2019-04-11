#include "Map.h"

///////////////////
/// Begin, End ///
//////////////////

template<typename Key,
		 typename T>
constexpr
typename MapIterator<Key, T>::begin_t MapIterator<Key, T>::begin;

template<typename Key,
		 typename T>
constexpr
typename MapIterator<Key, T>::end_t MapIterator<Key, T>::end;

/////////////////////
/// Constructeurs ///
/////////////////////

template<typename Key,
		 typename T>
MapIterator<Key, T>::MapIterator(container_type& map) :
	container(&map),
	it(rb_iter_create())
{
}

template<typename Key,
		 typename T>
MapIterator<Key, T>::MapIterator(container_type &map, rb_iter *ptr) :
	MapIterator(map)
{
	it = ptr;
}

template<typename Key,
		 typename T>
MapIterator<Key, T>::MapIterator(container_type& map, const decltype(begin)&) :
	MapIterator(map)
{
   rb_iter_first(it, map.tree);
}

template<typename Key,
		 typename T>
MapIterator<Key, T>::MapIterator(container_type& map, const decltype(end)&) :
	MapIterator(map)
{
   rb_iter_last(it,  map.tree);

   if(it->node != nullptr) { //Si la map est vide, on ne pas peut aller à l'élément suivant (segfault sinon)
		rb_iter_next(it);
   }
}

template<typename Key,
		 typename T>
MapIterator<Key, T>::MapIterator(const MapIterator& copy) :
	MapIterator(*copy.container)
{
	*it = *copy.it;
}

///////////////////
/// Destructeur ///
///////////////////

template<typename Key,
		 typename T>
MapIterator<Key, T>::~MapIterator() {

	rb_iter_dealloc(it);
}

////////////////
/// Méthodes ///
////////////////

template<typename Key,
		 typename T>
MapIterator<Key, T>&
MapIterator<Key, T>::operator=(const MapIterator& copy) {
	if(this != &copy) {
		*it = *copy.it;
	}
	return *this;
}

template<typename Key,
		 typename T>
bool
MapIterator<Key, T>::operator!=(const MapIterator& other) const {
	return it->node != other.it->node;
}

template<typename T,
		 typename Key>
MapIterator<T, Key>&
MapIterator<T, Key>::operator++() {
	rb_iter_next(it);
	return *this;
}


template<typename T,
		 typename Key>
MapIterator<T, Key>&
MapIterator<T, Key>::operator--() {
	rb_iter_prev(it);
	return *this;
}

template<typename Key,
		 typename T>
auto
MapIterator<Key, T>::operator*()
-> value_type& {

	if(it->node == nullptr) {
		throw std::out_of_range("end iterator");
	}

	return *reinterpret_cast<value_type*>(it->node->value);
}
