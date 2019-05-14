#ifndef DEFINES_H
#define DEFINES_H

#include <utility>
#include <ostream>

///
/// Macro déclarations, fonctions
/// Globales au projet
///

/**
 * @brief Mettre à 1 pour utiliser un main() personalisé
 */
#define USE_CUSTOM_MAIN 0 || 1

/**
 * @brief Mettre à 1 pour effectuer tous les tests d'assertions (appel invalide, pointeur nul)
 */
#define DEBUG 1

#if DEBUG
#   include <exception>

/**
 * @brief Exception lancéé en cas d'assertion non vérifiée
 */
class AssertionError : public virtual std::logic_error {
public:
        using logic_error::logic_error;
};

/**
 * @brief utilisation de ASSERT: ASSERT(x != 0), par exemple.
 * Si x != 0 => le programme s'arrête
 */

#   define ASSERT_LOCATION(file, line) " Line " #line ", File " file
#   define ASSERT(x) do { if(!(x)) throw AssertionError(#x ASSERT_LOCATION(__FILE__, __LINE__)); } while(0)
#else
#   define ASSERT(x)
#endif

/**
 * @brief Namespace qui défini des fonctions sur le type std::pair
 */
namespace pair_operators {

    /**
     * @brief Affichage d'une paire sous forme {"first value", "second value"}
     */
    template<typename U, typename V>
    std::ostream& operator<<(std::ostream& lhs, const std::pair<U, V>& pair) {

        lhs << "{" << pair.first << "," << pair.second << "}";
        return lhs;
    }
}

#endif // DEFINES_H
