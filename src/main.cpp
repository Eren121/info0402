#include "multimap/Multimap.h"
#include "Defines.h"
#include <iostream>
#include <stdlib.h>

///
/// Main personalisé dans le cas de USE_CUSTOM_MAIN == 1
/// N'efectue pas les tests unitaires
///

#if USE_CUSTOM_MAIN

using std::cout;
using std::cin;
using std::endl;

/**
 * @brief Test de la map sur l'entrée standard en affichant à la fin de chaque opération l'état de la map
 *
 * Opérations:
 * -----------
 *
 *  0 => quitter le programme
 *  1 <key> <value> => insérer une valeur
 *  2 <key> => supprimer tous les éléments de même valeur
 *  3 <key> <value> => supression d'un élément spécifique
 *
 * Exemple d'entré
 * ---------------
 *
 * <-- début, la map est vide
 *
 * 1 1
 * 1 2
 * 1 3
 * 2 1
 *
 * <-- à partir d'ici la map est vide
 *
 */
int main(int, char*[]) {

    Multimap<int, std::string> mymap;
    decltype(mymap)::iterator it = mymap.end();
    std::pair<int, std::string> value;
    int choice;
    unsigned int nbRemoved;

    while(1) {

        cin >> choice;
        nbRemoved = 0;

        switch(choice) {

            case 0: // Quitter le programme

                return 0;

            case 1: // Insertion

                cin >> value.first;
                cin >> value.second;

                mymap.insert(value);

            break;

            case 2: // Supression de tous les éléments de la même clé

                cin >> value.first;

                nbRemoved = static_cast<decltype(nbRemoved)>(mymap.erase(value.first));

                cout << nbRemoved << " supprimé(s)" << endl;

            break;

            case 3: // Supression d'un élément spécifique

                cin >> value.first;
                cin >> value.second;

                it = mymap.find(value.first);

                while(it != mymap.end() && it->first == value.first
                   && it->second != value.second) {

                     ++it;
                }

                if(it != mymap.end() && it->first == value.first) {

                    mymap.erase(it);

                    cout << "1 supprimé(s)" << endl;
                }


            break;
        }

        cout << mymap;
    }
}

#endif
