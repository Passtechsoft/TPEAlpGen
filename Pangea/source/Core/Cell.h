#include "../Interface/utilities.h"

class World;


class Cell
{
public:
	Cell();
	Cell(sf::Vector2f);

	void FindVoisines(World* mondeptr);

private:
	sf::Vector2f m_position;
	sf::Vector2f m_contrainte;
	float m_distanceCorrecte;
	float m_distanceActuelle;
    std::vector<std::pair<Cell*,float> > m_voisines[6];//stocke un vector de cellules voisines avec la force du lien qui relie notre cellule à elles (lien de 0 et 100(en pourcentage))
};
