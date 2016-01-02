#ifndef 2DMAP_H_INCLUDED
#define 2DMAP_H_INCLUDED

#include "utilities.h"

class Cell;

class 2Dmap
{
public:
	2Ddmap() {};
	void addCell(Cell cellule);
	bool loadFromFile(std::string fileName);
	std::vector<Cell *> findCells(Cell &cellule, float distance);
private:
	std::vector<Cell> m_cellules;
};

#endif //2DMAP_H_INCLUDED
