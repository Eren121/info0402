#ifndef TRAITS_H
#define TRAITS_H

///
/// Traits de type
///

#include <multimap/Multimap.h>
#include <map>

namespace traits {

    /**
     * Class permettant de savoir si un type provient de la classe Multimap OU de la classe std::multimap
     * Si c'est le cas, le type type = V est défini.
     */
    template<typename T, typename V = T>
        struct is_multimap {};

    template<typename T, typename U, typename V, typename W>
        struct is_multimap<Multimap<T, U, V>, W> {

        using type = W;
    };

    template<typename T, typename U, typename V, typename W>
        struct is_multimap<std::multimap<T, U, V>, W> {

        using type = W;
    };

    template<typename T, typename V = T>
        using is_multimap_t = typename is_multimap<T, V>::type;
}

#endif // TRAITS_H
