#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "List.h"
#include "BSTree.h"
#include "MultimapIterator.h"
#include <iostream>   // std::ostream
#include <functional> // std::less
#include <utility>    // std::pair
#include <map>	      // std::multimap
#include <limits>     // std::numeric_limits::max()

template<
    class Key,
    class T,
    class Compare = std::less<Key>
    > class Multimap;

template<
    class Key,
    class T,
    class Compare
> class Multimap {
public:
    /// Ordre des déclarations (alias/prototypes)
    /// Selon https://en.cppreference.com/w/cpp/container/multimap
    /// Trié par surcharge, dans l'ordre aussi
    /// Mis à part structure interne

    /// Types membres
    /// ------------
    /// key_type, mapped_type, value_type
    /// size_type, difference_type
    /// key_compare, allocator_type
    /// reference, const_reference,
    /// pointer, const_pointer
    /// iterator, const_iterator
    /// reverse_iterator
    /// const_reverse_iterator
    /// node_type

    typedef Key key_type;
    typedef T mapped_type;
    typedef std::pair<const Key, T> value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Compare key_compare;
    typedef std::allocator<value_type> allocator_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef MultimapIterator<Key, T, Compare, false>
            iterator;
    typedef MultimapIterator<Key, T, Compare, true>
            const_iterator;

private:

    /**
     * @brief Type standard correspondant
     */
    typedef std::multimap<Key, T, Compare> stl_type;

    typedef BSTree<List<value_type>> Tree;
    typedef typename Tree::Node Node;
    typedef typename List<value_type>::Node SingleNode;

public:

    /// Classes membres
    /// ---------------
    /// value_compare

    /**
     * @brief Classe qui compare les paires
     */
    class value_compare {
    public:
        typedef bool result_type;
        typedef value_type first_argument_type ;
        typedef value_type second_argument_type ;

        bool operator()(const value_type& lhs,
                        const value_type& rhs) const
        { return comp(lhs.first, rhs.first); }

    protected:
        value_compare(Compare c) :
            comp(c)
        {}
        Compare comp;

        friend class Multimap;
    };

    /// Constructeurs

    Multimap() :
            Multimap(Compare()) {}

    explicit Multimap(const Compare& comp) :
            m_comp(comp),
            m_tree() {
    }

    template<typename InputIt>
    Multimap(InputIt first, InputIt last,
                     const Compare& comp = Compare()) :
                            Multimap(comp) {
            insert(first, last);
    }

    Multimap(const Multimap& other) :
            Multimap(other.cbegin(), other.cend(),
                             other.key_comp()) {}

    Multimap(Multimap&& other) :
            m_comp(std::move(other.m_comp)),
            m_tree() {

            insert(other.cbegin(), other.cend());
    }

    Multimap(std::initializer_list<value_type> init,
                     const Compare& comp = Compare()) :
                            Multimap(init.begin(), init.end(),
                                             comp) {}

    /// Constructeur par copie de std::multimap
    explicit Multimap(const std::multimap<Key, T>& other) :
            Multimap(other.cbegin(), other.cend(),
                             other.key_comp()) {}

    explicit Multimap(std::multimap<Key, T>&& other) :
            Multimap(std::move(other.key_comp())) {

            for(value_type& val : other)
                    insert(std::move(val));

            other.clear();
    }

    /// Destructeur

    ~Multimap() = default;

    /// operator =

    Multimap& operator=(const Multimap& other) {

            if(this != &other) {

                    m_comp = other.m_comp;
                    clear();
                    insert(other.cbegin(), other.cend());
            }

            return *this;
    }

    Multimap& operator=(Multimap&& other) {

            if(this != &other) {

                    m_comp = std::move(other.m_comp);
                    m_tree = std::move(other.m_tree);
            }

            return *this;
    }

    Multimap& operator=(std::initializer_list<value_type> list) {

            clear();
            insert(list.begin(), list.end());
            return *this;
    }

    /// Assignation par copie de std::map
    Multimap& operator=(const std::map<Key, T, Compare>& other) {

            m_comp = other.key_comp();
            clear();
            insert(other.cbegin(), other.cend());
            return *this;
    }

    Multimap& operator=(std::map<Key, T, Compare>&& other) {

            m_comp = std::move(other.key_comp());
            clear();

            for(value_type& val : other)
                    insert(std::move(val));

            return *this;
    }

    allocator_type get_allocator() const {
            return allocator_type();
    }

    //// Itérateurs
    ///  ----------
    /// begin, cbegin, end, cend,
    /// rbegin, crbegin, rend, crend

    iterator begin() {
            return m_tree.empty() ? end() :
                iterator(*this, *m_tree.front(), iterator::FRONT);
    }

    const_iterator begin() const {
            return cbegin();
    }

    const_iterator cbegin() const {
            return m_tree.empty() ? cend() :
                    const_iterator(*this, *m_tree.front(), const_iterator::FRONT);
    }

    iterator end() {
            return iterator(*this);
    }

    const_iterator end() const {
            return cend();
    }

    const_iterator cend() const {
            return const_iterator(*this);
    }

    /// Capacité
    /// --------
    /// empty, size, max_size

    bool empty() const {
            return m_tree.empty();
    }

    size_type size() const {

        // Retourner pas seulement la taille de l'arbre
        // Mais la taille cumulée de tous les éléments de l'arbre
        size_type total = 0;

        if(m_tree.empty()) {

            return 0;
        }

        for(const Node* node = m_tree.front(); node != nullptr; node = node->next()) {

            total += node->data().size();
        }

        return total;
    }

    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(Node);
    }

    /// Modificateurs
    /// -------------
    /// clear, insert, emplace, emplace_hint, erase, swap, extract, merge

    void clear() {
        m_tree.clear();
    }

    iterator insert(iterator position, const value_type& val) {

        ///TODO
        return insert(val);
    }

    // Pour permettre l'appel en liste d'initialisation
    iterator insert(value_type&& pair) {
        return insert<value_type&&>(std::move(pair));
    }

    template<typename InputIt>
    void insert(InputIt first, InputIt last) {

        for(; first != last; ++first)
            insert(*first);
    }

    template<typename U>
    iterator insert(U&& pair) {

        iterator it, e = end(), tmp = end(), res = end();

        if(empty()) {

            m_tree.create_root(List<value_type>());
            m_tree.root()->data().push_back(std::forward<U>(pair));
            res = iterator(*this, *m_tree.root());
        }
        else {

            it = iterator(*this, *m_tree.root());

            while(it != e) {

                tmp = it;

                if(m_comp(pair.first, it->first)) {

                    it = it.child(iterator::ChildPosition::LEFT);

                    if(it == e) {

                        tmp.m_current->insert_left(List<value_type>());
                        tmp.m_current->left()->data().push_back(std::forward<U>(pair));
                        res = tmp.child(iterator::ChildPosition::LEFT);
                        break;
                    }
                }
                else if(m_comp(it->first, pair.first)) {

                    it = it.child(iterator::ChildPosition::RIGHT);

                    if(it == e) {

                        tmp.m_current->insert_right(List<value_type>());
                        tmp.m_current->right()->data().push_back(std::forward<U>(pair));
                        res = tmp.child(iterator::ChildPosition::RIGHT);
                        break;
                    }
                }
                else {

                    List<value_type>& list = tmp.m_current->data();
                    list.push_back(std::forward<U>(pair));
                    res = iterator(*this, *tmp.m_current, iterator::BACK);
                    break;
                }
            }
        }

        return res;
    }


    size_type erase(const key_type& key) {

        size_type ret = 0; // Nombre d'éléments supprimés
        iterator it = lower_bound(key),
                 e = end();

        if(it != e && !m_comp(key, it->first)) {

            ret = it.m_current->data().size();
            m_tree.erase(it.m_current);
        }

        return ret;
    }

    void erase(iterator first, iterator last) {

        iterator e = end();

        while(first != last) {

            if(first == e) {

                std::cout << "Erreur: first vaut end()" << std::endl;
                return;
            }

            erase(first++); // Attention à ne pas garder l'itérateur tel quel car il est invalidé
        }
    }

    iterator erase(iterator position) {
        iterator next = position;
        ++next;

        position.m_current->data().erase(position.m_single);

        if(position.m_current->data().empty()) {

            m_tree.erase(position.m_current);
        }

        return next;
    }

    /// Algorithme swap spécialisé pour les multimap
    void swap(Multimap& other) {

        Multimap&& tmp = std::move(*this);
        *this = std::move(other);
        other = std::move(tmp);
    }

    /// Lookup
    /// ------
    /// count, find, contains,
    /// equal_range, lower_bound, upper_bound


    size_type count(const key_type& key) {
        /// TODO: const (a besoin d'un lower_bound const et donc des const_iterators)
        iterator lb = lower_bound(key);

        if(lb == end() || m_comp(key, lb->first)) {
            // Si la clé trouvé est supérieur, il n'y a pas d'éléments de cette clé
            return 0;
        }
        else {
            return lb.m_current->size();
        }
    }

    iterator find(const key_type& key) {
        iterator lb = lower_bound(key);
        if(lb != end() && !m_comp(key, lb->first)) {
            return lb;
        }
        else {
            return end();
        }
    }

    bool contains(const Key& key) const {
        //TODO not const
        return const_cast<Multimap*>(this)->find(key) != end();
    }

    std::pair<iterator, iterator> equal_range(const Key& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    iterator lower_bound(const key_type& key) {

            iterator last_not_less = end();
            iterator it = root();

            while(it != end()) {

                    if(m_comp(it->first, key)) {
                            it = it.child(iterator::ChildPosition::RIGHT);
                    }
                    else {
                            last_not_less = it;
                            it = it.child(iterator::ChildPosition::LEFT);
                    }
            }

            return last_not_less;
    }

    iterator upper_bound(const key_type& key) {

        iterator res = lower_bound(key);

        if(res == end() || m_comp(key, res->second)) {
            // Si la clé trouvé est supérieur, on la retourne
            return res;
        }
        else {
            // Sinon, on retourne l'élément suivant (ou null si il n'y a pas d'élément suivant)

            Node* next = res.m_current->next();

            if(next != nullptr) {
                return iterator(*this, *next);
            }
            else {
                return end();
            }
        }
    }

    /// Observateurs
    /// ------------
    /// key_comp, value_comp

    key_compare key_comp() const {
        return m_comp;
    }

    value_compare value_comp() const {
        return value_compare(key_comp());
    }

    /// Fonctions non membres
    /// ---------------------
    /// operator==, operator!=,
    /// operator<, operator<=,
    /// operator>, operator>=
    /// std::swap, erase_if
    ///

    friend bool operator==(const Multimap& lhs, const Multimap& rhs) {
        return lhs.equals(rhs.begin(), rhs.end());
    }

    friend bool operator==(const Multimap& lhs, const stl_type& rhs) {
        return lhs.equals(rhs.begin(), rhs.end());
    }

    friend bool operator==(const stl_type& lhs, const Multimap& rhs) {
        return rhs.equals(lhs.begin(), lhs.end());
    }

    friend bool operator!=(const Multimap& lhs, const Multimap& rhs) {
        return !lhs.equals(rhs.begin(), rhs.end());
    }

    friend bool operator!=(const Multimap& lhs, const stl_type& rhs) {
        return !lhs.equals(rhs.begin(), rhs.end());
    }

    friend bool operator!=(const stl_type& lhs, const Multimap& rhs) {
        return !rhs.equals(lhs.begin(), lhs.end());
    }

    /// Fonctions non membres utiles
    /// ----------------------------

    friend std::ostream& operator<<(std::ostream& lhs, const Multimap& rhs) {

        const_iterator it = rhs.cbegin();
        const_iterator e = rhs.cend();

        using namespace pair_operators;

        while(it != e) {

            lhs << *it;
            ++it;

            if(it != e) {
                    lhs << ", ";
            }
        }

        return lhs;
    }

    bool equals(const std::initializer_list<value_type>& il) const {
        return equals(il.begin(), il.end());
    }

    /**
     * @brief Comparaison à un intervalle d'itérateurs
     * Retourne vrai si les contenus sont strictement indentiques
     */
    template<typename InputIt>
    bool equals(InputIt first, InputIt last) const {

        const_iterator it = begin(),
                       e = end();

        while(first != last) {

            if(it == e || *it != *first) {
                return false;
            }

            ++it;
            ++first;
        }

        return it == e;
    }

    /// Fonctions amies
    /// ---------------


    friend MultimapIterator<Key, T, Compare, false>;
    friend MultimapIterator<Key, T, Compare, true>;

    /// Implémentation
    /// --------------

private:
    Compare m_comp;
    Tree m_tree;

    iterator root() {
        return m_tree.empty() ? end() : iterator(*this, *m_tree.root());
    }
};

/// Fonctions globales
/// ------------------

/**
 * Affichage d'une std::multimap
 */
template<typename Key, typename T, typename Compare>
std::ostream& operator<<(std::ostream& lhs, const std::multimap<Key, T, Compare>& rhs) {

        auto it = rhs.cbegin();
        auto e = rhs.cend();

        using namespace pair_operators;

        while(it != e) {
                lhs << *it;
                ++it;

                if(it != e) {
                        lhs << ", ";
                }
        }

        return lhs;
}

/// Instanciation explicit des templates
/// ------------------------------------

template class Multimap<int, int>;

#endif // MULTIMAP_H
