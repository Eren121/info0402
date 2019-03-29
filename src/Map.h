#ifndef MAP_H
#define MAP_H

#include <utility> // std::pair
#include <stdexcept> // std::out_of_range
#include <limits> // std::numeric_limits
#include "rb_tree.h"

template<typename Key,
		 typename T> class Map; // forward declaration

/**
 *		Class MapIterator
 */
template<typename Key,
		 typename T>
class MapIterator {

public:

	// Suit, dans l'ordre donné, les indications de https://en.cppreference.com/w/cpp/named_req/Iterator

	//***LegacyIterator

	//******CopyCnstructible

	//******CopyAssignable

	//******Destructible

	//******Swappable


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

/**
 *		Classe Map
 */
template<typename Key, typename T>
class Map {

	friend class MapIterator<Key, T>;

public:

	/// Ordre des déclarations (alias/prototypes) selon https://fr.cppreference.com/w/cpp/container/map
	/// Trié par surcharge, dans l'ordre aussi

	typedef Key key_type;
	typedef T mapped_type;
	typedef std::pair<const Key, T> value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	//***key_compare
	//***allocator_type
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef MapIterator<Key, T> iterator;
	//***const_iterator
	//***reverse_iterator
	//***const_reverse_iterator

	//****value_compare

	/// Constructeurs
	Map();


	/// Destructeur
	~Map();

	///***operator=

	mapped_type& at(const key_type& key) noexcept(false);

	mapped_type& operator[](const key_type& key);

	iterator begin();
	//****cbegin
	iterator end();
	//***cend
	//***rbegin
	//***crbegin
	//***rend
	//***crend

	bool empty() const;

	size_type size() const;

	size_type max_size() const {
		return std::numeric_limits<size_type>::max() / sizeof(value_type);
	}

	void clear() noexcept;
	/**
	 *		hint: ou serait l'élément dans la map
	 *		Suggère donc d'insérer juste avant hint. Si la valeur n'est pas dans le bon ordre, recherche la bonne place de value sans utiliser hint.
	*/
	iterator insert(iterator hint, const value_type& value);

	std::pair<iterator, bool> insert(const value_type& val);

	//***emplace
	//***emplace_hint

	/**
	 *	Supprime un élément de la Map, retourne le nombre d'éléments supprimés (en l'occurence, zéro ou un)
	 *	@return 1 si l'élément a été trouvé et supprimé
	 *			0 si l'élément n'a pas été trouvé
	 */
	size_type erase(const key_type& key);

	//***swap

	size_type count(const key_type& key);

	iterator find(const key_type& key);

	//***equal_range
	iterator lower_bound(const key_type& key);
	iterator upper_bound(const key_type& key);

	//***key_comp
	//***value_comp

	//***operator==
	//***operator!=
	//***operator<
	//***operator<=
	//***operator>
	//***operator>=


	//***surcharge de std::swap


	///
	///		Implémentation privée
	///

private:

	static int comparator(rb_tree*, rb_node* nodeA, rb_node* nodeB) {

		const value_type& pairA = *reinterpret_cast<const value_type*>(nodeA->value);
		const value_type& pairB = *reinterpret_cast<const value_type*>(nodeB->value);
		const key_type& keyA = pairA.first;
		const key_type& keyB = pairB.first;

		if(keyA < keyB) {

			return -1;
		}
		else if(keyB < keyA) {

			return 1;
		}
		else {

			return 0;
		}
	}

	static void remover(rb_tree* self, rb_node* node) {

		const value_type* a = reinterpret_cast<const value_type*>(node->value);
		delete a;

		rb_tree_node_dealloc_cb(self, node);
	}

	rb_tree* tree;
};

#include "MapIterator.tpp"
#include "Map.tpp"

#endif // MAP_H
