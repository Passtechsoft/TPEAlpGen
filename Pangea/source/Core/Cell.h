#include "../Interface/utilities.h"

class Cell
{
public:
	Cell();
	Cell(sf::Vector3f);
	
	void FindVoisines();
	
private:
	sf::Vector3f m_position;
	sf::Vector3f m_contrainte;
	float m_distanceCorrecte;
	float m_distanceActuelle;
	Cell *m_voisines[6];
};
