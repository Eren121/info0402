#include "multimap/Multimap.h"
#include "catch.hpp"
#include <iostream>

/// Test des modificateurs

TEMPLATE_PRODUCT_TEST_CASE("Après un appel à clear(), la map doit être vide",
                           "[multimap][std::multimap][template][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType multimap{{0, 0}, {0, 1}, {1, 0}, {0, 0}};
    multimap.clear();
    REQUIRE(multimap.empty());
}

/// Test des modificateurs

TEMPLATE_PRODUCT_TEST_CASE("Après un appel à insert(), la taille de la map doit augmenter de un et la fonction doit retourner l'itérateur de la valeur",
                           "[multimap][std::multimap][template][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    using value_type = typename TestType::value_type;
    TestType multimap;

    for(int i = 1; i < 5; i++) {

        value_type val = {i / 2, i};

        auto it = multimap.insert(val);

        REQUIRE(*it == val);
        REQUIRE(multimap.size() == i);
    }
}

TEMPLATE_PRODUCT_TEST_CASE("L'ordre des valeur dont les clés sont identique est indentique à l'ordre d'insertion",
                           "[multimap][std::multimap][template][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType map;

    // Insertion à la racine:
    map.insert({0, 0});
    // Insertion à gauche:
    map.insert({-1, 0});
    // Insertion à droite
    map.insert({1, 0});

    // Insertion double racine:
    map.insert({0, 1});
    // Insertion double à gauche (valeur inférieure):
    map.insert({-1, -1});
    // Insertion double à droite (valeur inférieure):
    map.insert({1, -1});

    std::initializer_list<typename TestType::value_type> list =
        {{-1, 0}, {-1, -1}, {0, 0}, {0, 1}, {1, 0}, {1, -1}};
    REQUIRE(std::equal(list.begin(), list.end(), map.begin()));
}

TEMPLATE_PRODUCT_TEST_CASE("Après un appel à erase(), si la clé, toutes les clés doivent être supprimé de la map est la taille doit diminuer du nombre de clés supprimées",
                           "[multimap][std::multimap][template][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType map{{-2, 0}, {-1, 0}, {-1, -1}, {0, 0}, {0, 1}, {1, 0}, {1, -1}};

    map.erase(0);
    REQUIRE(map.size() == 5);

    map.erase(1);
    REQUIRE(map.size() == 3);

    map.erase(-2);
    REQUIRE(map.size() == 2);
}
