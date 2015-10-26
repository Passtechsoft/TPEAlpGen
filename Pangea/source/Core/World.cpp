#include "World.h"

using namespace std;
using namespace pg;

World::World(float m_width, float m_height, uint seaHeight)
{

}

void World::RunStep(int nbMil)
{

}

bool World::Save(std::string fileName)
{
	ofstream file(fileName);
	if(!file)
		return false;
	
	
	
	return true;
}

bool World::Load(std::string fileName)
{
	ifstream file(fileName);
	if(!file)
		return false;
	
	vector<string> mots;
	
	
	return true;
}
