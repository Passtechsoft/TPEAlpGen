#include "2Dmap.h"
#include "../Core/Cell.h"

using namespace std;

void addCell(Cell cellule)
{
	m_cellules.push_back(cellule);
}
///\todo terminer la fonction
bool loadFromFile(string fileName)
{
	ifstream file(fileName);
	if(!file)
		return false;
	
	return true;
}

std::vector<Cell *> findCells(Cell &cellule, float distance)
{
    std::vector<Cell *> tab;
    for(uint c=0; c!= m_cellules.size(); ++c) {
        if(Cell::distance(&cellule, &m_cellules[c]) <= distance)
            tab.push_back(&m_cellules[c]);
    }
    return tab;
}
