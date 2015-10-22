#include "Cell.h"

Cell::Cell(unsigned int color,
		   NatureType nature,
		   float courbure,
		   float vieil,
		   float altitude)
	:m_nature(nature),
	m_courbure(courbure),
	m_memoire(0),
	m_vieil(vieil),
	m_altitude(altitude)
{
}

Cell::~Cell()
{
}

