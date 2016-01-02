#include "Roches.h"

std::map<std::string, Roche *> roches;

bool done = false;

Roche* getRoche(std::string roche)
{
	if(!done) {
		//On initialise toutes les roches disponibles par défaut
		roches[test] = new Roche(1, 23, 42);

		done = true;
	}
	return roches[roche];
}
