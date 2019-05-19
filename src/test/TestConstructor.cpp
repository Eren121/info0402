#include "multimap/Multimap.h"
#include "catch.hpp"
#include <iostream>
#include <algorithm>

/// Test des constructeurs

using std::cout;
using std::endl;

struct CustomType {

    CustomType() = default;
    CustomType(const CustomType&) = default;
    CustomType& operator=(const CustomType&) = default;
    ~CustomType() = default;
};

TEMPLATE_PRODUCT_TEST_CASE("Après un constructeur par défaut, la map doit être vide",
                           "[multimap][std::multimap][template][constructor]",
                           (std::multimap, Multimap), ((int, CustomType), (int, int))) {
    TestType multimap;
    REQUIRE(multimap.empty());
    REQUIRE(multimap.size() == 0);
}

TEMPLATE_PRODUCT_TEST_CASE("Construction & insertion par intervalle d'itérateurs",
                           "[multimap][std::multimap][template][constructor][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    std::initializer_list<typename TestType::value_type> list =
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}}; // La liste doit être trié

    TestType multimap(list.begin(), list.end());
    REQUIRE(std::equal(list.begin(), list.end(), multimap.begin()));

    TestType multimap2(list.begin(), list.begin());
    REQUIRE(multimap2.empty());

    TestType multimap3;
    multimap3.insert(list.begin(), list.end());
    REQUIRE(std::equal(multimap3.begin(), multimap3.end(), multimap.begin()));
}

TEMPLATE_PRODUCT_TEST_CASE("Construction & insertion par liste d'initialisation",
                           "[multimap][std::multimap][template][constructor][modifier]",
                           (std::multimap, Multimap), ((int, int))) {

    std::initializer_list<typename TestType::value_type> list =
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}}; // La liste doit être trié

    TestType multimap(list);
    REQUIRE(std::equal(list.begin(), list.end(), multimap.begin()));

    TestType multimap2{};
    REQUIRE(multimap2.empty());

    TestType multimap3{{1, 1}};
    multimap3 = {};
    REQUIRE(multimap3.empty());
    multimap3 = list;
    REQUIRE(std::equal(list.begin(), list.end(), multimap.begin()));
}

TEMPLATE_PRODUCT_TEST_CASE("Après un constructeur ou une assignation par copie, le contenu des maps doit être identique",
                           "[multimap][std::multimap][template][constructor]",
                           (std::multimap, Multimap), ((int, int))) {

    TestType multimap{{0, 0}, {0, 1}, {1, 0}, {1, 1}}, multimap2(multimap);
    REQUIRE(multimap == multimap2);

    multimap2.clear();
    multimap2 = multimap;
    REQUIRE(multimap == multimap2);
}
