#include "multimap/Multimap.h"
#include "Defines.h"

#if OWN_MAIN

#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int, char*[]) {

	typedef Multimap<int, std::string> Multimap;
	Multimap mymap;
	Multimap::iterator it = mymap.end();
	pair<int, std::string> value;
	int choice;
	int nbRemoved;

	while(1) {

		cin >> choice;
		nbRemoved = 0;

		switch(choice) {
			case 0: return 0;
			case 1: // Insertion
				cin >> value.first;
				cin >> value.second;
				mymap.insert(value);
				break;
			case 2: // Supression de tous les éléments de la même clé
				cin >> value.first;
				nbRemoved = mymap.erase(value.first);
				cout << nbRemoved << " supprimé(s)" << endl;
				break;
			case 3: // Supression d'un élément spécifique
				cin >> value.first;
				cin >> value.second;
				it = mymap.find(value.first);

				if(it != mymap.end()) {

					while(it != mymap.end() && it->first == value.first && it->second != value.second) {
						++it;
					}
					if(it != mymap.end() && it->first == value.first) {
						mymap.erase(it);
						nbRemoved = 1;
					}
				}

				cout << nbRemoved << " supprimé(s)" << endl;
		}

		cout << mymap;
	}
}

#endif
