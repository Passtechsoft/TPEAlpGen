#include "../Interface/utilities.h"
#include "Zone.h"

///La classe Roche empaquette les propriétés relatives aux roches
class Roche
{
public:
	Roche();
private:
	float m_modulus;///Le module de la roche, qui détermine sa viscosité en GPA
	float m_seuil;
};

class Couche : public Zone
{
public:
	Couche() : Zone(){};
	Couche(Roche *roche) : m_roche(roche){Couche();};
private:
	Roche *m_roche=nullptr;//La roche constituant la couche
};
