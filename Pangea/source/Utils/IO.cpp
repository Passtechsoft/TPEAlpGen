#include "IO.h"

using namespace pg;
using namespace std;

TerrainLoader::TerrainLoader(){}

TerrainLoader::~TerrainLoader()
{
	delete m_fluxI, m_fluxO;
}


bool TerrainLoader::Load()
{

}

template<T>
T TerrainLoader::PickAttribute(string id)
{
	
}
