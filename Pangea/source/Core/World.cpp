#include "World.h"

using namespace std;
using namespace pg;

World::World(float m_width, float m_height, uint seaHeight)
{
	
}

void World::RunSteps(uint nbSteps)
{
	for(uint c=0; c!= nbSteps; ++c) {
		RunStep();
	} 
}

///\param nbMil L'échelle de temps à simuler, en milliers d'années
void World::RunStep(int nbMil)
{
	old = new World;
	*old=*this;
	old->PrugeConfs();
	m_oldConfs.push(old);
	if(m_oldConfs.size()+1 > m_nbreOldConfs && m_nbreOldConfs >= 0)
		m_oldConfs.pop();
	
	INFO("Rechargement du tableau de cellules");	
	m_cellules.reload();
		
	///\todo le code sérieux
	///La boucle principéle
	///\note Je multiplie le temps par deux pour avoir deux pas de simulation par millénaires
	for (uint c = 0; c != nbMil*1000*2; c += 2) {
		for(uint i = 0; i!= m_cellules.size(); ++c) {
			m_cellsDis[i]->applyVelocity();
		}
		
		for(uint i = 0; i!= m_cellsDis.size(); ++c) {
			m_cellsDis[i]->applyVelocity();
		}
	}
}

void World::PurgeConfs()
{
	m_oldConfs.clear();
}

bool World::LoadConditionInitiale(std::string fileName)
{
	
}

bool World::Save(std::string fileName)
{
	TiXmlDocument file(fileName);
	if(!file)
		return false;
	
	//TiXmlElement
	
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
