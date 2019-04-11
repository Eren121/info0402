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
	Counter(Counter&&);

	Counter& operator=(const Counter& other) {

		if(trace) {

			std::cout << "Assignement " << other.currentID << "->" << currentID
					  << "(" << balance << ")" << std::endl;
		}

		assignements++;
		return *this;
	}

	Counter& operator=(Counter&& other) {

		if(trace) {

			std::cout << "Assignement " << other.currentID << "->" << currentID
					  << "(" << balance << ")" << std::endl;
		}

		move_assignments++;
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
	static int constructions, copies, destructions, assignements, move_assignments, moves;
	const int currentID; // count lors de la création de l'objet
	static int count, nextID;
	bool trace;			 // Si on doit afficher les appels des constructers / destructeurs
};

#endif // COUNTER_H
