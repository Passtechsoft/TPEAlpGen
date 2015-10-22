#include "../Interface/utilities.h"

///La classe Roche empaquette les propriétés relatives aux roches
class Roche
{
	public:
		Roche();
	private:
		float modulus;//Le module de la roche, qui détermine sa viscosité en GPA
};

class Couche
{
public:
	Couche();
	Couche(Roche *roche){m_roche=roche};
private:
	Roche *m_roche=nullptr;//La roche constituant la couche
};
