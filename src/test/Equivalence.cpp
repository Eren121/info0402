#include "multimap/Multimap.h"
#include "catch.hpp"

/// Ce module permet de vérifier si à l'issue d'une suite d'opérations,
/// Le contenu des Multimap définies et les std::multimap standards sont identiques.

///
/// Types génériques testés
///

typedef int Key;
typedef int T;
typedef Multimap<Key, T> MyMultimap;
typedef std::multimap<Key, T> multimap;

/**
 * Foncteur qui permet d'effectuer une suite d'opération sur une multimap.
 * @tparam N Indice de la suite d'opération à tester.
 * L'ensemble des suites d'opérations TestSuite<I> ou I appartient à [0;n] sera testé (consécutive)
 * Où {n} est un entier positif,
 * Et où chaque classe TestSuite<I> est spécialisée et comporte un operator().
 **/
template<int N>
struct TestSuite {

	/**
	 * Générer la multimap de test.
	 * Pour la structure générique, on considère que le test n'existe pas,
	 * Il faut surcharger la classe pour en définir un
	 * ce qui est détecté par SFINAE si le membre emptyTest existe.
	 *
	 * @tparam multimap_t un type générique de Multimap
	 * @return La multimap généré à l'issu de l'algorithme.
	**/
	template<typename multimap_t>
	multimap_t operator()() = delete;

	enum {
		emptyTest = 1
	};
};

/**
 * Premier test, tester sur les insertions et suppressions d'intervalles
 */
template<>
struct TestSuite<0> {
	template<typename multimap_t>
	multimap_t operator()() {

		using iterator = typename multimap_t::iterator;

		const int max = 100;
		multimap_t mmap;

		// Insérer (0, 0), (0, 1), (1, 0), (1, 1), ..., (100, 0), (100, 1)

		for(int i = 0; i < max; i++) {
			mmap.insert({i, 0});
			mmap.insert({i, 1});
		}

		// Supprimer tout jusqu'à (25, 0) exclu

		for(int i = 0; i < max / 2; i++) {
			mmap.erase(mmap.begin());
		}

		// Supprimer de (75, 0) inclu à la fin

		for(int i = 0; i < max / 2; i++) {
			mmap.erase(--mmap.end());
		}

		// Supprimer les valeurs qui valent 0

		for(auto it = mmap.begin(); it != mmap.end(); ++it) {
			mmap.erase(it++);
		}

		// Supprimer tout de (50, 1) inclu à (70, 1) exclu

		mmap.erase(mmap.find(50), mmap.find(70));

		// Insérer puis supprimer plusieurs valeurs unique (grâce à equal_range)

		mmap.insert({30, 0});
		mmap.insert({30, 1}); // élément en double
		mmap.insert({30, 2});
		mmap.insert({30, 3});
		mmap.insert({31, 1}); // élément en double
		mmap.insert({31, 2});
		mmap.insert({31, 3});

		std::pair<iterator, iterator> pairIts = mmap.equal_range(31);
		mmap.erase(pairIts.first, pairIts.second);

		return mmap;
	}
};

/**
 * Deuxième test, insertions et suppressions aléatoires manuelles
 */
template<>
struct TestSuite<1> {
	template<typename multimap_t>
	multimap_t operator()() {

		const int max = 100000;
		multimap_t mmap;

		srand(12345);

		// Insertion de 10.000 éléments aléatoires (doublons possibles)

		for(int i = 0; i < 10000; i++) {

			mmap.insert({rand() % max - max / 2, rand() % max - max / 2});
		}

		// Suppression de 5.0000 éléments aléatoires (éléments non contenus possibles)

		for(int i = 0; i < 5000; i++) {

			mmap.erase(rand() % max - max / 2);
		}

		return mmap;
	}
};

/**
 * Implémentation du compteur de test.
 */
namespace impl {

	// "
	// When writing a specialization, be careful about its location; or to make it compile will be such a trial as to kindle its self-immolation
	// "

	template<typename T>
	constexpr bool testExists(decltype(T::emptyTest, int())) { return false; }

	template<typename T>
	constexpr bool testExists(long) { return true; }

	template<typename T>
	constexpr bool testExists() {

		return testExists<T>(0);
	}

	/**
	 * Vérifier que le test I existe.
	 * Assume que chaque test < I existe.
	 */
	template<int I, bool = testExists<TestSuite<I>>()>
	struct TestCount;

	template<int I>
	struct TestCount<I, true> {

		enum { count = TestCount<I + 1>::count };

		/**
		 * Effectue le test n°{I}, puis le test suivant s'il existe.
		 *
		 * @param[out] resultats L'ensemble des résultats des tests
		 *			   affecte à resultats[I] le résultat du test n°{I}
		 *
		 * @tparam I le test à vérifier
		 */
		static void doNextTest(bool* resultats) {

			TestSuite<I> suite;

			std::cout << std::endl << "Début test std::multimap" << std::endl;
			multimap mmap = suite.template operator()<multimap>();

			std::cout << std::endl << "Début test multimap" << std::endl;
			MyMultimap mymmap = suite.template operator()<MyMultimap>();

			if(mmap == mymmap) {

				resultats[I] = true;
			}
			else {

				resultats[I] = false;
			}

			TestCount<I + 1>::doNextTest(resultats);
		}
	};

	template<int I>
	struct TestCount<I, false> {
		enum { count = I };

		static void doNextTest(bool*) {}
	};
}

/**
 * Récupère le nombre total de tests effectués par la méthode doEachTest.
 * @return le nombre de tests consécutifs existant à partir de 0.
 */
constexpr int getTestsCount() {

	return impl::TestCount<0>::count;
}

/**
 * Effectue chaque test, commençant à zéro, puis consécutivement chaque test suivant.
 * Si un test n'existe pas, la fonction s'arrête.
 *
 * @return un tableau de booléens de taille {getTestsCount()} comportant, pour chaque case {i},
 *		   une variable indiquant si le test a réussi à l'issu de l'algorithme {i}
 *		   Le tableau a été alloué et doit être désalloué par l'appelant
 **/
bool* doEachTest() {

	constexpr int count = getTestsCount();
	bool* resultats = new bool[count];

	impl::TestCount<0>::doNextTest(resultats);

	return resultats;
}

TEST_CASE("Suite algorithmique", "[multimap][std::multimap][template]") {

	bool* resultats = doEachTest();

	for(int i = 0; i < getTestsCount(); i++) {
		REQUIRE(resultats[i]);
	}

	delete resultats;
}
