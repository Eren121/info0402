#include "multimap/Multimap.h"
#include "catch.hpp"
#include <iostream>

/// Test de Multimap::value_compare

using std::cout;
using std::endl;

/**
 * Type pour trier les valeurs dans l'ordre décroissant
 */
template<typename Key>
struct CustomComparator {
    bool operator()(const Key& lhs, const Key& rhs) {
        return lhs > rhs;
    }
};

TEMPLATE_PRODUCT_TEST_CASE("Avec un comparateur spécialisé, la map doit être trié dans l'ordre décroissant",
                           "[multimap][std::multimap][template][value_comp][key_comp]",
                           (std::multimap, Multimap), ((int, int, CustomComparator<int>))) {

    TestType multimap;

    for(int i = 0; i < 10; i++) {

        multimap.insert({rand() % 1000, rand() % 1000});
    }

    for(auto it = multimap.begin(); it != multimap.end(); ++it) {

        auto it2 = it;
        ++it2;

        if(it2 != multimap.end()) {

            REQUIRE((*it).first >= (*it2).first);
        }
    }
}
