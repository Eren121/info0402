#ifndef MULTIMAPITERATOR_H
#define MULTIMAPITERATOR_H

#include <functional>

template<class Key,
		 class Value,
		 class Compare>
class Multimap;

template<bool IsConst, class T>
struct add_const_if {
	typedef T type;
};

template<class T>
struct add_const_if<true, T> {
	typedef const T type;
};

template<bool IsConst, class T> using
	add_const_if_t =
		typename add_const_if<IsConst, T>::type;

template<class Key,
          class Value,
          class Compare,
          bool IsConst>
class MultimapIterator;

/// Itérateur
template<class Key,
         class Value,
         class Compare,
         bool IsConst>
class MultimapIterator {

private:

        typedef MultimapIterator<Key, Value, Compare, !IsConst>         IteratorIsntIsConst;
        typedef add_const_if_t<IsConst, Multimap<Key, Value, Compare>>	Container;
        typedef add_const_if_t<IsConst, typename Container::Node>	Node;
        typedef add_const_if_t<IsConst, typename Container::SingleNode>	SingleNode;
        typedef add_const_if_t<IsConst, typename Container::value_type>	Pair;

	/// Position de l'itérateur sur chaque noeud (début ou fin)
	enum Position { FRONT, BACK };

	/// Position d'un noeud enfant par rapport au noeud parent de l'arbre binaire
	enum struct ChildPosition { LEFT, RIGHT };

public:
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = Pair;
	using pointer = Pair*;
	using reference = Pair&;

	~MultimapIterator() = default;

	/// Constructeurs ----------------

	/// Construit un itérateur invalide dont la seule action valide est de l'assigner à un autre itérateur
	MultimapIterator();
	/// Itérateur de fin
    explicit MultimapIterator(Container& m);
	MultimapIterator(Container& m, Node& node, SingleNode& s);
	MultimapIterator(Container &m, Node& node, Position pos = FRONT);
	/// Constructeur par copie
	MultimapIterator(const MultimapIterator& o);


    /// Permet la construction de const à partir de non-const
    /// Le constructeur par copie est préféré si IsConst = IsConstIt
    template<bool IsConstIt>
    MultimapIterator(const MultimapIterator<Key, Value, Compare, IsConstIt>& o) :
        m_source(o.m_source),
        m_current(o.m_current),
        m_single(o.m_single) {

        static_assert(IsConst && !IsConstIt, "cannto construct iterator from const_iterator"); // optionnel, car la construction assigne des const à non-const
    }

    struct empty_t {};
    friend typename std::conditional<!IsConst, MultimapIterator<Key, Value, Compare, true>, empty_t>::type;

	/// Accesseurs ---------------------

	/// Récupère la clé
	const Key& key() const;
	/// Récupère la paire {clé, valeur}
	value_type& operator*() const;
	value_type* operator->() const;
	/// Prochain élément (préfixe)
	MultimapIterator& operator++();
	/// Prochain élément (postfixe)
	MultimapIterator operator++(int);
	/// Element précédent (préfixe)
	/// Si le conteneur est vide, comportement indéfini
	/// Si c'est le premier élément, comportement indéfini
	MultimapIterator& operator--();
	/// Elément précédent (postfixe)
	/// Si le conteneur est vide, comportement indéfini
	/// Si c'est le premier élément, comportement indéfini
	MultimapIterator operator--(int);
	/// Assignation par copie
	MultimapIterator& operator=(const MultimapIterator& o);

        /// Opérateurs --------------------

	/// Egalité
	/// Deux itérateurs sont égaux s'ils pointent vers le même élément de la même multimap
        /// Dans le cas de l'itérateur de fin, il faut que se soit l'itérateur de fin des même conteneur
        template<bool IsConstIt>
        bool operator==(const MultimapIterator<Key, Value, Compare, IsConstIt>& rhs) const {
            return m_source == rhs.m_source &&
                   m_current == rhs.m_current &&
                   m_single == rhs.m_single;
        }

        template<bool IsConstIt>
        bool operator!=(const MultimapIterator<Key, Value, Compare, IsConstIt>& rhs) const {
            return !(*this == rhs);
        }

        friend class Multimap<Key, Value, Compare>;
        friend IteratorIsntIsConst;

private:

