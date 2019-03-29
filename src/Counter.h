#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>

///
/// Classe de test pour vérifier la mémoire
/// Pour intercepter toute création d'objet, il faut redéfinir le constructeur par défaut,
/// le constructeur par copie et le destructeur.
/// Pour vérifier les fuites mémoires
///
class Counter
{

public:
	Counter(bool trace = true);
	~Counter();

	Counter(const Counter&);

	Counter& operator=(const Counter& other) {
		std::cout << "Assignement " << other.currentID << "->" << currentID
				  << "(" << balance << ")" << std::endl;
		return *this;
	}

	static void printCount();
	static void resetCount();
	static bool check();

	void printInfo() const {
		std::cout << "Counter n°" << currentID << std::endl;
	}

private:
	static int balance;  // -1 à chaque destruction, +1 à chaque création
	static int count;	 // Compteur de création
	const int currentID; // count lors de la création de l'objet
	bool trace;			 // Si on doit afficher les appels des constructers / destructeurs
};

#endif // COUNTER_H
