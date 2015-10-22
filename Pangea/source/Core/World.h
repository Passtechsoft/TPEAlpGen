#include "../Interface/utilities.h"
#define MILLION 000000

///La classe world crée et génère un mode carré
class World
{
public:
	World();
	RunStep(int nbYears=100#MILLION);
	
	// Getters/setters:
	Get
	
private:
	uint m_width, m_height;
	uint m_maxHeight, m_minHeight;
	float m_amplitude=1;///On va multiplier la hauteur du monde par ce facteur
	
	uint m_seaHeight=5000;///La hauteur de l'océan, en mètre relativement à \a minHeight
	float m_CO2percentage=.2f;///Le pourcentage de CO2 dans l'atmosphere, permet d'en déduire la température courante et ainsi de déclarer un âge flaciaire
};
