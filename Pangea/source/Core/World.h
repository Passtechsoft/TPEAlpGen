#include "../Interface/utilities.h"
#include "Cell.h"
#define MILLION 1000000

struct Point3Df
{
public:
	Point3Df(){};
	Point3Df(float _x, float _y, float _z=0) : x(_x), y(_y), z(_z){};

	float x, y, z;
};

typedef std::map<Point3Df, Cell> TableauCell;


///La classe world crée et génère un mode carré
class World
{
public:
	World();
	World(float m_width, float m_height, uint seaHeight);
	void RunStep(int nbYears=100 * MILLION);
	bool saveFile(std::string fileName);
	
	// Getters/setters:
	GetSet(m_amplitude, Amplitude);
	GetSet(m_width, Width);
	GetSet(m_height, Height);
	
private:
	uint m_width, m_height;
	uint m_maxHeight, m_minHeight;
	float m_amplitude=1;///On va multiplier la hauteur du monde par ce facteur
	
	uint m_seaHeight=5000;///La hauteur de l'océan, en mètre relativement à \a minHeight
	float m_CO2percentage=.2f;///Le pourcentage de CO2 dans l'atmosphere, permet d'en déduire la température courante et ainsi de déclarer un âge flaciaire
	TableauCell m_cells;
};


