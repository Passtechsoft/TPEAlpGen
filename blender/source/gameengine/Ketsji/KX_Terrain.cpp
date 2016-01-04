#include "KX_Terrain.h"

KX_Terrain::KX_Terrain()
	:m_size(10),
	m_interval(1.0f)
{
	// Initialisation des cellules en nid d'abeilles.
	for (unsigned int y = 0; y < m_size; ++y) {
		// Ne nombre de cellules en x : alternance de m_size et m_size - 1.
		unsigned int maxx = m_size - y % 2;
		// Le décalage en x de la première cellule : alternance 0 et m_interval / 2.
		float gapx = (y % 2) ? (m_interval / 2.0f) : 0.0f;
		for (unsigned int x = 0; x < maxx; ++x) {
			MT_Vector3 position(x * m_interval, y * m_interval, 0.0f);
			KX_Cell *cell = new KX_Cell(position);
			m_cells.push_back(cell);
		}
	}
}

KX_Terrain::~KX_Terrain()
{
	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		delete *it;
	}
}

void KX_Terrain::NextFront()
{
}

void KX_Terrain::Render()
{
	for (KX_CellList::iterator it = m_cells.begin(), end = m_cells.end(); it != end; ++it) {
		KX_Cell *cell = *it;
		cell->Render(255);
	}
}
