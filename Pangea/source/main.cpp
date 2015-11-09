//============================================================================
// Name        : PWorldGen.cpp
// Author      : Alexis Gros & Tristan Porterie
// Version     : 0.0.1
// Copyright   : GPLv3
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Interface/utilities.h"
#include "Core/World.h"
#include "UI/Window.h"

int main(int argc, char **argv)
{
	World monde(15000, 15000, 6000);
	
	monde.SetStepDuration(50);
	
	monde.RunStep(;
	INFO("Génération d'un terrain sur 100 millions d'années");
	
	
	
}
