#include "World.h"

using namespace std;
using namespace pg;

World::World(float m_width, float m_height, uint seaHeight)
{
	
}

void World::RunStep(int nbMil)
{
	old = new World;
	*old=*this;
	old->PrugeConfs();
	m_oldConfs.push(old);
	if(m_oldConfs.size()+1 > m_nbreOldConfs && m_nbreOldConfs >= 0)
		m_oldConfs.pop();
		
	//Et là commence(ra) le code sérieux
}

void World::PurgeConfs()
{
	m_oldConfs.clear();
}

bool World::Save(std::string fileName)
{
	TiXmlDocument file(fileName);
	if(!file)
		return false;
	
	TiXmlElement
	
	initAttributes();
	return true;
}

bool World::Load(std::string fileName)
{
	TiXmlDocument file(fileName);
	if(!file.LoadFile()){
		WARNING("error #" << doc.ErrorId() << " : " << doc.ErrorDesc());
		return false;
	}
	
	
	
	vector<string> mots;
	
	
	initAttributes();
	return true;
}
