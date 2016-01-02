#include "../Utils/utilities.h"
#include "../Utils/2dmap.h"
#include "Roches.cpp"
#include "Cell.h"

#define MILLION 1000000
typedef std::map<Point3Df, Cell> TableauCell;

class World;

struct Point3Df
{
public:
	Point3Df(){};
	Point3Df(float _x, float _y, float _z=0) : x(_x), y(_y), z(_z){};

	float x, y, z;
};


///La classe world contient et fait évoluer un mode carré, en plus d'implémenter son chargement et son importation
///\todo implémenter la sortie XML
class World
{
    friend Cell;

public:
	///largeur et hauteur en mètre
	World(float m_width, float m_height, uint seaHeight);
	///Simule un certain nombre de m_stepDuration
	void RunSteps(uint sbSteps);
	///\param nbMil le nombre d'années de simulation, en millénaires
	void RunStep(uint nbMil=m_stepDuration);
	void PurgeConfs(); //Supprime les configurations sauvegardés
	///Charge simplement les conditions préalables à la simulation
	bool LoadConditionInitiale(std::string fileName);
	//A implémenter:
	bool Save(std::string fileName);
	bool Load(std::string fileName);
	

	// Getters/setters:
	GetSet(m_amplitude, Amplitude);
	GetSet(m_width, Width);
	GetSet(m_height, Height);
	GetSet(m_stepDuration, StepDuration);

private:
	void InitAttributes();

	int m_nbOldConfs;//Le nombre de configurations qu'on sauvegrdera. Si le nombre est négatif, elles seront toutes sauvegardés.
	std::queue<World*> m_oldConfs;
	
	uint m_width, m_height;
	uint m_maxHeight, m_minHeight;
	float m_amplitude=1;///On va multiplier la hauteur du monde par ce facteur

	uint m_stepDuration = 100;//La durée d'un temps de modélisation, par défaut 100000 ans
	uint m_seaHeight=5000;///La hauteur de l'océan, en mètre relativement à \a minHeight
	float m_CO2percentage=.2f;///Le pourcentage de CO2 dans l'atmosphere, permet d'en déduire la température courante et ainsi de déclencher un âge glaciaire
    2dmap m_cellules;
    std::vector<Cell*> m_cellsDis;
};


