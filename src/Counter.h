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

	static void printCount();
	static void resetCount();

	void printInfo() const {
		std::cout << "Counter n°" << currentID << std::endl;
	}

	bool operator<(const Counter& other) const {
		return this->currentID < other.currentID;
	}

private:
	static int balance; // -1 à chaque destruction, +1 à chaque création
	static int count;	// Compteur de création
	int currentID;		// count lors de la création de l'objet
	bool trace;			// Si on doit afficher les appels des constructers / destructeurs
};

#endif // COUNTER_H