	/// Récupère le noeud enfant (droite ou gauche)
	/// Retourne l'itérateur de fin s'il n'existe pas
	MultimapIterator child(ChildPosition pos) const;

	Container* m_source; /// La multimap de l'itérateur
	Node* m_current; /// Le noeud de l'arbre
	SingleNode* m_single; /// La position dans la Liste du noeud actuel
};

/// MultimapIterator implementation ----------------------


template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
auto MultimapIterator<
	Key, Value, Compare, IsConst>
	::child(MultimapIterator::ChildPosition pos)
	const -> MultimapIterator {

	MultimapIterator res;
	switch(pos) {
	case ChildPosition::LEFT:
		if(m_current->has_left())
			res = MultimapIterator(*m_source, *m_current->left());
		else
			res = MultimapIterator(*m_source);
		break;

	case ChildPosition::RIGHT:
		if(m_current->has_right())
			res = MultimapIterator(*m_source, *m_current->right());
		else
			res = MultimapIterator(*m_source);
		break;
	}

	return res;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::MultimapIterator() :
		m_source(nullptr),
		m_current(nullptr),
		m_single(nullptr) {}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::MultimapIterator(Container& m) :
		m_source(&m),
		m_current(nullptr),
		m_single(nullptr) {}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::MultimapIterator(Container& m, Node& node, SingleNode& s) :
		m_source(&m),
		m_current(&node),
		m_single(&s) {}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::MultimapIterator(Container &m, Node& node, Position pos) :
		m_source(&m),
		m_current(&node),
		m_single(nullptr) {

	switch(pos) {
		case FRONT:
			m_single = node.data().front();
			break;
		case BACK:
			m_single = node.data().back();
		break;
	}
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::MultimapIterator(const MultimapIterator& o) :
		m_source(o.m_source),
		m_current(o.m_current),
		m_single(o.m_single) {}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
const Key&
MultimapIterator<Key, Value, Compare, IsConst>::key() const {
	return m_single->data().first;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
typename MultimapIterator<Key, Value, Compare, IsConst>::Pair&
MultimapIterator<Key, Value, Compare, IsConst>::operator*() const {
	ASSERT(m_current != nullptr && m_single != nullptr);
	return m_single->data();
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
typename MultimapIterator<Key, Value, Compare, IsConst>::Pair*
MultimapIterator<Key, Value, Compare, IsConst>::operator->() const {
	return &this->operator *();
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>&
MultimapIterator<Key, Value, Compare, IsConst>::operator++() {

	ASSERT(m_current != nullptr && m_single != nullptr);

	if(!m_single->is_last()) {
		m_single = m_single->next();
	}
	else {
		m_current = m_current->next();

		if(m_current) {
			m_single = m_current->data().front();
		}
		else {
			m_single = nullptr;
		}
	}
	return *this;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::operator++(int) {
	MultimapIterator tmp = *this;
	++(*this);
	return tmp;
}
template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>&
MultimapIterator<Key, Value, Compare, IsConst>::operator--() {

	if(!m_current) { // --end()

		m_current = m_source->tree.back();
		m_single = m_current->data().back();
	}
	else {

		if(m_single->is_first()) {

			m_current = m_current->prev();
			m_single  = m_current->data().back();
		}
		else {

			m_single = m_single->prev();
		}
	}
	return *this;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>
MultimapIterator<Key, Value, Compare, IsConst>::operator--(int) {
	MultimapIterator tmp = *this;
	--(*this);
	return tmp;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
MultimapIterator<Key, Value, Compare, IsConst>&
MultimapIterator<Key, Value, Compare, IsConst>::operator=(const MultimapIterator& o) {

	if(this != &o) {
		m_source = o.m_source;
		m_current = o.m_current;
		m_single = o.m_single;
	}

	return *this;
}

/*
template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
bool MultimapIterator<Key, Value, Compare, IsConst>::operator==(const MultimapIterator& o) const {
	return m_source == o.m_source && m_current == o.m_current && m_single == o.m_single;
}

template<class Key,
		 class Value,
		 class Compare,
		 bool IsConst>
bool MultimapIterator<Key, Value, Compare, IsConst>::operator!=(const MultimapIterator& o) const {
	return !(*this == o);
}
*/

#endif // MULTIMAPITERATOR_H


