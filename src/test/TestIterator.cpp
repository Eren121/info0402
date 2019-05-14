#include "multimap/Multimap.h"
#include "catch.hpp"
#include <iostream>

/// Test des itérateurs

TEMPLATE_PRODUCT_TEST_CASE("Dans une map vide, l'itérateur de début doit être égal à l'itérateur de fin",
                           "[multimap][std::multimap][template][iterator]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType multimap;
    REQUIRE(multimap.begin() == multimap.end());
}

TEMPLATE_PRODUCT_TEST_CASE("La méthode begin() doit retourner un itérateur sur le début de la map",
                           "[multimap][std::multimap][template][iterator]",
                           (std::multimap, Multimap), ((int, int))) {

    using value_type = typename TestType::value_type;
    TestType multimap{{0, 0}};

    REQUIRE(*multimap.begin() == value_type{0, 0});
}

TEMPLATE_PRODUCT_TEST_CASE("Décrémenter l'itérateur de fin de retourner un itérateur sur la fin de la map",
                           "[multimap][std::multimap][template][iterator]",
                           (std::multimap, Multimap), ((int, int))) {

    using value_type = typename TestType::value_type;
    TestType multimap{{0, 0}, {1, 0}, {2, 0}};

    auto it = multimap.end();
    --it;

    REQUIRE(*it == value_type{2, 0});
}

