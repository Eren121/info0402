#ifndef MAP_H
#define MAP_H

#include <utility> // std::pair
#include <stdexcept> // std::out_of_range
#include "rb_tree.h"

template<typename Key,
		 typename T>
class Map;

template<typename Key,
		 typename T>
class MapIterator {
public:
	typedef Map<Key, T> container_type;
	typedef std::input_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
	typedef typename container_type::mapped_type mapped_type;
	typedef typename container_type::key_type key_type;
	typedef typename container_type::value_type value_type;

private:
	MapIterator(container_type& map);

	struct begin_t {};
	struct end_t {};

public:
	static constexpr begin_t begin = {};
	static constexpr end_t end = {};

public:
	MapIterator(container_type& map, rb_iter* ptr);
	MapIterator(container_type& map, const decltype(begin)&);
	MapIterator(container_type& map, const decltype(end)&);
	MapIterator(const MapIterator& copy);
	MapIterator& operator=(const MapIterator& copy);
	~MapIterator();

	bool operator!=(const MapIterator& other) const;
	bool operator==(const MapIterator& other) const { return !(*this != other); }
	MapIterator& operator++();
	MapIterator& operator--();
	value_type& operator*();
	value_type* operator->() { return &**this; }

private:
	container_type* container;
	rb_iter* it;
};

template<typename Key,
		 typename T>
class Map {
	friend class MapIterator<Key, T>;

public:
	typedef Key key_type;
	typedef T mapped_type;
	typedef std::pair<const Key, T> value_type;
	typedef MapIterator<Key, T> iterator;
	typedef std::size_t size_type;

public:
	Map();
	~Map();

	/**
	 *	Supprime un élément de la Map, retourne le nombre d'éléments supprimés (en l'occurence, zéro ou un)
	 *	@return 1 si l'élément a été trouvé et supprimé
	 *			0 si l'élément n'a pas été trouvé
	 */
	size_type erase(const key_type& key);
	iterator begin();
	iterator end();

	bool empty() const;
	size_type size() const;
	iterator find(const key_type& key);
	mapped_type& at(const key_type& key) noexcept(false);
	std::pair<iterator, bool> insert(const value_type& val);

	iterator lower_bound(const key_type& key);
	iterator upper_bound(const key_type& key);
	size_type count(const key_type& key);


	/**
	 *		Ajoute un élément juste avant hint, si possible
	 *		Sinon n'insère pas l'élément et retourne end()
	 *		@return un itérateur vers l'élément inséré
	 */
	iterator insert(iterator hint, const value_type& value);

private:

	static int comparator(rb_tree*, rb_node* nodeA, rb_node* nodeB) {

		const T& a = *reinterpret_cast<const T*>(nodeA->value);
		const T& b = *reinterpret_cast<const T*>(nodeB->value);

		if(a < b) {

			return -1;
		}
		else if(b < a) {

			return 1;
		}
		else {

			return 0;
		}
	}

	static void remover(rb_tree* self, rb_node* node) {

		T* a = reinterpret_cast<T*>(node->value);
		delete a;

		rb_tree_node_dealloc_cb(self, node);
	}

	rb_tree* tree;
};

#include "MapIterator.tpp"
#include "Map.tpp"

#endif // MAP_H
