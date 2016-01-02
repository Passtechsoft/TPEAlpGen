#ifndef ROCHES_H_INCLUDED
#define ROCHES_H_INCLUDED

#include <iostream>
#include <map>

///Stocke les propriétés inhérentes à une roche
class Roche
{
public:
	Roche(){};
	Roche(float mV_, float viscosity_, float coefTemp_)
        : masseVolumique(mV_), viscosity(viscosity_), coefTemper(coefTemp) {};

    float masseVolumique = 0;//En kg par mètre cube
	float viscosity = 0;
	float coefTemp = 0;
};

Roche* getRoche(std::string roche);

#endif //ROCHES_H_INCLUDED
