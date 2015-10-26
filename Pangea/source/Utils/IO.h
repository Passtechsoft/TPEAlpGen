#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "utilities.h"

namespace pg{
///La classe \a pg::TerrainLoader sert d'interface (IO) entre la classe World et les fichiers de sauvegarde

class TerrainLoader
{
public:
	TerrainLoader();
	//Ex: TerrainLoader tr(new ifstream("fichierIn"), new ofstream("fichierOut"));
	TerrainLoader(ifstream *fluxI, ofstream *fluxO);
	~TerrainLoader();
	
	///\return état du fichier
	bool Load();
	///\return état du fichier
	bool Save();
	
	template<T>
	///\return un nombre ou un string en fonction du type d'attribut
	T PickAttribute(std::string id);
	
	
	//Getters/setters:     (voir la macro dans utilities.h)
	GetSet(m_fluxI, Input)
	GetSet(m_fluxO, Output)
private:
	ifstream *m_fluxI;
	ofstream *m_fluxO;
	
	pg::BufferWorldObjects
};
}
#endif //#ifndef IO_H_INCLUDED
