#include "../Utils/utilities.h"
#include "../Utils/2Dmap.h"

class World;

///The container for handle the behaviour of a cell
class Cell
{
public:
	Cell();
	Cell(const sf::Vector2f &position, World *mondeptr);

	///Cette fonction est appelée par les autres cellules quand il est nécessaire de donner sa contrainte
	float queryConstrains();
	///Cette fonction calcule la vélocité théorique
	void computeVelocity();
	///Applique la position calculée précédemment
	void applyVelocity();
	///Envoie a la cellule en question le résultat de sa résistance à la contrainte
	float getConstrainResult();
	///Ajoute une roche méangée avec cette cellule
	void addRock(Roche *rock, uint pourcentage);

	static float distance(Cell &cell1, Cell &cell2);

	void SetColor(float R, float G, float B);
	GetSet(m_position, Position);
	Setter(m_tableauCells, TabCells);
	Getter(m_rayon, Rayon);

private:
	///\warning Cette Fonction attend bien un tableau de 4 cellules adjascentes, surtout pas moins.
	void findVoisines(std::pair<Cell *,float> &tableau);

    //Variables utilisées dans le calcul de la vélocité d'une cellule mère:
	bool m_queryed = false;
    vector2f m_backVelocity;
    //Variables utilisées lors de calculs de vélocité par cellules filles:
    float m_deformX;//The constrain x of the cell
    float m_deformY;

    //Variables de la cellules:
	bool m_emmiting = false;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_distanceCorrecte;
	float m_distanceActuelle;
	float m_rayon;
    float m_compression;
	float m_mass;//The mass effect!
	2Dmap *m_tableauCells;
	///stocke le pourcentage de telle ou telle roche composant cette cellule
	std::vector<std::pair<Roche *, uint> > m_roches;
	///stocke un vector de cellules voisines avec la force du lien qui relie notre cellule à elles (lien de 0 et 100(en pourcentage))
    std::vector<std::pair<Cell *,float> > *m_voisines;

    float m_colR, m_colG, m_colB;
};
